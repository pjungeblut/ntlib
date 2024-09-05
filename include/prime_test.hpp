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
 * Miller-Selfridge-Rabin primality test.
 * Tests if `n` is a strong propable prime.
 *
 * @param n The number to be tested, odd, `n >= 5`.
 * @param a The base to test with, `1 < a < n-1`.
 * @return `true` if and only if the `n` is a strong probable prime to base `a`.
 */
template<typename T>
[[nodiscard]] constexpr
bool miller_selfridge_rabin_test(T n, T a) noexcept {
  assert(n >= T{5});
  assert(is_odd(n));
  assert(a > T{1});
  assert(a < n - T{1});

  // Decompose, such that `n-1 = o*2^e`.
  auto [e, o] = odd_part(n - T{1});

  const auto mod_n = [n](T x) { return mod(x, n); };
  T p = mod_pow(a, o, mod_n);

  if (p == T{1}) { return true; }
  do {
    if (p == n - T{1}) { return true; }
    p = mod(p * p, n);
  } while (--e > T{0} && p > T{1});
  return false;
}

/**
 * Checks whether `n` is a Lucas probable prime.
 *
 * @param n The number to check.
 * @return `true` if n is a strong Lucas probable prime.
 */
template<typename U, typename S = std::make_signed_t<U>>
[[nodiscard]] constexpr
bool is_strong_lucas_probable_prime(U n) noexcept {
  assert(n > U{2});
  assert(is_odd(n));

  // Find parameters P, Q and D for Lucas sequences.
  const std::size_t ITERATIONS_BEFORE_SQUARE_TEST = 10;
  S D{5};
  bool found_d = false;
  for (std::size_t i = 0; i < ITERATIONS_BEFORE_SQUARE_TEST; ++i) {
    if (jacobi(D, static_cast<S>(n)) == S{-1}) {
      found_d = true;
      break;
    }
    D = D > S{0} ? -(D + S{2}) : -(D - S{2});
  }
  // If no value for D was found yet, then it might be that n is a perfect
  // square. In this case no D exists.
  if (!found_d && is_square(n)) { return false; }

  // If n is not a perfect square we continue looking for a D. It must exist.
  while (jacobi(D, static_cast<S>(n)) != S{-1}) {
    D = D > S{0} ? -(D + S{2}) : -(D - S{2});
  }
  const S P{1};
  const S Q{(S{1} - D) / S{4}};

  // Decompose such that `n+1 = o*2^e`.
  auto [e, o] = odd_part(n + U{1});

  // Strong Lucas probable prime test.
  auto [u, v] = lucas_nth_term_mod(o, P, Q, static_cast<S>(n));
  if (u == S{0} || v == S{0}) { return true; }
  while (--e) {
    S uu = mod(u * v, static_cast<S>(n));
    S vv = v * v + D * u * u;
    if (is_odd(vv)) { vv += n; }
    vv /= S{2};
    vv = mod(vv, static_cast<S>(n));
    u = uu;
    v = vv;
    if (v == S{0}) { return true; }
  }

  return false;
}

/**
 * Tests whether a given number `n` is (probable) prime.
 * Uses a Baillie-PSW-Test, which is deterministic for all values `n < 2^64`.
 *
 * @param n The number to test.
 * @return Whether `n` is a prime number.
 */
template<typename U, typename S = std::make_signed_t<U>>
[[nodiscard]] constexpr
bool is_prime(U n) noexcept {
  // Trivial cases.
  if (n <= U{1}) { return false; }

  // Trial division with some small prime factors.
  for (const U p : SMALL_PRIMES) {
    if (n == p) { return true; }
    if (n % p == 0) { return false; }
  }

  // Strong probable prime test for base 2.
  if (!miller_selfridge_rabin_test(n, U{2})) return false;

  // Strong Lucas probable prime test.
  if (!is_strong_lucas_probable_prime<U, S>(n)) return false;

  // Most probably prime.
  return true;
}

}
