#pragma once

#include <cstddef>
#include <cstdlib>
#include <utility>
#include <vector>

#include "base.hpp"
#include "prime_generation.hpp"
#include "prime_test.hpp"

namespace ntlib {

/**
 * Represents a single prime power `p^e`.
 */
template<typename T>
struct prime_power {
  T p;
  T e;
};

/**
 * Computes the prime decomposition of a given number `n` with respect to a
 * given list `primes` of potential prime divisors.
 * 
 * @param n The given number.
 * @param primes The list of potential prime divisors. Must be sorted
 *     ascendingly and must not contain holes.
 * @return A pair with a vector of prime powers as its first element, and the
 *     remainder (coprime with all primes in `primes`) as its second element.
 */
template<typename C, typename T = typename C::value_type>
[[nodiscard]] constexpr
std::pair<std::vector<prime_power<T>>,T> prime_decomposition_list(
    T n, const C &primes) {
  assert(n > T{0});

  std::vector<prime_power<T>> factors;
  for (const T p : primes) {
    if (p * p > n) { break; }
    if (n % p == 0) {
      T e{0};
      do {
        n /= p;
        ++e;
      } while (n % p == 0);
      factors.push_back(prime_power<T> {p, e});
    }
  }
  return std::make_pair(factors, n);
}

/**
 * Similar to `prime_decomposition_list` but also adds the remainder to the
 * result (with an exponent of `1`).
 * 
 * If `primes` contains all prime factors up to `floor(sqrt(n))`, then the
 * result will be correct.
 * 
 * @param n The number to decompose.
 * @return A prime decompositon of `n`, i.e., a vector of prime powers.
 */
template<typename C, typename T = typename C::value_type>
[[nodiscard]] constexpr
std::vector<prime_power<T>> prime_decomposition_complete_list(
    T n, const C &primes) {
  auto res_list = prime_decomposition_list(n, primes);
  if (res_list.second != 1) {
    res_list.first.push_back(prime_power<T>{res_list.second, T{1}});
  }
  return res_list.first;
}

/**
 * Computes the prime decomposition of a given number `n`.
 *
 * @param n The given number.
 * @return A vector of prime powers.
 */
template<typename T>
[[nodiscard]] constexpr
std::vector<prime_power<T>> prime_decomposition(T n) {
  // Start by checking small prime factors without using a sieve.
  auto res_list = prime_decomposition_list(n, SMALL_PRIMES);
  auto factors{std::move(res_list.first)};
  const T remainder = res_list.second;
  if (remainder == 1) { return factors; }
  if (is_prime(remainder)) {
    factors.push_back(prime_power<T>{remainder, T{1}});
    return factors;
  }

  // Continue with the remainder.
  // All its prime factors must be bigger than `SMALL_PRIMES_BIGGEST`.
  const T iroot = isqrt(remainder);
  std::vector<T> primes;
  prime_sieve(iroot, primes);
  auto rem_list = prime_decomposition_complete_list(remainder, primes);

  // Append factors of the remainder.
  factors.insert(factors.end(), rem_list.begin(), rem_list.end());
  return factors;
}

/**
 * Generates a list of all divisors of a number `n`.
 * TODO: Move this to `divisor_function.hpp
 *
 * @param factors The prime factorization of `n`.
 * @return All divisors of `n`.
 */
template<typename T>
[[nodiscard]] constexpr
std::vector<T> enumerate_divisors(const std::vector<prime_power<T>> &factors) {
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
