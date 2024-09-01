#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <bitset>
#include <limits>
#include <map>
#include <set>
#include <vector>

#include "base.hpp"
#include "prime_decomposition.hpp"
#include "prime_generation.hpp"

static constexpr int min_int = std::numeric_limits<int>::min();
static constexpr int max_int = std::numeric_limits<int>::max();
static constexpr unsigned int max_uint =
    std::numeric_limits<unsigned int>::max();
static constexpr int64_t max_int64 = std::numeric_limits<int64_t>::max();
static constexpr uint64_t max_uint64 = std::numeric_limits<uint64_t>::max();
static constexpr __int128_t max_int128 =
    (__int128_t(1) << 126) - 1 +
    (__int128_t(1) << 126);
static constexpr __uint128_t max_uint128 =
    (__uint128_t(1) << 127) - 1 +
    (__uint128_t(1) << 127);

TEST(SmallPrimes, ListContainsOnlyPrimes) {
  const auto sieve = ntlib::prime_sieve(ntlib::SMALL_PRIMES_UPPER_BOUND);
  for (uint32_t p : ntlib::SMALL_PRIMES) {
    EXPECT_TRUE(sieve[p]);
  }
}

TEST(SmallPrimes, ListComplete) {
  std::vector<uint32_t> primes;
  ntlib::prime_sieve(ntlib::SMALL_PRIMES_UPPER_BOUND, primes);
  // The `prime_sieve` method might return a larger sieve than requested.
  // In this case, we remove the additional primes in `primes`.
  while (primes.back() >= ntlib::SMALL_PRIMES_UPPER_BOUND) {
    primes.pop_back();
  }
  EXPECT_TRUE(std::ranges::equal(primes, ntlib::SMALL_PRIMES));
}

TEST(Odd, SmallValues) {
  EXPECT_FALSE(ntlib::is_odd(-2));
  EXPECT_TRUE(ntlib::is_odd(-1));
  EXPECT_FALSE(ntlib::is_odd(0));
  EXPECT_TRUE(ntlib::is_odd(1));
  EXPECT_FALSE(ntlib::is_odd(2));
}

