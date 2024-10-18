#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <numeric>
#include <set>
#include <vector>

#include "prime_test.hpp"

import base;
import prime_decomposition;
import prime_generation;

static constexpr auto composites = std::to_array<uint64_t>({
  1,
  2ull * 2,
  2ull * 3 * 5 * 7 * 11 * 13 * 17 * 19,
  2ull * 2 * 3 * 3 * 5 * 5 * 7 * 7 * 11 * 11 * 13 * 13 * 17 * 17 * 19 * 19,
  1'000'003ull * 1'000'003,
  1'000'000'007ull * 1'000'000'007,
  4'294'967'291ull * 4'294'967'291,
  3ull * 5 * 17 * 257 * 641 * 65'537 * 6'700'416 // = 2^64-1
});

static constexpr auto primes = std::to_array<uint64_t>({
  2ull,
  3ull,
  1'000'003ull,
  1'000'000'007ull,
  995'831'643'093'719ull,
  18'446'744'073'709'551'557ull // = largest prime fitting into `uint64_t`
});


template<typename T>
[[nodiscard]] constexpr
bool is_prime_decomposition(
    T n, const std::vector<ntlib::prime_power<T>> &factors) noexcept {
  // Check that each `p` is really a prime, that there are no duplicates and
  // that each `e` is at least `1`.
  std::set<T> ps;
  for (const auto [p, e] : factors) {
    ps.insert(p);
    if (!ntlib::is_prime(p)) { return false; }
    if (e < 1) { return false; }
  }
  if (ps.size() != factors.size()) { return false; }

  // Check that prime decomposition adds up to the desired value.
  return n == std::accumulate(factors.begin(), factors.end(), T{1},
      [](T prod, ntlib::prime_power<T> pp) {
    return prod * ntlib::pow(pp.p, pp.e);
  });
}

TEST(PrimeDecompositionList, Composites) {
  for (const uint64_t n : composites) {
    if (n >= ntlib::SMALL_PRIMES_BIGGEST * ntlib::SMALL_PRIMES_BIGGEST) {
      continue;
    }
    const auto res = ntlib::prime_decomposition_list(n, ntlib::SMALL_PRIMES);
    EXPECT_TRUE(is_prime_decomposition(n, res));
  }
}

TEST(PrimeDecompositionList, Primes) {
  for (const uint64_t n : primes) {
    const auto res = ntlib::prime_decomposition_list(n, ntlib::SMALL_PRIMES);
    EXPECT_TRUE(is_prime_decomposition(n, res));
  }
}

TEST(PrimeDecomposition, Composites) {
  for (const uint64_t n : composites) {
    const auto res = ntlib::prime_decomposition(n);
    EXPECT_TRUE(is_prime_decomposition(n, res));
  }
}

TEST(PrimeDecomposition, Primes) {
  for (const uint64_t n : primes) {
    const auto res = ntlib::prime_decomposition(n);
    EXPECT_TRUE(is_prime_decomposition(n, res));
    EXPECT_EQ(res.size(), 1);
  }
}
