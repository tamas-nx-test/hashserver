#pragma once

#include <memory>

#include <boost/asio/ip/tcp.hpp>

#include "hashserver/detail/asio_fwd.hpp"
#include "hashserver/hasher.hpp"

namespace hss {

class Connection;

class Server {
public:
  Server(boost::asio::io_context &io_context, short port);

private:
  void do_accept();

private:
  boost::asio::io_context &io_context_;
  boost::asio::ip::tcp::acceptor acceptor_;
};

} // namespace hss
