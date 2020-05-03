#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "prime_generation.hpp"
#include "prime_test.hpp"

const uint32_t N = 1'000'000;
const uint32_t M = 1'000'000'000;

TEST(Eratosthenes, Sieve) {
  std::vector<bool> sieve;
  ntlib::eratosthenes_sieve(N, sieve);
  for (uint32_t i = 0; i <= N; ++i) {
    EXPECT_EQ(sieve[i], ntlib::is_prime_naive(i));
  }
}

TEST(Eratosthenes, SieveList) {
  std::vector<bool> sieve;
  std::vector<uint32_t> primes;
  ntlib::eratosthenes_sieve_list(N, sieve, primes);
  uint32_t counter = 0;
  for (uint32_t i = 0; i <= N; ++i) {
    counter += sieve[i];
    EXPECT_EQ(sieve[i], ntlib::is_prime_naive(i));
  }

  EXPECT_EQ(primes.size(), counter);
  for (uint32_t p : primes) EXPECT_TRUE(ntlib::is_prime_naive(p));
}

TEST(Eratosthenes, List) {
  std::vector<uint32_t> primes;
  ntlib::eratosthenes_list(N, primes);
  for (uint32_t p : primes) EXPECT_TRUE(ntlib::is_prime_naive(p));
}

TEST(Eratosthenes, VariantsMatch) {
  std::vector<bool> sieve1, sieve2;
  std::vector<uint32_t> list1, list2;

  ntlib::eratosthenes_sieve(M, sieve1);
  ntlib::eratosthenes_sieve_list(M, sieve2, list1);
  ntlib::eratosthenes_list(M, list2);

  EXPECT_EQ(sieve1, sieve2);
  EXPECT_EQ(list1, list2);
}
