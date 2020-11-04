#pragma once

/**
 * Implementations for prime number tests.
 */

#include <algorithm>
#include <cstddef>
#include <limits>
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
  if (n > 2 && n % 2 == 0) return 0;
  for (T i = 3; i * i <= n; i += 2) {
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
  static const T bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

  // Base cases.
  if (n < 2) return false;
  for (T b : bases) {
    if (n == b) return true;
    else if (n % b == 0) return false;
  }

  // Possible bases.
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
  // Until this value the naive method is preferred over the Miller Rabin test.
  static const std::size_t THRESHOLD_NAIVE_MILLER_RABIN = 400'000;

  // Until this value the Miller Rabin test is assumed to be exact.
  // With all primes p <= 37 as bases, it is actually exact unitl
  // n < 318'665'857'834'031'151'167'461, which is bigger than any value that
  // can be stored in an unsigned 64 bit integer.
  static const std::size_t THRESHOLD_MILLER_RABIN_EXACT =
      std::numeric_limits<uint64_t>::max();

  if constexpr (allow_probable_prime) {
    if (n <= THRESHOLD_NAIVE_MILLER_RABIN) return is_prime_naive(n);
    else return is_prime_miller_rabin(n);
  } else {
    if (n < THRESHOLD_MILLER_RABIN_EXACT) {
      if (n <= THRESHOLD_NAIVE_MILLER_RABIN) return is_prime_naive(n);
      else return is_prime_miller_rabin(n);
    } else {
      // This will only work if n has only small prime factors...
      return is_prime_naive(n);
    }
  }
}

}
