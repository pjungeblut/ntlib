#pragma once

/**
 * Implementations to compute binomial coefficients.
 */
#include <cassert>
#include <vector>

namespace ntlib {

/**
 * Computes the binomial coefficient binom(n,k).
 * Runtime: O(k)
 *
 * Intermediate results are at most k times as big as the result.
 *
 * @param n Size of universe.
 * @param k Size of the subsets.
 * @return The number of k-element subsets of {1,...,n}.
 */
template<typename N, typename K>
std::common_type_t<N,K> binom(N n, K k) {
  assert(n >= 0);

  using C = std::common_type_t<N,K>;

  if (k < 0 || k > n) return C{0};

  C binom = 1;
  for (K i = 1; i <= k; ++i) {
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
template<typename T>
void binom_table(T N, std::vector<std::vector<T>> &binoms) {
  assert(N >= 0);

  binoms.assign(N + 1, std::vector<T>(N + 1, T{0}));
  binoms[0][0] = T {1};
  for (T n = 1; n <= N; ++n) {
    binoms[n][0] = T {1};
    for (T k = 1; k <= n; ++k) {
      binoms[n][k] = binoms[n - 1][k - 1] + binoms[n - 1][k];
    }
  }
}

}
