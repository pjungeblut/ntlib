#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <vector>

#include "int128.hpp"
#include "prime_generation.hpp"
#include "prime_test.hpp"

static constexpr uint64_t N = 1'000'000;
static const auto SIEVE = ntlib::prime_sieve(N);

// Large composites.
static constexpr auto COMPOSITES = std::to_array<ntlib::u128>({
    1'000'000'007LL * 1'000'000'007LL,
    4'120'038'565'055'551LL,
    2LL * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2,
    2LL * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29 * 31 * 37 * 41 * 43 * 47
});

// Large primes.
static constexpr auto PRIMES = std::to_array<ntlib::u128>({
    1'000'000'007ull,
    952'016'363'681'739'749ull,
    301'697'296'732'166'057ull,
    9'223'372'036'854'775'783ull // = largest prime fitting into `int64_t`
});

TEST(TrialDivision, FirstN) {
  // Make sure to don't go out of the correct range for trial division.
  const uint64_t largest = ntlib::SMALL_PRIMES_BIGGEST;
  auto UPPER_BOUND = std::min(N, largest * largest);
  for (uint64_t i = 0; i <= UPPER_BOUND; ++i) {
    EXPECT_EQ(ntlib::is_prime_trial_division(i, ntlib::SMALL_PRIMES), SIEVE[i])
        << "i = " << i;
  }
}

TEST(TrialDivision, OutOfRangeUnknown) {
  // Find smallest prime not in ntlib::SMALL_PRIMES.
  const uint64_t largest = ntlib::SMALL_PRIMES_BIGGEST;
  const uint64_t prime_too_big = ntlib::next_prime(largest * largest);

  // Prime, too big to tell.
  const auto res1 = ntlib::is_prime_trial_division(
      prime_too_big, ntlib::SMALL_PRIMES);
  EXPECT_FALSE(res1.has_value());

  // Composite, too big to tell.
  const auto res2 = ntlib::is_prime_trial_division(
      prime_too_big * prime_too_big, ntlib::SMALL_PRIMES);
  EXPECT_FALSE(res2.has_value());
}

TEST(TrialDivision, OutOfRangeKnown) {
  const auto res1 = ntlib::is_prime_trial_division(
      2 * ntlib::SMALL_PRIMES_BIGGEST, ntlib::SMALL_PRIMES);
  EXPECT_TRUE(res1.has_value());
  EXPECT_FALSE(res1.value());

  const auto res2 = ntlib::is_prime_trial_division(
      ntlib::SMALL_PRIMES_BIGGEST * ntlib::SMALL_PRIMES_BIGGEST,
      ntlib::SMALL_PRIMES);
  EXPECT_TRUE(res2.has_value());
  EXPECT_FALSE(res2.value());
}

TEST(MillerSelfridgeRabin, Base2StrongLiars) {
  // From: https://oeis.org/A001262
  constexpr auto liars = std::to_array<uint64_t>({2047, 3277, 4033, 4681, 8321,
      15841, 29341, 42799, 49141, 52633, 65281, 74665, 80581, 85489, 88357,
      90751, 104653, 130561, 196093, 220729, 233017, 252601, 253241, 256999,
      271951, 280601, 314821, 357761, 390937, 458989, 476971, 486737});
  for (auto l : liars) {
    EXPECT_FALSE(ntlib::is_prime(l));
    EXPECT_TRUE(ntlib::miller_selfridge_rabin_test(l, 2uL)) << "l = " << l;
  }
}

TEST(MillerSelfridgeRabin, Base3StrongLiars) {
  // From: https://oeis.org/A020229
  constexpr auto liars = std::to_array<uint64_t>({121, 703, 1891, 3281, 8401,
      8911, 10585, 12403, 16531, 18721, 19345, 23521, 31621, 44287, 47197,
      55969, 63139, 74593, 79003, 82513, 87913, 88573, 97567, 105163, 111361,
      112141, 148417, 152551, 182527, 188191, 211411, 218791, 221761, 226801});
  for (auto l : liars) {
    EXPECT_FALSE(ntlib::is_prime(l));
    EXPECT_TRUE(ntlib::miller_selfridge_rabin_test(l, 3uL)) << "l = " << l;
  }
}

