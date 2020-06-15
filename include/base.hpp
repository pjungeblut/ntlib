#pragma once

#include <algorithm>
#include <cassert>
#include <random>
#include <type_traits>

namespace ntlib {

/**
 * Computes the absolute value of a number.
 *
 * @param n The number to take the absolute value of.
 * @return The absolute value of n. Will be an unsigned integer.
 */
template<typename T>
T abs(T n) {
  return n >= 0 ? n : -n;
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
template<typename T>
T gcd(T a, T b) {
  assert(!(a == 0 && b == 0));
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
template<typename T>
T lcm(T a, T b) {
  assert(a != 0);
  assert(b != 0);
  return abs(a) * (abs(b) / gcd(a, b));
}

/**
 * Extended Euclidean Algorithm.
 * Finds whole number solutions for a*x + b*y = gcd(a,b).
 *
 * @param a Parameter a.
 * @param b Parameter b.
 * @param x Value for x.
 * @param y Value for y.
 * @return The greatest common divisor gcd(a,b) of a and b.
 */
template<typename T>
T extended_euclid_positive(T a, T b, T &x, T &y) {
  if (a == 0) {
    x = 0;
    y = 1;
    return b;
  }
  T xx, yy, gcd = extended_euclid_positive(b % a, a, xx, yy);
  x = yy - (b / a) * xx;
  y = xx;
  return gcd;
}
template<typename T>
T extended_euclid(T a, T b, T &x, T &y) {
  assert(!(a == 0 && b == 0));
  if (a < 0 && b < 0) {
    T gcd = extended_euclid_positive(abs(a), abs(b), x, y);
    x = -x;
    y = -y;
    return gcd;
  } else if (a < 0) {
    T gcd = extended_euclid_positive(abs(a), b, x, y);
    x = -x;
    return gcd;
  } else if (b < 0) {
    T gcd = extended_euclid_positive(a, abs(b), x, y);
    y = -y;
    return gcd;
  } else return extended_euclid_positive(a, b, x, y);
}

/**
 * Computes a^b using binary exponentation.
 * Runtime: O(log b)
 *
 * @param a The base.
 * @param b The exponent, non-negative.
 * @return a^b
 */
template<typename T>
T pow(T a, T b) {
  assert(b >= 0);
  assert(!(a == 0 && b == 0));
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
 * @param n The number to compute the integer square root of. Non-negative.
 * @return isqrt(n)
 */
template<typename T>
T isqrt(T n) {
  assert(n >= 0);

  T u = 1;
  while (u * u <= n) u *= 2;
  T l = u / 2;

  while (u - l > 16) {
    T m = (u + l) / 2;
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
template<typename T>
bool is_square(T n) {
  if (n < 0) return false;
  if (n == 0) return true;

  T last_digit = n % 10;
  T second_last_digit = n / 10 % 10;
  T third_last_digit = n / 100 % 10;

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
  T iroot = isqrt(n);
  return iroot * iroot == n;
}

/**
 * Computes the factorial n!.
 *
 * @param n The base.
 * @return The factorial n!.
 */
template<typename T>
T factorial(T n) {
  assert(n >= 0);
  T result = 1;
  for (T i = 2; i <= n; ++i) result *= i;
  return result;
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
template<typename T>
T mod_pow(T a, T b, T n) {
  assert(!(a == 0 && b == 0));
  assert(a >= 0);
  assert(b >= 0);
  assert(n > 0);
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
template<typename T>
T mod_mult_inv(T n, T m) {
  assert(gcd(n, m) == 1);
  using ST = typename std::make_signed<T>::type;
  ST x, y;
  extended_euclid(static_cast<ST>(n), static_cast<ST>(m), x, y);
  return x >= 0 ? x % m : m - (-x % m);
}

/**
 * Tests, if n is a quadratic residue mod p.
 * Uses Euler's Criterion to compute Legendre Symbol (a/p).
 *
 * TODO: Add assert that p is a prime number.
 *
 * @param n The number to test.
 * @param p The modulus. Must be prime number.
 * @return True, if and only if there is an x, such that x^2 = a (mod p).
 */
template<typename T>
bool mod_is_square(T n, T p) {
  if (n == 0) return true;
  if (p == 2) return true;
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
template<typename T>
T mod_sqrt(T n, T p) {
  assert(mod_is_square(n, p));

  // Find q, s with p-1 = q*2^s.
  T q = p - 1;
  T s = 0;
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
  T z = dis(gen);
  while (mod_is_square(z, p)) z = dis(gen);

  T c = mod_pow(z, q, p);
  T x = mod_pow(n, (q + 1) / 2, p);
  T t = mod_pow(n, q, p);
  T m = s;

  while (t % p != 1) {
    // Find lowest 0 < i < m such that t^2^i = 1 (mod p).
    T i = 0;
    T test = t;
    while (test != 1) {
      test = test * test % p;
      ++i;
    }

    // U cexp = mod_pow(static_cast<U>(2), m - i - 1, p - 1);
    T cexp = static_cast<T>(1) << (m - i - 1);
    T b = mod_pow(c, cexp, p);
    x = x * b % p;
    t = t * b % p * b % p;
    c = b * b % p;
    m = i;
  }

  return std::min(x, p - x);
}

}
