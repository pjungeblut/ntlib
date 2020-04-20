#pragma once

/**
 * Implementations to compute binomial coefficients.
 */
#include <cassert>

#include "integral.hpp"

namespace ntlib {

/**
 * Computes the binomial coefficient binom(n,k).
 * Runtime: O(k)
 *
 * Intermediate results are at most k times as big as the result.
 * Be aware of overflows for large numbers.
 *
 * @param n Size of universe.
 * @param k Size of the subsets.
 * @return The number of k-element subsets of {1,...,n}.
 */
template<Integral I>
I binom(I n, I k) {
  assert(n >= 0);

  if (k < 0 || k > n) return 0;

  I binom = 1;
  for (I i = 1; i <= k; ++i) {
    binom *= n--;
    binom /= i;
  }
  return binom;
}

/**
 * Computes a table of all binomial coefficiets binom(i,j) with 0 <= i,j <= N.
 * Intermediate results do not exceed results.
 * Runtime: O(N^2)
 *
 * @param N The parameter N.
 * @param binoms Two-dimensional vector to store the binomial coefficients.
 */
template<Integral I>
void binom_table(I N, std::vector<std::vector<I>> &binoms) {
  assert(N >= 0);

  binoms.assign(N + 1, std::vector<I>(N + 1, 0));
  binoms[0][0] = 1;
  for (I n = 1; n <= N; ++n) {
    binoms[n][0] = 1;
    for (I k = 1; k <= n; ++k) {
      binoms[n][k] = binoms[n - 1][k - 1] + binoms[n - 1][k];
    }
  }
}

/**
 * Computes a table of all binomial coefficients binom(i,j) mod m with
 * 0 <= i,j <= N.
 * Runtime: O(N^2)
 *
 * @param N The parameter N.
 * @param m The modulus.
 * @param binoms Two-dimensional vector to store the binomial coefficients.
 */
template<Integral I>
void mod_binom_table(I N, I m, std::vector<std::vector<I>> &binoms) {
  assert(N >= 0);
  assert(m > 0);

  binoms.assign(N + 1, std::vector<I>(N + 1, 0));
  binoms[0][0] = 1;
  for (I n = 1; n <= N; ++n) {
    binoms[n][0] = 1;
    for (I k = 1; k <= n; ++k) {
      binoms[n][k] = (binoms[n - 1][k - 1] + binoms[n - 1][k]) % m;
    }
  }
}

}
