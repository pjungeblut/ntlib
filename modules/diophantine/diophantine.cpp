/**
 * @file
 * @brief Primary module interface unit for module `diophantine`.
 */
module;

#include <cassert>
#include <tuple>

/**
 * @module diophantine
 * @brief Algorithms to solve diophantine equations.
 */
export module diophantine;

import base;

namespace ntlib {

/**
 * @brief Compute integer solutions to linear univariate diophantine equations.
 * 
 * Computes an integer solution in \f$x\f$ to the diophantine equation
 * \f$ax = b\f$.
 * A solution exists if and only if \f$b\f$ is a multiple of \f$a\f$.
 * For \f$a = b = 0\f$ each \f$x \in \mathbb{Z}\f$ is a solution and \f$x = 0\f$
 * is returned. In all other cases the solution is unique.
 *
 * @tparam T An integer-like type.
 * @param a The coefficient of \f$x\f$.
 * @param b The absolute offset. Must be a multiple of \f$a\f$.
 * @return The solution in \f$x\f$.
 */
export template<Integer T>
[[nodiscard]] constexpr
T diophantine_linear_univariate(T a, T b) noexcept {
  // If a = 0 then we must also have b = 0.
  // In this case, there are inifinitely many solutions and we return 0.
  assert(a != 0 || b == 0);
  if (a == 0) { return 0; }

  // There is a unique solution if and only if b is a multiple of a.
  assert(b % a == 0);
  return b / a;
}

/**
 * @brief Computes integer solutions to linear bivariate diophantine equations.
 * 
 * Computes an integer solution \f$(x,y)\f$ to the diophantine equation
 * \f$ax + by = c\f$.
 * A solution exists if and only if \f$c\f$ is a multiple of
 * \f$\mathrm{gcd}(a,b)\f$. In this case, there are infinitely many solutions.
 *
 * @tparam T An integer-like type.
 * @param a The coefficient of \f$x\f$.
 * @param b The coefficient of \f$y\f$.
 * @param c The absolute offset. Must be a multiple of \f$\mathrm{gcd}(a,b)\f$.
 * @return A triple with \f$x\f$, \f$y\f$ and \f$\mathrm{gcd}(a,b)\f$ where the
 *     latter can be used to compute other solutions using via Bezout's
 *     Identity. In case \f$a = b = 0\f$, the third element is \f$0\f$.
 */
export template<Integer T>
[[nodiscard]] constexpr
std::tuple<T,T,T> diophantine_linear_bivariate(T a, T b, T c) noexcept {
  // Special case: a = 0 and b = 0.
  assert(a != 0 || b != 0 || c == 0);
  if (a == 0 && b == 0) { return std::make_tuple(0, 0, 0); }

  // Special case: a = 0.
  assert(a != 0 || c % b == 0);
  if (a == 0) { return std::make_tuple(0, c / b, b); }

  // Special case: b = 0.
  assert(b != 0 || c % a == 0);
  if (b == 0) { return std::make_tuple(c / a, 0, a); }

  // General case.
  auto [gcd, x, y] = ntlib::extended_euclid(ntlib::abs(a), ntlib::abs(b));
  assert(c % gcd == 0);
  x *= c / gcd;
  y *= c / gcd;
  if (a < 0) x = -x;
  if (b < 0) y = -y;
  return std::make_tuple(x, y, gcd);
}

} // namespace ntlib
