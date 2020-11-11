#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "prime_generation.hpp"
#include "prime_test.hpp"

TEST(Naive, SmallValues) {
  uint32_t N = 1'000'000;
  auto sieve = ntlib::prime_sieve(N);
  for (uint32_t n = 0; n <= N; ++n) {
    EXPECT_EQ(ntlib::is_prime_naive(n), sieve[n]);
  }
}

TEST(Naive, NegativeValues) {
  EXPECT_FALSE(ntlib::is_prime_naive(-1));
}

TEST(MillerRabin, SmallValues) {
  uint64_t N = 1'000'000;
  auto sieve = ntlib::prime_sieve(N);
  for (uint64_t n = 0; n <= N; ++n) {
    EXPECT_EQ(ntlib::is_prime_miller_rabin(n), sieve[n]);
  }
}

TEST(MillerRabin, NegativeValues) {
  EXPECT_FALSE(ntlib::is_prime_miller_rabin(-1));
}

TEST(MillerRabin, LargeComposites) {
  __uint128_t n1 = 1'000'000'007LL * 1'000'000'007LL;
  __uint128_t n2 = 4'120'038'565'055'551LL;
  EXPECT_FALSE(ntlib::is_prime_miller_rabin(n1));
  EXPECT_FALSE(ntlib::is_prime_miller_rabin(n2));
}

TEST(MillerRabin, LargePrimes) {
  __uint128_t n1 = 952'016'363'681'739'749LL;
  __uint128_t n2 = 301'697'296'732'166'057LL;
  EXPECT_TRUE(ntlib::is_prime_miller_rabin(n1));
  EXPECT_TRUE(ntlib::is_prime_miller_rabin(n2));
}

TEST(MillerRabin, Base2StrongLiars) {
  // From: https://oeis.org/A001262
  uint64_t liars[] = {2047, 3277, 4033, 4681, 8321, 15841, 29341, 42799, 49141,
      52633, 65281, 74665, 80581, 85489, 88357, 90751, 104653, 130561, 196093,
      220729, 233017, 252601, 253241, 256999, 271951, 280601, 314821, 357761,
      390937, 458989, 476971, 486737};
  for (uint64_t l : liars) {
    EXPECT_FALSE(ntlib::is_prime(l));
    EXPECT_TRUE(ntlib::miller_rabin_test(l, 2uL));
  }
}

TEST(MillerRabin, Base3StrongLiars) {
  // From: https://oeis.org/A020229
  uint64_t liars[] = {121, 703, 1891, 3281, 8401, 8911, 10585, 12403, 16531,
      18721, 19345, 23521, 31621, 44287, 47197, 55969, 63139, 74593, 79003,
      82513, 87913, 88573, 97567, 105163, 111361, 112141, 148417, 152551,
      182527, 188191, 211411, 218791, 221761, 226801};
  for (uint64_t l : liars) {
    EXPECT_FALSE(ntlib::is_prime(l));
    EXPECT_TRUE(ntlib::miller_rabin_test(l, 3uL));
  }
}

TEST(MillerRabin, Base5StrongLiars) {
  // From: https://oeis.org/A020231
  uint64_t liars[] = {781, 1541, 5461, 5611, 7813, 13021, 14981, 15751, 24211,
      25351, 29539, 38081, 40501, 44801, 53971, 79381, 100651, 102311, 104721,
      112141, 121463, 133141, 141361, 146611, 195313, 211951, 216457, 222301,
      251521, 289081, 290629, 298271, 315121};
  for (uint64_t l : liars) {
    EXPECT_FALSE(ntlib::is_prime(l));
    EXPECT_TRUE(ntlib::miller_rabin_test(l, 5uL));
  }
}

TEST(MillerRabin, Base7StrongLiars) {
  // From: https://oeis.org/A020233
  uint64_t liars[] = {25, 325, 703, 2101, 2353, 4525, 11041, 14089, 20197,
      29857, 29891, 39331, 49241, 58825, 64681, 76627, 78937, 79381, 87673,
      88399, 88831, 102943, 109061, 137257, 144901, 149171, 173951, 178709,
      188191, 197633, 219781, 227767, 231793, 245281};
  for (uint64_t l : liars) {
    EXPECT_FALSE(ntlib::is_prime(l));
    EXPECT_TRUE(ntlib::miller_rabin_test(l, 7uL));
  }
}

TEST(Combined, First10Million) {
  uint64_t N = 10'000'000;
  auto sieve = ntlib::prime_sieve(N);
  for (uint64_t n = 0; n <= N; ++n) {
    EXPECT_EQ(ntlib::is_prime(n), sieve[n]);
  }
}
