#include <openssl/evp.h>
#include <openssl/evperr.h>
#include <stdexcept>
#include <system_error>

#include <fmt/format.h>
#include <fmt/printf.h>
#include <openssl/err.h>

#include "hashserver/hasher.hpp"

namespace hss {

constexpr int OK = 1; // Some OpenSSL functions return 1 on success

void init(EVP_MD_CTX *ctx) {
  if (!EVP_DigestInit_ex2(ctx, EVP_sha1(), NULL)) {
    throw std::runtime_error(
        fmt::format("Failed to initialize hash context: {}",
                    ERR_error_string(ERR_get_error(), NULL)));
  }
}

Hasher::Hasher() : ctx_(EVP_MD_CTX_new(), EVP_MD_CTX_free) { init(ctx_.get()); }

void Hasher::update(boost::string_view data) {
  if (data.empty()) {
    return;
  }
  if (int e = EVP_DigestUpdate(ctx_.get(), data.data(), data.size()) != OK) {
    throw std::runtime_error(fmt::format("Failed to update hash context: {}",
                                         ERR_error_string(e, NULL)));
  }
}

void Hasher::update(char data) {
  if (int e = EVP_DigestUpdate(ctx_.get(), &data, 1) != OK) {
    throw std::runtime_error(fmt::format("Failed to update hash context: {}",
                                         ERR_error_string(e, NULL)));
  }
}

std::string Hasher::finalize() const {
  unsigned char md_value[EVP_MAX_MD_SIZE];
  unsigned int md_len;

  if (int e = EVP_DigestFinal_ex(ctx_.get(), md_value, &md_len) != OK) {
    throw std::runtime_error(fmt::format("Failed to finalize hash context: {}",
                                         ERR_error_string(e, NULL)));
  }

  std::string hash;
  hash.resize(md_len * 2 + 1); // SHA-1 hash is 160 bits = 20 bytes, represented
                               // as 40 hexadecimal characters
  std::size_t strlength = 0;
  if (int e = OPENSSL_buf2hexstr_ex(hash.data(), hash.size(), &strlength,
                                    md_value, md_len, '\0') != OK) {
    throw std::runtime_error(fmt::format("Failed to convert hash to string: {}",
                                         ERR_error_string(e, NULL)));
  }
  hash.resize(strlength - 1); // remove the trailing '\0'

  init(ctx_.get());

  return hash;
}

} // namespace hss
