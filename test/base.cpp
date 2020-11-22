#include <gtest/gtest.h>

#include <algorithm>
#include <bitset>
#include <limits>
#include <map>
#include <set>
#include <vector>

#include "base.hpp"
#include "prime_decomposition.hpp"
#include "prime_generation.hpp"

static const int min_int = std::numeric_limits<int>::min();
static const int max_int = std::numeric_limits<int>::max();
static const unsigned int max_uint = std::numeric_limits<unsigned int>::max();

TEST(PrimesBelow100, ListContainsOnlyPrimes) {
  const auto sieve = ntlib::prime_sieve(100);
  for (uint32_t p : ntlib::PRIMES_BELOW_100) {
    EXPECT_TRUE(sieve[p]);
  }
}

TEST(PrimesBelow100, ListComplete) {
  std::vector<uint32_t> primes;
  ntlib::prime_sieve(100u, primes);
  while (primes.back() >= 100) primes.pop_back();
  EXPECT_EQ(primes, ntlib::PRIMES_BELOW_100);
}

TEST(AbsoluteValue, SmallValues) {
  EXPECT_EQ(ntlib::abs(1), 1);
  EXPECT_EQ(ntlib::abs(-1), 1);
  EXPECT_EQ(ntlib::abs(0), 0);
  EXPECT_EQ(ntlib::abs(-0), 0);
}

TEST(AbsoluteValue, CornerCases) {
  EXPECT_EQ(ntlib::abs(min_int + 1), max_int);
  EXPECT_EQ(ntlib::abs(max_int), max_int);
  EXPECT_EQ(ntlib::abs(max_uint), max_uint);
}

TEST(GreatestCommonDivisor, OneParameterZero) {
  EXPECT_EQ(ntlib::gcd(10, 0), 10);
  EXPECT_EQ(ntlib::gcd(0, 10), 10);
}

TEST(GreatestCommonDivisor, Coprime) {
  EXPECT_EQ(ntlib::gcd(1, 2), 1);
  EXPECT_EQ(ntlib::gcd(2, 3), 1);
  EXPECT_EQ(ntlib::gcd(7, 15), 1);
}

TEST(GreatestCommonDivisor, NonCoprime) {
  EXPECT_EQ(ntlib::gcd(2, 2), 2);
  EXPECT_EQ(ntlib::gcd(2, 4), 2);
  EXPECT_EQ(ntlib::gcd(4, 6), 2);
}

TEST(GreatestCommonDivisor, NegativeValues) {
  EXPECT_EQ(ntlib::gcd(10, -5), 5);
  EXPECT_EQ(ntlib::gcd(-10, -5), 5);
  EXPECT_EQ(ntlib::gcd(-10, 5), 5);
}

TEST(GreatestCommonDivisor, CornerCases) {
  // 2^31-1 is prime.
  EXPECT_EQ(ntlib::gcd(max_int, 2), 1);

  // 2^32-1 is composite.
  EXPECT_EQ(ntlib::gcd(max_uint, 2u), 1);
  EXPECT_EQ(ntlib::gcd(max_uint, 3u), 3);
  EXPECT_EQ(ntlib::gcd(max_uint, 9u), 3);
  EXPECT_EQ(ntlib::gcd(max_uint, 65537u), 65537);
  EXPECT_EQ(ntlib::gcd(max_uint, 10u * 65537), 5 * 65537);

  EXPECT_EQ(ntlib::gcd(min_int, 2), 2);
  EXPECT_EQ(ntlib::gcd(min_int, -2), 2);
}

TEST(LeastCommonMultiple, Multiples) {
  EXPECT_EQ(ntlib::lcm(2, 8), 8);
  EXPECT_EQ(ntlib::lcm(3, 9), 9);
}

TEST(LeastCommonMultiple, NonCoprime) {
  EXPECT_EQ(ntlib::lcm(4, 4), 4);
  EXPECT_EQ(ntlib::lcm(4, 6), 12);
}

TEST(LeastCommonMultiple, Coprime) {
  EXPECT_EQ(ntlib::lcm(1, 2), 2);
  EXPECT_EQ(ntlib::lcm(2, 3), 6);
}

TEST(LeastCommonMultiple, NegativeValues) {
  EXPECT_EQ(ntlib::lcm(2, -3), 6);
  EXPECT_EQ(ntlib::lcm(-2, 3), 6);
  EXPECT_EQ(ntlib::lcm(-2, -3), 6);
}

