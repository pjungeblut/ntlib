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
template<Integral I>
struct rational {
  I numerator;
  I denominator;

  /**
   * Construct rational from a numerator and a denominator.
   *
   * @param n The numerator.
   * @param d The denominator. Defaults to 1.
   */
  rational(I n, I d = 1) {
    I f = gcd(n, d);
    numerator = n / f;
    denominator = d / f;
  }
};

}
