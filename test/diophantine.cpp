#include <gtest/gtest.h>

#include "diophantine.hpp"

TEST(LinearUnivariate, SmallValues) {
  // Special case 0x = 0.
  EXPECT_EQ(ntlib::diophantine_linear_univariate(0, 0), 0);

  // Small values.
  const uint32_t MAX_A = 100;
  const uint32_t MAX_FACTOR = 100;
  for (uint32_t a = 0; a <= MAX_A; ++a) {
    for (uint32_t f = 0; f <= MAX_FACTOR; ++f) {
      uint32_t b = a * f;
      uint32_t x = ntlib::diophantine_linear_univariate(a, b);
      EXPECT_EQ(a * x, b);
    }
  }
}

TEST(LinearBivariate, SmallValues) {
  const int32_t MIN_A = -100, MAX_A = 100;
  const int32_t MIN_B = -100, MAX_B = 100;
  const int32_t MIN_C = -100, MAX_C = 100;

  // Special case: a = b = c = 0.
  EXPECT_EQ(ntlib::diophantine_linear_bivariate(0, 0, 0),
      std::make_tuple(0, 0, 0));

  // Special case: a = 0.
  for (int32_t b = 1; b <= MAX_B; ++b) {
    for (int32_t c = 1; c <= MAX_C; ++c) {
      if (c % b) continue;
      const auto [x, y, gcd] = ntlib::diophantine_linear_bivariate(0, b, c);
      EXPECT_EQ(y * b, c);
    }
  }

  // Special case: b = 0.
  for (int32_t a = 1; a <= MAX_A; ++a) {
    for (int32_t c = 1; c <= MAX_C; ++c) {
      if (c % a) continue;
      const auto [x, y, gcd] = ntlib::diophantine_linear_bivariate(a, 0, c);
      EXPECT_EQ(x * a, c);
    }
  }

  // Small values.
  for (int32_t a = MIN_A; a <= MAX_A; ++a) {
    for (int32_t b = MIN_B; b <= MAX_B; ++b) {
      for (int32_t c = MIN_C; c <= MAX_C; ++c) {
        if (a == 0 && b == 0) continue;
        if (c % ntlib::gcd(a, b)) continue;
        const auto [x, y, gcd] = ntlib::diophantine_linear_bivariate(a, b, c);
        EXPECT_EQ(a * x + b * y, c);
      }
    }
  }
}
