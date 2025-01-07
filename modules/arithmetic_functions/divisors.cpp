/**
 * @file
 * @brief Primary module interface unit for module `divisors`.
 */
module;

#include <algorithm>
#include <cassert>
#include <vector>

/**
 * @module divisors
 * @brief Work with the divisors of an integer.
 * 
 * Provides function templates to count and enumerate divisors of integers or
 * to compute the divisor function.
 */
export module divisors;

import base;
import prime_decomposition;

namespace ntlib {

/**
 * @brief Counts the number of divisors of a given number `n`.
 *
 * @tparam T An integer-like type.
 * @param factors The prime decomposition of `n`.
 * @return The number of divisors of `n`.
 */
export template<typename T>
[[nodiscard]] constexpr
T count_divisors(const prime_factors<T> &factors) noexcept {
  return std::ranges::fold_left(factors, T{1}, [](T res, prime_power<T> pp) {
    return res *= pp.e + 1;
  });
}

/**
 * @brief Computes the divisor function for a given number `n`.
 * 
 * The divisor function for `n` is defined as
 * \f$\sigma_x(n) = \sum_{d \mid n} d^x\f$.
 *
 * @tparam T An integer-like type.
 * @tparam Exp An integer-like type.
 * @param factors The prime decomposition of `n`.
 * @param x The power to which all divisors should be taken.
 *     Must be non-negative.
 * @return The value of \f$\sigma_x(n)\f$.
 */
export template<typename T, typename Exp>
[[nodiscard]] constexpr
T divisor_function(const prime_factors<T> &factors, Exp x) noexcept {
  assert(x >= Exp{0});

  // The general formula below requires `x > 0`.
  // Therefore, for `x == 0`, we use `count_divisors` instead.
  if (x == Exp{0}) { return count_divisors(factors); }

  return std::ranges::fold_left(factors, T{1}, [x](T res, prime_power<T> pp) {
    T sum{1};
    for (T i{1}; i <= pp.e; ++i) {
      sum += pow(pp.p, i * x);
    }
    return res *= sum;
  });
}

/**
 * @brief Enumerates all divisors of a given number `n`.
 * 
 * In particular, this is equal to \f$\sigma_0(n)\f$.
 *
 * @tparam T An integer-like type.
 * @param factors The prime decomposition of `n`.
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