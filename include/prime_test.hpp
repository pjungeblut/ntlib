#pragma once

/**
 * Implementations for prime number tests.
 */

#include <algorithm>
#include <cstddef>
#include <vector>

#include "base.hpp"

namespace ntlib {

/**
 * Prime number test, naive.
 * Runtime: O(sqrt(n))
 *
 * @param n The number to be tested.
 */
template<typename T>
bool is_prime_naive(const T &n) {
  if (n < 2) return false;
  for (T i = 2; i * i <= n; ++i) {
    if (n % i == 0) return false;
  }
  return true;
}

/**
 * Miller Rabin primality test.
 *
 * @param n The number to be tested. Must be odd and `n*n` must still fit into
 *          type `T`.
 * @prarm a The base to test with. Must be 1 < a < n - 1.
 * @return False, if the number is composite. True, if the number is probable
 *         prime.
 */
template<typename T>
bool miller_rabin_test(const T &n, const T &a) {
   const T m = n - 1;
   T d = m >> 1;
   T e = 1;
   while (!(d & 1)) {
     d >>= 1;
     ++e;
   }
   T p = a;
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
 * Deterministic for n < 318'665'857'834'031'151'167'461.
 * Runtime: O(log n)
 *
 * @param n The number to be tested.
 */
template<typename T>
bool is_prime_miller_rabin(const T &n) {
  // Base cases.
	if (n == 2) return true;
	if (n < 2 || n % 2 == 0) return false;

  // Possible bases.
  std::vector<T> bases = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
  for (T a : bases) {
    if (a >= n - 1) break;
    if (!miller_rabin_test(n, a)) return false;
	}
	return true;
}

/**
 * Tests whether a given number is (probable) prime.
 */
template<typename T, bool allow_probable_prime = true>
bool is_prime(const T &n) {
  static const std::size_t THRESHOLD_NAIVE_MILLER_RABIN = 500'000;

  if constexpr (allow_probable_prime) {
    if (n <= THRESHOLD_NAIVE_MILLER_RABIN) return is_prime_naive(n);
    else return is_prime_miller_rabin(n);
  } else {
    return is_prime_naive(n);
  }
}

}
