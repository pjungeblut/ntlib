#pragma once

/**
 * Contains base functionality.
 */

namespace ntlib {

/**
 * A tuple of two elements.
 */
template<typename T>
struct tuple {
  T a;
  T b;
};

/**
 * A triple of three elements.
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
 * Uses the Euclidean Algorithm.
 * Runtime: O(log a + log b).
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
 * Runtime: O(log a + log b)
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

/**
 * Computes the integer square root using binary search.
 * isqrt(n) := floor(sqrt(n))
 * Runtime: O(log n)
 *
 * @param n The number to compute the integer square root of.
 * @return isqrt(n)
 */
template<typename NumberType>
NumberType isqrt(NumberType n) {
  NumberType l = 0;
  NumberType u = static_cast<NumberType>(1) << (4 * sizeof(NumberType));
  while (u - l > 1) {
    NumberType m = (u + l) / 2;
    if (m * m <= n) l = m;
    else u = m;
  }
  return l;
}

/**
 * Tests, if n is a perfect square.
 * Runtime: O(log n)
 *
 * Uses ideas from
 * https://math.stackexchange.com/questions/131330/detecting-perfect-squares-faster-than-by-extracting-square-root/712818#712818
 * to identify non-squares quickly.
 *
 * @param n The number to test.
 * @return True, iff n is a perfect square.
 */
template<typename NumberType>
bool is_square(NumberType n) {
  if (n == 0) return true;

  NumberType last_digit = n % 10;
  NumberType second_last_digit = n / 10 % 10;
  NumberType third_last_digit = n / 100 % 10;

  // If n is a multiple of four, we can look at n/4 instead.
  while ((n & 3) == 0) n >>= 2;

  // If n is not divisible by four (it is not by above test), its binary
  // representation must end with 001.
  if (!((n & 7) == 1)) return false;

  // All squares end in the numbers 0, 1, 4, 5, 6, or 9.
  if (last_digit == 2 || last_digit == 3 || last_digit == 7 || last_digit == 8) return false;

  // The last two digits cannot both be odd.
  if ((last_digit & 1) && (second_last_digit & 1)) return false;

  // If the last digit is 1 or 9, the two digits befor must be a multiple of 4.
  if (last_digit == 1 || last_digit == 9) {
    if ((third_last_digit * 10 + second_last_digit) % 4 != 0) return false;
  }

  // If the last digit is 4, the digit before it must be even.
  if (last_digit == 4 && (second_last_digit & 1)) return false;

  // If the last digit is 6, the digit before it must be odd.
  if (last_digit == 6 && !(second_last_digit & 1)) return false;

  // If the last digit is 5, the digit before it must be 2.
  if (last_digit == 5 && second_last_digit != 2) return false;

  // Take the integer root and square it to check, if the real root is an integer.
  NumberType iroot = isqrt(n);
  return iroot * iroot == n;
}

}
