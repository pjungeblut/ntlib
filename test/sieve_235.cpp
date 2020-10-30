#include <gtest/gtest.h>

#include "sieve_235.hpp"

const static std::size_t min_capacity = 40;
const static std::size_t capacity = (min_capacity + 29) / 30;

TEST(Subscript, ChangeElements) {
  ntlib::sieve_235 sieve(min_capacity);
  for (std::size_t i = 0; i < capacity; ++i) {
    EXPECT_FALSE(sieve[i]);
    sieve[i] = !sieve[i];
    if (i % 2 == 0 || i % 3 == 0 || i % 5 == 0) EXPECT_FALSE(sieve[i]);
    else EXPECT_TRUE(sieve[i]);
  }
}

TEST(Subscript, SelfAssignment) {
  ntlib::sieve_235 sieve(min_capacity);
  for (std::size_t i = 0; i < capacity; ++i) {
    EXPECT_FALSE(sieve[i]);
    sieve[i] = sieve[i];
    EXPECT_FALSE(sieve[i]);
  }
}
