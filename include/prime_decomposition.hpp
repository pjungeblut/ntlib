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
#include "include/integral.hpp"
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
template<Integral I>
void prime_decomposition_list(I n, std::map<I, I> &factors,
    const std::vector<I> &primes) {
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
template<Integral I>
void prime_decomposition(I n, std::map<I, I> &factors) {
  I iroot = isqrt(n);
  std::vector<I> primes;
  sieve_eratosthenes_list_segmented(iroot, primes);
  prime_decomposition_list(n, factors, primes);
}

/**
 * Generates a list of all divisors.
 *
 * @param factors The prime factors and their multiplicities.
 * @param divisors All divisors of the number.
 */
template<Integral I>
void enumerate_divisors(const std::map<I, I> &factors,
    std::vector<I> &divisors) {
  divisors.assign(1, static_cast<I>(1));
  for (auto [factor, multiplicity] : factors) {
    const std::size_t cur_num_divisors = divisors.size();
    I power = factor;
    while (multiplicity--) {
      for (std::size_t i = 0; i < cur_num_divisors; ++i) {
        divisors.push_back(divisors[i] * power);
      }
      power *= factor;
    }
  }
}

}
