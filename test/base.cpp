#include <gtest/gtest.h>

#include <bitset>
#include <map>
#include <set>

#include "base.hpp"
#include "prime_decomposition.hpp"

TEST(ArithmeticFunctions, AbsoluteValue) {
  EXPECT_EQ(ntlib::abs(5), 5);
  EXPECT_EQ(ntlib::abs(-5), 5);
  EXPECT_EQ(ntlib::abs(0), 0);
  EXPECT_EQ(ntlib::abs(-0), 0);
  EXPECT_EQ(ntlib::abs(-0.0), 0);
}

TEST(ArithmeticFunctions, GreatestCommonDivisor) {
  // Basic tests with (non-)coprime integers.
  EXPECT_EQ(ntlib::gcd(10, 15), 5);
  EXPECT_EQ(ntlib::gcd(10, 7), 1);

  // At least one parameter is zero.
  EXPECT_EQ(ntlib::gcd(10, 0), 10);
  EXPECT_EQ(ntlib::gcd(0, 10), 10);

  // Negative values.
  // The gcd must always be positive.
  EXPECT_EQ(ntlib::gcd(10, -5), 5);
  EXPECT_EQ(ntlib::gcd(-10, -5), 5);
  EXPECT_EQ(ntlib::gcd(-10, 5), 5);
}

TEST(ArithmeticFunctions, LowestCommonMultiple) {
  // Basic examples with (non-multiples).
  EXPECT_EQ(ntlib::lcm(2, 3), 6);
  EXPECT_EQ(ntlib::lcm(3, 9), 9);

  // Negative values.
  // The lcm must always be positive.
  EXPECT_EQ(ntlib::lcm(2, -3), 6);
  EXPECT_EQ(ntlib::lcm(-2, 3), 6);
  EXPECT_EQ(ntlib::lcm(-2, -3), 6);
}

TEST(ArithmeticFunctions, ExtendedEuclid) {
  // General example.
  int32_t x, y;
  uint32_t g = ntlib::extended_euclid(99, 78, x, y);
  EXPECT_EQ(g, 3);
  EXPECT_EQ(x * 99 + y * 78, g);

  // At least one parameter zero.
  EXPECT_EQ(ntlib::extended_euclid(10, 0, x, y), 10);
  EXPECT_EQ(ntlib::extended_euclid(0, 10, x, y), 10);

  // Negative values.
  EXPECT_EQ(ntlib::extended_euclid(-7, 3, x, y), 1);
  EXPECT_EQ(x * (-7) + 3 * y, 1);
  EXPECT_EQ(ntlib::extended_euclid(7, -3, x, y), 1);
  EXPECT_EQ(x * 7 + (-3) * y, 1);
  EXPECT_EQ(ntlib::extended_euclid(-7, -3, x, y), 1);
  EXPECT_EQ(x * (-7) + (-3) * y, 1);
}

TEST(Arithmetic, BinaryExponentation) {
  // Positive values.
  for (int32_t i = 0; i <= 10; ++i) {
    EXPECT_EQ(ntlib::pow(2, i), 1 << i);
  }

  // Negative values.
  for (int32_t i = 0; i <= 10; ++i) {
    EXPECT_EQ(ntlib::pow(-2, i), (i % 2 ? -1 : 1) * (1 << i));
  }
}

TEST(Arithmetic, CeilLog2) {
  for (__uint128_t i = 1; i < 1'000'000; ++i) {
    __uint128_t cl2 = ntlib::ceil_log2(i);
    EXPECT_GE(static_cast<__uint128_t>(1) << cl2, i);
    if (i > 1) {
      EXPECT_LT(static_cast<__uint128_t>(1) << (cl2 - 1), i);
    }
  }
}

TEST(Arithmetic, IntegerSquareRoot) {
  // General values.
  for (uint32_t i = 0; i <= 1'000'000; ++i) {
    uint32_t root = ntlib::isqrt(i);
    EXPECT_LE(root * root, i);
    EXPECT_GT((root + 1) * (root + 1), i);
  }
}

TEST(Arithmetic, SquareTest) {
  // Negative values.
  EXPECT_FALSE(ntlib::is_square(-1));

  // Squares.
  std::set<int32_t> squares;
  for (int32_t i = 0; i * i <= 1'000'000'000; ++i) {
    EXPECT_TRUE(ntlib::is_square(i * i));
    if (i * i <= 1'000'000) squares.insert(i * i);
  }

  // Small non-squares.
  for (int32_t i = 0; i <= 1'000'000; ++i) {
    if (squares.find(i) != squares.end()) continue;
    EXPECT_FALSE(ntlib::is_square(i));
  }
}

