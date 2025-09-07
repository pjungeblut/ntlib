/**
 * @file
 * @brief Primary module interface unit for module `prime_test`.
 */
module;

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <optional>
#include <span>
#include <type_traits>

/**
 * @module prime_test
 * @brief Test whether a given number is prime.
 */
export module prime_test;

import base;
import int128;
import lucas_sequence;
import mod_int;
import modulo;

namespace ntlib {

/**
 * @brief Uses trial division to test whether a given number is prime relative
 * to a given precomputed list of primes.
 * 
 * Assumes that the given list of primes is sorted and contains no gaps.
 * If \f$p\f$ is the largest prime in the given list, then the primality of all
 * numbers up to \f$p^2\f$ can be determined correctly.
 * For greater numbers, no decision is made.
 * 
 * @tparam T An integer-like type.
 * @tparam R An input-range.
 * @param n The given number.
 * @param list The given list of primes.
 * @retval std::optional<bool>{true} If the number is prime.
 * @retval std::optional<bool>{false} If the number is composite.
 * @retval std::nullopt If primality could not be determined by the given list
 *     of primes.
 */
export template<typename T, std::ranges::input_range R>
[[nodiscard]] constexpr
std::optional<bool> is_prime_trial_division(T n, R &&list) noexcept
    requires std::convertible_to<std::ranges::range_value_t<R>, T> {
  // Trivial cases.
  if (n <= T{1}) { return false; }

  // Trial division with some small prime factors.
  for (const auto p : list) {
    if (n == p) { return true; }
    if (n % p == T{0}) { return false; }
    // Known: `n` is not divisible by any prime less than `p`.
    // Thus: All non-primes until `p^2` have already been ruled out.
    if (n <= p * p) { return true; }
  }

  return std::nullopt;
}

/**
 * @brief Miller-Selfridge-Rabin primality test.
 * 
 * Given an odd integer \f$n > 2\f$, tests if \f$n\f$ is a strong propable
 * prime with respect to a given base.
 *
 * @tparam T An integer-like type.
 * @param n The given number. Must be odd and greater than \f$2\f$
 * @param a The given base.
 * @return Whether \f$n\f$ is a strong probable prime with respect to base
 *     \f$a\f$.
 */
export template<typename T>
[[nodiscard]] constexpr
bool miller_selfridge_rabin_test(T n, T a) noexcept {
  assert(n > T{2});
  assert(ntlib::is_odd(n));

  // Handle bases with `a < n`.
  a = ntlib::mod(a, n);
  if (a == T{0}) { return true; }

  // Decompose, such that `n-1 = o*2^e`.
  const T n_minus_1 = n - T{1};
  auto [e, o] = ntlib::odd_part(n_minus_1);

  T p = ntlib::mod_pow(a, o, n, ntlib::mod<T>);
  if (p == T{1} || p == n_minus_1) { return true; }


  for (T r{1}; r < e && p > T{1}; ++r) {
    p = ntlib::mod(p * p, n);
    if (p == n_minus_1) { return true; }
  }
  return false;
}

/**
 * @brief Subroutine for optimized prime test for a given 32-bit integer.
 * 
 * Based on hashing and Miller-Selfridge-Rabin.
 * See: https://ceur-ws.org/Vol-1326/020-Forisek.pdf
 * 
 * Does not check bases cases (numbers divisible by 2, 3, 5, 7)!
 * 
 * @param n The given number.
 * @return Whether \f$n\f$ is prime.
 */
[[nodiscard]] constexpr
bool forisek_jancina_no_base_cases(uint32_t n) noexcept {
  constexpr auto bases = std::to_array<uint64_t>({
      15591, 2018, 166, 7429, 8064, 16045, 10503, 4399, 1949, 1295, 2776, 3620,
      560, 3128, 5212, 2657, 2300, 2021, 4652, 1471, 9336, 4018, 2398, 20462,
      10277, 8028, 2213, 6219, 620, 3763, 4852, 5012, 3185, 1333, 6227, 5298,
      1074, 2391, 5113, 7061, 803, 1269, 3875, 422, 751, 580, 4729, 10239, 746,
      2951, 556, 2206, 3778, 481, 1522, 3476, 481, 2487, 3266, 5633, 488, 3373,
      6441, 3344, 17, 15105, 1490, 4154, 2036, 1882, 1813, 467, 3307, 14042,
      6371, 658, 1005, 903, 737, 1887, 7447, 1888, 2848, 1784, 7559, 3400, 951,
      13969, 4304, 177, 41, 19875, 3110, 13221, 8726, 571, 7043, 6943, 1199,
      352, 6435, 165, 1169, 3315, 978, 233, 3003, 2562, 2994, 10587, 10030,
      2377, 1902, 5354, 4447, 1555, 263, 27027, 2283, 305, 669, 1912, 601, 6186,
      429, 1930, 14873, 1784, 1661, 524, 3577, 236, 2360, 6146, 2850, 55637,
      1753, 4178, 8466, 222, 2579, 2743, 2031, 2226, 2276, 374, 2132, 813,
      23788, 1610, 4422, 5159, 1725, 3597, 3366, 14336, 579, 165, 1375, 10018,
      12616, 9816, 1371, 536, 1867, 10864, 857, 2206, 5788, 434, 8085, 17618,
      727, 3639, 1595, 4944, 2129, 2029, 8195, 8344, 6232, 9183, 8126, 1870,
      3296, 7455, 8947, 25017, 541, 19115, 368, 566, 5674, 411, 522, 1027, 8215,
      2050, 6544, 10049, 614, 774, 2333, 3007, 35201, 4706, 1152, 1785, 1028,
      1540, 3743, 493, 4474, 2521, 26845, 8354, 864, 18915, 5465, 2447, 42,
      4511, 1660, 166, 1249, 6259, 2553, 304, 272, 7286, 73, 6554, 899, 2816,
      5197, 13330, 7054, 2818, 3199, 811, 922, 350, 7514, 4452, 3449, 2663,
      4708, 418, 1621, 1171, 3471, 88, 11345, 412, 1559, 194});

  // Hashing to find the correct base.
  uint64_t h = n;
  h = ((h >> 16) ^ h) * 0x45D9F3B;
  h = ((h >> 16) ^ h) * 0x45D9F3B;
  h = ((h >> 16) ^ h) & 0xFF;
  return ntlib::miller_selfridge_rabin_test(static_cast<uint64_t>(n), bases[h]);
}

/**
 * @brief Deterministic prime test optimized for a given 32-bit integer.
 * 
 * Correct if the given number is at most \f$2^32 - 1\f$.
 * 
 * @param n The given number.
 * @return Whether \f$n\f$ is prime.
 */
[[nodiscard]] constexpr
bool is_prime_32(uint32_t n) noexcept {
  // Handle small numbers by trial division.
  std::optional<bool> trial_division =
      ntlib::is_prime_trial_division(n, SMALL_PRIMES<uint32_t>);
  if (trial_division.has_value()) { return trial_division.value(); }

  // Use prime test by Forisek and Jancina for larger values.
  return ntlib::forisek_jancina_no_base_cases(n);
}

/**
 * @brief Deterministic prime test optimized for a given 64-bit integer.
 * 
 * Correct if the given number is at most \f$2^64 - 1\f$.
 * 
 * @param n The given number.
 * @return Whether \f$n\f$ is prime.
 */
[[nodiscard]] constexpr
bool is_prime_64(uint64_t n) noexcept {
  // Handle small numbers by trial division.
  std::optional<bool> trial_division =
      ntlib::is_prime_trial_division(n, SMALL_PRIMES<uint64_t>);
  if (trial_division.has_value()) { return trial_division.value(); }

  // Use deterministic Miller-Selfridge-Rabin test for larger values.
  // For set of bases: https://miller-rabin.appspot.com/
  constexpr auto bases64 = std::to_array<uint64_t>({
      2, 325, 9'375, 28'178, 450'775, 9'780'504, 1'795'265'022});
  return std::ranges::all_of(bases64, [n](uint64_t a) {
    return ntlib::miller_selfridge_rabin_test(
        static_cast<ntlib::u128>(n), static_cast<ntlib::u128>(a));
  });
}

/**
 * @brief Checks whether a given number is a Lucas probable prime.
 *
 * @tparam T An integer-like type.
 * @tparam S The signed type corresponding to `T`.
 * @param n The given number.
 * @return Whether the given number is a strong Lucas probable prime.
 */
export template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr
bool is_strong_lucas_probable_prime(T n) noexcept {
  assert(n > T{2});
  assert(ntlib::is_odd(n));

  // Find a `D`, such that `jacobi(D,n) = -1`.
  const auto find_D = [](T n) {
    // Function to generate the next candidate value for `D`.
    const auto next_D_candidate = [](S D) {
      return D > S{0} ? S{-2} - D : S{2} - D;
    };

    // Start by testing a few candidates.
    const std::size_t ITERATIONS_BEFORE_SQUARE_TEST = 5;
    S D{5};
    bool found_d = false;
    for (std::size_t i = 0; i < ITERATIONS_BEFORE_SQUARE_TEST; ++i) {
      if (ntlib::jacobi(D, static_cast<S>(n)) == S{-1}) {
        found_d = true;
        break;
      }
      D = next_D_candidate(D);
    }

    // If no value for `D` was found yet, then it might be that `n` is a perfect
    // square. Then, no `D` exists.
    if (!found_d && ntlib::is_square(n)) { return std::optional<S>{}; }

    // If `n` is not a perfect square we continue looking for a `D`.
    // It must exist.
    while (ntlib::jacobi(D, static_cast<S>(n)) != S{-1}) {
      D = next_D_candidate(D);
    }

    return std::optional<S>{D};
  };

  // Compute parameters `P`, `Q` and `D` for the Lucas sequence.
  // If no suitable `D` exists, then `n` is composite.
  const auto optional_D = find_D(n);
  if (!optional_D.has_value()) { return false; }
  const S D = optional_D.value();
  const S P{1};
  const S Q{(S{1} - D) / S{4}};

  // Decompose such that `n+1 = o*2^e`.
  auto [e, o] = ntlib::odd_part(n + T{1});

  // Strong Lucas probable prime test.
  auto [u, v] = ntlib::mod_lucas_nth_term(o, P, Q, static_cast<S>(n));
  if (u == S{0} || v == S{0}) { return true; }
  while (--e) {
    const S uu = ntlib::mod(u * v, static_cast<S>(n));
    S vv = v * v + D * u * u;
    if (ntlib::is_odd(vv)) { vv += n; }
    vv /= S{2};
    vv = ntlib::mod(vv, static_cast<S>(n));
    u = uu;
    v = vv;
    if (v == S{0}) { return true; }
  }

  return false;
}

/**
 * @brief Baillie-PSW (probable) prime test.
 * 
 * Deterministic for all integers \f$n \leq 2^64\f$.
 *
 * @tparam T An integer-like type.
 * @tparam S The signed type corresponding to `T`.
 * @param n The given number.
 * @return Whether \f$n\f$ is a prime number.
 */
export template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr
bool is_prime_baillie_psw(T n) noexcept {
  // Trivial cases.
  if (n <= T{1}) { return false; }

  // Trial division with some small prime factors.
  const auto trial_division =
      ntlib::is_prime_trial_division(n, SMALL_PRIMES<T>);
  if (trial_division.has_value()) { return trial_division.value(); }

  // Strong probable prime test for base 2.
  if (!ntlib::miller_selfridge_rabin_test(n, T{2})) { return false; }

  // Strong Lucas probable prime test.
  return ntlib::is_strong_lucas_probable_prime<T, S>(n);
}

/**
 * @brief Prime test
 * 
 * Deterministic for all integers \f$n \leq 2^64\f$.
 * Probabilistic for larger values.
 * Use this function template whenever there is no strong reason to use any
 * other.
 * 
 * @tparam T An integer-like type.
 * @tparam S The signed type corresponding to `T`.
 * @param n The number to test for primality.
 * @return Whether \f$n\f$ is prime.
 */
export template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr
bool is_prime(T n) noexcept {
  if constexpr (std::is_integral_v<T> && sizeof(T) <= 4) {
    if constexpr (std::is_signed_v<T>) {
      if (n < 0) { return false; }
      else { return ntlib::is_prime_32(static_cast<uint32_t>(n)); }
    } else { return ntlib::is_prime_32(static_cast<uint32_t>(n)); }
  } else if constexpr (std::is_integral_v<T> && sizeof(T) <= 8) {
    if constexpr (std::is_signed_v<T>) {
      if (n < 0) { return false; }
      else { return ntlib::is_prime_64(static_cast<uint64_t>(n)); }
    } else { return ntlib::is_prime_64(static_cast<uint64_t>(n)); }
  } else {
    return ntlib::is_prime_baillie_psw<T,S>(n);
  }
}

} // namespace ntlib
