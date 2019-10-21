#pragma once

/**
 * Implementations to count the number of divisors of a number.
 */

#include <map>

#include "include/base.hpp"
#include "include/prime_decomposition.hpp"

namespace ntlib {

/**
 * Computes the divisor function for a number n.
 *
 * @tparam NumberType Integral data type used for natural numbers.
 *
 * @param factors The prime factors of n and their multiplicities.
 * @param exponent The power to which all divisors should be taken.
 *                 It must be 0 < exponent.
 * @return The value of d_x(n)
 */
template<typename NumberType>
NumberType divisor_function(std::map<NumberType, NumberType> &factors, NumberType exponent) {
  NumberType result = 1;
  for (auto &[factor, mulitplicity] : factors) {
    result *= (pow(factor, exponent * (mulitplicity + 1)) - 1) / (pow(factor, exponent) - 1);
  }
  return result;
}

/**
 * Counts the number of divisors of a given number given its prime
 * factorization.
 *
 * @tparam NumberType Integral data type used for natural numbers.
 *
 * @param factors The prime factors and their multiplicities.
 * @return The number of divisors.
 */
template<typename NumberType>
NumberType count_divisors(std::map<NumberType, NumberType> &factors) {
  NumberType divisors = 1;
  for (auto &f : factors) {
    divisors *= f.second + 1;
  }
  return divisors;
}

}
