#pragma once

/**
 * Implementations to generate polygonal numbers.
 */

namespace ntlib {

/**
 * Computes the n-th triangle number.
 *
 * @tparam NumberType Integral data type used for natural numbers.
 *
 * @param n Which triangle number to gnerate.
 * @return The n-th triangle number.
 */
template<typename NumberType>
NumberType triangle_number(NumberType n) {
  return n * (n + 1) / 2;
}

/**
 * Computes the n-th square number.
 *
 * @tparam NumberType Integral data type used for natural numbers.
 *
 * @param n Which square number to gnerate.
 * @return The n-th square number.
 */
template<typename NumberType>
NumberType square_number(NumberType n) {
  return n * n;
}

/**
 * Computes the n-th pentagonal number.
 *
 * @tparam NumberType Integral data type used for natural numbers.
 *
 * @param n Which pentagonal number to gnerate.
 * @return The n-th pentagonal number.
 */
template<typename NumberType>
NumberType pentagonal_number(NumberType n) {
  return n * (3 * n - 1) / 2;
}

/**
 * Computes the n-th hexagonal number.
 *
 * @tparam NumberType Integral data type used for natural numbers.
 *
 * @param n Which hexagonal number to gnerate.
 * @return The n-th hexagonal number.
 */
template<typename NumberType>
NumberType hexagonal_number(NumberType n) {
  return n * (2 * n - 1);
}

/**
 * Computes the n-th heptagonal number.
 *
 * @tparam NumberType Integral data type used for natural numbers.
 *
 * @param n Which heptagonal number to gnerate.
 * @return The n-th heptagonal number.
 */
template<typename NumberType>
NumberType heptagonal_number(NumberType n) {
  return n * (5 * n - 3) / 2;
}

/**
 * Computes the n-th octagonal number.
 *
 * @tparam NumberType Integral data type used for natural numbers.
 *
 * @param n Which octagonal number to gnerate.
 * @return The n-th octagonal number.
 */
template<typename NumberType>
NumberType octagonal_number(NumberType n) {
  return n * (3 * n - 2);
}

}
