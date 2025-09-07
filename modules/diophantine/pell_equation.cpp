/**
 * @file
 * @brief Primary module interface unit for module `pell_equation`.
 */
module;

#include <cassert>
#include <tuple>
#include <vector>

/**
 * @module pell_equation
 * @brief Finds integer solutions to Pell's equation.
 * 
 * Given a non-square integer \f$D \in \mathbb{N}\f$, finds integers
 * \f$x, y \in \mathbb{Z}\f$ satisfying \f$x^2 - D \cdot y^2 = 1\f$.
 */
export module pell_equation;

import base;
import continued_fraction;
import rational;

namespace ntlib {

/**
 * @brief Compute the fundamental solution.
 * 
 * There are infinitely many integer solutions. The fundamental solution is the
 * one minimal in \f$x\f$ (by absolute value) and therefore also in \f$y\f$.
 *
 * @tparam T An integer-like type.
 * @param D Parameter \f$D \in \mathbb{N}\f$. Must not be a square number.
 * @return Fundamental solution \f$(x,y)\f$.
 */
export template<Integer T>
[[nodiscard]] constexpr
std::tuple<T,T> pell_fundamental_solution(T D) noexcept {
  assert(D > 0);
  assert(!ntlib::is_square(D));
  
  const std::vector<T> cf = ntlib::quadratic_irrational_cf(D);
  const std::size_t period = cf.size() - 1;
  const std::size_t n = ntlib::is_odd(period)
      ? 2 * period - 1
      : period - 1;
  const ntlib::rational<T> pq =
      ntlib::nth_convergent_quadratic_irrational_cf(n, cf);
  return std::make_tuple(pq.get_numerator(), pq.get_denominator());
}

/**
 * @brief Generates the next larger solution to Pell's equation.
 *
 * @tparam T An integer like type.
 * @param D The parameter \f$D\f$.
 * @param initial The fundamental solution, i.e., the one minimal in x by
 *     absolute value.
 * @param current Any solution.
 * @return The next bigger solution than current (in x, by absolute value).
 */
export template<Integer T>
[[nodiscard]] constexpr
std::tuple<T,T> pell_next_solution(
    T D,
    const std::tuple<T,T> &initial,
    const std::tuple<T,T> &current
) noexcept {
  // Given the fundamental solution (x_0, y_0) and any solution (x_i, y_i):
  // x_{i+1} = x_0*x_i + D*y_0*y_i
  // y_{i+1} = y_0*x_i + x_0*y_i
  const auto &[ix, iy] = initial;
  const auto &[cx, cy] = current;
  return std::make_tuple(ix * cx + D * iy * cy, iy * cx + ix * cy);
}

} // namespace ntlib
