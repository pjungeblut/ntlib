#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "../benchmarks/experiments/prime_generation.hpp"
#include "prime_generation.hpp"
#include "prime_test.hpp"

const uint64_t N = 10'000;
const uint64_t M = 10'000'000;

TEST(Experiments, VariantsMatch) {
  auto textbook = ntlib::experiments::eratosthenes_textbook(M);
  auto jumps = ntlib::experiments::eratosthenes_jumps(M);
  for (std::size_t i = 0; i <= M; ++i) EXPECT_EQ(textbook[i], jumps[i]);

  auto segmented = ntlib::prime_sieve(M);
  for (std::size_t i = 0; i <= M; ++i) EXPECT_EQ(textbook[i], segmented[i]);
}
