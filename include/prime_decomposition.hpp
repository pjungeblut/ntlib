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
#include <map>
#include <vector>

#include "include/base.hpp"
#include "include/prime_test.hpp"
#include "include/sieve_eratosthenes.hpp"

namespace ntlib {
  
/**
 * Computes the prime decomposition of a number.
 * Takes a list of primes used as possible prime factors.
 *
 * @param n The number to decompose.
 * @param factors The prime factors.
 * @param primes List of primes used as possible prime factors.
 *               Must contain all prime factors p, such that p*p <= n to work
 *               correctly.
 */
template<typename NumberType>
void prime_decomposition_list(NumberType n, std::map<NumberType, NumberType> &factors,
    const std::vector<NumberType> &primes) {
  for (std::size_t i = 0; i < primes.size() && primes[i] * primes[i] <= n; ++i) {
    while (n % primes[i] == 0) {
      n /= primes[i];
      factors[primes[i]]++;
    }
  }
  if (n != 1) factors[n]++;
}

/**
 * Computes the prime decomposition of a number.
 *
 * @param n The number to decompose.
 * @param factors The prime factors.
 */
template<typename NumberType>
void prime_decomposition(NumberType n, std::map<NumberType, NumberType> &factors) {
  NumberType iroot = isqrt(n);
  std::vector<NumberType> primes;
  sieve_eratosthenes_list_segmented(iroot, primes);
  prime_decomposition_list(n, factors, primes);
}

}
