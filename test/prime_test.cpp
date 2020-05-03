#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "prime_generation.hpp"
#include "prime_test.hpp"

TEST(Naive, SmallValues) {
  uint32_t N = 1'000'000;
  std::vector<bool> sieve;
  ntlib::eratosthenes_sieve(N, sieve);
  for (uint32_t n = 0; n <= N; ++n) {
    EXPECT_EQ(ntlib::is_prime_naive(n), sieve[n]);
  }
}

TEST(Naive, NegativeValues) {
  EXPECT_FALSE(ntlib::is_prime_naive(-1));
}

TEST(MillerRabin, SmallValues) {
  uint64_t N = 1'000'000;
  std::vector<bool> sieve;
  ntlib::eratosthenes_sieve(N, sieve);
  for (uint64_t n = 0; n <= N; ++n) {
    EXPECT_EQ(ntlib::is_prime_miller_rabin(n), sieve[n]);
  }
}

TEST(MillerRabin, NegativeValues) {
  EXPECT_FALSE(ntlib::is_prime_miller_rabin(-1));
}

TEST(MillerRabin, LargeComposites) {
  __uint128_t n1 = 1'000'000'007LL * 1'000'000'007LL;
  __uint128_t n2 = 4'120'038'565'055'551LL;
  EXPECT_FALSE(ntlib::is_prime_miller_rabin(n1));
  EXPECT_FALSE(ntlib::is_prime_miller_rabin(n2));
}

TEST(MillerRabin, LargePrimes) {
  __uint128_t n1 = 952'016'363'681'739'749LL;
  __uint128_t n2 = 301'697'296'732'166'057LL;
  EXPECT_TRUE(ntlib::is_prime_miller_rabin(n1));
  EXPECT_TRUE(ntlib::is_prime_miller_rabin(n2));
}
