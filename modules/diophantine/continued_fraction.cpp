/**
 * @file
 * @brief Primary module interface unit for module `continued_fraction`.
 */
module;

#include <cmath>
#include <vector>

/**
 * @module continued_fraction
 * @brief Contininued fraction expansion for quadratic irrationals.
 */
export module continued_fraction;

import base;
import rational;

namespace ntlib {

/**
 * @brief Continued fraction representation for a quadratic irrational.
 * 
 * Let \f$n\f$ be a non-square natural number. This function template computes
 * the continued fraction representation of \f$\sqrt{n}\f$.
 *
 * For non-squares (and only these) the continued fraction is regular and
 * periodic.
 *
 * @tparam T An integer-like type.
 * @param n The number n. Must not be a perfect square.
 * @return The regular, periodic continued fraction representing sqrt(n). The
 *     first position gives the whole number part, the second until the last one
 *     form the period.
 */
export template<Integer T>
std::vector<T> quadratic_irrational_cf(const T &n) {
  const T m0{0};
  const T d0{1};
  std::vector<T> cf(T{1}, isqrt(n));

  T m = d0 * cf[0] - m0, m1 = m;
  T d = (n - m * m) / d0, d1 = d;
  T c = (cf[0] + m) / d, c1 = c;
  cf.push_back(c);

  for (std::size_t i = 2; ; ++i) {
    m = d * cf.back() - m;
    d = (n - m * m) / d;
    c = (cf[0] + m) / d;

    if (i > 1 && m == m1 && d == d1 && c == c1) { return cf; }
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
export template<Integer T>
rational<T> nth_convergent_quadratic_irrational_cf(std::size_t n,
    const std::vector<T> &cf) {
  T p[3];
  T q[3];

  p[0] = cf[0];
  p[1] = cf[0] * cf[1] + 1;
  q[0] = 1;
  q[1] = cf[1];
  for (std::size_t i = 2; i <= n; ++i) {
    std::size_t cf_idx = i >= cf.size() ? (i - 1) % (cf.size() - 1) + 1: i;
    p[i % 3] = cf[cf_idx] * p[(i + 2) % 3] + p[(i + 1) % 3];
    q[i % 3] = cf[cf_idx] * q[(i + 2) % 3] + q[(i + 1) % 3];
  }
  return rational<T> {p[n % 3], q[n % 3]};
}

}