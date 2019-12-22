#pragma once

/**
 * Implementations to generate polygonal numbers.
 */

#include "integral.hpp"

namespace ntlib {

/**
 * Computes the n-th triangle number.
 *
 * @param n Which triangle number to gnerate.
 * @return The n-th triangle number.
 */
template<Integral I>
I triangle_number(I n) {
  return n * (n + 1) / 2;
}

/**
 * Computes the n-th square number.
 *
 * @param n Which square number to gnerate.
 * @return The n-th square number.
 */
template<Integral I>
I square_number(I n) {
  return n * n;
}

/**
 * Computes the n-th pentagonal number.
 *
 * @param n Which pentagonal number to gnerate.
 * @return The n-th pentagonal number.
 */
template<Integral I>
I pentagonal_number(I n) {
  return n * (3 * n - 1) / 2;
}

/**
 * Computes the n-th hexagonal number.
 *
 * @param n Which hexagonal number to gnerate.
 * @return The n-th hexagonal number.
 */
template<Integral I>
I hexagonal_number(I n) {
  return n * (2 * n - 1);
}

/**
 * Computes the n-th heptagonal number.
 *
 * @param n Which heptagonal number to gnerate.
 * @return The n-th heptagonal number.
 */
template<Integral I>
I heptagonal_number(I n) {
  return n * (5 * n - 3) / 2;
}

/**
 * Computes the n-th octagonal number.
 *
 * @param n Which octagonal number to gnerate.
 * @return The n-th octagonal number.
 */
template<Integral I>
I octagonal_number(I n) {
  return n * (3 * n - 2);
}

}
