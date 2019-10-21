#pragma once

/**
 * Implemenatations for the Sieve of Eratosthenes.
 *
 * File contains four different implementations:
 * Classic: Basic sieve functionality. The sieve is accessible afterwards. Can
 *          be used for fast 'is_prime' lookup.
 * Generate: Like the classic variant but also fills a vector with all primes
 *           in ascending order. The sieve stays accessible afterwards.
 * Linear: Like the generate variant but works in linear time. The advantage
 *         seems to be mostly theoretical, as N needs to get VERY big until this
 *         becomes the fastest option. The sieve stays accessible afterwards.
 * Segmented: Like the generate variant. Only used an internal sieve, so it does
 *            not stay accessible afterwards. Memory efficient, since complexity
 *            is pi(N).
 *            Seems to be the fastest for all tested inputs.
 */

#include <algorithm>
#include <cstddef>
#include <vector>

namespace ntlib {

/**
 * Identify all prime numbers up to N.
 * Runtime: O(N log log N)
 *
 * @param N Identify all primes up to N. Sieve array must be at least N+1
 *          elements big.
 * @param sieve The sieve array. Must be initialized with true.
 */
void sieve_eratosthenes_classic(std::size_t N, std::vector<bool> &sieve) {
  sieve[0] = 0;
  if (N >= 1) sieve[1] = 0;
  for (std::size_t i = 4; i <= N; i += 2) sieve[i] = 0;
  for (std::size_t i = 3; i * i <= N; i += 2) {
    if (sieve[i]) {
      for (std::size_t j = i * i; j <= N; j += i) {
        sieve[j] = 0;
      }
    }
  }
}

/**
 * Identifies all prime numbers up to N and fills an array.
 * Runtime: O(N log log N)
 *
 * @tparam NumberType Integral data type used for natural numbers.
 *
 * @param N Identify all primes up to N. Sieve array must be at least N+1
 *          elements big.
 * @param sieve The sieve array. Must be initialized with true.
 * @param primes The array to write the primes to.
 */
template<typename NumberType>
void sieve_eratosthenes_generate(std::size_t N, std::vector<bool> &sieve, std::vector<NumberType> &primes) {
  sieve[0] = 0;
  if (N >= 1) sieve[1] = 0;
  if (N >= 2) primes.push_back(2);
  for (std::size_t i = 4; i <= N; i += 2) sieve[i] = 0;
  std::size_t i = 3;
  for (; i * i <= N; i += 2) {
    if (sieve[i]) {
      primes.push_back(i);
      for (std::size_t j = i * i; j <= N; j += i) {
        sieve[j] = 0;
      }
    }
  }
  for (; i <= N; i += 2) {
    if (sieve[i]) {
      primes.push_back(i);
    }
  }
}

/**
 * Identifies all prime numbers up to N and fills an array.
 * Runtime: O(N)
 *
 * @tparam NumberType Integral data type used for natural numbers.
 *
 * @param N Identify all primes up to N. Sieve array must be at least N+1
 *          elements big.
 * @param sieve The sieve array. Must be initialized with true.
 * @param primes The array to write the primes to.
 */
template<typename NumberType>
void sieve_eratosthenes_generate_linear(std::size_t N, std::vector<bool> &sieve, std::vector<NumberType> &primes) {
  sieve[0] = 0;
  if (N >= 1) sieve[1] = 0;

  for (std::size_t i = 2; i <= N; ++i) {
    if (sieve[i]) primes.push_back(i);
    for (std::size_t j = 0; j < primes.size() && i * primes[j] <= N; ++j) {
      sieve[i * primes[j]] = 0;
      if (i % primes[j] == 0) break;
    }
  }
}

/**
 * Identifies all prime numbers up to N and fills an array.
 * Segments the array to have better cache utilization.
 * Runtime: O(N log log N)
 *
 * @tparam NumberType Integral data type used for natural numbers.
 * @tparam SEGMENT_SIZE The size of each segment.
 *
 * @param N Identify all primes up to N. Sieve array must be at least N+1
 *          elements big.
 * @param primes The array to write the primes to.
 */
template<typename NumberType, std::size_t SEGMENT_SIZE = 1'000'000>
void sieve_eratosthenes_generate_segmented(std::size_t N, std::vector<NumberType> &primes) {
  std::vector<bool> sieve(SEGMENT_SIZE + 1, 1);
  std::size_t mini = 0;
  std::size_t maxi = std::min(N, SEGMENT_SIZE);

  // Classical sieve for first block.
  sieve_eratosthenes_generate(maxi, sieve, primes);

  // Remaining blocks.
  while (maxi != N) {
    mini = maxi + 1;
    maxi = std::min(N, maxi + SEGMENT_SIZE);

    std::fill(sieve.begin(), sieve.end(), 1);
    for (std::size_t i = 0; i < primes.size() && primes[i] * primes[i] <= maxi; ++i) {
      NumberType multiple = (mini + primes[i] - 1) / primes[i] * primes[i];
      while (multiple <= maxi) {
        sieve[multiple - mini] = 0;
        multiple += primes[i];
      }
    }
    for (std::size_t i = mini; i <= maxi; ++i) {
      if (sieve[i - mini]) primes.push_back(i);
    }
  }
}

}
