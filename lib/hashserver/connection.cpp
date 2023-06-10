#include <cstddef>
#include <iterator>
#include <memory>

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/utility/string_view.hpp>
#include <fmt/printf.h>

#include "hashserver/connection.hpp"
#include "hashserver/hasher.hpp"

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

          // process the input buffer one byte at a time
          for (auto &it : boost::make_iterator_range(
                   input_buffer_.data(), input_buffer_.data() + length)) {
            // Whenever we encounter a newline, we send back the hash of the
            // currently accumulated data (excluding the newline). This also
            // resets the hasher.
            if (it == '\n') {
              send_hash(socket_, hasher_);
              continue;
            }
            hasher_.update(it);
          }
        }
        do_read();
      });
}

} // namespace hss
