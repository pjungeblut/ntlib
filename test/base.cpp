#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <limits>
#include <map>
#include <set>
#include <vector>

#include "base.hpp"
#include "int128.hpp"
#include "prime_generation.hpp"

static constexpr int min_int = std::numeric_limits<int>::min();
static constexpr int max_int = std::numeric_limits<int>::max();
static constexpr unsigned int max_uint =
    std::numeric_limits<unsigned int>::max();
static constexpr int64_t max_int64 = std::numeric_limits<int64_t>::max();
static constexpr uint64_t max_uint64 = std::numeric_limits<uint64_t>::max();
static constexpr i128 max_int128 = (i128(1) << 126) - 1 + (i128(1) << 126);
static constexpr u128 max_uint128 = (u128(1) << 127) - 1 + (u128(1) << 127);

TEST(SmallPrimes, ListContainsOnlyPrimes) {
  const auto sieve = ntlib::prime_sieve(ntlib::SMALL_PRIMES_BIGGEST);
  for (uint32_t p : ntlib::SMALL_PRIMES) {
    EXPECT_TRUE(sieve[p]);
  }
}

TEST(SmallPrimes, ListComplete) {
  std::vector<uint32_t> primes;
  ntlib::prime_sieve(ntlib::SMALL_PRIMES_BIGGEST, primes);
  // The `prime_sieve` method might return a larger sieve than requested.
  // In this case, we remove the additional primes in `primes`.
  while (primes.back() > ntlib::SMALL_PRIMES_BIGGEST) {
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

TEST(OddPart, SmallValues) {
  EXPECT_EQ(ntlib::odd_part(-3), std::make_pair(0, -3));
  EXPECT_EQ(ntlib::odd_part(-2), std::make_pair(1, -1));
  EXPECT_EQ(ntlib::odd_part(-1), std::make_pair(0, -1));
  EXPECT_EQ(ntlib::odd_part(0), std::make_pair(0, 0));
  EXPECT_EQ(ntlib::odd_part(1), std::make_pair(0, 1));
  EXPECT_EQ(ntlib::odd_part(2), std::make_pair(1, 1));
  EXPECT_EQ(ntlib::odd_part(3), std::make_pair(0, 3));
}

TEST(OddPart, PowersOfTwo) {
  for (uint32_t i = 0; i < 32; ++i) {
    EXPECT_EQ(ntlib::odd_part(1u << i), std::make_pair(i, 1u));
  }
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
  for (u128 i = 1; i < 1'000'000; ++i) {
    u128 cl2 = ntlib::ilog2(i);
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
  for (u128 i = 0; i <= 1'000'000; ++i) {
    u128 root = ntlib::isqrt(i);
    EXPECT_LE(root * root, i);
    EXPECT_GT((root + 1) * (root + 1), i);
  }
}

TEST(IntegerSquareRoot, CornerCases) {
  EXPECT_EQ(ntlib::isqrt(max_int), 46'340);
  EXPECT_EQ(ntlib::isqrt(max_uint), 65'535);

  EXPECT_EQ(ntlib::isqrt(max_int64), 3'037'000'499LL);
  EXPECT_EQ(ntlib::isqrt(max_uint64), 4'294'967'295LL);

  i128 iroot128_ntlib = ntlib::isqrt(max_int128);
  i128 iroot128_expected =
      i128(2) * 2 * 3 * 3 * 3 * 991 * 283'183 * 430'368'163;
  EXPECT_EQ(iroot128_ntlib, iroot128_expected);

  u128 uroot128_ntlib = ntlib::isqrt(max_uint128);
  u128 uroot128_expected =
      u128(3) * 5 * 17 * 257 * 641 * 65537 * 6'700'417;
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
