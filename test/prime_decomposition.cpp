#include <gtest/gtest.h>

#include <cstdint>
#include <map>
#include <vector>

#include "base.hpp"
#include "prime_decomposition.hpp"
#include "prime_generation.hpp"

const uint64_t numbers[] = {
  1'000'003LL * 1'000'003LL, // Composite.
  2LL * 2 * 2 * 3 * 3 * 3 * 4 * 4 * 4, // Composite.
  995'831'643'093'719LL // Prime.
};

template<typename T>
bool is_prime_decomposition(T n, std::map<T,T> &factors) {
  T test = 1;
  for (auto [k,v] : factors) {
    while (v--) test *= k;
  }
  return test == n;
}

TEST(PrimeDecomposition, Classical) {
  std::map<uint64_t, uint64_t> factors;
  for (const uint64_t n : numbers) {
    ntlib::prime_decomposition(n, factors);
    EXPECT_TRUE(is_prime_decomposition(n, factors));
  }
}

TEST(PrimeDecomposition, PrimeList) {
  std::map<uint64_t, uint64_t> factors;
  for (const uint64_t n : numbers) {
    std::vector<uint64_t> primes;
    ntlib::prime_sieve(ntlib::isqrt(n), primes);
    ntlib::prime_decomposition(n, primes, factors);
    EXPECT_TRUE(is_prime_decomposition(n, factors));
  }
}
