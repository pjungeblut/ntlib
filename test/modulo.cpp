#include <gtest/gtest.h>

#include <bitset>

import base;
import modulo;
import prime_decomposition;

TEST(FloorDiv, SmallValues) {
  EXPECT_EQ(ntlib::floor_div(6, 2), 3);
  EXPECT_EQ(ntlib::floor_div(6, -2), -3);
  EXPECT_EQ(ntlib::floor_div(-6, 2), -3);
  EXPECT_EQ(ntlib::floor_div(-6, -2), 3);

  EXPECT_EQ(ntlib::floor_div(9, 4), 2);
  EXPECT_EQ(ntlib::floor_div(9, -4), -3);
  EXPECT_EQ(ntlib::floor_div(-9, 4), -3);
  EXPECT_EQ(ntlib::floor_div(-9, -4), 2);
}

TEST(CeilDiv, SmallValues) {
  EXPECT_EQ(ntlib::ceil_div(6, 2), 3);
  EXPECT_EQ(ntlib::ceil_div(6, -2), -3);
  EXPECT_EQ(ntlib::ceil_div(-6, 2), -3);
  EXPECT_EQ(ntlib::ceil_div(-6, -2), 3);

  EXPECT_EQ(ntlib::ceil_div(9, 4), 3);
  EXPECT_EQ(ntlib::ceil_div(9, -4), -2);
  EXPECT_EQ(ntlib::ceil_div(-9, 4), -2);
  EXPECT_EQ(ntlib::ceil_div(-9, -4), 3);
}

TEST(Modulo, KnuthExamples) {
  // Examples from Knuth's Concrete Mathematics.
  EXPECT_EQ(ntlib::mod(5, 3), 2);
  EXPECT_EQ(ntlib::mod(5, -3), -1);
  EXPECT_EQ(ntlib::mod(-5, 3), 1);
  EXPECT_EQ(ntlib::mod(-5, -3), -2);
}

TEST(Modulo, Divides) {
  EXPECT_EQ(ntlib::mod(6, 3), 0);
  EXPECT_EQ(ntlib::mod(6, -3), 0);
  EXPECT_EQ(ntlib::mod(-6, 3), 0);
  EXPECT_EQ(ntlib::mod(-6, -3), 0);
}

TEST(Modulo, DoesNotDivide) {
  EXPECT_EQ(ntlib::mod(6, 5), 1);
  EXPECT_EQ(ntlib::mod(6, -5), -4);
  EXPECT_EQ(ntlib::mod(-6, 5), 4);
  EXPECT_EQ(ntlib::mod(-6, -5), -1);
}

TEST(ModularExponentiation, BaseCases) {
  EXPECT_EQ(ntlib::mod_pow(2, 0, 3, ntlib::mod<int>), 1);
  EXPECT_EQ(ntlib::mod_pow(2, 1, 3, ntlib::mod<int>), 2);
  EXPECT_EQ(ntlib::mod_pow(4, 1, 3, ntlib::mod<int>), 1);
  EXPECT_EQ(ntlib::mod_pow(0, 1, 3, ntlib::mod<int>), 0);
  EXPECT_EQ(ntlib::mod_pow(-1, 1, 3, ntlib::mod<int>), 2);
  EXPECT_EQ(ntlib::mod_pow(-2, 1, 3, ntlib::mod<int>), 1);
}

TEST(ModularExponentiation, PowersOf2) {
  for (int32_t i = 1; i < 30; ++i) {
    EXPECT_EQ(ntlib::mod_pow(2, i, 2, ntlib::mod<int>), 0);
    EXPECT_EQ(ntlib::mod_pow(2, i, 3, ntlib::mod<int>), (1 << i) % 3);
  }
}

TEST(ModularExponentiation, PowersOfMinus2) {
  for (int32_t i = 1; i < 30; ++i) {
    EXPECT_EQ(ntlib::mod_pow(-2, i, 2, ntlib::mod<int>), 0) << "i = " << i;
    EXPECT_EQ(ntlib::mod_pow(-2, i, 3, ntlib::mod<int>), 1) << "i = " << i;
  }
}

TEST(ModularExponentiation, SmallValues) {
  uint32_t p = 509;
  for (uint32_t i = 1; i < p; ++i) {
    for (uint32_t j = 1; j < p; ++j) {
      uint32_t correct = 1;
      for (uint32_t k = 1; k <= j; ++k) {
        correct *= i;
        correct %= p;
      }
      EXPECT_EQ(ntlib::mod_pow(i, j, p, ntlib::mod<uint32_t>), correct);
    }
  }
}

