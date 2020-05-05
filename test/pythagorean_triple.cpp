#include <gtest/gtest.h>

#include <cstdint>
#include <tuple>
#include <vector>

#include "pythagorean_triple.hpp"

TEST(PythagoreanTriple, Primitive) {
  const uint32_t maxi = 1000;
  std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> triples;
  ntlib::primitive_pythagorean_triples(maxi, triples);

  for (const auto &[a,b,c] : triples) {
    // Check validity.
    EXPECT_EQ(a * a + b * b, c * c);

    // Check order.
    EXPECT_LE(a, b);
    EXPECT_LE(b, c);

    // Check primitivity.
    uint32_t gcd = ntlib::gcd(a, ntlib::gcd(b, c));
    EXPECT_EQ(gcd, 1);
  }
}
