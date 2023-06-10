#include "hashserver/hasher.hpp"
#include <gtest/gtest.h>

TEST(Hasher, EmptyHash) {
  hss::Hasher hasher;
  EXPECT_EQ(hasher.finalize(), "DA39A3EE5E6B4B0D3255BFEF95601890AFD80709");
}

TEST(Hasher, EmptyAddIsNoop) {
  hss::Hasher hasher;
  hasher.update("");
  EXPECT_EQ(hasher.finalize(), "DA39A3EE5E6B4B0D3255BFEF95601890AFD80709");
}

TEST(Hasher, HashFooBar) {
  hss::Hasher hasher;
  hasher.update("foobar");
  EXPECT_EQ(hasher.finalize(), "8843D7F92416211DE9EBB963FF4CE28125932878");
}

TEST(Hasher, HashFooBarParts) {
  hss::Hasher hasher;
  hasher.update("foo");
  hasher.update("bar");
  EXPECT_EQ(hasher.finalize(), "8843D7F92416211DE9EBB963FF4CE28125932878");
}

TEST(Hasher, HashFooBarPartsWithEmpty) {
  hss::Hasher hasher;
  hasher.update("foo");
  hasher.update("");
  hasher.update("bar");
  EXPECT_EQ(hasher.finalize(), "8843D7F92416211DE9EBB963FF4CE28125932878");
}

TEST(Hasher, HashResetsAfterFinalize) {
  hss::Hasher hasher;
  hasher.update("foobar");
  EXPECT_EQ(hasher.finalize(), "8843D7F92416211DE9EBB963FF4CE28125932878");
  // now we should get an empty string SHA-1
  EXPECT_EQ(hasher.finalize(), "DA39A3EE5E6B4B0D3255BFEF95601890AFD80709");
}

TEST(Hasher, HashSingleCharacter) {
  hss::Hasher hasher;
  hasher.update('a');
  EXPECT_EQ(hasher.finalize(), "86F7E437FAA5A7FCE15D1DDCB9EAEAEA377667B8");
}
