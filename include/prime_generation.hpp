#pragma once

/**
 * Implemenatations to generate prime numbers.
 * Some functions generate sieves, some lists and some both.
 */

#include <algorithm>
#include <cstddef>
#include <vector>

namespace ntlib {

/**
 * Identify all prime numbers up to N.
 * Runtime: O(N log log N)
 *
 * @param N Identify all primes up to N.
 * @param sieve The vector to be used as a sieve.
 */
template<typename T>
void eratosthenes_sieve_classic(T N, std::vector<bool> &sieve) {
  sieve.assign(N + 1, true);
  sieve[0] = 0;
  if (N >= 1) sieve[1] = 0;
  for (T i = 4; i <= N; i += 2) sieve[i] = 0;
  for (T i = 5; i * i <= N; i += 2) {
    if (sieve[i]) {
      for (T j = i * i; j <= N; j += 2 * i) sieve[j] = 0;
    }
  }
}

/**
 * Identify all prime numbers up to N.
 * Runtime: O(N log log N)
 *
 * @param N Identify all primes up to N.
 * @param sieve The vector to be used as a sieve.
 */
template<typename T>
void eratosthenes_sieve(T N, std::vector<bool> &sieve) {
  sieve.clear();
  sieve.reserve(N + 1);
  sieve.push_back(0); // 0
  sieve.push_back(0); // 1
  sieve.push_back(1); // 2
  sieve.push_back(1); // 3
  sieve.push_back(0); // 4
  for (std::size_t i = 5; i <= N; i += 6) {
    sieve.push_back(1);
    sieve.push_back(0);
    sieve.push_back(1);
    sieve.push_back(0);
    sieve.push_back(0);
    sieve.push_back(0);
  }
  for (T i = 5; i * i <= N; i += 4) {
    if (sieve[i]) for (T j = i * i; j <= N; j += 2 * i) sieve[j] = 0;
    i += 2;
    if (sieve[i]) for (T j = i * i; j <= N; j += 2 * i) sieve[j] = 0;
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
template<typename T>
void eratosthenes_sieve_list(T N, std::vector<bool> &sieve,
    std::vector<T> &primes) {
  sieve.assign(N + 1, true);
  sieve[0] = 0;
  if (N >= 1) sieve[1] = 0;
  if (N >= 2) primes.push_back(2);
  for (T i = 4; i <= N; i += 2) sieve[i] = 0;
  T i = 3;
  for (; i * i <= N; i += 2) {
    if (sieve[i]) {
      primes.push_back(i);
      for (T j = i * i; j <= N; j += i) sieve[j] = 0;
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
template<typename T>
void eratosthenes_list(T N, std::vector<T> &primes) {
  const T SEGMENT_SIZE = std::min(N, static_cast<T>(1'000'000));
  T mini = 0;
  T maxi = SEGMENT_SIZE;
  std::vector<bool> sieve;

  // Classical sieve for first block.
  eratosthenes_sieve_list(maxi, sieve, primes);

  // Remaining blocks.
  while (maxi != N) {
    mini = maxi + 1;
    maxi = std::min(N, maxi + SEGMENT_SIZE);

    std::fill(sieve.begin(), sieve.end(), 1);
    for (T i = 0; i < primes.size() && primes[i] * primes[i] <= maxi; ++i) {
      T multiple = (mini + primes[i] - 1) / primes[i] * primes[i];
      while (multiple <= maxi) {
        sieve[multiple - mini] = 0;
        multiple += primes[i];
      }
    }
    for (T i = mini; i <= maxi; ++i) {
      if (sieve[i - mini]) primes.push_back(i);
    }
  }
}

/**
 * Identify all prime numbers up to N using the sieve of Sundaram.
 * Runtime: O(N log log N)
 *
 * Slow and it is easy to get overflows.
 *
 * @param N Identify all primes up to N.
 * @param sieve The vector to be used as a sieve.
 */
template<typename T>
void sundaram_sieve(T N, std::vector<bool> &sieve) {
  T K = (N - 2) / 2;
  sieve.reserve(N + 1);
  sieve.assign(K + 1, true);
  sieve.resize(N + 1, false);

  for (T i = 1; i <= K; ++i) {
    for (T j = i; i + j + 2 * i * j <= K; ++j) {
      sieve[i + j + 2 * i * j] = false;
    }
  }
  for (T i = K; i > 0; --i) {
    if (sieve[i]) {
      sieve[2 * i + 1] = true;
      sieve[i] = false;
    }
  }
  sieve[0] = 0;
  if (N >= 2) sieve[2] = 1;
}

}