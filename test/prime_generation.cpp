#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "../benchmarks/experiments/prime_generation.hpp"
#include "prime_generation.hpp"
#include "prime_test.hpp"

const uint64_t N = 10'000;
const uint64_t M = 10'000'000;

TEST(Eratosthenes, Sieve) {
  std::vector<bool> sieve;
  ntlib::eratosthenes_sieve(N, sieve);
  for (uint32_t i = 0; i <= N; ++i) {
    EXPECT_EQ(sieve[i], ntlib::is_prime_naive(i));
  }
}

TEST(Eratosthenes, SieveList) {
  std::vector<bool> sieve;
  std::vector<uint64_t> primes;
  ntlib::eratosthenes_sieve_list(N, sieve, primes);
  uint64_t counter = 0;
  for (uint64_t i = 0; i <= N; ++i) {
    counter += sieve[i];
    EXPECT_EQ(sieve[i], ntlib::is_prime_naive(i));
  }

  EXPECT_EQ(primes.size(), counter);
  for (uint64_t p : primes) EXPECT_TRUE(ntlib::is_prime_naive(p));
}

TEST(Eratosthenes, List) {
  std::vector<uint64_t> primes;
  ntlib::eratosthenes_list(N, primes);
  for (uint64_t p : primes) EXPECT_TRUE(ntlib::is_prime_naive(p));
}

TEST(Eratosthenes, VariantsMatch) {
  std::vector<bool> sieve1, sieve2;
  std::vector<uint64_t> list1, list2;

  ntlib::eratosthenes_sieve(M, sieve1);
  ntlib::eratosthenes_sieve_list(M, sieve2, list1);
  ntlib::eratosthenes_list(M, list2);

  EXPECT_EQ(sieve1, sieve2);
  EXPECT_EQ(list1, list2);
}

TEST(Sundaram, Sieve) {
  std::vector<bool> sieve;
  ntlib::sundaram_sieve(N, sieve);
  for (uint64_t i = 0; i <= N; ++i) {
    EXPECT_EQ(sieve[i], ntlib::is_prime_naive(i)) << " at value " << i;
  }
}

TEST(Sieves, VariantsMatch) {
  std::vector<bool> sieve1, sieve2;

  ntlib::eratosthenes_sieve(M, sieve1);
  ntlib::sundaram_sieve(M, sieve2);

  EXPECT_EQ(sieve1, sieve2);
}

TEST(Experiments, VariantsMatch) {
  std::vector<bool> eratosthenes;
  ntlib::eratosthenes_sieve(M, eratosthenes);

  auto textbook = ntlib::experiments::eratosthenes_textbook(M);
  for (std::size_t i = 0; i <= M; ++i) EXPECT_EQ(eratosthenes[i], textbook[i]);

  auto jumps = ntlib::experiments::eratosthenes_jumps(M);
  for (std::size_t i = 0; i <= M; ++i) EXPECT_EQ(eratosthenes[i], jumps[i]);

  auto segmented = ntlib::experiments::eratosthenes_segmented(M);
  for (std::size_t i = 0; i <= M; ++i) EXPECT_EQ(eratosthenes[i], segmented[i]);
}
