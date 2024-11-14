/**
 * @file
 * @brief Primary module interface unit for module `base`.
 */

/**
 * @module divisors
 * @brief Function templates related to the divisors of an integer.
 * 
 * Count and enumerate divisors of integers or compute the divisor function.
 */
module;

#include <cassert>
#include <numeric>
#include <vector>

export module divisors;

import base;
import prime_decomposition;

namespace ntlib {

/**
 * @brief Counts the number of divisors of a given number `n`.
 *
 * @tparam T An integer-like type.
 * @param factors The prime factorization of `n`.
 * @return The number of divisors.
 */
export template<typename T>
[[nodiscard]] constexpr
T count_divisors(const prime_factors<T> &factors) noexcept {
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
export template<typename T>
[[nodiscard]] constexpr
T divisor_function(const prime_factors<T> &factors, T x) noexcept {
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

/**
 * Enumerates all divisors of a number `n`.
 *
 * @param factors The prime factorization of `n`.
 * @return All divisors of `n`.
 */
export template<typename T>
[[nodiscard]] constexpr
std::vector<T> enumerate_divisors(const prime_factors<T> &factors) {
  std::vector<T> divisors(1, T{1});
  for (auto [p, e] : factors) {
    const std::size_t cur_num_divisors = divisors.size();
    T power = p;
    while (e--) {
      for (std::size_t i = 0; i < cur_num_divisors; ++i) {
        divisors.push_back(divisors[i] * power);
      }
      power *= p;
    }
  }
  return divisors;
}

}