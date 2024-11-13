#pragma once

#include <algorithm>
#include <array>
#include <type_traits>
#include <vector>

import base;
import modulo;
import prime_generation;
import prime_test;

namespace ntlib {
namespace experiments {

// These bases guarantee that the Miller-Selfridge-Rabin primality test is
// deterministic for all 32- and 64-bit numbers.
// See: https://miller-rabin.appspot.com/
constexpr auto bases32 = std::to_array<uint64_t>({2, 7, 61});
constexpr auto bases64 = std::to_array<uint64_t>({
    2, 325, 9'375, 28'178, 450'775, 9'780'504, 1'795'265'022});

/**
 * Prime number test, naive.
 * Runtime: `O(sqrt(n))`
 *
 * @param n The number to be tested.
 */
template<typename T>
[[nodiscard]] constexpr
bool is_prime_naive(T n) noexcept {
  if (n < T{2}) { return false; }
  for (const T p : SMALL_PRIMES<T>) {
    if (p >= n) { return true; }
    else if (n % p == 0) { return false; }
  }
  for (T i{ntlib::SMALL_PRIMES_BIGGEST<T> + 1}; i * i <= n; i += T{2}) {
    if (n % i == T{0}) { return false; }
  }
  return true;
}

/**
 * Prime number test, trial division with prime list.
 *
 * @param n The number to test for primality.
 * @return Whether `n` is prime.
 */
template<typename T>
[[nodiscard]] constexpr
bool is_prime_list(T n) noexcept {
  std::vector<T> primes;
  prime_sieve(isqrt(n) + T{1}, primes);

  for (const T p : primes) {
    if (n == p) return true;
    else if (n % p == T{0}) return false;
  }
  return true;
}

/**
 * Prime number test by Miller, Selfridge and Rabin.
 * 
 * Using the default set of bases the test is deterministic for all `n <= 2^64`.
 * 
 * Runtime: O(log n), assuming `|bases|` is constant.
 *
 * @param n The number to be tested.
 * @return Whether `n` is a strong probable prime to all given bases.
 */
template<typename T,
    std::size_t NUM_BASES,
    std::array<T, NUM_BASES> BASES>
[[nodiscard]] constexpr
bool is_prime_miller_selfridge_rabin(T n) noexcept {
  // Base cases.
  if (n == T{2} || n == T{3} || n == T{5} || n == T{7}) { return true; }
  if (n % T{2} == T{0} || n % T{3} == T{0} ||
      n % T{5} == T{0} || n % T{7} == T{0}) { return false; }
  if (n < T{121}) { return n > T{1}; }

  // Possible bases.
  return std::ranges::all_of(BASES, [n](T a) {
    T mod_base = mod(a, n);
    if (mod_base == 0) { return true; }
    else { return miller_selfridge_rabin_test(n, mod(a, n)); }
  });
}

[[nodiscard]] constexpr
bool is_prime_miller_selfridge_rabin(uint32_t n) noexcept {
  return is_prime_miller_selfridge_rabin<uint64_t, bases32.size(), bases32>(n);
}

[[nodiscard]] constexpr
bool is_prime_miller_selfridge_rabin(uint64_t n) noexcept {
  return is_prime_miller_selfridge_rabin<uint64_t, bases64.size(), bases64>(n);
}

/**
 * Optimized prime test for 32-bit integers.
 * Based on hashing and Miller-Selfridge-Rabin.
 * See: https://ceur-ws.org/Vol-1326/020-Forisek.pdf
 * 
 * @param n The number to test for primality.
 * @return Whether `n` is prime.
 */
[[nodiscard]] constexpr
bool is_prime_forisek_jancina(uint32_t n) {
  // Base cases.
  if (n == 2 || n == 3 || n == 5 || n == 7) { return true; }
  if (n % 2 == 0 || n % 3 == 0 || n % 5 == 0 || n % 7 == 0) { return false; }
  // Every composite below 120 has a prime factor in {2, 3, 5, 7}.
  if (n < 121) { return n > 1; }

  return forisek_jancina_no_base_cases(n);
}

}
}
