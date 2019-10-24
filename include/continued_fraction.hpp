#pragma once

/**
 * Implementations regarding the continued fraction expressions of certain
 * numbers.
 */

#include <cmath>
#include <vector>

#include "include/rational.hpp"

namespace ntlib {

/**
 * Computes the continued fraction representation of sqrt(n) for a natural
 * number n that is not a perfect square.
 *
 * For these (and only these) numbers the continued fraction is regular and
 * periodic.
 *
 * Algorithm from:
 * https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Continued_fraction_expansion
 *
 * @param n The number n, to get the representation for sqrt(n). Must not be a
 *          perfect square.
 * @param The regular, periodic continued fraction representing sqrt(n). The
 *        first position gives the whole number part, the second until the last
 *        one form the period.
 * @return The period length.
 */
template<typename NumberType>
NumberType quadratic_irrational_cf(NumberType n, std::vector<NumberType> &cf) {
  std::vector<NumberType> m(1, 0);
  std::vector<NumberType> d(1, 1);
  cf.push_back(isqrt(n));

  for (NumberType i = 1; ; ++i) {
    NumberType m2 = d.back() * cf.back() - m.back();
    NumberType d2 = (n - m2 * m2) / d.back();
    NumberType cf2 = (cf[0] + m2) / d2;

    if (i > 1 && m2 == m[1] && d2 == d[1] && cf2 == cf[1]) return i - 1;

    m.push_back(m2);
    d.push_back(d2);
    cf.push_back(cf2);
  }
}

/**
 * Computes the n-th convergent of the regular, periodic continued fraction for
 * a quadratic irrational.
 *
 * Algorithm from:
 * http://mathworld.wolfram.com/PellEquation.html
 *
 * @param n The n-th convergent is calculated.
 * @param cf The continued fraction representation of the quadratic irrational.
 * @return The numerator and denominator of the n-th convergent.
 */
template<typename NumberType>
rational<NumberType> nth_convergent_quadratic_irrational_cf(std::size_t n,
    const std::vector<NumberType> &cf) {
  NumberType p[3];
  NumberType q[3];

  p[0] = cf[0];
  p[1] = cf[0] * cf[1] + 1;
  q[0] = 1;
  q[1] = cf[1];
  for (NumberType i = 2; i <= n; ++i) {
    NumberType cf_idx = i >= cf.size() ? (i - 1) % (cf.size() - 1) + 1: i;
    p[i % 3] = cf[cf_idx] * p[(i + 2) % 3] + p[(i + 1) % 3];
    q[i % 3] = cf[cf_idx] * q[(i + 2) % 3] + q[(i + 1) % 3];
  }
  return rational<NumberType> {p[n % 3], q[n % 3]};
}

}
