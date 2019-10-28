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
template<Integral T>
struct rational {
  T numerator;
  T denominator;

  /**
   * Construct rational from a numerator and a denominator.
   *
   * @param n The numerator.
   * @param d The denominator. Defaults to 1.
   */
  rational(T n, T d = 1) {
    T f = gcd(n, d);
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
template<Integral T>
rational<T> operator+(const rational<T> &a, const rational<T> &b) {
  T main_denominator = lcm(a.denominator, b.denominator);
  T fa = main_denominator / a.denominator;
  T fb = main_denominator / b.denominator;
  return rational<T> {fa * a.numerator + fb * b.numerator, main_denominator};
}

}
