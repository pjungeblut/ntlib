#pragma once

#include <type_traits>
#include <vector>

#include "../../include/base.hpp"
#include "../../include/prime_generation.hpp"
#include "../../include/prime_test.hpp"

namespace ntlib {
namespace experiments {

/**
 * Prime number test, trial division with prime list.
 *
 * @param n The number to test for primality.
 * @return Whether `n` is prime.
 */
template<typename T>
bool is_prime_list(const T &n) {
  std::vector<T> primes;
  prime_sieve(isqrt(n) + 1, primes);
  for (const T &p : primes) {
    if (n == p) return true;
    else if (n % p == 0) return false;
  }
  return true;
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

}
}
