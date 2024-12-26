/**
 * @file
 * @brief Primary module interface unit for module `prime_generation`.
 */
module;

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <vector>

/**
 * @module prime_generation
 * @brief Generate prime numbers.
 * 
 * There are function templates to genrate single prime numbers, lists of primes
 * and prime sieves.
 */
export module prime_generation;

import base;
import prime_test;
export import :sieve_235;

namespace ntlib {

/**
 * @brief Generates a prime sieve.
 * 
 * @tparam T An integer-like type.
 * @tparam Allocator The allocator to use for the list of primes.
 * @tparam SieveType The datastructure to use as a prime sieve.
 * @tparam SEGMENT_SIZE Portion of the sieve to be processed at once. This
 *     should be small enough that `SEGMENT_SIZE` sieve fields fit into an L3
 *     cache line.
 * @tparam CREATE_LIST Whether the sieve should also create a list of primes.
 * @param N The largest number to be accessable in the sieve.
 * @param primes The `std::vector<T>` to receive the primes.
 * @return The sieve.
 */
export template<
    typename T,
    typename Allocator,
    typename SieveType,
    std::size_t SEGMENT_SIZE,
    bool CREATE_LIST>
SieveType eratosthenes_segmented(T N, std::vector<T> &primes) {
  const T OFFSETS[8] = {6, 4, 2, 4, 2, 4, 6, 2};
  const auto clear_multiples_unitl = [&OFFSETS](SieveType &sieve, T p, T &m,
      T &o, T until) {
    for (; m <= until; m += OFFSETS[++o % 8] * p) {
      sieve[m] = false;
    }
  };

  N = N / 30 * 30 + 29;
  SieveType sieve(N + 1);
  sieve.init235();

  T R = ntlib::isqrt(N) / 30 * 30 + 29;

  sieve[1] = false;
  sieve[2] = true;
  sieve[3] = true;
  sieve[5] = true;

  primes = {2, 3, 5};
  std::vector<T, Allocator> multiples = {0, 0, 0};
  std::vector<T, Allocator> offsets = {0, 0, 0};
  const T primes_until_root = (R * 4 / 15) + 2;
  if constexpr (CREATE_LIST) {
    const T primes_until_N = (N * 4 / 15) + 2;
    primes.reserve(primes_until_N);
  } else {
    primes.reserve(primes_until_root);
  }
  multiples.reserve(primes_until_root);
  offsets.reserve(primes_until_root);

  static const T REMAINDERS[8] = {1, 7, 11, 13, 17, 19, 23, 29};
  T i = 0;
  while (i * i <= R) {
    for (T j = 0; j < 8; ++j) {
      T cand = i + REMAINDERS[j];
      if (sieve[cand]) {
        primes.push_back(cand);
        multiples.push_back(cand * cand);
        offsets.push_back(j + 7);
        clear_multiples_unitl(
            sieve, primes.back(), multiples.back(), offsets.back(), R);
      }
    }
    i += 30;
  }
  while (i <= R) {
    for (T j = 0; j < 8; ++j) {
      T cand = i + REMAINDERS[j];
      if (sieve[cand]) {
        primes.push_back(cand);
        multiples.push_back(cand * cand);
        offsets.push_back(j + 7);
      }
    }
    i += 30;
  }

  const T blocks = (N - R) / SEGMENT_SIZE;
  for (T b = 1; b <= blocks; ++b) {
    const T maxi = R + b * SEGMENT_SIZE;

    for (T idx = 3; idx < static_cast<T>(primes.size()); ++idx) {
      clear_multiples_unitl(
          sieve, primes[idx], multiples[idx], offsets[idx], maxi);
    }
  }

  for (T idx = 3; idx < static_cast<T>(primes.size()); ++idx) {
    clear_multiples_unitl(
        sieve, primes[idx], multiples[idx], offsets[idx], N);
  }

  if constexpr (CREATE_LIST) {
    while (i <= N) {
      for (T j = 0; j < 8; ++j) {
        if (sieve[i + REMAINDERS[j]]) {
          primes.push_back(i + REMAINDERS[j]);
        }
      }
      i += 30;
    }
  }

  return sieve;
}

/**
 * @brief Generates a prime sieve.
 *
 * @tparam T An integer-like type.
 * @tparam Allocator The allocator to use for the list of primes.
 * @tparam SieveType The datastructure to use as a prime sieve.
 * @tparam SEGMENT_SIZE Portion of the sieve to be processed at once. This
 *     should be small enough that `SEGMENT_SIZE` sieve fields fit into an L3
 *     cache line.
 * @param N The largest number to be accessable in the sieve.
 * @return The sieve.
 */
export template<
    typename T,
    typename Allocator = std::allocator<T>,
    typename SieveType = ntlib::sieve_235<>,
    std::size_t SEGMENT_SIZE = (1 << 18)>
SieveType prime_sieve(T N) {
  std::vector<T> primes;
  return ntlib::eratosthenes_segmented<
      T,
      Allocator,
      SieveType,
      SEGMENT_SIZE,
      false>(
          N, primes);
}

/**
 * @brief Generates a prime sieve.
 *
 * @tparam T An integer-like type.
 * @tparam Allocator The allocator to use for the list of primes.
 * @tparam SieveType The datastructure to use as a prime sieve.
 * @tparam SEGMENT_SIZE Portion of the sieve to be processed at once. This
 *     should be small enough that `SEGMENT_SIZE` sieve fields fit into an L3
 *     cache line.
 * @param N The largest number to be accessable in the sieve.
 * @param primes The `std::vector<T>` to receive the primes.
 * @return The sieve.
 */
export template<
    typename T,
    typename Allocator = std::allocator<T>,
    typename SieveType = ntlib::sieve_235<>,
    std::size_t SEGMENT_SIZE = (1 << 18)>
SieveType prime_sieve(T N, std::vector<T> &primes) {
  return ntlib::eratosthenes_segmented<
      T,
      Allocator,
      SieveType,
      SEGMENT_SIZE,
      true>(
        N, primes);
}

/**
 * @brief Find the smallest prime bigger than a given number.
 * 
 * @param n The given number.
 * @return The smallest prime larget than \f$n\f$.
 */
export template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr
T next_prime(T n) noexcept {
  // Base cases.
  if (n <= T{1}) { return T{2}; }
  if (n == T{2}) { return T{3}; }

  T result {n + (T{1} + static_cast<T>(ntlib::is_odd(n)))};
  while (!ntlib::is_prime<T,S>(result)) { result += T{2}; }
  return result;
}

}