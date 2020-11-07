#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "../benchmarks/experiments/prime_generation.hpp"
#include "prime_generation.hpp"
#include "prime_test.hpp"

const uint64_t N = 10'000'000;

TEST(Sieve, SmallValues) {
  std::vector<std::size_t> primes;
  const auto sieve = ntlib::prime_sieve(10, primes);
  EXPECT_FALSE(sieve[0]);
  EXPECT_FALSE(sieve[1]);
  EXPECT_TRUE(sieve[2]);
  EXPECT_TRUE(sieve[3]);
  EXPECT_FALSE(sieve[4]);
  EXPECT_TRUE(sieve[5]);
  EXPECT_FALSE(sieve[6]);
  EXPECT_TRUE(sieve[7]);
  EXPECT_FALSE(sieve[8]);
  EXPECT_FALSE(sieve[9]);
  EXPECT_FALSE(sieve[10]);

  EXPECT_GE(primes.size(), 4);
  EXPECT_EQ(primes[0], 2);
  EXPECT_EQ(primes[1], 3);
  EXPECT_EQ(primes[2], 5);
  EXPECT_EQ(primes[3], 7);
}

TEST(Sieve, First10Million) {
  const auto sieve = ntlib::prime_sieve(N);
  for (std::size_t i = 0; i <= N; ++i) {
    EXPECT_EQ(sieve[i], ntlib::is_prime(i));
  }
}

TEST(Sieve, PrimeList) {
  std::vector<std::size_t> primes;
  const auto sieve = ntlib::prime_sieve(N, primes);
  std::vector<std::size_t> list;
  for (std::size_t i = 0; i <= N; ++i) {
    if (sieve[i]) {
      list.push_back(i);
    }
  }
  for (std::size_t i = 0; i < list.size(); ++i) {
    EXPECT_EQ(primes[i], list[i]);
  }
}

TEST(Experiments, VariantsMatch) {
  auto textbook = ntlib::experiments::eratosthenes_textbook(N);
  auto jumps = ntlib::experiments::eratosthenes_jumps(N);
  for (std::size_t i = 0; i <= N; ++i) EXPECT_EQ(textbook[i], jumps[i]);

  auto segmented = ntlib::prime_sieve(N);
  for (std::size_t i = 0; i <= N; ++i) EXPECT_EQ(textbook[i], segmented[i]);
}
