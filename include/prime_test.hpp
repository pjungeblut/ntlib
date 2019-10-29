#pragma once

/**
 * Implementations for prime number tests.
 *
 * Contains different implementations:
 * Naive: Tests all possible factors <= sqrt(n).
 * Miller Rabin: Deterministic variant of Miller Rabin Prime number test for
 *               64 bit (unsigned) integers.
 */

#include <algorithm>
#include <vector>

#include "include/base.hpp"
#include "include/integral.hpp"

namespace ntlib {

/**
 * Prime number test, naive.
 * Runtime: O(sqrt(n))
 *
 * @param n The number to be tested.
 */
template<UnsignedIntegral U>
bool is_prime_naive(U n) {
  for (U i = 2; i * i <= n; ++i) {
    if (n % i == 0) return false;
  }
  return true;
}

/**
 * Miller Rabin primality test.
 *
 * @param n The number to be tested. Must be odd and fit into a 64 bit
 *          (unsigned) integer.
 * @prarm a The base to test with. Must be 1 < a < n - 1.
 * @return False, if the number is composite. True, if the number is probable
 *         prime.
 */
bool miller_rabin_test(const __uint128_t n, const __uint128_t a) {
   const __uint128_t m = n - 1;
   __uint128_t d = m >> 1;
   __uint128_t e = 1;
   while (!(d & 1)) {
     d >>= 1;
     ++e;
   }
   __uint128_t p = a;
   p = ntlib::mod_pow(a, d, n);
   if (p == 1 || p == m) return true;
   while (--e) {
      p *= p, p %= n;
      if (p == m) return true;
      if (p <= 1) break;
   }
   return false;
}

/**
 * Prime number test.
 * Deterministic variant of Miller Rabin prime test.
 * Deterministic for n < 318.665.857.834.031.151.167.461.
 * Runtime: O(log n)
 *
 * @param n The number to be tested.
 */
template<UnsignedIntegral U>
bool is_prime_miller_rabin(U n) {
  // Base cases.
	if (n == 2) return true;
	if (n < 2 || n % 2 == 0) return false;

  // Possible bases.
  std::vector<__uint128_t> bases = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
  for (__uint128_t a : bases) {
    if (a >= n - 1) break;
    if (!miller_rabin_test(n, a)) return false;
	}
	return true;
}

}
