#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <cmath>
#include <functional>
#include <numeric>
#include <random>
#include <type_traits>
#include <vector>

namespace ntlib {

/**
 * A list with all prime numbers below SMALL_PRIMES_UPPER_BOUND.
 * Useful as these are used frequently.
 */
static constexpr uint32_t SMALL_PRIMES_UPPER_BOUND = 100;
static constexpr auto SMALL_PRIMES = std::to_array<uint32_t>({2, 3, 5, 7, 11, 13,
    17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97});

/**
 * Computes the absolute value of a number.
 *
 * @param n The number to take the absolute value of.
 * @return The absolute value of n.
 */
template<typename T>
constexpr T abs(T n) {
  return n >= 0 ? n : -n;
}

/**
 * Computes the greatest common divisor of a and b.
 * Uses the Euclidean Algorithm.
 * Runtime: O(log a + log b).
 *
 * The greatest common divisor of two numbers `a` and `b` (not both zero) is the
 * smallest positive number that divides both `a` and `b`.
 *
 * @param a The first number.
 * @param b The second number.
 * @return The greatest common divisor of a and b.
 */
template<typename A, typename B>
constexpr std::common_type_t<A,B> gcd(A a, B b) {
  assert(a != 0 || b != 0);
  return b == 0 ? abs(a) : gcd(b, a % b);
}

/**
 * Computes the least common multiple of a and b.
 * Runtime: O(log a + log b)
 *
 * The least common multiple of two non-zero integers `a` and `b` is the
 * smallest positive integer that can be divided by both `a` and `b`.
 *
 * If `lcm(a,b)` fits into T, then there are no integer overflows.
 *
 * @param a The first number.
 * @param b The second number.
 * @return The least common multiple of a and b.
 */
template<typename A, typename B>
constexpr std::common_type_t<A,B> lcm(A a, B b) {
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
template<typename A, typename B,
    typename S = std::make_signed_t<std::common_type_t<A,B>>>
auto extended_euclid(A a, B b, S &x, S &y) {
  assert(a != 0 || b != 0);

  using C = std::common_type_t<A,B>;

  // Extended Euclidean Algorithm for non-negative values.
  const std::function<C(A, B, S&, S&)> extended_euclid_non_negative =
      [&extended_euclid_non_negative](A a, B b, S &x, S &y) {
    if (a == 0) {
      x = 0;
      y = 1;
      return b;
    }
    S xx, yy;
    C gcd = extended_euclid_non_negative(b % a, a, xx, yy);
    x = yy - (b / a) * xx;
    y = xx;
    return gcd;
  };

  C gcd = extended_euclid_non_negative(abs(a), abs(b), x, y);
  if (a < 0) x = -x;
  if (b < 0) y = -y;
  return gcd;
}

/**
 * Computes a^b using binary exponentation.
 * Runtime: O(log b)
 *
 * @param a The base.
 * @param b The exponent, non-negative.
 * @param unit The unit element of the group.
 * @return a^b
 */
template<typename B, typename E>
B pow(B a, E b, B unit = 1) {
  assert(b >= 0);
  assert(a != 0 || b != 0);

  if (b == 0) return unit;
  if (b == 1) return a;
  if (b & 1) return pow(a, b - 1, unit) * a;
  return pow(a * a, b / 2, unit);
}

/**
 * Computes the integer part of the binary logarithm.
 *
 * @param n The number to compute the ceiling of the binary logarithm for.
 * @return floor(log2(n))
 */
template<typename T>
inline T ilog2(T n) {
  assert(n > 0);

  const T length = sizeof(T) * CHAR_BIT;
  if constexpr (std::is_same<T, unsigned int>::value ||
      std::is_same<T, int>::value) {
    return length - __builtin_clz(static_cast<unsigned int>(n)) - 1;
  } else if constexpr (std::is_same<T, unsigned long>::value ||
      std::is_same<T, long>::value) {
    return length - __builtin_clzl(static_cast<unsigned long>(n)) - 1;
  } else if constexpr (std::is_same<T, unsigned long long>::value ||
      std::is_same<T, long long>::value) {
    return length - __builtin_clzll(static_cast<unsigned long long>(n)) - 1;
  } else {
    T result = 0;
    while (n >>= 1) ++result;
    return result;
  }
}

/**
 * Computes the integer square.
 * isqrt(n) := floor(sqrt(n))
 *
 * @param n The number to compute the integer square root of. Non-negative.
 * @return isqrt(n)
 */
template<typename T>
inline T isqrt(T n) {
  assert(n >= 0);

  if constexpr ((std::is_integral<T>::value && sizeof(T) <= 4) ||
      std::is_same<T, double>::value) {
    return floor(sqrt(n));
  } else if constexpr (std::is_same<T, float>::value) {
    return floorf(sqrtf(n));
  } else if constexpr (std::is_same<T, long double>::value) {
    return floorl(sqrtl(n));
  } else {
    // Checks without overflows that `a*a <= b`.
    const auto square_atmost = [](T a, T b) {
      return a <= b / a;
    };

    T result = 0, summand = 1;
    while (square_atmost(result + summand, n)) {
      while (square_atmost(result + 2 * summand, n)) summand *= 2;
      result += summand;
      summand = 1;
    }
    return result;
  }
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
 * Returns the quotient of `a/b` rounded down.
 *
 * @param a The dividend.
 * @param b The divisor.
 * @return The quotient, rounded down.
 */
template<typename A, typename B>
auto floor_div(A a, B b) {
  assert(b != 0);

  auto quotient = a / b;
  if (((a < 0) ^ (b < 0)) && a % b != 0) --quotient;
  return quotient;
}

/**
 * Returns the quotient of `a/b` rounded up.
 *
 * @param a The dividend.
 * @param m The divisor.
 * @return The quotient, rounded up.
 */
template<typename A, typename B>
auto ceil_div(A a, B b) {
  assert(b != 0);

  auto quotient = a / b;
  if (((a > 0 && b > 0) || (a < 0 && b < 0)) && a % b != 0) ++quotient;
  return quotient;
}

/**
 * The mathematical modulo operation.
 *
 * @param a The number to take modulo.
 * @param m The modulus.
 * @return `a mod m` in the mathematical sense.
 *         If m is positive, then so is the result.
 */
template<typename A, typename M>
M mod(A a, M m) {
  return a - m * floor_div(a, m);
}

/**
 * Computes a^b mod m using binary exponentation.
 * Runtime: O(log b)
 *
 * Caution: `m*m` must be small enough to fit into type `T`.
 *
 * @param a The base.
 * @param b The exponent, must be non-negative.
 * @param m The modulus, must be positive.
 * @param unit The unit element of the group.
 * @return a^b mod m, in particular the return value is in [0,m-1].
 */
template<typename T, typename U = std::make_unsigned_t<T>>
U mod_pow(T a, U b, U m, U unit = 1u) {
  assert(a != 0 || b != 0);
  assert(b >= 0);
  assert(m > 0);

  U absa = abs(a);

  if (b == 0) return unit;
  if (b == 1) {
    U pos_res = mod(absa, m);
    if (a < 0 && pos_res != 0) return m - pos_res;
    else return pos_res;
  }
  if (b & 1) {
    U pos_res = mod(mod_pow(absa, b - 1, m, unit) * absa, m);
    if (a < 0 && pos_res != 0) return m - pos_res;
    else return pos_res;
  }
  return mod_pow(mod(absa * absa, m), b / 2, m, unit);
}

/**
 * Computes the multiplicative inverse of a (mod m).
 * Only exists, if gcd(a, m) = 1.
 * This condition is not checked inside the function.
 *
 * @param a The number to invert.
 * @param m The order of the group.
 * @return The multiplicative inverse of a (mod m).
 */
template<typename A, typename M,
    typename S = std::make_signed_t<std::common_type_t<A, M>>>
auto mod_mult_inv(A a, M m) {
  assert(a >= 0);
  assert(m > 0);

  S x, y;
  extended_euclid(a, m, x, y);
  return x >= 0 ? x % m : m - (-x % m);
}

/**
 * Tests, if `a` is a quadratic residue modulo `p`.
 * Uses Euler's Criterion to compute Legendre Symbol (a/p).
 *
 * @param a The number to test.
 * @param p The modulus. Must be prime number.
 * @return True, if and only if there is an `x`, such that `x^2 = a mod p`.
 */
template<typename A, typename P>
bool mod_is_square(A a, P p) {
  if (a == 0) return true;
  if (p == 2) return true;
  return mod_pow(a, (p - 1) / 2, p) == 1;
}

/**
 * Computes square roots modular a prime number, that is an integer solution for
 * `x` in the equation `x^2 = a mod p`.
 * Uses the Tonelli-Shankes algorithm.
 *
 * @param a Parameter `a`. `0 <= a < p`.
 * @param p Odd prime number `p > 2`.
 * @return `0 <= x < p` such that `x^2 = a mod p`.
 *         There are two solutions, the other one is `p - x`.
 *         Returns the smaller one.
 */
template<typename A, typename P>
auto mod_sqrt(A n, P p) {
  using C = std::common_type_t<A, P>;

  // Find q, s with p-1 = q*2^s.
  C q = p - 1;
  C s = 0;
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
  C z = dis(gen);
  while (mod_is_square(z, p)) z = dis(gen);

  C c = mod_pow(z, q, p);
  C x = mod_pow(n, (q + 1) / 2, p);
  C t = mod_pow(n, q, p);
  C m = s;

  while (t % p != 1) {
    // Find lowest 0 < i < m such that t^2^i = 1 (mod p).
    C i = 0;
    C test = t;
    while (test != 1) {
      test = test * test % p;
      ++i;
    }

    C cexp = static_cast<C>(1) << (m - i - 1);
    C b = mod_pow(c, cexp, p);
    x = x * b % p;
    t = t * b % p * b % p;
    c = b * b % p;
    m = i;
  }

  return std::min(x, p - x);
}

/**
 * Computes the Legendre Symbol (a/p).
 *
 * @param a An integer.
 * @param p An odd prime number.
 * @return The Legendre Symbol (a/p).
 */
template<typename A, typename P, typename S = std::make_signed_t<A>>
S legendre(A a, P p) {
  assert(p != 2);
  auto rem = mod_pow(a, (p - 1) / 2, p);
  return rem <= 1 ? rem : -1;
}

/**
 * Computes the Jacobi Symbol (a/b).
 *
 * @param a The "numerator".
 * @param b The "denominator".
 * @return The Jacobi Symbol (a/b).
 */
template<typename A, typename B>
auto jacobi(A a, B b) {
  assert(b > 0);
  assert(b % 2 == 1);

  using C = std::common_type_t<A,B>;

  a = mod(a, b);
  // From here, both a and b are non-negative.
  C t = 1;
  while (a != 0) {
    C s = 0;
    while (!(a & 1)) {
      a /= 2;
      ++s;
    }
    if ((s & 1) && (b % 8 == 3 || b % 8 == 5)) t = -t;

    std::swap(a, b);
    if (a % 4 == 3 && b % 4 == 3) t = -t;
    a = mod(a, b);
  }
  if (b == 1) return t;
  else return static_cast<C>(0);
}

}
