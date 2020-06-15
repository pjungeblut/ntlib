#pragma once

/**
 * Implementations to count the number of divisors of a number.
 */

#include <map>

#include "base.hpp"
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
template<typename T>
T divisor_function(std::map<T, T> &factors, T exponent) {
  T result = 1;
  for (auto &[f, m] : factors) {
    result *= (pow(f, exponent * (m + 1)) - 1) / (pow(f, exponent) - 1);
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
  for (auto &[k,v] : factors) divisors *= v + 1;
  return divisors;
}

}
