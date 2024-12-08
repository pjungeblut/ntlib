/**
 * @file
 * @brief Primary module interface unit for module `prime_decomposition`.
 */
module;

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <limits>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#include "prime_list.hpp"

/**
 * @module prime_decomposition
 * @brief Function templates to decompose a natural number into its unique prime
 * decomposition.
 */
export module prime_decomposition;

import base;
import int128;
import prime_generation;
import prime_test;

namespace ntlib {

/**
 * @brief Represents a single prime power.
 * 
 * For a prime \f$p\f$ and a integer \f$e \geq 1\f$, this represents the prime
 * power \f$p^e\f$.
 * 
 * @tparam T An integer-like type.
 */
export template<typename T>
struct prime_power {
  /**
   * @brief The prime number.
   */
  T p;

  /**
   * @brief The exponent.
   */
  T e;
};

/**
 * @brief Represents a prime factorization.
 * 
 * @tparam T An integer-like type.
 */
export template<typename T>
using prime_factors = std::vector<prime_power<T>>;

/**
 * @brief Computes a prime decomposition of a given number using a provided list
 * of potential prime divisors.
 * 
 * The remainder (the largest divisor coprime to all potential prime divisors)
 * will be returned seperately. 
 * 
 * @tparam T An integer-like type.
 * @tparam C A container with elements of type `T`.
 * @param n The given number.
 * @param primes The list of potential prime divisors. Must be sorted and
 *     contain all primes up to some upper bound.
 * @return A `std::pair` containing a list of prime powers dividing `n` as its
 *     first element and the remainder (coprime with all primes in `primes`) as
 *     its second element.
 */
template<typename C, typename T = typename C::value_type>
[[nodiscard]] constexpr
std::pair<prime_factors<T>,T> prime_decomposition_list_remainder(
    T n, const C &primes) {
  assert(n > T{0});

  prime_factors<T> factors;
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
 * @brief Computes a prime decomposition of a given number using a provided list
 * of potential prime divisors.
 * 
 * The remainder (the largest divisor coprime to all potential prime divisors)
 * will be appended to the prime decomposition with an exponent of \f$1\f$.
 * 
 * @note When the list of potential prime divisors contains all primes up to
 * \f$\lfloor\sqrt{n}\rfloor\f$, then the return value is the correct prime
 * decomposition.
 * 
 * @tparam T An integer-like type.
 * @tparam C A container with elements of type `T`.
 * @param n The number to decompose.
 * @param primes The list of potential prime divisors.
 * @return A prime decompositon of.
 */
export template<typename C, typename T = typename C::value_type>
[[nodiscard]] constexpr
prime_factors<T> prime_decomposition_list(T n, const C &primes) {
  auto res_list = ntlib::prime_decomposition_list_remainder(n, primes);
  if (res_list.second != 1) {
    res_list.first.push_back(prime_power<T>{res_list.second, T{1}});
  }
  return res_list.first;
}

/**
 * @brief Computes the prime decomposition of a number up to \f$2^32 - 1\f$.
 * 
 * @tparam T An integer-like type.
 * @param n The given number.
 * @return A vector of prime powers.
 */
template<typename T>
[[nodiscard]] constexpr
prime_factors<T> prime_decomposition_32(T n) {
  assert(n >= T{0});
  assert(static_cast<std::make_unsigned_t<T>>(n) <=
      std::numeric_limits<uint32_t>::max());
  
  return prime_decomposition_list(n, PRIMES_BELOW_2_16);
}

/**
 * @brief Finds a factor using Pollard's rho algorithm.
 * 
 * Given an odd composite number \f$n\f$, this function template tries to find a
 * non-trivial (possibly non-prime) factor of \f$n\f$ using Pollard's rho
 * algorithm.
 * Cycles are detected using Floyd's algorithm.
 *
 * @tparam T An integer-like type.
 * @tparam F A function object.
 * @param n The odd composite.
 * @param f A polynomial function to generate a "pseudorandom" sequence. A usual
 *     choice for the polynomial function \f$f\f$ would be:
 *     \f$f(x) = (x^2 + 1) \mod n\f$.
 * @param x Initial value for parameter \f$x\f$.
 * @param MULTIPLICATIONS The number of multiplications to do instead of
 *     expensive gcd-calls.
 * @return A `std::optional<T>` that is either empty or contains a non-trivial
 *     factor.
 */
template<typename T, typename F>
[[nodiscard]] constexpr
std::optional<T> find_factor_pollard_rho_mult(
    T n, F f, T x, const std::size_t MULTIPLICATIONS = 128) noexcept {
  T y = x;
  T g{1};

  T xs, ys;
  while (g == T{1}) {
    T prod{1};
    xs = x;
    ys = y;
    for (std::size_t i = 0; i < MULTIPLICATIONS; ++i) {
      x = f(x);
      y = f(f(y));
      prod = u128{prod} * ntlib::difference(x, y) % n;
    }
    g = ntlib::gcd(prod, n);
  }

  if (g == n) {
    do {
      xs = f(xs);
      ys = f(f(ys));
      g = ntlib::gcd(ntlib::difference(xs, ys), n);
    } while (g == T{1});
  }

  if (g == n) { return std::optional<T>(); }
  else { return g; }
}

/**
 * @brief Finds a non-trivial factor of a given composite number.
 * 
 * @tparam T An integer-like type.
 * @param n The given number.
 * @return A non-trivial factor.
 */
export template<typename T>
[[nodiscard]] constexpr
T find_factor(T n) noexcept {
  // A polynomial (modulo `n`) simulating a pseudorandom function.
  const auto poly = [n](T x) {
    if constexpr (std::is_integral_v<T> && sizeof(T) <= 8) {
      return static_cast<T>((u128{x} * x + 1) % n);
    } else {
      return (x * x + 1) % n;
    }
  };

  T x0 = 2;
  std::optional<T> res = ntlib::find_factor_pollard_rho_mult(n, poly, x0);
  while (!res.has_value()) {
    ++x0;
    res = ntlib::find_factor_pollard_rho_mult(n, poly, x0);
  }
  return res.value();
}

/**
 * @brief Decompose a given number into its prime decomposition.
 * 
 * Does not use trial division to find small prime factors, so this function
 * should only be calles for numbers that do not have small prime factors.
 * 
 * @tparam T An integer-like type.
 * @param n The given number.
 * @return A prime decomposition.
 */
template<typename T>
[[nodiscard]] constexpr
prime_factors<T> prime_decomposition_large(T n) {
  // Base cases.
  if (ntlib::is_prime(n)) { return {prime_power<T>{n, T{1}}}; }

  // Find a non-trivial factor.
  const T f = find_factor(n);
  n /= f;

  // Decompose factor and reduce `n` by all found prime factors.
  prime_factors<T> factors = ntlib::prime_decomposition_large(f);
  for (auto &[p, e] : factors) {
    while (n % p == 0) {
      n /= p;
      ++e;
    }
  }
  if (n == 1) { return factors; }

  // Decompose remainder and add its prime factors to the result.
  // No duplicate entries as remainder is coprime to all previous factors.
  const prime_factors<T> rem = ntlib::prime_decomposition_large(n);
#ifdef __cpp_lib_containers_ranges
  factors.append_range(rem);
#else
  factors.insert(factors.end(), rem.begin(), rem.end());
#endif
  return factors;
}

/**
 * @brief Computes the prime decomposition of a given number.
 *
 * @tparam T An integer-like type.
 * @param n The given number.
 * @return A vector of prime powers.
 */
export template<typename T>
[[nodiscard]] constexpr
prime_factors<T> prime_decomposition(T n) {
  assert(n >= T{1});

  if constexpr (std::is_integral_v<T> && sizeof(T) <= 4) {
    return ntlib::prime_decomposition_32(n);
  } else {
    // Start by trial division with small primes.
    auto res_list =
        ntlib::prime_decomposition_list_remainder(n, SMALL_PRIMES<T>);
    std::vector<prime_power<T>> factors{std::move(res_list.first)};
    const T remainder = res_list.second;
    if (remainder == 1) { return factors; }

    // Continue with the remainder.
    const prime_factors<T> factors_rem =
        ntlib::prime_decomposition_large(remainder);

    // Concatenate lists of prime powers.
#ifdef __cpp_lib_containers_ranges
    factors.append_range(factors_rem);
#else
    factors.insert(factors.end(), factors_rem.begin(), factors_rem.end());
#endif
    return factors;
  }
}

}