#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

import prime_generation;
import prime_test;

const int64_t N = 1'000'000;

TEST(Sieve, SmallValues) {
  std::vector<int32_t> primes;
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

TEST(Sieve, FirstN) {
  const auto sieve = ntlib::prime_sieve(N);
  for (int64_t i = 0; i <= N; ++i) {
    EXPECT_EQ(sieve[i], ntlib::is_prime(i));
  }
}

TEST(Sieve, PrimeList) {
  std::vector<int64_t> primes;
  const auto sieve = ntlib::prime_sieve(N, primes);
  std::vector<int64_t> list;
  for (int64_t i = 0; i <= N; ++i) {
    if (sieve[i]) {
      list.push_back(i);
    }
  }
  for (std::size_t i = 0; i < list.size(); ++i) {
    EXPECT_EQ(primes[i], list[i]);
  }
}

TEST(NextPrime, SmallValues) {
  EXPECT_EQ(ntlib::next_prime(-1), 2);
  EXPECT_EQ(ntlib::next_prime(0), 2);
  EXPECT_EQ(ntlib::next_prime(1), 2);
  EXPECT_EQ(ntlib::next_prime(2), 3);
  EXPECT_EQ(ntlib::next_prime(3), 5);
  EXPECT_EQ(ntlib::next_prime(4), 5);
  EXPECT_EQ(ntlib::next_prime(5), 7);
  EXPECT_EQ(ntlib::next_prime(6), 7);
  EXPECT_EQ(ntlib::next_prime(7), 11);
}

TEST(NextPrime, FirstN) {
  auto sieve = ntlib::prime_sieve<int32_t>(2 * N);
  for (int32_t i = 0; i <= N; ++i) {
    int32_t nxt = ntlib::next_prime(i);
    EXPECT_TRUE(sieve[nxt]);
    for (int32_t j = i + 1; j < nxt; ++j) {
      EXPECT_FALSE(sieve[j]);
    }
  }
}
