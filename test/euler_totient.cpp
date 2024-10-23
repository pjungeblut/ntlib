#include <gtest/gtest.h>

#include <array>
#include <cstdint>

import euler_totient;
import prime_decomposition;

// https://oeis.org/A000010
// Prefixed with `0` to be `1`-indexed.
constexpr auto truth = std::to_array<uint32_t>({0, 1, 1, 2, 2, 4, 2, 6, 4, 6, 4,
    10, 4, 12, 6, 8, 8, 16, 6, 18, 8, 12, 10, 22, 8, 20, 12, 18, 12, 28, 8, 30,
    16, 20, 16, 24, 12, 36, 18, 24, 16, 40, 12, 42, 20, 24, 22, 46, 16, 42, 20,
    32, 24, 52, 18, 40, 24, 36, 28, 58, 16, 60, 30, 36, 32, 48, 20, 66, 32, 44
});

TEST(OneShot, SmallValues) {
  for (uint32_t i = 1; i < truth.size(); ++i) {
    const auto pd = ntlib::prime_decomposition(i);
    EXPECT_EQ(ntlib::euler_totient(pd), truth[i]);
  }
}

TEST(Sieve, SmallValues) {
  const std::size_t N = truth.size() - 1;
  const auto sieve = ntlib::euler_totient_sieve(N);
  EXPECT_EQ(sieve.size(), truth.size());
  for (std::size_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(sieve[i], truth[i]);
  }
}
