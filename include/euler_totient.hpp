#pragma once

#include <vector>

#include "integral.hpp"

namespace ntlib {

/**
 * Computes the value of Euler's totient function phi(i) for all numbers n
 * in [1..N].
 * Runtime: O(N log log N)
 *
 * @param N The number until which all values should be computed.
 * @param sieve The sieve to put the values into.
 */
template<Integral I>
void sieve_phi(I N, std::vector<I> &sieve) {
  sieve.resize(N + 1);
  for (I i = 1; i <= N; ++i) sieve[i] = i;
  for (I i = 2; i <= N; ++i) {
    if (sieve[i] == i) {
      for (I j = i; j <= N; j += i) {
        sieve[j] /= i;
        sieve[j] *= i - 1;
      }
    }
  }
}

}
