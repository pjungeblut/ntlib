#pragma once

#include <numeric>
#include <vector>

#include "base.hpp"
#include "prime_decomposition.hpp"

namespace ntlib {

/**
 * Counts the number of divisors of a given number `n`.
 *
 * @param factors The prime factorization of `n`.
 * @return The number of divisors.
 */
template<typename T>
T count_divisors(const std::vector<prime_power<T>> &factors) {
  return std::accumulate(factors.begin(), factors.end(), T{1},
      [](T res, prime_power<T> pp) {
    return res *= pp.e + 1;
  });
}

/**
 * Computes the divisor function for a number `n`.
 *
 * @param factors The prime factorization of `n`.
 * @param exponent The power to which all divisors should be taken.
 *     Must be non-negative.
 * @return The value of d_x(n) = \sum_{d|n} d^x.
 */
template<typename T>
T divisor_function(const std::vector<prime_power<T>> &factors, T exponent) {
  assert(exponent >= T{0});

  // General formula below requires `exponent > 0`, so we use `count_divisors`
  // instead.
  if (exponent == 0) { return count_divisors(factors); }

  return std::accumulate(factors.begin(), factors.end(), T{1},
      [exponent](T res, prime_power<T> pp) {
    T sum{1};
    for (T i{1}; i <= pp.e; ++i) {
      sum += pow(pp.p, i * exponent);
    }
    return res *= sum;
  });
}

}
