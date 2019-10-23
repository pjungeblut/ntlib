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
  ntlib::rational<NumberType> pq = ntlib::nth_convergent_quadratic_irrational_cf(n, cf);
  return tuple<NumberType> {pq.numerator, pq.denominator};
}

}
