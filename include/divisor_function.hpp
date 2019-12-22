#pragma once

/**
 * Implementations to count the number of divisors of a number.
 */

#include <map>

#include "base.hpp"
#include "integral.hpp"
#include "prime_decomposition.hpp"

namespace ntlib {

/**
 * Computes the divisor function for a number n.
 *
 * @param factors The prime factors of n and their multiplicities.
 * @param exponent The power to which all divisors should be taken.
 *                 It must be 0 < exponent.
 * @return The value of d_x(n) = \sum_{d|n} d^x.
 */
template<Integral I>
I divisor_function(std::map<I, I> &factors, I exponent) {
  I result = 1;
  for (auto &[factor, mulitplicity] : factors) {
    result *= (pow(factor, exponent * (mulitplicity + 1)) - 1) / (pow(factor, exponent) - 1);
  }
  return result;
}

/**
 * Counts the number of divisors of a given number given its prime
 * factorization.
 *
 * @param factors The prime factors and their multiplicities.
 * @return The number of divisors.
 */
template<Integral I>
I count_divisors(std::map<I, I> &factors) {
  I divisors = 1;
  for (auto &f : factors) divisors *= f.second + 1;
  return divisors;
}

}
