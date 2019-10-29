#pragma once

/**
 * Implementations to easily work with rational numbers.
 */

#include "include/base.hpp"
#include "include/integral.hpp"

namespace ntlib {

/**
 * Represents a rational number.
 */
template<Integral I, UnsignedIntegral U>
struct rational {
  I numerator;
  U denominator;

  /**
   * Construct rational from a numerator and a denominator.
   *
   * @param n The numerator.
   * @param d The denominator. Defaults to 1.
   */
  rational(I n, U d = 1) {
    U f = gcd(n, d);
    numerator = n / f;
    denominator = d / f;
  }
};

}
