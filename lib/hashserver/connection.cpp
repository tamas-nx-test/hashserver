#include <cstddef>
#include <fmt/core.h>
#include <iterator>
#include <memory>
#include <string_view>

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <boost/tokenizer.hpp>
#include <fmt/printf.h>

#include "hashserver/connection.hpp"

using std::begin;
using std::end;
using std::next;

namespace hss {

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
          // special case: a single character that is a newline
          if (length == 1 && input_buffer_[0] == '\n') {
            auto hash = std::make_shared<std::string>(hasher_.finalize());
            boost::asio::async_write(
                socket_, boost::asio::buffer(*hash),
                [this, self, hash](boost::system::error_code ec,
                                   std::size_t /*length*/) {
                  if (!ec) {
                    fmt::print("Sent hash: {}\n", *hash);
                  }
                });
            do_read();
            return;
          }

          // we have read some amount of characters, some of which may be
          // newlines
          std::string_view text(input_buffer_.data(), length);
          boost::char_separator<char> sep("\n");
          boost::tokenizer<boost::char_separator<char>> tokens(text, sep);
          bool terminated = text.back() == '\n';
          auto num_tokens = std::distance(begin(tokens), end(tokens));

          for (auto it = begin(tokens); it != end(tokens); ++it) {
            hasher_.update(*it);

            bool is_last = next(it) == end(tokens);

            if (!is_last || terminated) {
              // the last token is potentially an incomplete line (no newline at
              // the end) so only finalize and send back the hash if it's
              // complete. We only want to send back the hash if we have a
              // complete line. If it's not the last token, we know it's a
              // complete line because we split on newlines.
              auto hash = std::make_shared<std::string>(hasher_.finalize());
              boost::asio::async_write(
                  socket_, boost::asio::buffer(*hash),
                  [this, self, hash](boost::system::error_code ec,
                                     std::size_t /*length*/) {
                    if (!ec) {
                      fmt::print("Sent hash: {}\n", *hash);
                    }
                  });
            }
          }
        }
        do_read();
      });
}

} // namespace hss
