#pragma once

/**
 * Implementations regarding the continued fraction expressions of cetain
 * numbers.
 */

#include <cmath>
#include <vector>

#include "include/base.hpp"

namespace ntlib {

/**
 * Computes the continued fraction representation of sqrt(n) for a natural
 * number n that is not a perfect square.
 *
 * For these (and only these) numbers the continued fraction is regular and
 * periodic.
 *
 * @tparam NumberType Integral data type used for natural numbers.
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
    NumberType cf2 = static_cast<NumberType>(std::floor((cf[0] + m2) / d2));

    if (i > 1 && m2 == m[1] && d2 == d[1] && cf2 == cf[1]) return i - 1;

    m.push_back(m2);
    d.push_back(d2);
    cf.push_back(cf2);
  }
}

}
