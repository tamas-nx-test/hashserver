#include <boost/utility/string_view_fwd.hpp>
#include <cstddef>
#include <iterator>
#include <locale>
#include <memory>

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/utility/string_view.hpp>
#include <fmt/printf.h>
#include <variant>

#include "hashserver/connection.hpp"
#include "hashserver/hasher.hpp"

using boost::string_view;
using std::begin;

namespace hss {

namespace {

void send_hash(boost::asio::ip::tcp::socket &socket, Hasher &hasher) {
  auto hash = std::make_shared<std::string>(hasher.finalize());
  boost::asio::async_write(
      socket, boost::asio::buffer(*hash),
      [hash](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          fmt::print("Sent hash: {}\n", *hash);
        }
      });
}

} // namespace

Connection::Connection(boost::asio::ip::tcp::socket socket)
    : socket_(std::move(socket)) {
  input_buffer_.resize(BUFFER_SIZE);
}

void Connection::start() { do_read(); }

void Connection::do_read() {
  auto self(shared_from_this());
  socket_.async_read_some(
      boost::asio::buffer(input_buffer_.data(), input_buffer_.size()),
      [this, self](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
          fmt::print("Read {} bytes\n", length);

          for (auto it = begin(input_buffer_),
                    end = begin(input_buffer_) + length;
               it != end;) {
            // Input represents the the full input within the input buffer
            // (which can be up to intpu_buffer_.size() bytes long).
            string_view input(&*it, std::distance(it, end));

            // Construct a string_view from the current character until the
            // first newline or the end of the input and consume that chunk.
            string_view::size_type newline = input.find('\n');
            std::size_t slen = std::distance(
                it, newline != string_view::npos ? it + newline : end);
            string_view chunk(&*it, slen);
            hasher_.update(chunk);

            // If there was no newline, that means we've reached the end of the
            // input buffer and we need to wait for further input with a newline
            // in the next callback.
            if (newline == string_view::npos) {
              break;
            }
            send_hash(socket_, hasher_);
            it = it + newline + 1;
          }
        }
        do_read();
      });
}

} // namespace hss