TEST(Even, SmallValues) {
  EXPECT_TRUE(ntlib::is_even(-2));
  EXPECT_FALSE(ntlib::is_even(-1));
  EXPECT_TRUE(ntlib::is_even(0));
  EXPECT_FALSE(ntlib::is_even(1));
  EXPECT_TRUE(ntlib::is_even(2));
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
  EXPECT_EQ(ntlib::gcd(max_uint, 65'537u), 65'537);
  EXPECT_EQ(ntlib::gcd(max_uint, 10u * 65'537), 5 * 65'537);

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
  EXPECT_EQ(ntlib::lcm(3u * 5 * 17 * 257, 65'537u), max_uint);
}

TEST(ExtendedEuclid, SmallValues) {
  auto [gcd, x, y] = ntlib::extended_euclid(99, 78);
  EXPECT_EQ(gcd, ntlib::gcd(99, 78));
  EXPECT_EQ(x * 99 + y * 78, gcd);
}

TEST(ExtendedEuclid, OneParameterZero) {
  auto [gcd1, x1, y1] = ntlib::extended_euclid(10, 0);
  EXPECT_EQ(gcd1, ntlib::gcd(10, 0));
  EXPECT_EQ(x1 * 10 + y1 * 0, gcd1);

  auto [gcd2, x2, y2] = ntlib::extended_euclid(0, 10);
  EXPECT_EQ(gcd2, ntlib::gcd(0, 10));
  EXPECT_EQ(x2 * 0 + y2 * 10, gcd2);
}

TEST(ExtendedEuclid, NegativeValues) {
  auto [gcd1, x1, y1] = ntlib::extended_euclid(-7, 3);
  EXPECT_EQ(gcd1, ntlib::gcd(-7, 3));
  EXPECT_EQ(x1 * (-7) + 3 * y1, gcd1);

  auto [gcd2, x2, y2] = ntlib::extended_euclid(7, -3);
  EXPECT_EQ(gcd2, ntlib::gcd(7, -3));
  EXPECT_EQ(x2 * 7 + (-3) * y2, gcd2);

  auto [gcd3, x3, y3] = ntlib::extended_euclid(-7, -3);
  EXPECT_EQ(gcd3, ntlib::gcd(-7, -3));
  EXPECT_EQ(x3 * (-7) + (-3) * y3, gcd3);
}

TEST(ExtendedEuclid, CornerCases) {
  // 2^31-1 is prime.
  auto [gcd1, x1, y1] = ntlib::extended_euclid(max_int, 2);
  EXPECT_EQ(gcd1, ntlib::gcd(max_int, 2));
  EXPECT_EQ(x1 * max_int + y1 * 2, gcd1);

  auto [gcd2, x2, y2] = ntlib::extended_euclid(-max_int, 2);
  EXPECT_EQ(gcd2, ntlib::gcd(-max_int, 2));
  EXPECT_EQ(x2 * -max_int + y2 * 2, gcd2);
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
    int sign = e & 1 ? -1 : 1;
    EXPECT_EQ(ntlib::pow(-2, e), sign * (1 << e));
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
  EXPECT_EQ(ntlib::ilog2(max_int128), 126);
  EXPECT_EQ(ntlib::ilog2(max_uint128), 127);
}

TEST(IntegerSquareRoot, Integral) {
  for (uint32_t i = 0; i <= 1'000'000; ++i) {
    uint32_t root = ntlib::isqrt(i);
    EXPECT_LE(root * root, i);
    EXPECT_GT((root + 1) * (root + 1), i);
  }
}

TEST(IntegerSquareRoot, NoBuiltin) {
  for (__uint128_t i = 0; i <= 1'000'000; ++i) {
    __uint128_t root = ntlib::isqrt(i);
    EXPECT_LE(root * root, i);
    EXPECT_GT((root + 1) * (root + 1), i);
  }
}

TEST(IntegerSquareRoot, CornerCases) {
  EXPECT_EQ(ntlib::isqrt(max_int), 46'340);
  EXPECT_EQ(ntlib::isqrt(max_uint), 65'535);

  EXPECT_EQ(ntlib::isqrt(max_int64), 3'037'000'499LL);
  EXPECT_EQ(ntlib::isqrt(max_uint64), 4'294'967'295LL);

  __int128_t iroot128_ntlib = ntlib::isqrt(max_int128);
  __int128_t iroot128_expected =
      __int128_t(2) * 2 * 3 * 3 * 3 * 991 * 283'183 * 430'368'163;
  EXPECT_EQ(iroot128_ntlib, iroot128_expected);

  __uint128_t uroot128_ntlib = ntlib::isqrt(max_uint128);
  __uint128_t uroot128_expected =
      __uint128_t(3) * 5 * 17 * 257 * 641 * 65537 * 6'700'417;
  EXPECT_EQ(uroot128_ntlib, uroot128_expected);
}

TEST(SquareTest, NegativeValues) {
  EXPECT_FALSE(ntlib::is_square(-1));
}

TEST(SquareTest, Squares) {
  for (int32_t i = 0; i * i <= 1'000'000'000; ++i) {
    EXPECT_TRUE(ntlib::is_square(i * i));
  }
}

TEST(SquareTest, NonSquares) {
  std::set<int32_t> squares;
  for (int32_t i = 0; i * i <= 1'000'000; ++i) squares.insert(i * i);
  for (int32_t i = 0; i <= 1'000'000; ++i) {
    if (squares.find(i) != squares.end()) continue;
    EXPECT_FALSE(ntlib::is_square(i));
  }
}

TEST(SquareTest, CornerCases) {
  EXPECT_TRUE(ntlib::is_square(0));
  EXPECT_FALSE(ntlib::is_square(max_int));
  EXPECT_FALSE(ntlib::is_square(max_uint));
  EXPECT_TRUE(ntlib::is_square(1 << 30));
}

TEST(Factorial, First20) {
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
  EXPECT_EQ(ntlib::factorial(11), 39'916'800);
  EXPECT_EQ(ntlib::factorial(12), 479'001'600);
  EXPECT_EQ(ntlib::factorial(13LL), 6'227'020'800LL);
  EXPECT_EQ(ntlib::factorial(14LL), 87'178'291'200LL);
  EXPECT_EQ(ntlib::factorial(15LL), 1'307'674'368'000LL);
  EXPECT_EQ(ntlib::factorial(16LL), 20'922'789'888'000LL);
  EXPECT_EQ(ntlib::factorial(17LL), 355'687'428'096'000LL);
  EXPECT_EQ(ntlib::factorial(18LL), 6'402'373'705'728'000LL);
  EXPECT_EQ(ntlib::factorial(19LL), 121'645'100'408'832'000LL);
  EXPECT_EQ(ntlib::factorial(20LL), 2'432'902'008'176'640'000LL);
}

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
  EXPECT_EQ(ntlib::mod_pow(2, 0, 3), 1);
  EXPECT_EQ(ntlib::mod_pow(2, 1, 3), 2);
  EXPECT_EQ(ntlib::mod_pow(4, 1, 3), 1);
  EXPECT_EQ(ntlib::mod_pow(0, 1, 3), 0);
  EXPECT_EQ(ntlib::mod_pow(-1, 1, 3), 2);
  EXPECT_EQ(ntlib::mod_pow(-2, 1, 3), 1);
}

TEST(ModularExponentiation, PowersOf2) {
  for (int32_t i = 1; i < 30; ++i) {
    EXPECT_EQ(ntlib::mod_pow(2, i, 2), 0);
    EXPECT_EQ(ntlib::mod_pow(2, i, 3), (1 << i) % 3);
  }
}

TEST(ModularExponentiation, PowersOfMinus2) {
  for (int32_t i = 1; i < 30; ++i) {
    EXPECT_EQ(ntlib::mod_pow(-2, i, 2), 0) << "i = " << i;
    EXPECT_EQ(ntlib::mod_pow(-2, i, 3), 1) << "i = " << i;
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
      EXPECT_EQ(ntlib::mod_pow(i, j, p), correct);
    }
  }
}

TEST(MultiplicativeInverse, SmallValues) {
  const uint32_t m = 50;
  for (uint32_t n = 1; n < m; ++n) {
    if (ntlib::gcd(n, m) == 1) {
      uint32_t inv = ntlib::mod_mult_inv(n, m);
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

TEST(LegendreSymbol, Prime3) {
  const auto l = std::to_array<int32_t>({2, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1,
      0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0});
  for (uint32_t i = 1; i < l.size(); ++i) {
    EXPECT_EQ(ntlib::legendre(i, 3u), l[i]);
  }
}

TEST(LegendreSymbol, Prime127) {
  const auto l = std::to_array<int32_t>({2, 1, 1, -1, 1, -1, -1, -1, 1, 1, -1,
      1, -1, 1, -1, 1, 1, 1, 1, 1, -1, 1, 1, -1, -1, 1, 1, -1, -1, -1, 1});
  for (uint32_t i = 1; i < l.size(); ++i) {
    EXPECT_EQ(ntlib::legendre(i, 127u), l[i]);
  }
}

TEST(LegendreSymbol, PeriodicInTopElement) {
  for (uint32_t p : ntlib::SMALL_PRIMES) {
    if (p == 2) continue;
    for (uint32_t i = 0; i <= 1'000; ++i) {
      EXPECT_EQ(ntlib::legendre(i, p), ntlib::legendre(i + p, p));
    }
  }
}

TEST(LegendreSymbol, Multiplicative) {
  for (uint32_t p : ntlib::SMALL_PRIMES) {
    if (p == 2) continue;
    for (uint32_t i = 0; i <= 100; ++i) {
      for (uint32_t j = 0; j <= 100; ++j) {
        EXPECT_EQ(ntlib::legendre(i, p) * ntlib::legendre(j, p),
            ntlib::legendre(i * j, p));
      }
    }
  }
}

TEST(LegendreSymbol, Squares) {
  for (uint32_t p : ntlib::SMALL_PRIMES) {
    if (p == 2) continue;
    for (uint32_t i = 0; i <= 100; ++i) {
      int32_t result = i % p ? 1 : 0;
      EXPECT_EQ(ntlib::legendre(i * i, p), result);
    }
  }
}

TEST(LegendreSymbol, MinusOne) {
  for (uint32_t p : ntlib::SMALL_PRIMES) {
    if (p == 2) continue;
    int32_t result = (p % 4 == 1) ? 1 : -1;
    EXPECT_EQ(ntlib::legendre(-1, static_cast<int32_t>(p)), result);
  }
}

TEST(LegendreSymbol, Two) {
  for (int32_t p : ntlib::SMALL_PRIMES) {
    if (p == 2) continue;
    int32_t result = (p % 8 == 1 || p % 8 == 7) ? 1 : -1;
    EXPECT_EQ(ntlib::legendre(2, p), result);
  }
}

// TEST(JacobiSymbol, EmptyProduct) {
//   for (int32_t a = 0; a <= 10; ++a) {
//     EXPECT_EQ(ntlib::jacobi(a, 1), 1);
//   }
// }

// TEST(JacobiSymbol, PrimeDenominator) {
//   for (int32_t n : ntlib::SMALL_PRIMES) {
//     if (n == 2) continue;
//     for (int32_t k = 0; k <= 1'000; ++k) {
//       EXPECT_EQ(ntlib::jacobi(k, n), ntlib::legendre(k, n));
//     }
//   }
// }

// TEST(JacobiSymbol, PrimeDecompositoin) {
//   for (int32_t n = 1; n <= 1'000; n += 2) {
//     std::map<int32_t, int32_t> factors;
//     ntlib::prime_decomposition(n, factors);
//     for (int32_t k = 0; k <= 1'000; ++k) {
//       int32_t prod = 1;
//       for (auto [f, m] : factors) {
//         while (m--) prod *= ntlib::legendre(k, f);
//       }
//       EXPECT_EQ(ntlib::jacobi(k, n), prod);
//     }
//   }
// }

// TEST(JacobiSymbol, SpecialValues1) {
//   auto j = ntlib::jacobi(-11, 35);
//   EXPECT_EQ(j, -1);
// }