TEST(MultiplicativeInverse, SmallValues) {
  const int32_t m = 50;
  for (int32_t n = 1; n < m; ++n) {
    if (ntlib::gcd(n, m) == 1) {
      int32_t inv = ntlib::mod_mult_inv(n, m);
      EXPECT_EQ(n * inv % m, 1);
    }
  }
}

TEST(ModularSquareTest, SmallValues) {
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

TEST(ModularSquareRoot, SmallValues) {
  const uint32_t m = 59;
  for (uint32_t n = 0; n < m; ++n) {
    if (ntlib::mod_is_square(n, m)) {
      uint32_t root = ntlib::mod_sqrt(n, m);
      EXPECT_EQ(root * root % m, n);
    }
  }
}

TEST(ModularFactorial, SmallValues) {
  const uint64_t m = 1009;
  for (uint64_t n = 0; n <= 20; ++n) {
    EXPECT_EQ(ntlib::mod(ntlib::factorial(n), m), ntlib::mod_factorial(n, m));
  }
}

TEST(LegendreSymbol, Prime3) {
  const auto l = std::to_array<int32_t>({2, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1,
      0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0});
  for (int32_t i = 1; i < static_cast<int32_t>(l.size()); ++i) {
    EXPECT_EQ(ntlib::legendre(i, 3), l[i]);
  }
}

TEST(LegendreSymbol, Prime127) {
  const auto l = std::to_array<int32_t>({2, 1, 1, -1, 1, -1, -1, -1, 1, 1, -1,
      1, -1, 1, -1, 1, 1, 1, 1, 1, -1, 1, 1, -1, -1, 1, 1, -1, -1, -1, 1});
  for (int32_t i = 1; i < static_cast<int32_t>(l.size()); ++i) {
    EXPECT_EQ(ntlib::legendre(i, 127), l[i]);
  }
}

TEST(LegendreSymbol, PeriodicInTopElement) {
  for (int32_t p : ntlib::SMALL_PRIMES<int32_t>) {
    if (p == 2) continue;
    for (int32_t i = 0; i <= 1'000; ++i) {
      EXPECT_EQ(ntlib::legendre(i, p), ntlib::legendre(i + p, p));
    }
  }
}

TEST(LegendreSymbol, Multiplicative) {
  for (int32_t p : ntlib::SMALL_PRIMES<int32_t>) {
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
  for (int32_t p : ntlib::SMALL_PRIMES<int32_t>) {
    if (p == 2) continue;
    for (int32_t i = 0; i <= 100; ++i) {
      int32_t result = i % p ? 1 : 0;
      EXPECT_EQ(ntlib::legendre(i * i, p), result);
    }
  }
}

TEST(LegendreSymbol, MinusOne) {
  for (uint32_t p : ntlib::SMALL_PRIMES<uint32_t>) {
    if (p == 2) continue;
    int32_t result = (p % 4 == 1) ? 1 : -1;
    EXPECT_EQ(ntlib::legendre(-1, static_cast<int32_t>(p)), result);
  }
}

TEST(LegendreSymbol, Two) {
  for (int32_t p : ntlib::SMALL_PRIMES<int32_t>) {
    if (p == 2) continue;
    int32_t result = (p % 8 == 1 || p % 8 == 7) ? 1 : -1;
    EXPECT_EQ(ntlib::legendre(2, p), result);
  }
}

TEST(JacobiSymbol, EmptyProduct) {
  for (int32_t a = 0; a <= 10; ++a) {
    EXPECT_EQ(ntlib::jacobi(a, 1), 1);
  }
}

TEST(JacobiSymbol, PrimeDenominator) {
  for (int32_t n : ntlib::SMALL_PRIMES<int32_t>) {
    if (n == 2) continue;
    for (int32_t k = 0; k <= 1'000; ++k) {
      EXPECT_EQ(ntlib::jacobi(k, n), ntlib::legendre(k, n));
    }
  }
}

TEST(JacobiSymbol, PrimeDecompositoin) {
  for (int32_t n = 1; n <= 1'000; n += 2) {
    const auto factors = ntlib::prime_decomposition(n);
    for (int32_t k = 0; k <= 1'000; ++k) {
      int32_t prod = 1;
      for (auto [p, e] : factors) {
        while (e--) prod *= ntlib::legendre(k, p);
      }
      EXPECT_EQ(ntlib::jacobi(k, n), prod);
    }
  }
}

TEST(JacobiSymbol, SpecialValues1) {
  auto j = ntlib::jacobi(-11, 35);
  EXPECT_EQ(j, -1);
}