TEST(MillerSelfridgeRabin, Base5StrongLiars) {
  // From: https://oeis.org/A020231
  constexpr auto liars = std::to_array<uint64_t>({781, 1541, 5461, 5611, 7813,
      13021, 14981, 15751, 24211, 25351, 29539, 38081, 40501, 44801, 53971,
      79381, 100651, 102311, 104721, 112141, 121463, 133141, 141361, 146611,
      195313, 211951, 216457, 222301, 251521, 289081, 290629, 298271, 315121});
  for (auto l : liars) {
    EXPECT_FALSE(ntlib::is_prime(l));
    EXPECT_TRUE(ntlib::miller_selfridge_rabin_test(l, 5uL)) << "l = " << l;
  }
}

TEST(MillerSelfridgeRabin, Base7StrongLiars) {
  // From: https://oeis.org/A020233
  constexpr auto liars = std::to_array<uint64_t>({25, 325, 703, 2101, 2353,
      4525, 11041, 14089, 20197, 29857, 29891, 39331, 49241, 58825, 64681,
      76627, 78937, 79381, 87673, 88399, 88831, 102943, 109061, 137257, 144901,
      149171, 173951, 178709, 188191, 197633, 219781, 227767, 231793, 245281});
  for (uint64_t l : liars) {
    EXPECT_FALSE(ntlib::is_prime(l));
    EXPECT_TRUE(ntlib::miller_selfridge_rabin_test(l, 7uL));
  }
}

TEST(ForisekJancina, FirstN) {
  // Skip small numbers, as the function does not check base cases.
  for (uint32_t i = 121; i <= N; i += 2) {
    if (i % 2 == 0 || i % 3 == 0 || i % 5 == 0 || i % 7 == 0) { continue; }
    EXPECT_EQ(ntlib::forisek_jancina_no_base_cases(i), SIEVE[i]) << "i = " << i;
  }
}

TEST(StrongLucas, SmallValues) {
  // Strong Lucas Pseudoprimes.
  // https://oeis.org/A217255
  const std::set<uint64_t> pseudo({5459, 5777, 10877, 16109, 18971, 22499,
      24569, 25199, 40309, 58519, 75077, 97439, 100127, 113573, 115639, 130139,
      155819, 158399, 161027, 162133, 176399, 176471, 189419, 192509, 197801,
      224369, 230691, 231703, 243629, 253259, 268349, 288919, 313499, 324899});
  const uint64_t MAX = *std::ranges::max_element(pseudo);
  const uint64_t M = std::min(N, MAX);
  for (uint64_t n = 3; n <= M; n += 2) {
    if (pseudo.find(n) != pseudo.end()) {
      EXPECT_NE(ntlib::is_strong_lucas_probable_prime(n), SIEVE[n]);
    } else {
      EXPECT_EQ(ntlib::is_strong_lucas_probable_prime(n), SIEVE[n]);
    }
  }
}

TEST(BailliePSW, FirstN) {
  for (uint64_t n = 0; n <= N; ++n) {
    EXPECT_EQ(ntlib::is_prime(n), SIEVE[n]);
  }
}

TEST(BailliePSW, NegativeValues) {
  EXPECT_FALSE(ntlib::is_prime(-1));
}

TEST(BailliePSW, LargeComposites) {
  for (auto c : COMPOSITES) {
    EXPECT_FALSE((ntlib::is_prime<ntlib::u128, ntlib::i128>(c)));
  }
}

TEST(BailliePSW, LargePrimes) {
  for (auto p : PRIMES) {
    EXPECT_TRUE((ntlib::is_prime<ntlib::u128, ntlib::i128>(p)));
  }
}
