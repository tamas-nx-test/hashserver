#include <boost/asio/io_context.hpp>
#include <boost/system/error_code.hpp>
#include <memory>

#include <fmt/printf.h>

#include "hashserver/connection.hpp"
#include "hashserver/server.hpp"

using namespace boost::asio::ip;

namespace hss {

Server::Server(boost::asio::io_context &io_context, short port)
    : io_context_(io_context),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
  do_accept();
}

void Server::do_accept() {
  fmt::print("do_accept\n");
  acceptor_.async_accept(
      [this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
          fmt::print("Accepted connection\n");
          std::make_shared<Connection>(std::move(socket))->start();
        } else {
          fmt::print("Error accepting connection: {}\n", ec.message());
        }

        do_accept();
      });
}

} // namespace hss
