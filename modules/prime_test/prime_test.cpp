/**
 * Implementations for prime number tests.
 */

module;

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <optional>
#include <span>
#include <type_traits>

export module prime_test;

import base;
import int128;
import lucas_sequence;
import mod_int;
import modulo;

namespace ntlib {

/**
 * @brief Primality test by trial division of a given number using a precomputed list
 * of primes.
 * 
 * Assumes that the list of primes is sorted and contains no holes.
 * 
 * @tparam C A container of primes of type `T`.
 * @tparam T An integer-like type.
 * @param n The given number.
 * @param primes A list of small primes to divide by.
 * @return Whether `n` is prime (relative to all primes in `primes`).
 */
export template<typename C, typename T = C::value_type>
[[nodiscard]] constexpr
std::optional<bool> is_prime_trial_division(T n,
    const C &primes) {
  // Trivial cases.
  if (n <= T{1}) { return false; }

  // Trial division with some small prime factors.
  for (const auto p : primes) {
    if (n == p) { return true; }
    if (n % p == T{0}) { return false; }
    // Known: `n` is not divisible by any prime less than `p`.
    // Thus: All non-primes until `p^2` have already been ruled out.
    if (n <= p * p) { return true; }
  }

  return std::nullopt;
}

/**
 * Miller-Selfridge-Rabin primality test.
 * Tests if `n` is a strong propable prime.
 *
 * @param n The number to be tested, odd, `n > 2`.
 * @param a The base to test with.
 * @return `true` if and only if the `n` is a strong probable prime to base `a`.
 */
export template<typename T>
[[nodiscard]] constexpr
bool miller_selfridge_rabin_test(T n, T a) noexcept {
  assert(n > T{2});
  assert(is_odd(n));

  // Handle bases with `a < n`.
  mod_int<T> a_mod_n(a, n);
  if (a_mod_n.get() == T{0}) { return true; }

  // Decompose, such that `n-1 = o*2^e`.
  T n_minus_1 = n - T{1};
  auto [e, o] = odd_part(n_minus_1);

  auto p_mod_n = pow(a_mod_n, o);

  if (p_mod_n.get() == T{1} || p_mod_n.get() == n_minus_1) { return true; }
  for (T r = 1; r < e && p_mod_n.get() > T{1}; ++r) {
    p_mod_n *= p_mod_n;
    if (p_mod_n.get() == n_minus_1) { return true; }
  }
  return false;
}

/**
 * Optimized prime test for 32-bit integers.
 * Based on hashing and Miller-Selfridge-Rabin.
 * See: https://ceur-ws.org/Vol-1326/020-Forisek.pdf
 * 
 * Does not check bases cases (numbers divisible by 2, 3, 5, 7)!
 * 
 * @param n The number to test for primality.
 * @return Whether `n` is prime.
 */
export [[nodiscard]] constexpr
bool forisek_jancina_no_base_cases(uint32_t n) {
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
  return miller_selfridge_rabin_test(static_cast<uint64_t>(n), bases[h]);
}

/**
 * Prime test, optimized for 32-bit values, i.e., `n <= 2^32 - 1`.
 * Deterministic.
 * 
 * @param n The number to test for primality.
 * @return Whether `n` is prime.
 */
[[nodiscard]] constexpr
bool is_prime_32(uint32_t n) {
  // Handle small numbers by trial division.
  std::optional<bool> trial_division =
      is_prime_trial_division(n, SMALL_PRIMES<uint32_t>);
  if (trial_division.has_value()) { return trial_division.value(); }

  // Use prime test by Forisek and Jancina for larger values.
  return forisek_jancina_no_base_cases(n);
}

/**
 * Prime test, optimized for 64-bit values, i.e., `n <= 2^64 - 1`.
 * Deterministic.
 * 
 * @param n The number to test for primality.
 * @return Whether `n` is prime.
 */
[[nodiscard]] constexpr
bool is_prime_64(uint64_t n) {
  // Handle small numbers by trial division.
  std::optional<bool> trial_division =
      is_prime_trial_division(n, SMALL_PRIMES<uint64_t>);
  if (trial_division.has_value()) { return trial_division.value(); }

  // Use deterministic Miller-Selfridge-Rabin test for larger values.
  // For set of bases: https://miller-rabin.appspot.com/
  constexpr auto bases64 = std::to_array<uint64_t>({
      2, 325, 9'375, 28'178, 450'775, 9'780'504, 1'795'265'022});
  return std::ranges::all_of(bases64, [n](uint64_t a) {
    return miller_selfridge_rabin_test(
        static_cast<u128>(n), static_cast<u128>(a));
  });
}

/**
 * Checks whether `n` is a Lucas probable prime.
 *
 * @param n The number to check.
 * @return `true` if and only if `n` is a strong Lucas probable prime.
 */
export template<typename U, typename S = std::make_signed_t<U>>
[[nodiscard]] constexpr
bool is_strong_lucas_probable_prime(U n) noexcept {
  assert(n > U{2});
  assert(is_odd(n));

  // Find a `D`, such that `jacobi(D,n) = -1`.
  const auto find_D = [](U n) {
    // Function to generate the next candidate value for `D`.
    const auto next_D_candidate = [](S D) {
      return D > S{0} ? S{-2} - D : S{2} - D;
    };

    // Start by testing a few candidates.
    const std::size_t ITERATIONS_BEFORE_SQUARE_TEST = 5;
    S D{5};
    bool found_d = false;
    for (std::size_t i = 0; i < ITERATIONS_BEFORE_SQUARE_TEST; ++i) {
      if (jacobi(D, static_cast<S>(n)) == S{-1}) {
        found_d = true;
        break;
      }
      D = next_D_candidate(D);
    }

    // If no value for `D` was found yet, then it might be that `n` is a perfect
    // square. Then, no `D` exists.
    if (!found_d && is_square(n)) { return std::optional<S>{}; }

    // If `n` is not a perfect square we continue looking for a `D`.
    // It must exist.
    while (jacobi(D, static_cast<S>(n)) != S{-1}) {
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
  auto [e, o] = odd_part(n + U{1});

  // Strong Lucas probable prime test.
  auto [u, v] = mod_lucas_nth_term(o, P, Q, static_cast<S>(n));
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
 * Uses a Baillie-PSW-Test, which is deterministic for all values `n <= 2^64`.
 *
 * @param n The number to test for primality.
 * @return Whether `n` is a prime number.
 */
export template<typename U, typename S = std::make_signed_t<U>>
[[nodiscard]] constexpr
bool is_prime_baillie_psw(U n) noexcept {
  // Trivial cases.
  if (n <= U{1}) { return false; }

  // Trial division with some small prime factors.
  auto trial_division = is_prime_trial_division(n, SMALL_PRIMES<U>);
  if (trial_division.has_value()) { return trial_division.value(); }

  // Strong probable prime test for base 2.
  if (!miller_selfridge_rabin_test(n, U{2})) return false;

  // Strong Lucas probable prime test.
  if (!is_strong_lucas_probable_prime<U, S>(n)) return false;

  // Most probably prime.
  return true;
}

/**
 * Prime test.
 * Deterministic for `n <= 2^64`.
 * Probabilistic for larger values.
 * 
 * @param n The number to test for primality.
 * @return Whether `n` is prime.
 */
export template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr
bool is_prime(T n) noexcept {
  if constexpr (std::is_integral_v<T> && sizeof(T) <= 4) {
    if constexpr (std::is_signed_v<T>) {
      if (n < 0) { return false; }
      else { return is_prime_32(static_cast<uint32_t>(n)); }
    } else { return is_prime_32(static_cast<uint32_t>(n)); }
  } else if constexpr (std::is_integral_v<T> && sizeof(T) <= 8) {
    if constexpr (std::is_signed_v<T>) {
      if (n < 0) { return false; }
      else { return is_prime_64(static_cast<uint64_t>(n)); }
    } else { return is_prime_64(static_cast<uint64_t>(n)); }
  } else {
    return is_prime_baillie_psw<T,S>(n);
  }
}

}
