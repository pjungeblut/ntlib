#pragma once

#include <array>
#include <type_traits>
#include <vector>

#include "../../include/base.hpp"
#include "../../include/prime_generation.hpp"
#include "../../include/prime_test.hpp"

namespace ntlib {
namespace experiments {

// These bases guarantee that the Miller-Selfridge-Rabin primality test is
// deterministic for all 32- and 64-bit numbers.
// See: https://miller-rabin.appspot.com/
constexpr auto bases32 = std::to_array<uint32_t>({2, 7, 61});
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
  for (const T p : SMALL_PRIMES) {
    if (p >= n) { return true; }
    else if (n % p == 0) { return false; }
  }
  for (T i{ntlib::SMALL_PRIMES_UPPER_BOUND + 1}; i * i <= n; i += T{2}) {
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
  if (n < T{2}) { return false; }
  else if (n == T{2}) {return true; }
  else if (is_even(n)) { return false; }
  else if (n == T{3}) { return true; }
  else if (n == T{4}) { return false; }

  // Possible bases.
  return std::all_of(BASES.begin(), BASES.end(), [n](T a) {
    T mod_base = mod(a, n);
    if (mod_base == 0) { return true; }
    else { return miller_selfridge_rabin_test(n, mod(a, n)); }
  });
}

[[nodiscard]] constexpr
bool is_prime_miller_selfridge_rabin(uint32_t n) noexcept {
  return is_prime_miller_selfridge_rabin<uint32_t, bases32.size(), bases32>(n);
}

[[nodiscard]] constexpr
bool is_prime_miller_selfridge_rabin(uint64_t n) noexcept {
  return is_prime_miller_selfridge_rabin<uint64_t, bases64.size(), bases64>(n);
}

}
}
