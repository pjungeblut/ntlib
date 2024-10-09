#include <gtest/gtest.h>

#include <cassert>
#include <vector>

#include "binomial_coefficient.hpp"
#include "modulo.hpp"
#include "prime_test.hpp"

// Prime modulus.
static constexpr uint32_t MOD_P = 1009;
// Prime power modulus.
static constexpr uint32_t BASE_PRIME = 2;
static constexpr uint32_t EXPONENT = 10;
static constexpr uint32_t MOD_PP = ntlib::pow(BASE_PRIME, EXPONENT);
// Non-prime power modulus.
static constexpr uint32_t MOD = 2 * 2 * 3 * 3;

// Chosen such that `binom(n, k)` fits into a 32 bit integer for all
// `n,k <= MAX_N`.
static constexpr uint32_t MAX_N = 29;

TEST(OneShot, SmallValues) {
  // Test some sample values.
  EXPECT_EQ(ntlib::binom(10, -1), 0);
  EXPECT_EQ(ntlib::binom(10, 0), 1);
  EXPECT_EQ(ntlib::binom(10, 1), 10);
  EXPECT_EQ(ntlib::binom(10, 2), 45);
  EXPECT_EQ(ntlib::binom(10, 3), 120);
  EXPECT_EQ(ntlib::binom(10, 4), 210);
  EXPECT_EQ(ntlib::binom(10, 5), 252);
  EXPECT_EQ(ntlib::binom(10, 6), ntlib::binom(10, 4));
  EXPECT_EQ(ntlib::binom(10, 7), ntlib::binom(10, 3));
  EXPECT_EQ(ntlib::binom(10, 8), ntlib::binom(10, 2));
  EXPECT_EQ(ntlib::binom(10, 9), ntlib::binom(10, 1));
  EXPECT_EQ(ntlib::binom(10, 10), ntlib::binom(10, 0));
  EXPECT_EQ(ntlib::binom(10, 11), 0);
}

TEST(OneShot, ModuloLargePrime) {
  static_assert(MOD_P > MAX_N);

  for (uint32_t n = 0; n <= MAX_N; ++n) {
    for (uint32_t k = 0; k <= MAX_N; ++k) {
      EXPECT_EQ(ntlib::mod(ntlib::binom(n, k), MOD_P),
          ntlib::mod_p_binom(n, k, MOD_P));
    }
  }
}

TEST(OneShot, ModuloPrimePower) {
  for (uint32_t n = 0; n <= MAX_N; ++n) {
    for (uint32_t k = 0; k <= MAX_N; ++k) {
      EXPECT_EQ(ntlib::mod(ntlib::binom(n, k), MOD_PP),
          ntlib::mod_pp_binom(n, k, BASE_PRIME, EXPONENT));
    }
  }
}

TEST(Table, SmallValues) {
  const auto binoms = ntlib::binom_table(MAX_N);
  for (uint32_t n = 0; n <= MAX_N; ++n) {
    for (uint32_t k = 0; k <= n; ++k) {
      EXPECT_EQ(binoms[n][k], ntlib::binom(n, k));
    }
  }
}

TEST(Table, Modulo) {
  const auto binoms = ntlib::binom_table(MAX_N);
  const auto mod_binoms = ntlib::mod_binom_table(MAX_N, MOD);
  for (uint32_t n = 0; n <= MAX_N; ++n) {
    for (uint32_t k = 0; k <= n; ++k) {
      EXPECT_EQ(ntlib::mod(binoms[n][k], MOD), mod_binoms[n][k]);
    }
  }
}
