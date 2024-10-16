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
[[nodiscard]] constexpr
T count_divisors(const std::vector<prime_power<T>> &factors) noexcept {
  return std::accumulate(factors.begin(), factors.end(), T{1},
      [](T res, prime_power<T> pp) {
    return res *= pp.e + 1;
  });
}

/**
 * Computes the divisor function for a number `n`.
 *
 * @param factors The prime factorization of `n`.
 * @param x The power to which all divisors should be taken.
 *     Must be non-negative.
 * @return The value of d_x(n) = \sum_{d|n} d^x.
 */
template<typename T>
[[nodiscard]] constexpr
T divisor_function(
    const std::vector<prime_power<T>> &factors, T x) noexcept {
  assert(x >= T{0});

  // General formula below requires `x > 0`, so we use `count_divisors` instead.
  if (x == 0) { return count_divisors(factors); }

  return std::accumulate(factors.begin(), factors.end(), T{1},
      [x](T res, prime_power<T> pp) {
    T sum{1};
    for (T i{1}; i <= pp.e; ++i) {
      sum += pow(pp.p, i * x);
    }
    return res *= sum;
  });
}

}
