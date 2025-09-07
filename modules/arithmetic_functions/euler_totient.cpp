/**
 * @file
 * @brief Primary module interfact unit for module `euler_totient`.
 */
module;

#include <algorithm>
#include <numeric>
#include <vector>

/**
 * @module euler_totient
 * @brief Compute Euler's totient function \f$\phi\f$.
 * 
 * Euler's totient function counts the number of integers up to `n` that are
 * coprime to `n`.
 * 
 * The module provides function templates to compute Euler's totient function
 * \f$\phi\f$. Both, to compute \f$\phi(n)\f$ for a single number \f$n\f$ or by
 * using a sieve for all \f$n \leq N\f$.
 */
export module euler_totient;

import base;
import prime_decomposition;

namespace ntlib {

/**
 * @brief Computes Euler's totient function \f$\phi(n)\f$ for a given number
 * `n`.
 * 
 * @tparam T An integer-like type.
 * @param factors The prime decomposition of `n`.
 * @return Euler's totient function \f$\phi(n)\f$.
 */
export template<Integer T>
[[nodiscard]] constexpr
T euler_totient(const prime_factors<T> &factors) noexcept {
  return std::ranges::fold_left(factors, T{1}, [](T prod, const auto &pp) {
    return prod * ntlib::pow(pp.p, pp.e - 1) * (pp.p - 1);
  });
}

/**
 * @brief Computes Euler's totient function \f$\phi(n)\f$ for a given number.
 * 
 * @tparam T An integer-like type.
 * @param n The given number.
 * @return Euler's totient function \f$\phi(n)\f$.
 */
export template<Integer T>
[[nodiscard]] constexpr
T euler_totient(T n) noexcept {
  return ntlib::euler_totient(ntlib::prime_decomposition(n));
}

/**
 * @brief Computes Euler's totient function \f$\phi(n)\f$ for all integers
 * up to a given number `N`.
 * 
 * Runtime: \f$O(N \log(\log(N))\f$
 *
 * @tparam T An integer-like type.
 * @param N The number until which all values should be computed.
 * @return A `std::vector<T>` containing all values. The element at index `0`
 *     will be set to `0`.
 */
export template<Integer T>
[[nodiscard]]
std::vector<T> euler_totient_sieve(std::size_t N) {
  std::vector<T> sieve(N + 1);
  std::ranges::iota(sieve, 0);
  for (std::size_t i = 2; i <= N; ++i) {
    if (sieve[i] == i) {
      for (std::size_t j = i; j <= N; j += i) {
        sieve[j] /= i;
        sieve[j] *= i - 1;
      }
    }
  }
  return sieve;
}

}