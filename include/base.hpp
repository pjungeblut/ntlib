#pragma once

/**
 * Contains base functionality.
 */

namespace ntlib {

/**
 * A triple of three elements.
 *
 * @tparam T Data type for triple elements.
 */
template<typename T>
struct triple {
  T a;
  T b;
  T c;
};

/**
 * Computes the absolute value of a number.
 *
 * @tparam NumberType Integral data type used for whole numbers.
 *
 * @param n The number to take the absolute value of.
 * @return The absolute value of n.
 */
template<typename NumberType>
NumberType abs(NumberType n) {
  if (n >= 0) return n;
  return -n;
}

/**
 * Computes the greatest common divisor of a and b.
 *
 * @tparam NumberType Integral data type used for whole numbers.
 *
 * @param a The first number.
 * @param b The second number.
 * @return The greatest common divisor of a and b.
 */
template<typename NumberType>
NumberType gcd(NumberType a, NumberType b) {
  return b == 0 ? a : gcd(b, a % b);
}

/**
 * Computes the least common multiple of a and b.
 *
 * @tparam NumberType Integral data type used for whole numbers.
 *
 * @param a The first number.
 * @param b The second number.
 * @return The least common multiple of a and b.
 */
template<typename NumberType>
NumberType lcm(NumberType a, NumberType b) {
  return a * (b / gcd(a, b));
}

/**
 * Computes a^b using binary exponentation.
 * Runtime: O(log b)
 *
 * @tparam NumberType Integral data type used for whole numbers.
 *
 * @param a The base.
 * @param b The exponent.
 * @return a^b
 */
template<typename NumberType>
NumberType pow(NumberType a, NumberType b) {
  if (b == 0) return 1;
  if (b == 1) return a;
  if (b & 1) return pow(a, b - 1) * a;
  return pow(a * a, b / 2);
}

/**
 * Computes a^b mod n using binary exponentation.
 * Runtime: O(log b)
 *
 * @tparam NumberType Integral data type used for whole numbers.
 *
 * @param a The base.
 * @param b The exponent.
 * @param n The modulus.
 * @return a^b mod n
 */
template<typename NumberType>
NumberType mod_pow(NumberType a, NumberType b, NumberType n) {
  if (b == 0) return 1;
  if (b == 1) return a % n;
  if (b & 1) return (mod_pow(a, b - 1, n) * a) % n;
  return mod_pow((a * a) % n, b / 2, n);
}

}
