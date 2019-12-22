#pragma once

/**
 * Implementations regarding the continued fraction expressions of certain
 * numbers.
 */

#include <cmath>
#include <vector>

#include "integral.hpp"
#include "rational.hpp"

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
 * TODO: Change this to work in constant memory. There is no need to use vectors
 *       as only the very first and last elements are used in each step.
 *
 * @param n The number n, to get the representation for sqrt(n). Must not be a
 *          perfect square.
 * @param The regular, periodic continued fraction representing sqrt(n). The
 *        first position gives the whole number part, the second until the last
 *        one form the period.
 * @return The period length.
 */
template<Integral I>
I quadratic_irrational_cf(I n, std::vector<I> &cf) {
  std::vector<I> m(1, 0);
  std::vector<I> d(1, 1);
  cf.push_back(isqrt(n));

  for (I i = 1; ; ++i) {
    I m2 = d.back() * cf.back() - m.back();
    I d2 = (n - m2 * m2) / d.back();
    I cf2 = (cf[0] + m2) / d2;

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
template<Integral I>
rational<I> nth_convergent_quadratic_irrational_cf(I n,
    const std::vector<I> &cf) {
  I p[3];
  I q[3];

  p[0] = cf[0];
  p[1] = cf[0] * cf[1] + 1;
  q[0] = 1;
  q[1] = cf[1];
  for (I i = 2; i <= n; ++i) {
    I cf_idx = i >= cf.size() ? (i - 1) % (cf.size() - 1) + 1: i;
    p[(std::size_t)(i % 3)] =
        cf[(std::size_t)cf_idx] * p[(std::size_t)((i + 2) % 3)] +
        p[(std::size_t)((i + 1) % 3)];
    q[(std::size_t)(i % 3)] =
        cf[(std::size_t)cf_idx] * q[(std::size_t)((i + 2) % 3)] +
        q[(std::size_t)((i + 1) % 3)];
  }
  return rational<I> {p[(std::size_t)(n % 3)], q[(std::size_t)(n % 3)]};
}

}