TEST(Arithmetic, Factorial) {
  // Small values.
  EXPECT_EQ(ntlib::factorial(0), 1);
  EXPECT_EQ(ntlib::factorial(1), 1);
  EXPECT_EQ(ntlib::factorial(2), 2);
  EXPECT_EQ(ntlib::factorial(3), 6);
  EXPECT_EQ(ntlib::factorial(4), 24);
  EXPECT_EQ(ntlib::factorial(5), 120);
  EXPECT_EQ(ntlib::factorial(6), 720);
  EXPECT_EQ(ntlib::factorial(7), 5'040);
  EXPECT_EQ(ntlib::factorial(8), 40'320);
  EXPECT_EQ(ntlib::factorial(9), 362'880);
  EXPECT_EQ(ntlib::factorial(10), 3'628'800);
  EXPECT_EQ(ntlib::factorial(20LL), 2'432'902'008'176'640'000LL);
}

TEST(ModularArithmetic, Exponentiation) {
  // General tests.
  uint32_t p = 509;
  for (uint32_t i = 1; i < p; ++i) {
    for (uint32_t j = 1; j < p; ++j) {
      uint32_t correct = 1;
      for (uint32_t k = 1; k <= j; ++k) {
        correct *= i;
        correct %= p;
      }
      EXPECT_EQ(ntlib::mod_pow(i, j, p), correct);
    }
  }
}

TEST(ModularArithmetic, MultiplicativeInverse) {
  const uint32_t m = 50;
  for (uint32_t n = 1; n < m; ++n) {
    if (ntlib::gcd(n, m) == 1) {
      uint32_t inv = ntlib::mod_mult_inv(n, m);
      EXPECT_EQ(n * inv % m, 1);
    }
  }
}

TEST(ModularArithmetic, LegendreSymbol) {
  const uint32_t p = 509;
  std::bitset<p> square;
  for (uint32_t n = 0; n < p; ++n) {
    uint32_t nn = n * n % p;
    square[nn] = true;
    EXPECT_TRUE(ntlib::mod_is_square(nn, p));
  }
  for (uint32_t n = 0; n < p; ++n) {
    if (!square[n]) {
      EXPECT_FALSE(ntlib::mod_is_square(n, p));
    }
  }
}

TEST(ModularArithmetic, SquareRoots) {
  const uint32_t m = 59;
  for (uint32_t n = 0; n < m; ++n) {
    if (ntlib::mod_is_square(n, m)) {
      uint32_t root = ntlib::mod_sqrt(n, m);
      EXPECT_EQ(root * root % m, n);
    }
  }
}

TEST(LegendreSymbol, Prime3) {
  const int32_t l[] = {2, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1,
      -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0};
  for (int32_t i = 1; i <= 30; ++i) {
    EXPECT_EQ(ntlib::legendre(i, 3), l[i]);
  }
}

TEST(LegendreSymbol, Prime127) {
  const int32_t l[] = {2, 1, 1, -1, 1, -1, -1, -1, 1, 1, -1, 1, -1, 1, -1, 1, 1,
      1, 1, 1, -1, 1, 1, -1, -1, 1, 1, -1, -1, -1, 1};
  for (int32_t i = 1; i <= 30; ++i) {
    EXPECT_EQ(ntlib::legendre(i, 127), l[i]);
  }
}

TEST(LegendreSymbol, PeriodicInTopElement) {
  for (int32_t p : ntlib::PRIMES_BELOW_100) {
    if (p == 2) continue;
    for (int32_t i = 0; i <= 1'000; ++i) {
      EXPECT_EQ(ntlib::legendre(i, p), ntlib::legendre(i + p, p));
    }
  }
}

TEST(LegendreSymbol, Multiplicative) {
  for (int32_t p : ntlib::PRIMES_BELOW_100) {
    if (p == 2) continue;
    for (int32_t i = 0; i <= 100; ++i) {
      for (int32_t j = 0; j <= 100; ++j) {
        EXPECT_EQ(ntlib::legendre(i, p) * ntlib::legendre(j, p),
            ntlib::legendre(i * j, p));
      }
    }
  }
}

TEST(LegendreSymbol, Squares) {
  for (int32_t p : ntlib::PRIMES_BELOW_100) {
    if (p == 2) continue;
    for (int32_t i = 0; i <= 100; ++i) {
      if (i % p == 0) {
        EXPECT_EQ(ntlib::legendre(i * i, p), 0);
      } else {
        EXPECT_EQ(ntlib::legendre(i * i, p), 1);
      }
    }
  }
}

TEST(LegendreSymbol, MinusOne) {
  for (int32_t p : ntlib::PRIMES_BELOW_100) {
    if (p == 2) continue;
    if (p % 4 == 1) {
      EXPECT_EQ(ntlib::legendre(-1, p), 1);
    } else {
      EXPECT_EQ(ntlib::legendre(-1, p), -1);
    }
  }
}

TEST(LegendreSymbol, Two) {
  for (int32_t p : ntlib::PRIMES_BELOW_100) {
    if (p == 2) continue;
    if (p % 8 == 1 || p % 8 == 7) {
      EXPECT_EQ(ntlib::legendre(2, p), 1);
    } else {
      EXPECT_EQ(ntlib::legendre(2, p), -1);
    }
  }
}

TEST(JacobiSymbol, EmptyProduct) {
  for (int32_t a = 0; a <= 10; ++a) {
    EXPECT_EQ(ntlib::jacobi(a, 1), 1);
  }
}

TEST(JacobiSymbol, PrimeDenominator) {
  for (int32_t n : ntlib::PRIMES_BELOW_100) {
    if (n == 2) continue;
    for (int32_t k = 0; k <= 1'000; ++k) {
      EXPECT_EQ(ntlib::jacobi(k, n), ntlib::legendre(k, n));
    }
  }
}

TEST(JacobiSymbol, PrimeDecompositoin) {
  for (int32_t n = 1; n <= 1'000; n += 2) {
    std::map<int32_t, int32_t> factors;
    ntlib::prime_decomposition(n, factors);
    for (int32_t k = 0; k <= 1'000; ++k) {
      int32_t prod = 1;
      for (auto [f, m] : factors) {
        while (m--) prod *= ntlib::legendre(k, f);
      }
      EXPECT_EQ(ntlib::jacobi(k, n), prod);
    }
  }
}

TEST(JacobiSymbol, SpecialValues1) {
  auto j = ntlib::jacobi(-11, 35);
  EXPECT_EQ(j, -1);
}
