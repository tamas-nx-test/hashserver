#pragma once

#include <cstdint>
#include <memory>

#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "hashserver/detail/asio_fwd.hpp"
#include "hashserver/hasher.hpp"

namespace hss {

class Connection : public std::enable_shared_from_this<Connection> {
public:
  static constexpr std::size_t BUFFER_SIZE = 1024;

  Connection(boost::asio::ip::tcp::socket socket);
  boost::asio::ip::tcp::socket &socket();

  void start();

private:
  void handle_write();
  void do_read();

  boost::asio::ip::tcp::socket socket_;
  std::string input_buffer_;
  Hasher hasher_;
};

} // namespace hss
