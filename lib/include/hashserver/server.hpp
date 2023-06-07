#pragma once

#include <memory>

#include <boost/asio/ip/tcp.hpp>

#include "hashserver/detail/asio_fwd.hpp"

namespace hss {

class Connection;

//! \brief A server that manages connections from clients.
class Server {
public:
  //! \brief Construct a server that listens on the given port.
  //! \param io_context The io_context to use for this server. The lifetime of
  //! this io_context must be managed by the caller and must exceed the lifetime
  //! of this server.
  Server(boost::asio::io_context &io_context, short port);

private:
  void do_accept();

private:
  boost::asio::io_context &io_context_;
  boost::asio::ip::tcp::acceptor acceptor_;
};

} // namespace hss
