#pragma once

/**
 * Implemenatations to generate prime numbers.
 * Some functions generate sieves, some lists and some both.
 */

#include <algorithm>
#include <cstddef>
#include <vector>

#include "base.hpp"
#include "sieve.hpp"

namespace ntlib {

/**
 * Generates a prime sieve.
 * See other overload for documentation.
 */
template<
    typename SieveType = ntlib::sieve<>,
    std::size_t SEGMENT_SIZE = (1 << 18),
    typename Allocator = std::allocator<std::size_t>,
    bool CREATE_LIST = true>
SieveType eratosthenes_segmented(std::size_t N,
    std::vector<std::size_t> &primes) {

  const std::size_t OFFSETS[8] = {6, 4, 2, 4, 2, 4, 6, 2};
  const auto clear_multiples_unitl = [OFFSETS](SieveType &sieve, std::size_t p,
      std::size_t &m, std::size_t &o, std::size_t until) {
    for (; m <= until; m += OFFSETS[++o % 8] * p) {
      sieve[m] = false;
    }
  };

  N = N / 30 * 30 + 29;
  SieveType sieve(N + 1);
  sieve.init235();

  std::size_t R = ntlib::isqrt(N) + 1;

  sieve[1] = false;
  sieve[2] = true;
  sieve[3] = true;
  sieve[5] = true;

  // TODO: Reserve space to avoid reallocations.
  primes = {2, 3, 5};
  std::vector<std::size_t, Allocator> multiples = {0, 0, 0};
  std::vector<std::size_t, Allocator> offsets = {0, 0, 0};
  std::size_t primes_until_root = (R * 4 / 15) + 2;
  if constexpr (CREATE_LIST) {
    std::size_t primes_until_N = (N * 4 / 15) + 2;
    primes.reserve(primes_until_N);
  } else {
    primes.reserve(primes_until_root);
  }
  multiples.reserve(primes_until_root);
  offsets.reserve(primes_until_root);

  static const std::size_t REMAINDERS[8] = {1, 7, 11, 13, 17, 19, 23, 29};
  std::size_t i = 0;
  while (i * i <= R) {
    for (std::size_t j = 0; j < 8; ++j) {
      std::size_t cand = i + REMAINDERS[j];
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
    for (std::size_t j = 0; j < 8; ++j) {
      std::size_t cand = i + REMAINDERS[j];
      if (sieve[cand]) {
        primes.push_back(cand);
        multiples.push_back(cand * cand);
        offsets.push_back(j + 7);
      }
    }
    i += 30;
  }

  const std::size_t blocks = (N - R) / SEGMENT_SIZE;
  for (std::size_t b = 1; b <= blocks; ++b) {
    const std::size_t maxi = R + b * SEGMENT_SIZE;

    for (std::size_t idx = 3; idx < primes.size(); ++idx) {
      clear_multiples_unitl(
          sieve, primes[idx], multiples[idx], offsets[idx], maxi);
    }

    if constexpr (CREATE_LIST) {
      while (i <= maxi) {
        for (std::size_t j = 0; j < 8; ++j) {
          if (sieve[i + REMAINDERS[j]]) {
            primes.push_back(i + REMAINDERS[j]);
          }
        }
        i += 30;
      }
    }
  }

  for (std::size_t idx = 3; idx < primes.size(); ++idx) {
    clear_multiples_unitl(
        sieve, primes[idx], multiples[idx], offsets[idx], N);
  }

  if constexpr (CREATE_LIST) {
    while (i <= N) {
      for (std::size_t j = 0; j < 8; ++j) {
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
 * Generates a prime sieve.
 *
 * @tparam SieveType The datastructure to use as a prime sieve. NTLib provides
 *                   a classical array like type and a 235-compressed version.
 *                   The latter one can save half the time to generate the
 *                   primes in a tradeoff for slower access times.
 * @tparam SEGMENT_SIZE Portion of the sieve to be processed at once. This
 *                   should be small enough that `SEGMENT_SIZE` sieve fields fit
 *                   easily into the L3 cache.
 * @tparam Allocator The allocator to use for the list of primes.
 * @param N Generate a prime sieve with all values up to `N`.
 * @return The sieve.
 */
template<
    typename SieveType = ntlib::sieve<>,
    std::size_t SEGMENT_SIZE = (1 << 18),
    typename Allocator = std::allocator<std::size_t>>
SieveType prime_sieve(std::size_t N) {
  std::vector<std::size_t> primes;
  return eratosthenes_segmented<SieveType, SEGMENT_SIZE, Allocator, false>(
      N, primes);
}

/**
 * Generates a prime sieve.
 *
 * @tparam SieveType The datastructure to use as a prime sieve. NTLib provides
 *                   a classical array like type and a 235-compressed version.
 *                   The latter one can save half the time to generate the
 *                   primes in a tradeoff for slower access times.
 * @tparam SEGMENT_SIZE Portion of the sieve to be processed at once. This
 *                   should be small enough that `SEGMENT_SIZE` sieve fields fit
 *                   easily into the L3 cache.
 * @tparam Allocator The allocator to use for the list of primes.
 * @param N Generate a prime sieve with all values up to `N`.
 * @param primes A vector that will be filled with all primes up to N.
 * @return The sieve.
 */
template<
    typename SieveType = ntlib::sieve<>,
    std::size_t SEGMENT_SIZE = (1 << 18),
    typename Allocator = std::allocator<std::size_t>>
SieveType prime_sieve(std::size_t N, std::vector<std::size_t> &primes) {
  return eratosthenes_segmented<SieveType, SEGMENT_SIZE, Allocator>(N, primes);
}

}
