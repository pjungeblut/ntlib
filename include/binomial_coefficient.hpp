#pragma once

/**
 * Implementations to compute binomial coefficients.
 */

#include "include/integral.hpp"

namespace ntlib {

/**
 * Computes the binomial coefficient binom(n,k).
 * Counts the number of k-element subsets of an n-element universe.
 * Runtime: O(k)
 *
 * Intermediate results are at most k times as big as the result.
 * Be aware of overflows for large numbers.
 *
 * @param n Size of universe.
 * @param k Size of the subsets.
 * @return The number of k-element subsets of {1,...,n}.
 */
template<UnsignedIntegral U>
U binom(U n, U k) {
  U binom = 1;
  for (U i = 1; i <= k; ++i) {
    binom *= n--;
    binom /= i;
  }
  return binom;
}

}
