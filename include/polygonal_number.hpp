#pragma once

/**
 * Implementations to generate polygonal numbers.
 */

#include "include/integral.hpp"

namespace ntlib {

/**
 * Computes the n-th triangle number.
 *
 * @param n Which triangle number to gnerate.
 * @return The n-th triangle number.
 */
template<UnsignedIntegral U>
U triangle_number(U n) {
  return n * (n + 1) / 2;
}

/**
 * Computes the n-th square number.
 *
 * @param n Which square number to gnerate.
 * @return The n-th square number.
 */
template<UnsignedIntegral U>
U square_number(U n) {
  return n * n;
}

/**
 * Computes the n-th pentagonal number.
 *
 * @param n Which pentagonal number to gnerate.
 * @return The n-th pentagonal number.
 */
template<UnsignedIntegral U>
U pentagonal_number(U n) {
  return n * (3 * n - 1) / 2;
}

/**
 * Computes the n-th hexagonal number.
 *
 * @param n Which hexagonal number to gnerate.
 * @return The n-th hexagonal number.
 */
template<UnsignedIntegral U>
U hexagonal_number(U n) {
  return n * (2 * n - 1);
}

/**
 * Computes the n-th heptagonal number.
 *
 * @param n Which heptagonal number to gnerate.
 * @return The n-th heptagonal number.
 */
template<UnsignedIntegral U>
U heptagonal_number(U n) {
  return n * (5 * n - 3) / 2;
}

/**
 * Computes the n-th octagonal number.
 *
 * @param n Which octagonal number to gnerate.
 * @return The n-th octagonal number.
 */
template<UnsignedIntegral U>
U octagonal_number(U n) {
  return n * (3 * n - 2);
}

}
