#pragma once

#include <cstdint>
#include <memory>

#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "hashserver/detail/asio_fwd.hpp"
#include "hashserver/hasher.hpp"

namespace hss {

//! \brief Represents a single connection from a client & manages the hash
//! computation
class Connection : public std::enable_shared_from_this<Connection> {
public:
  static constexpr std::size_t BUFFER_SIZE = 1024;

  //! \brief Construct a connection from a socket
  //! \param socket The socket to use for this connection. This instance will
  //! take ownership of the socket.
  Connection(boost::asio::ip::tcp::socket socket);

  //! \brief Start the async read chain.
  void start();

private:
  void do_read();

  boost::asio::ip::tcp::socket socket_;
  std::string input_buffer_;
  Hasher hasher_;
};

} // namespace hss
