#pragma once

/**
 * Implementations for prime number tests.
 */

#include <cstdint>
#include <type_traits>

#include "base.hpp"
#include "lucas_sequence.hpp"

namespace ntlib {

/**
 * Prime number test, naive.
 * Runtime: O(sqrt(n))
 *
 * @param n The number to be tested.
 */
template<typename T>
bool is_prime_naive(T n) {
  if (n < 2) return false;
  for (const T p : PRIMES_BELOW_100) {
    if (n == p) return true;
    if (n % p == 0) return false;
  }
  for (T i = 101; i * i <= n; i += 2) {
    if (n % i == 0) return false;
  }
  return true;
}

/**
 * Miller Rabin primality test.
 * Tests if `n` is a strong propable prime.
 *
 * @param n The number to be tested.
 * @prarm a The base to test with. Must be `1 < a < n-1`.
 * @return `true` if the `n` is a strong pseudoprime to base `a`.
 */
template<typename T>
bool miller_rabin_test(T n, T a) {
  assert(a > 1);
  assert(a < n - 1);

  // Decompose `n-1` into `d*2^e`.
  const T m = n - 1;
  T d = m >> 1;
  T e = 1;
  while (!(d & 1)) {
    d >>= 1;
    ++e;
  }

  // Strong pseudoprime test.
  T p = mod_pow(a, d, n);
  if (p == 1 || p == m) return true;
  while (--e) {
    p *= p;
    p %= n;
    if (p == m) return true;
  }
  return false;
}

/**
 * Checks whether `n` is a Lucas probable prime.
 *
 * @param n The number to check.
 * @return `true` if n is a strong lucas probable prime.
 */
template<typename U, typename S = std::make_signed<U>::type>
bool is_strong_lucas_probable_prime(U n) {
  assert(n > 2);
  assert(n % 2 == 1);

  // Find parameters P, Q and D for Lucas sequences.
  static const int ITERATIONS_BEFORE_SQUARE_TEST = 10;
  S D = 5;
  bool found_d = false;
  for (int i = 0; i < ITERATIONS_BEFORE_SQUARE_TEST; ++i) {
    if (jacobi(D, static_cast<S>(n)) == -1) {
      found_d = true;
      break;
    }
    D = D > 0 ? -(D + 2) : -(D - 2);
  }
  // In no value for D was found yet, then it might be that n is a perfect
  // square. In this case no D exists.
  if (!found_d && is_square(n)) return false;
  // If n is not a perfect square we continue looking for a D. It must exist.
  while (jacobi(D, static_cast<S>(n)) != -1) {
    D = D > 0 ? -(D + 2) : -(D - 2);
  }
  const S P = 1;
  const S Q = (1 - D) / 4;

  // Decompose `n+1` into `d*2^s`.
  S d = (n + 1) / 2;
  S s = 1;
  while (!(d & 1)) {
    d /= 2;
    ++s;
  }

  // Strong Lucas probable prime test.
  auto [u, v] = lucas_nth_term_mod(d, P, Q, static_cast<S>(n));
  if (u == 0 || v == 0) return true;
  while (--s) {
    auto uu = mod(u * v, static_cast<S>(n));
    auto vv = v * v + D * u * u;
    if (vv & 1) vv += n;
    vv /= 2;
    vv = mod(vv, static_cast<S>(n));
    u = uu;
    v = vv;
    if (v == 0) return true;
  }

  return false;
}

/**
 * Tests whether a given number is (probable) prime.
 * Uses a Baillie-PSW-Test, which is deterministic for all values `n<2^64`.
 *
 * @param n The number to test.
 */
template<typename U, typename S = std::make_signed<U>::type>
bool is_prime(U n) {
  // Trivial cases.
  if (n <= 1) return false;

  // Trial division with some small prime factors.
  for (U p : PRIMES_BELOW_100) {
    if (n == p) return true;
    if (n % p == 0) return false;
  }

  // Strong probable prime test for base 2.
  if (!ntlib::miller_rabin_test(n, static_cast<U>(2))) return false;

  // Strong Lucas probable prime test.
  if (!is_strong_lucas_probable_prime<U, S>(n)) return false;

  // Most probably prime.
  return true;
}

}
