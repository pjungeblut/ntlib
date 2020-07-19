#pragma once

/**
 * Algorithms to solve diophantine equations.
 * Preconditions are stated in the comments and must be checked by the caller.
 */

#include <cassert>

#include "base.hpp"

namespace ntlib {

/**
 * Computes an integer solution in x to the diophantine equation ax = b.
 * A solution exists if and only if b is a multiple of a.
 * For a = b = 0 each x \in \mathbb{Z} is a solution and x = 0 is returned.
 * In all other cases the solution is unique.
 *
 * @param a Parameter a.
 * @param b Parameter b, must be a multiple of a.
 * @param x Output parameter for value x.
 * return Whether a solution was found.
 */
template<typename T>
bool diophantine_linear_univariate(const T &a, const T &b, T &x) {
  // If a = 0 then we must also have b = 0.
  // In this case, there are inifinitely many solutions and we return 0.
  if (a == 0) {
    if (b == 0) {
      x = 0;
      return true;
    }
    return false;
  }

  // There is a unique solution if and only if b is a multiple of a.
  if (b % a) return false;
  x = b / a;
  return true;
}

/**
 * Computes an integer solution (x,y) to the diophantine equation ax + by = c.
 * A solution exists if and only if c is a multiple of gcd(a,b).
 * If a solution exists, there are infinitely many of them.
 *
 * @param a Parameter a.
 * @param b Parameter b.
 * @param c Parameter c, must be a multiple of gcd(a,b).
 * @param x Output parameter for value x.
 * @param y Output parameter for value y.
 * @param gcd Output parameter for value gcd(a,b). This is needed to compute
 *            further solutions vio Beyout's Identity. In case a = b = 0 this
 *            will not be set.
 * @return Whether a solution was found.
 */
template<typename T>
bool diophantine_linear_bivariate(const T &a, const T &b, const T &c, T &x, T &y, T &gcd) {
  // Special case: a = 0 and b = 0.
  if (a == 0 && b == 0) {
    if (c == 0) {
      x = 0, y = 0;
      return true;
    }
    return false;
  }

  // Special case: a = 0.
  if (a == 0) {
    gcd = b;
    return diophantine_linear_univariate(b, c, y);
  }

  // Special case: b = 0.
  if (b == 0) {
    gcd = a;
    return diophantine_linear_univariate(a, c, x);
  }

  // General case.
  gcd = extended_euclid(abs(a), abs(b), x, y);
  if (c % gcd != 0) return false;
  x *= c / gcd;
  y *= c / gcd;
  if (a < 0) x = -x;
  if (b < 0) y = -y;
  return true;
}

/**
 * Computes a fundamental solution (x,y) to the diophantine equation
 * x^2 - d * y^2 = n where d is not a perfect square with gcd(x,y) = 1.
 *
 * The algorithm is taken from:
 * http://www.numbertheory.org/pdfs/talk_2004.pdf
 *
 * @param d Parameter d, must not be a perfect square.
 * @param n Parameter n.
 * @param x Output parameter for value x.
 * @param y Output parameter for value y.
 * @return Whether a solution was found.
 */
template<typename T>
bool diophantine_quadratic_generalized_pell(
    const T &d, const T &n, T &x, T &y) {
  if (n * n < d) {
    // In case abs(N) < sqrt(D), then x/y is a convergent of sqrt(D).
  } else {

  }
  return false;
}

}
