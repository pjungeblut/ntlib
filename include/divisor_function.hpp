#pragma once

/**
 * Implementations to count the number of divisors of a number.
 */

#include <map>

#include "include/base.hpp"
#include "include/integral.hpp"
#include "include/prime_decomposition.hpp"

namespace ntlib {

/**
 * Computes the divisor function for a number n.
 *
 * @param factors The prime factors of n and their multiplicities.
 * @param exponent The power to which all divisors should be taken.
 *                 It must be 0 < exponent.
 * @return The value of d_x(n) = \sum_{d|n} d^x.
 */
template<Integral T>
T divisor_function(std::map<T, T> &factors, T exponent) {
  T result = 1;
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
template<Integral T>
T count_divisors(std::map<T, T> &factors) {
  T divisors = 1;
  for (auto &f : factors) divisors *= f.second + 1;
  return divisors;
}

}
