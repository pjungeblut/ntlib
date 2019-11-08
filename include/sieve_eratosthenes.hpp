#pragma once

/**
 * Implemenatations for the Sieve of Eratosthenes.
 *
 * File contains three different implementations:
 * Classic: Basic sieve functionality. The sieve is accessible afterwards. Can
 *          be used for fast 'is_prime' lookup.
 * List: Like the classic variant but also fills a vector with all primes in
 *       ascending order. The sieve stays accessible afterwards.
 * Segmented: Like the generate variant. Only used an internal sieve, so it does
 *            not stay accessible afterwards. Memory efficient, since complexity
 *            is pi(N).
 *            Seems to be the fastest for all tested inputs.
 */

#include <algorithm>
#include <cstddef>
#include <vector>

#include "include/integral.hpp"

namespace ntlib {

/**
 * Identify all prime numbers up to N.
 * Runtime: O(N log log N)
 *
 * @param N Identify all primes up to N.
 * @param sieve The vector to be used as a sieve.
 */
template<Integral I>
void sieve_eratosthenes(I N, std::vector<bool> &sieve) {
  sieve.assign(N + 1, true);
  sieve[0] = 0;
  if (N >= 1) sieve[1] = 0;
  for (I i = 4; i <= N; i += 2) sieve[i] = 0;
  for (I i = 3; i * i <= N; i += 2) {
    if (sieve[i]) {
      for (I j = i * i; j <= N; j += i) sieve[j] = 0;
    }
  }
}

/**
 * Identifies all prime numbers up to N and fills an array.
 * Runtime: O(N log log N)
 *
 * @param N Identify all primes up to N.
 * @param sieve The array to be used as a sieve.
 * @param primes The array to write the primes to.
 */
template<Integral I>
void sieve_eratosthenes_list(I N, std::vector<bool> &sieve,
    std::vector<I> &primes) {
  sieve.assign(N + 1, true);
  sieve[0] = 0;
  if (N >= 1) sieve[1] = 0;
  if (N >= 2) primes.push_back(2);
  for (I i = 4; i <= N; i += 2) sieve[i] = 0;
  I i = 3;
  for (; i * i <= N; i += 2) {
    if (sieve[i]) {
      primes.push_back(i);
      for (I j = i * i; j <= N; j += i) sieve[j] = 0;
    }
  }
  for (; i <= N; i += 2) {
    if (sieve[i]) primes.push_back(i);
  }
}

/**
 * Identifies all prime numbers up to N and fills an array.
 * Segments the array to have better cache utilization.
 * Runtime: O(N log log N)
 *
 * @param N Identify all primes up to N.
 * @param primes The array to write the primes to.
 */
template<Integral I>
void sieve_eratosthenes_list_segmented(I N, std::vector<I> &primes) {
  const I SEGMENT_SIZE = std::min(N, static_cast<I>(1'000'000));
  I mini = 0;
  I maxi = SEGMENT_SIZE;
  std::vector<bool> sieve;

  // Classical sieve for first block.
  sieve_eratosthenes_list(maxi, sieve, primes);

  // Remaining blocks.
  while (maxi != N) {
    mini = maxi + 1;
    maxi = std::min(N, maxi + SEGMENT_SIZE);

    std::fill(sieve.begin(), sieve.end(), 1);
    for (I i = 0; i < primes.size() && primes[i] * primes[i] <= maxi; ++i) {
      I multiple = (mini + primes[i] - 1) / primes[i] * primes[i];
      while (multiple <= maxi) {
        sieve[multiple - mini] = 0;
        multiple += primes[i];
      }
    }
    for (I i = mini; i <= maxi; ++i) {
      if (sieve[i - mini]) primes.push_back(i);
    }
  }
}

}
