#pragma once

#include <memory>
#include <string>

#include <boost/utility/string_view.hpp>
#include <openssl/evp.h>

#include "serverlib_export.hpp"

namespace hss {

/// \brief Hasher class. The main purpose of this class is to hold the hashing
///   state to allow for incremental hashing.
class Hasher {
public:
  HSS_EXPORT Hasher();
  /// \brief Feed data to the hash context
  /// \param data The data to be hashed. Can be empty (resulting in no-op).
  HSS_EXPORT void update(boost::string_view data);
  /// \brief Finalize the digest and get the hexadecimal representation of the
  /// hash, then reset the context. After this call, the hasher is ready to
  /// receive new data.
  /// \return The hexadecimal representation of the computed hash.
  HSS_EXPORT std::string finalize() const;

private:
  std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> ctx_;
  std::string buffer_;
};

} // namespace hss
