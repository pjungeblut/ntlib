#include <gtest/gtest.h>

#include "diophantine.hpp"

TEST(LinearUnivariate, InvalidArguments) {
  int32_t x;

  // If a = 0 must imply b = 0.
  EXPECT_FALSE(ntlib::diophantine_linear_univariate(0, 1, x));

  // There is no solution if b is not a multiple of a.
  EXPECT_FALSE(ntlib::diophantine_linear_univariate(2, 3, x));
}

TEST(LinearUnivariate, SmallValues) {
  int32_t x;

  // Special case 0x = 0.
  EXPECT_TRUE(ntlib::diophantine_linear_univariate(0, 0, x));
  EXPECT_EQ(x, 0);

  // Small values.
  const int32_t MAX_A = 100;
  const int32_t MAX_FACTOR = 100;
  for (int32_t a = 0; a <= MAX_A; ++a) {
    for (int32_t f = 0; f <= MAX_FACTOR; ++f) {
      int32_t b = a * f;
      EXPECT_TRUE(ntlib::diophantine_linear_univariate(a, b, x));
      EXPECT_EQ(a * x, b);
    }
  }
}

TEST(LinearBivariate, InvalidArguments) {
  int32_t x, y, gcd;

  // There are no solutions if a = b = 0 and c != 0.
  EXPECT_FALSE(ntlib::diophantine_linear_bivariate(0, 0, 1, x, y, gcd));

  // There are no solutions if a = 0 and c % b != 0.
  EXPECT_FALSE(ntlib::diophantine_linear_bivariate(0, 2, 3, x, y, gcd));

  // There are no solutions if b = 0 and c % a != 0.
  EXPECT_FALSE(ntlib::diophantine_linear_bivariate(2, 0, 3, x, y, gcd));

  // There are no solutions if a != 0, b != 0 and c % gcd(a,b) != 0.
  EXPECT_FALSE(ntlib::diophantine_linear_bivariate(2, 4, 3, x, y, gcd));
}

TEST(LinearBivariate, SmallValues) {
  int32_t x, y, gcd;
  const int32_t MIN_A = -100, MAX_A = 100;
  const int32_t MIN_B = -100, MAX_B = 100;
  const int32_t MIN_C = -100, MAX_C = 100;

  // Special case: a = b = c = 0.
  EXPECT_TRUE(ntlib::diophantine_linear_bivariate(0, 0, 0, x, y, gcd));
  EXPECT_EQ(x, 0);
  EXPECT_EQ(y, 0);

  // Special case: a = 0.
  for (int32_t b = 1; b <= MAX_B; ++b) {
    for (int32_t c = 1; c <= MAX_C; ++c) {
      if (c % b) continue;
      EXPECT_TRUE(ntlib::diophantine_linear_bivariate(0, b, c, x, y, gcd));
      EXPECT_EQ(y * b, c);
    }
  }

  // Special case: b = 0.
  for (int32_t a = 1; a <= MAX_A; ++a) {
    for (int32_t c = 1; c <= MAX_C; ++c) {
      if (c % a) continue;
      EXPECT_TRUE(ntlib::diophantine_linear_bivariate(a, 0, c, x, y, gcd));
      EXPECT_EQ(x * a, c);
    }
  }

  // Small values.
  for (int32_t a = MIN_A; a <= MAX_A; ++a) {
    for (int32_t b = MIN_B; b <= MAX_B; ++b) {
      for (int32_t c = MIN_C; c <= MAX_C; ++c) {
        if (a == 0 && b == 0) continue;
        if (c % ntlib::gcd(a, b)) continue;
        EXPECT_TRUE(ntlib::diophantine_linear_bivariate(a, b, c, x, y, gcd));
        EXPECT_EQ(a * x + b * y, c);
      }
    }
  }
}
