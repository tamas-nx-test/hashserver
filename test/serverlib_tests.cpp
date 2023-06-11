#include "hashserver/hasher.hpp"
#include <gtest/gtest.h>

TEST(Hasher, empty_hash) {
  hss::Hasher hasher;
  EXPECT_EQ(hasher.finalize(), "DA39A3EE5E6B4B0D3255BFEF95601890AFD80709");
}

TEST(Hasher, empty_add_is_noop) {
  hss::Hasher hasher;
  hasher.update("");
  EXPECT_EQ(hasher.finalize(), "DA39A3EE5E6B4B0D3255BFEF95601890AFD80709");
}

TEST(Hasher, hash_foobar) {
  hss::Hasher hasher;
  hasher.update("foobar");
  EXPECT_EQ(hasher.finalize(), "8843D7F92416211DE9EBB963FF4CE28125932878");
}

TEST(Hasher, hash_foobar_parts) {
  hss::Hasher hasher;
  hasher.update("foo");
  hasher.update("bar");
  EXPECT_EQ(hasher.finalize(), "8843D7F92416211DE9EBB963FF4CE28125932878");
}

TEST(Hasher, hash_foobar_with_empty) {
  hss::Hasher hasher;
  hasher.update("foo");
  hasher.update("");
  hasher.update("bar");
  EXPECT_EQ(hasher.finalize(), "8843D7F92416211DE9EBB963FF4CE28125932878");
}

TEST(Hasher, hash_resets_after_finalize) {
  hss::Hasher hasher;
  hasher.update("foobar");
  EXPECT_EQ(hasher.finalize(), "8843D7F92416211DE9EBB963FF4CE28125932878");
  // now we should get an empty string SHA-1
  EXPECT_EQ(hasher.finalize(), "DA39A3EE5E6B4B0D3255BFEF95601890AFD80709");
}

TEST(Hasher, hash_single_character) {
  hss::Hasher hasher;
  hasher.update('a');
  EXPECT_EQ(hasher.finalize(), "86F7E437FAA5A7FCE15D1DDCB9EAEAEA377667B8");
}
