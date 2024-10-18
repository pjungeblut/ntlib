#include <gtest/gtest.h>

import sieve_235;

const static std::size_t min_capacity = 1'000;
const static std::size_t capacity = (min_capacity + 29) / 30 * 30;

static ntlib::sieve_235<> get_dummy_sieve(std::size_t min_capacity) {
  ntlib::sieve_235 sieve(min_capacity);
  for (std::size_t i = 0; i < sieve.size(); ++i) sieve[i] = i % 7;
  return sieve;
}

static const ntlib::sieve_235 dummy = get_dummy_sieve(min_capacity);

TEST(Construction, DefaultConstruction) {
  ntlib::sieve_235 sieve;
  EXPECT_TRUE(sieve.empty());
}

TEST(Construction, BySize) {
  ntlib::sieve_235 sieve(min_capacity);
  EXPECT_EQ(sieve.size(), capacity);
}

TEST(Construction, BySizeEmpty) {
  ntlib::sieve_235 sieve(0);
  EXPECT_TRUE(sieve.empty());
}

TEST(Access, ConstAndMutable) {
  const ntlib::sieve_235 s1 = get_dummy_sieve(min_capacity);
  ntlib::sieve_235 s2(min_capacity);
  for (std::size_t i = 0; i < capacity; ++i) {
    s2[i] = s1[i];
    EXPECT_EQ(s1[i], s2[i]);
  }
}