TEST(LeastCommonMultiple, CornerCases) {
  EXPECT_EQ(ntlib::lcm(max_int, 1), max_int);
  EXPECT_EQ(ntlib::lcm(min_int + 1, 1), max_int);
  EXPECT_EQ(ntlib::lcm(3u * 5 * 17 * 257, 65537u), max_uint);
}

TEST(ExtendedEuclid, SmallValues) {
  int x, y;
  int g = ntlib::extended_euclid(99, 78, x, y);
  EXPECT_EQ(g, ntlib::gcd(99, 78));
  EXPECT_EQ(x * 99 + y * 78, g);
}

TEST(ExtendedEuclid, OneParameterZero) {
  int x, y;
  int gcd = ntlib::extended_euclid(10, 0, x, y);
  EXPECT_EQ(gcd, ntlib::gcd(10, 0));
  EXPECT_EQ(x * 10 + y * 0, gcd);

  gcd = ntlib::extended_euclid(0, 10, x, y);
  EXPECT_EQ(gcd, ntlib::gcd(0, 10));
  EXPECT_EQ(x * 0 + y * 10, gcd);
}

TEST(ExtendedEuclid, NegativeValues) {
  int x, y;
  EXPECT_EQ(ntlib::extended_euclid(-7, 3, x, y), 1);
  EXPECT_EQ(x * (-7) + 3 * y, 1);
  EXPECT_EQ(ntlib::extended_euclid(7, -3, x, y), 1);
  EXPECT_EQ(x * 7 + (-3) * y, 1);
  EXPECT_EQ(ntlib::extended_euclid(-7, -3, x, y), 1);
  EXPECT_EQ(x * (-7) + (-3) * y, 1);
}

TEST(ExtendedEuclid, CornerCases) {
  // 2^31-1 is prime.
  int x, y;
  EXPECT_EQ(ntlib::extended_euclid(max_int, 2, x, y), 1);
  EXPECT_EQ(x * max_int + y * 2, 1);
  EXPECT_EQ(ntlib::extended_euclid(-max_int, 2, x, y), 1);
  EXPECT_EQ(x * -max_int + y * 2, 1);
}

TEST(Exponentiation, BaseCases) {
  EXPECT_EQ(ntlib::pow(2, 0), 1);
  EXPECT_EQ(ntlib::pow(2, 1), 2);
  EXPECT_EQ(ntlib::pow(0, 1), 0);
}

TEST(Exponentiation, PowersOf2) {
  for (unsigned int e = 0; e <= 31; ++e) {
    EXPECT_EQ(ntlib::pow(2, e), 1 << e);
  }
}

TEST(Exponentiation, PowersOfMinus2) {
  for (int e = 0; e <= 30; ++e) {
    if (e & 1) {
      EXPECT_EQ(ntlib::pow(-2, e), -(1 << e));
    } else {
      EXPECT_EQ(ntlib::pow(-2, e), 1 << e);
    }
  }
}

TEST(IntegerLog2, Signed) {
  for (int i = 1; i < 1'000'000; ++i) {
    int cl2 = ntlib::ilog2(i);
    EXPECT_LE(1 << cl2, i);
    EXPECT_GT(1 << (cl2 + 1), i);
  }
}

TEST(IntegerLog2, Unsigned) {
  for (unsigned int i = 1; i < 1'000'000; ++i) {
    unsigned int cl2 = ntlib::ilog2(i);
    EXPECT_LE(1 << cl2, i);
    EXPECT_GT(1 << (cl2 + 1), i);
  }
}

TEST(IntegerLog2, NoBuiltin) {
  for (__uint128_t i = 1; i < 1'000'000; ++i) {
    __uint128_t cl2 = ntlib::ilog2(i);
    EXPECT_LE(1 << cl2, i);
    EXPECT_GT(1 << (cl2 + 1), i);
  }
}

TEST(IntegerLog2, CornerCases) {
  EXPECT_EQ(ntlib::ilog2(max_int), 30);
  EXPECT_EQ(ntlib::ilog2(max_uint), 31);
  __uint128_t power127 = static_cast<__uint128_t>(1) << 127;
  EXPECT_EQ(ntlib::ilog2(power127), 127);
  EXPECT_EQ(ntlib::ilog2(power127 - 1 + power127), 127);
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
