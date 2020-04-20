#include <gtest/gtest.h>

#include <vector>

#include "binomial_coefficient.hpp"

TEST(BinomialCoefficient, SingleValue) {
  // Test invalid inputs (n  negative).
  EXPECT_DEATH(ntlib::binom(-1, 9), "");

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

TEST(BinomialCoefficient, Table) {
  std::vector<std::vector<int32_t>> binoms;

  // Test invalid input.
  EXPECT_DEATH(ntlib::binom_table(-1, binoms), "");

  // Test some values.
  // N = 29 is the biggest value such that ntlib::binom(N,_) does not overflow.
  const int32_t N = 29;
  ntlib::binom_table(N, binoms);
  for (int32_t n = 0; n <= N; ++n) {
    for (int32_t k = 0; k <= n; ++k) {
      EXPECT_EQ(binoms[n][k], ntlib::binom(n, k));
    }
  }
}

TEST(ModularBinomialCoefficient, Table) {
  std::vector<std::vector<int32_t>> binoms;

  // Test invalid input.
  EXPECT_DEATH(ntlib::mod_binom_table(-1, 10, binoms), "");
  EXPECT_DEATH(ntlib::mod_binom_table(10, 0, binoms), "");

  // Test some values.
  // N = 29 is the biggest value such that ntlib::binom(N,_) does not overflow.
  const int32_t m = 509;
  const int32_t N = 29;
  ntlib::mod_binom_table(N, m, binoms);
  for (int32_t n = 0; n <= N; ++n) {
    for (int32_t k = 0; k <= n; ++k) {
      EXPECT_EQ(binoms[n][k], ntlib::binom(n, k) % m);
    }
  }
}
