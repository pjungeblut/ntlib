#include <gtest/gtest.h>

import sieve;

const static std::size_t capacity = 1'000;

static ntlib::sieve<> get_dummy_sieve(std::size_t N) {
  ntlib::sieve sieve(N);
  for (std::size_t i = 0; i < N; ++i) sieve[i] = i & 1;
  return sieve;
}

static const ntlib::sieve dummy = get_dummy_sieve(capacity);

TEST(Construction, DefaultConstruction) {
  ntlib::sieve sieve;
  EXPECT_EQ(sieve.size(), 0);
  EXPECT_TRUE(sieve.empty());
}

TEST(Construction, BySizeEmpty) {
  ntlib::sieve sieve(0);
  EXPECT_TRUE(sieve.empty());
}

TEST(Access, ConstAndMutable) {
  const ntlib::sieve s1 = get_dummy_sieve(capacity);
  ntlib::sieve s2(capacity);
  for (std::size_t i = 0; i < capacity; ++i) {
    s2[i] = s1[i];
    EXPECT_EQ(s1[i], s2[i]);
  }
}
