/**
 * Algorithms to solve diophantine equations.
 * Preconditions are stated in the comments and must be checked by the caller.
 */

module;

#include <cassert>
#include <tuple>

import base;

export module diophantine;

namespace ntlib {

/**
 * Computes an integer solution in x to the diophantine equation ax = b.
 * A solution exists if and only if b is a multiple of a.
 * For a = b = 0 each x \in \mathbb{Z} is a solution and x = 0 is returned.
 * In all other cases the solution is unique.
 *
 * @param a Parameter a.
 * @param b Parameter b, must be a multiple of a.
 * @return The solution in x.
 */
export template<typename T>
T diophantine_linear_univariate(const T &a, const T &b) {
  // If a = 0 then we must also have b = 0.
  // In this case, there are inifinitely many solutions and we return 0.
  assert(a != 0 || b == 0);
  if (a == 0) return 0;

  // There is a unique solution if and only if b is a multiple of a.
  assert(b % a == 0);
  return b / a;
}

/**
 * Computes an integer solution (x,y) to the diophantine equation ax + by = c.
 * A solution exists if and only if c is a multiple of gcd(a,b).
 * If a solution exists, there are infinitely many of them.
 *
 * @param a Parameter a.
 * @param b Parameter b.
 * @param c Parameter c, must be a multiple of gcd(a,b).
 * @return A triple with x, y and gcd(a,b) where the latter is needed to compute
 *         further solution via Bezout's Identity. In case a = b = 0, the third
 *         element is 0.
 */
export template<typename T>
std::tuple<T,T,T> diophantine_linear_bivariate(T a, T b, T c) {
  // Special case: a = 0 and b = 0.
  assert(a != 0 || b != 0 || c == 0);
  if (a == 0 && b == 0) return std::make_tuple(0, 0, 0);

  // Special case: a = 0.
  assert(a != 0 || c % b == 0);
  if (a == 0) return std::make_tuple(0, c / b, b);

  // Special case: b = 0.
  assert(b != 0 || c % a == 0);
  if (b == 0) return std::make_tuple(c / a, 0, a);

  // General case.
  auto [gcd, x, y] = extended_euclid(abs(a), abs(b));
  assert(c % gcd == 0);
  x *= c / gcd;
  y *= c / gcd;
  if (a < 0) x = -x;
  if (b < 0) y = -y;
  return std::make_tuple(x, y, gcd);
}

}