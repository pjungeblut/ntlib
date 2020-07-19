#pragma once

/**
 * Contininued fraction expansion for quadratic irrationals.
 */

#include <cmath>
#include <vector>

#include "rational.hpp"

namespace ntlib {

/**
 * Computes the continued fraction representation of sqrt(n) for a natural
 * number n that is not a perfect square.
 *
 * For these (and only these) numbers the continued fraction is regular and
 * periodic.
 *
 * @param n The number n, to get the representation for sqrt(n). Must not be a
 *          perfect square.
 * @param The regular, periodic continued fraction representing sqrt(n). The
 *        first position gives the whole number part, the second until the last
 *        one form the period.
 * @return The period length.
 */
template<typename T>
T quadratic_irrational_cf(const T &n, std::vector<T> &cf) {
  const T m0 = 0;
  const T d0 = 1;
  cf.assign(1, isqrt(n));

  T m = d0 * cf[0] - m0, m1 = m;
  T d = (n - m * m) / d0, d1 = d;
  T c = (cf[0] + m) / d, c1 = c;
  cf.push_back(c);

  for (T i = 2; ; ++i) {
    m = d * cf.back() - m;
    d = (n - m * m) / d;
    c = (cf[0] + m) / d;

    if (i > 1 && m == m1 && d == d1 && c == c1) return i - 1;
    cf.push_back(c);
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
template<typename T>
rational<T> nth_convergent_quadratic_irrational_cf(T n,
    const std::vector<T> &cf) {
  T p[3];
  T q[3];

  p[0] = cf[0];
  p[1] = cf[0] * cf[1] + 1;
  q[0] = 1;
  q[1] = cf[1];
  for (T i = 2; i <= n; ++i) {
    T cf_idx = i >= cf.size() ? (i - 1) % (cf.size() - 1) + 1: i;
    p[(std::size_t)(i % 3)] =
        cf[(std::size_t)cf_idx] * p[(std::size_t)((i + 2) % 3)] +
        p[(std::size_t)((i + 1) % 3)];
    q[(std::size_t)(i % 3)] =
        cf[(std::size_t)cf_idx] * q[(std::size_t)((i + 2) % 3)] +
        q[(std::size_t)((i + 1) % 3)];
  }
  return rational<T> {p[(std::size_t)(n % 3)], q[(std::size_t)(n % 3)]};
}

}
