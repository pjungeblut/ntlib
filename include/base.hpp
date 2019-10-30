#pragma once

/**
 * Contains base functionality.
 */

#include <type_traits>

#include "include/integral.hpp"

namespace ntlib {

/**
 * A tuple of two elements.
 */
template<typename T1, typename T2>
struct tuple {
  T1 a;
  T2 b;
};

/**
 * A triple of three elements.
 */
template<typename T1, typename T2, typename T3>
struct triple {
  T1 a;
  T2 b;
  T3 c;
};

/**
 * Computes the absolute value of a number.
 *
 * @param n The number to take the absolute value of.
 * @return The absolute value of n. Will be an unsigned integer.
 */
template<Integral I>
auto abs(I n) -> std::make_unsigned<I>::type {
  using return_type = std::make_unsigned<I>::type;
  if (n >= 0) return static_cast<return_type>(n);
  return static_cast<return_type>(-n);
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
template<Integral I>
auto gcd(I a, I b) -> std::make_unsigned<I>::type {
  return b == 0 ? abs(a) : gcd(b, a % b);
}

/**
 * Computes the least common multiple of a and b.
 * Runtime: O(log a + log b)
 *
 * @param a The first number.
 * @param b The second number.
 * @return The least common multiple of a and b.
 */
template<Integral I>
auto lcm(I a, I b) -> std::make_unsigned<I>::type {
  typename std::make_unsigned<I>::type d = gcd(a, b);
  return abs(a) * (abs(b) / d);
}

/**
 * Computes a^b using binary exponentation.
 * Runtime: O(log b)
 *
 * @param a The base.
 * @param b The exponent, non-negative.
 * @return a^b
 */
template<Integral I, UnsignedIntegral U>
I pow(I a, U b) {
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
template<UnsignedIntegral U>
U mod_pow(U a, U b, U n) {
  if (b == 0) return 1;
  if (b == 1) return a % n;
  if (b & 1) return (mod_pow(a, b - 1, n) * a) % n;
  return mod_pow((a * a) % n, b / 2, n);
}

/**
 * Extended Euclidean Algorithm.
 * Finds whole number solutions for a*x + b*y = gcd(a,b).
 *
 * TODO: Find out minimality of solution.
 * TODO: Rewrite to return triple (gcd(a,b),x,y).
 *
 * @param a Parameter a. Must be non-negative.
 * @param b Parameter b. Must be non-negative.
 * @return A triple (gcd(a,b), x, y).
 */
template<UnsignedIntegral U>
auto extended_euclid(U a, U b) ->
    triple<U, typename std::make_signed<U>::type, typename std::make_signed<U>::type> {
  using I = std::make_signed<U>::type;

  if (a == 0) return triple<U,I,I> {b, 0, 1};
  triple<U,I,I> gxy = extended_euclid(b % a, a);
  return triple<U,I,I> {
      gxy.a,
      static_cast<I>(gxy.c) - static_cast<I>((b / a)) * static_cast<I>(gxy.b),
      gxy.b};
}

/**
 * Computes the multiplicative inverse of n (mod m).
 * Only exists, if gcd(n, m) = 1. This condition is not checked inside the
 * function.
 *
 * @param n The number to invert.
 * @param m The size of the group.
 * @return The multiplicative inverse of n (mod m).
 */
template<UnsignedIntegral U>
U mod_mult_inv(U n, U m) {
  using I = std::make_signed<U>::type;
  auto gxy = extended_euclid(n, m);
  return ((gxy.b % static_cast<I>(m)) + m) % m;
}

/**
 * Computes the integer square root using binary search.
 * isqrt(n) := floor(sqrt(n))
 * Runtime: O(log n)
 *
 * @param n The number to compute the integer square root of.
 * @return isqrt(n)
 */
template<UnsignedIntegral U>
U isqrt(U n) {
  U l = 0;
  U u = static_cast<U>(1) << (4 * sizeof(U));
  while (u - l > 16) {
    U m = (u + l) / 2;
    if (m * m <= n) l = m;
    else u = m;
  }
  while (l * l <= n) ++l;
  return l - 1;
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
template<Integral I>
bool is_square(I n) {
  if (n < 0) return false;
  if (n == 0) return true;

  I last_digit = n % 10;
  I second_last_digit = n / 10 % 10;
  I third_last_digit = n / 100 % 10;

  // If n is a multiple of four, we can look at n/4 instead.
  while ((n & 3) == 0) n >>= 2;

  // If n is not divisible by four (it is not by above test), its binary
  // representation must end with 001.
  if (!((n & 7) == 1)) return false;

  // All squares end in the numbers 0, 1, 4, 5, 6, or 9.
  if (last_digit == 2 || last_digit == 3 || last_digit == 7 ||
      last_digit == 8) {
    return false;
  }

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

  // Take the integer root and square it to check, if the real root is an
  // integer.
  I iroot = isqrt(n);
  return iroot * iroot == n;
}

/**
 * Tests, if n is a quadratic residue mod p.
 * Uses Euler's Criterion to compute Legendre Symbol (a/p).
 *
 * @param n The number to test.
 * @param p A prime number.
 * @return True, if and only if there is an x, such that x^2 = a (mod p).
 */
template<UnsignedIntegral U>
bool mod_is_square(U n, U p) {
  if (p == 2) return n & 1;
  return mod_pow(n, (p - 1) / 2) == 1;
}

}
