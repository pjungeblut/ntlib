#pragma once

#include <algorithm>
#include <random>

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
I abs(I n) {
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
template<Integral I>
I gcd(I a, I b) {
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
I lcm(I a, I b) {
  return abs(a) * (abs(b) / gcd(a, b));
}

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
triple<I,I,I> extended_euclid(I a, I b) {
  if (a == 0) return triple<I,I,I> {b, 0, 1};
  triple<I,I,I> gxy = extended_euclid(b % a, a);
  return triple<I,I,I> {gxy.a, gxy.c - (b / a) * gxy.b, gxy.b};
}

/**
 * Computes a^b using binary exponentation.
 * Runtime: O(log b)
 *
 * @param a The base.
 * @param b The exponent, non-negative.
 * @return a^b
 */
template<Integral I>
I pow(I a, I b) {
  if (b == 0) return 1;
  if (b == 1) return a;
  if (b & 1) return pow(a, b - 1) * a;
  return pow(a * a, b / 2);
}

/**
 * Computes the integer square root using binary search.
 * isqrt(n) := floor(sqrt(n))
 * Runtime: O(log n)
 *
 * @param n The number to compute the integer square root of.
 * @return isqrt(n)
 */
template<Integral I>
I isqrt(I n) {
  I l = 0;
  I u = n;
  while (u - l > 16) {
    I m = (u + l) / 2;
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
  while (n && (n & 3) == 0) n >>= 2;
  if (n == 0) return true;

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
 * Computes a^b mod n using binary exponentation.
 * Runtime: O(log b)
 *
 * @param a The base.
 * @param b The exponent.
 * @param n The modulus.
 * @return a^b mod n
 */
template<Integral I>
I mod_pow(I a, I b, I n) {
  if (b == 0) return 1;
  if (b == 1) return a % n;
  if (b & 1) return (mod_pow(a, b - 1, n) * a) % n;
  return mod_pow((a * a) % n, b / 2, n);
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
template<Integral I>
I mod_mult_inv(I n, I m) {
  auto gxy = extended_euclid(n, m);
  return ((gxy.b % m) + m) % m;
}

/**
 * Tests, if n is a quadratic residue mod p.
 * Uses Euler's Criterion to compute Legendre Symbol (a/p).
 *
 * @param n The number to test.
 * @return True, if and only if there is an x, such that x^2 = a (mod p).
 */
template<Integral I>
bool mod_is_square(I n, I p) {
  if (p == 2) return n & 1;
  return mod_pow(n, (p - 1) / 2, p) == 1;
}

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
I mod_sqrt(I n, I p) {
  // Find q, s with p-1 = q*2^s.
  I q = p - 1;
  I s = 0;
  while (!(q & 1)) {
    q /= 2;
    ++s;
  }

  // If and only if s == 1, we have p = 3 (mod 4).
  // In this case we can compute root x directly.
  if (s == 1) return mod_pow(n, (p + 1) / 4, p);

  // Find a quadratic non-residue z.
  // Half the numbers in 1, ..., p-1 are, so we randomly guess.
  // On average, two tries are necessary.
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, p - 1);
  I z = dis(gen);
  while (mod_is_square(z, p)) z = dis(gen);

  I c = mod_pow(z, q, p);
  I x = mod_pow(n, (q + 1) / 2, p);
  I t = mod_pow(n, q, p);
  I m = s;

  while (t % p != 1) {
    // Find lowest 0 < i < m such that t^2^i = 1 (mod p).
    I i = 0;
    I test = t;
    while (test != 1) {
      test = test * test % p;
      ++i;
    }

    // U cexp = mod_pow(static_cast<U>(2), m - i - 1, p - 1);
    I cexp = static_cast<I>(1) << (m - i - 1);
    I b = mod_pow(c, cexp, p);
    x = x * b % p;
    t = t * b % p * b % p;
    c = b * b % p;
    m = i;
  }

  return std::min(x, p - x);
}

}
