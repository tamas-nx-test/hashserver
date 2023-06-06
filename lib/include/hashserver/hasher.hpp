#pragma once

#include <memory>
#include <string>
#include <string_view>

#include <openssl/evp.h>

#include "serverlib_export.hpp"

namespace hss {

constexpr size_t HASH_SIZE = 5; // 160 bits

/// \brief Hasher class. The main purpose of this class is to hold the hashing
///   state to allow for incremental hashing.
class Hasher {
public:
  HSS_EXPORT Hasher();
  /// \brief Feed data to the hash context
  HSS_EXPORT void update(std::string_view data);
  /// \brief Finalize the digest and get the hexadecimal representation of the
  /// hash, then reset the context. After this call, the hasher is ready to
  /// receive new data.
  HSS_EXPORT std::string finalize() const;

private:
  std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> ctx_;
};

} // namespace hss
