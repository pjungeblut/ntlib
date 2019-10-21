#pragma once

/**
 * Implementations to count the number of divisors of a number.
 */

#include <map>

#include "include/prime_decomposition.hpp"

namespace ntlib {

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
