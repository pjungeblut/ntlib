#pragma once

/**
 * Implementations to compute binomial coefficients.
 */

namespace ntlib {

/**
 * Computes the binomial coefficient binom(n,k).
 * Counts the number of k-element subsets of an n-element universe.
 *
 * @tparam NumberType Integral data type used for natural numbers.
 *
 * @param n Size of universe.
 * @param k Size of the subsets.
 * @return The number of k-element subsets of {1,...,n}.
 */
template<typename NumberType>
NumberType binom(NumberType n, NumberType k) {
  NumberType binom = 1;
  for (NumberType i = 1; i <= k; ++i) {
    binom *= n--;
    binom /= i;
  }
  return binom;
}

}
