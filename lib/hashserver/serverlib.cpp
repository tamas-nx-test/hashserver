#include "hashserver/server.hpp"
#include "serverlib_export.hpp"
#include <fmt/printf.h>

using namespace hss;

extern "C" int HSS_EXPORT start_server() {
  try {
    boost::asio::io_context io_context;

    Server srv(io_context, 8001);
    fmt::print("Starting server...\n");
    io_context.run();
  } catch (std::exception &e) {
    fmt::print("Exception: {}\n", e.what());
  }

  return 0;
}
