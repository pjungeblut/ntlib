#pragma once

/**
 * Implementations to easily work with rational numbers.
 */

#include "include/base.hpp"

namespace ntlib {

/**
 * Represents a rational number.
 */
template<typename NumberType>
struct rational {
  NumberType numerator;
  NumberType denominator;

  /**
   * Construct rational from a numerator and a denominator.
   *
   * @param n The numerator.
   * @param d The denominator. Defaults to 1.
   */
  rational(NumberType n, NumberType d = 1) {
    NumberType f = gcd(n, d);
    numerator = n / f;
    denominator = d / f;
  }
};

/**
 * Allows to add two rational numbers.
 *
 * @param a The first summand.
 * @param b The second summand.
 * @return The sum of a and b, cancelled.
 */
template<typename NumberType>
rational<NumberType> operator+(const rational<NumberType> &a, const rational<NumberType> &b) {
  NumberType main_denominator = lcm(a.denominator, b.denominator);
  NumberType fa = main_denominator / a.denominator;
  NumberType fb = main_denominator / b.denominator;
  return rational<NumberType> {fa * a.numerator + fb * b.numerator, main_denominator};
}

}
