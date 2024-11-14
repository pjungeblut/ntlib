module;

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#include "prime_list.hpp"

export module prime_decomposition;

import base;
import int128;
import prime_generation;
import prime_test;

namespace ntlib {

/**
 * Represents a single prime power `p^e`.
 */
export template<typename T>
struct prime_power {
  T p;
  T e;
};

/**
 * Represents a prime factorization.
 */
export template<typename T>
using prime_factors = std::vector<prime_power<T>>;

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
 * Similar to `prime_decomposition_list_remainder` but also adds the remainder
 * to the result (with an exponent of `1`).
 * 
 * If `primes` contains all prime factors up to `floor(sqrt(n))`, then the
 * result will be correct.
 * 
 * @param n The number to decompose.
 * @return A prime decompositon of `n`, i.e., a vector of prime powers.
 */
export template<typename C, typename T = typename C::value_type>
[[nodiscard]] constexpr
prime_factors<T> prime_decomposition_list(
    T n, const C &primes) {
  auto res_list = prime_decomposition_list_remainder(n, primes);
  if (res_list.second != 1) {
    res_list.first.push_back(prime_power<T>{res_list.second, T{1}});
  }
  return res_list.first;
}

/**
 * Computes the prime decomposition of a 32 bit number `n`.
 * 
 * @param n The given number.
 * @return A vector of prime powers.
 */
template<typename T>
[[nodiscard]] constexpr
prime_factors<T> prime_decomposition_32(T n) {
  static_assert(sizeof(T) <= 4);
  return prime_decomposition_list(n, PRIMES_BELOW_2_16);
}

/**
 * Given an odd composite number `n`, tries to find a non-trivial (possibly
 * non-prime) factor of `n` using Pollard's rho algorithm.
 * Cycle detection by using Floyd's algorithm.
 *
 * A usual choice for the polynomial function `f` would be:
 * `f(x) = (x^2 + 1) mod n`.
 *
 * @param n The odd composite.
 * @param f A polynomial function to generate a "pseudorandom" sequence.
 * @param x Initial value for parameter `x`.
 * @param MULTIPLICATIONS The number of multiplications to do instead of
 *     expensive gcd-calls.
 * @return If successful, a non-trivial factor.
 */
template<typename T, typename F>
[[nodiscard]] constexpr
std::optional<T> find_factor_pollard_rho_mult(T n, F f, T x,
    const std::size_t MULTIPLICATIONS = 128) noexcept {
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
      prod = u128{prod} * difference(x, y) % n;
    }
    g = gcd(prod, n);
  }

  if (g == n) {
    do {
      xs = f(xs);
      ys = f(f(ys));
      g = gcd(difference(xs, ys), n);
    } while (g == T{1});
  }

  if (g == n) { return std::optional<T>(); }
  else { return g; }
}

/**
 * Given a composite number `n`, finds a non-trivial factor.
 * 
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
  std::optional<T> res = find_factor_pollard_rho_mult(n, poly, x0);
  while (!res.has_value()) {
    ++x0;
    res = find_factor_pollard_rho_mult(n, poly, x0);
  }
  return res.value();
}

/**
 * Decomposes a given number `n` into its prime decomposition.
 * Does not use trial division with small prime factors.
 * 
 * @param n The given number.
 * @return A vector of prime powers.
 */
template<typename T>
[[nodiscard]] constexpr
prime_factors<T> prime_decomposition_large(T n) {
  // Base cases.
  if (is_prime(n)) { return {prime_power<T>{n, T{1}}}; }

  // Find a non-trivial factor.
  const T f = find_factor(n);
  n /= f;

  // Decompose factor and reduce `n` by all found prime factors.
  prime_factors<T> factors = prime_decomposition_large(f);
  for (auto &[p, e] : factors) {
    while (n % p == 0) {
      n /= p;
      ++e;
    }
  }
  if (n == 1) { return factors; }

  // Decompose remainder and add its prime factors to the result.
  // No duplicate entries as remainder is coprime to all previous factors.
  const prime_factors<T> rem = prime_decomposition_large(n);
  factors.insert(factors.end(), rem.begin(), rem.end());
  return factors;
}

/**
 * Computes the prime decomposition of a given number `n`.
 *
 * @param n The given number.
 * @return A vector of prime powers.
 */
export template<typename T>
[[nodiscard]] constexpr
prime_factors<T> prime_decomposition(T n) {
  assert(n >= T{1});

  if constexpr (std::is_integral_v<T> && sizeof(T) <= 4) {
    return prime_decomposition_32(n);
  } else {
    // Start by trial division with small primes.
    auto res_list = prime_decomposition_list_remainder(n, SMALL_PRIMES<T>);
    std::vector<prime_power<T>> factors{std::move(res_list.first)};
    const T remainder = res_list.second;
    if (remainder == 1) { return factors; }

    // Continue with the remainder.
    const prime_factors<T> factors_rem =
        prime_decomposition_large(remainder);

    // Concatenate lists of prime powers.
    factors.insert(factors.end(), factors_rem.begin(), factors_rem.end());
    return factors;
  }
}

}