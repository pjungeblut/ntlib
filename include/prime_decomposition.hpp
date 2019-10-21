#pragma once

/**
 * Implementations to get the prime decomposition of a natural number.
 *
 * Contains different implementations:
 * Naive: Tests every possible number <= sqrt(n) as a prime factor of n.
 * List: Takes a list of all primes <= sqrt(n) and only tests those as possible
 *       prime factors. This list can be generated before using the Sieve of
 *       Eratosthenes.
 */

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <vector>

#include "include/base.hpp"
#include "include/prime_test.hpp"

namespace ntlib {
  /**
   * Computes the prime decomposition of a number.
   *
   * @tparam NumberType Integral data type used for natural numbers.
   *
   * @param n The number to decompose.
   * @param factors The prime factors.
   */
  template<typename NumberType>
  void prime_decomposition_naive(NumberType n, std::vector<NumberType> &factors) {
    for (NumberType i = 2; i * i <= n; ++i) {
      while (n % i == 0) {
        n /= i;
        factors.push_back(i);
      }
    }
    if (n != 1) factors.push_back(n);
  }

  /**
   * Computes the prime decomposition of a number.
   * Takes a list of primes used as possible prime factors.
   *
   * @tparam NumberType Integral data type used for natural numbers.
   *
   * @param n The number to decompose.
   * @param factors The prime factors.
   * @param primes List of primes used as possible prime factors.
   *               Must contain all prime factors p, such that p*p <= n to work
   *               correctly.
   */
  template<typename NumberType>
  void prime_decomposition_list(NumberType n, std::vector<NumberType> &factors,
      std::vector<NumberType> &primes) {
    for (std::size_t i = 0; i < primes.size() && primes[i] * primes[i] <= n; ++i) {
      while (n % primes[i] == 0) {
        n /= primes[i];
        factors.push_back(primes[i]);
      }
    }
    if (n != 1) factors.push_back(n);
  }

  /**
   * Finds a non-trivial factor of n (if one exists).
   *
   * @tparam NumberType Integral data type used for natural numbers.
   *
   * @param n The number to find a factor of.
   */
  template<typename NumberType>
  NumberType rho(NumberType n) {
    if (!(n & 1)) return 2;
    NumberType x = std::rand() % n;
    NumberType y = x;
    NumberType c = std::rand() % n;
    NumberType d = 1;
    while (d == 1) {
      x = (x * x % n + c) % n;
      y = (y * y % n + c) % n;
      y = (y * y % n + c) % n;
      d = gcd(abs(x - y), n);
    }
    return d == n ? rho(n) : d;
  }

  /**
   * Computes the prime decomposition of a number.
   * Uses Pollard's Rho Method to find a non-trivial factor.
   *
   * @tparam NumberType Integral data type used for natural numbers.
   *
   * @param n The number to decompose.
   * @param factors The prime factors.
   */
  template<typename NumberType>
  void prime_decomposition_rho(NumberType n, std::vector<NumberType> &factors) {
    if (n == 1) return;
    if (is_prime_miller_rabin(n)) {
      factors.push_back(n);
      return;
    }
    NumberType f = rho(n);
    prime_decomposition_rho(f, factors);
    prime_decomposition_rho(n / f, factors);
  }

}
