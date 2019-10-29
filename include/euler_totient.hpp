#pragma once

#include <vector>

#include "include/integral.hpp"

namespace ntlib {

/**
 * Computes the value of Euler's totient function phi(i) for all numbers n
 * in [1..N].
 * Runtime: O(N log log N)
 *
 * @param N The number until which all values should be computed.
 * @param sieve The sieve to put the values into.
 */
template<UnsignedIntegral U>
void sieve_phi(U N, std::vector<U> &sieve) {
  sieve.resize(N + 1);
  for (U i = 1; i <= N; ++i) sieve[i] = i;
  for (U i = 2; i <= N; ++i) {
    if (sieve[i] == i) {
      for (U j = i; j <= N; j += i) {
        sieve[j] /= i;
        sieve[j] *= i - 1;
      }
    }
  }
}

}
