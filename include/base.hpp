#pragma once

/**
 * Contains base functionality.
 */

#include "integral.hpp"

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
I abs(I n);

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
I gcd(I a, I b);

/**
 * Computes the least common multiple of a and b.
 * Runtime: O(log a + log b)
 *
 * @param a The first number.
 * @param b The second number.
 * @return The least common multiple of a and b.
 */
template<Integral I>
I lcm(I a, I b);

/**
 * Extended Euclidean Algorithm.
 * Finds whole number solutions for a*x + b*y = gcd(a,b).
 *
 * TODO: Find out minimality of solution.
 *
 * @param a Parameter a. Must be non-negative.
 * @param b Parameter b. Must be non-negative.
 * @return A triple (gcd(a,b), x, y).
 */
template<Integral I>
triple<I,I,I> extended_euclid(I a, I b);

/**
 * Computes a^b using binary exponentation.
 * Runtime: O(log b)
 *
 * @param a The base.
 * @param b The exponent, non-negative.
 * @return a^b
 */
template<Integral I>
I pow(I a, I b);

/**
 * Computes the integer square root using binary search.
 * isqrt(n) := floor(sqrt(n))
 * Runtime: O(log n)
 *
 * @param n The number to compute the integer square root of.
 * @return isqrt(n)
 */
template<Integral I>
I isqrt(I n);

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
bool is_square(I n);

/**
 * Computes a^b mod n using binary exponentation.
 * Runtime: O(log b)
 *
 * @param a The base.
 * @param b The exponent.
 * @param n The modulus.
 * @return a^b mod n
 */
template<Integral I>
I mod_pow(I a, I b, I n);

/**
 * Computes the multiplicative inverse of n (mod m).
 * Only exists, if gcd(n, m) = 1. This condition is not checked inside the
 * function.
 *
 * @param n The number to invert.
 * @param m The size of the group.
 * @return The multiplicative inverse of n (mod m).
 */
template<Integral I>
I mod_mult_inv(I n, I m);

/**
 * Tests, if n is a quadratic residue mod p.
 * Uses Euler's Criterion to compute Legendre Symbol (a/p).
 *
 * @param n The number to test.
 * @return True, if and only if there is an x, such that x^2 = a (mod p).
 */
template<Integral I>
bool mod_is_square(I n, I p);

/**
 * Computes square roots modular a prime number, that is an integer solution for
 * x in the equation x^2 = n (mod p).
 * Uses the Tonelli-Shankes algorithm.
 *
 * @param n Parameter n. 0 <= n < p.
 * @param p Odd prime number p > 2.
 * @return 0 <= x < p such that x^2 = n (mod p). There are two solutions, the
 *         other one is p - x. Returns the smaller one.
 */
template<Integral I>
I mod_sqrt(I n, I p);

}
