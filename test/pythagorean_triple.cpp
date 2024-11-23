#include <gtest/gtest.h>

#include <cstdint>

import base;
import pythagorean_triple;

TEST(PythagoreanTriple, Primitive) {
  const uint32_t N = 1'000;
  const auto triples = ntlib::primitive_pythagorean_triples(N);

  for (const auto [a,b,c] : triples) {
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
