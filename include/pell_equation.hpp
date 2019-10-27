#pragma once

#include <vector>

#include "include/base.hpp"
#include "include/continued_fraction.hpp"

namespace ntlib {

/**
 * Computes the minimal solution (in a) of Pell's equation:
 * a^2 - d * b^2 = 1.
 *
 * @param d Parameter d, must not be square.
 * @return Values for a and b, minimal in b.
 */
template<typename NumberType>
tuple<NumberType> min_pell_solution(NumberType d) {
  std::vector<NumberType> cf;
  NumberType period = ntlib::quadratic_irrational_cf(d, cf);
  NumberType n = period & 1 ? 2 * period - 1 : period - 1;
  ntlib::rational<NumberType> pq =
      ntlib::nth_convergent_quadratic_irrational_cf(n, cf);
  return tuple<NumberType> {pq.numerator, pq.denominator};
}

/**
 * Generates the next solution to Pell's equation given a current and the
 * minimal solution.
 * x^2 - d * y^2 = 1
 *
 * Solutions can be generated by:
 * x_{i+1} = x_0*x_i + d*y_0*y_i
 * y_{i+1} = y_0*x_i + x_0*y_i
 *
 * @param d The parameter d.
 * @param initial The initial solution (the one minimal in x).
 * @param current Any solution.
 * @return The next bigger solution than current (in x).
 */
template<typename NumberType>
tuple<NumberType> next_pell_solution(NumberType d,
    const tuple<NumberType> &initial, const tuple<NumberType> &current) {
  return tuple<NumberType> {
      initial.a * current.a + d * initial.b * current.b,
      initial.b * current.a + initial.a * current.b};
}

}
