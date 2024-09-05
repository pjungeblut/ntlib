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
  if (n == T{2} || n == T{3} || n == T{5} || n == T{7}) { return true; }
  if (n % T{2} == T{0} || n % T{3} == T{0} ||
      n % T{5} == T{0} || n % T{7} == T{0}) { return false; }
  if (n < T{121}) { return n > T{1}; }

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
  
  // Base cases.
  if (n == 2 || n == 3 || n == 5 || n == 7) { return true; }
  if (n % 2 == 0 || n % 3 == 0 || n % 5 == 0 || n % 7 == 0) { return false; }
  // Every composite below 120 has a prime factor in {2, 3, 5, 7}.
  if (n < 121) return n > 1;

  // Hashing to find the correct base.
  uint64_t h = n;
  h = ((h >> 16) ^ h) * 0x45D9F3B;
  h = ((h >> 16) ^ h) * 0x45D9F3B;
  h = ((h >> 16) ^ h) & 0xFF;
  return miller_selfridge_rabin_test(static_cast<uint64_t>(n), bases[h]);
}

}
}
