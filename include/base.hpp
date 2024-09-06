#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <cmath>
#include <functional>
#include <numeric>
#include <type_traits>
#include <vector>

namespace ntlib {

/**
 * A list with all prime numbers up to, SMALL_PRIMES_BIGGEST.
 * Useful as these are used frequently.
 */
static constexpr auto SMALL_PRIMES = std::to_array<uint32_t>({
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
    157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
    239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317,
    331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419,
    421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503,
    509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607,
    613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701,
    709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811,
    821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911,
    919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997, 1'009});
static constexpr uint32_t SMALL_PRIMES_BIGGEST =
    *std::ranges::max_element(SMALL_PRIMES);

/**
 * Checks whether a number is odd.
 * 
 * @param n The number.
 * @return Whether n is odd.
 */
template<typename T>
[[nodiscard]] constexpr bool is_odd(T n) noexcept {
  return n & T{1};
}

/**
 * Checks whether a number is even.
 * 
 * @param n The number.
 * @return Whether n is even.
 */
template<typename T>
[[nodiscard]] constexpr bool is_even(T n) noexcept {
  return !is_odd(n);
}

/**
 * Computes the absolute value of a number.
 *
 * @param n The number to take the absolute value of.
 * @return The absolute value of n.
 */
template<typename T>
[[nodiscard]] constexpr T abs(T n) noexcept {
  return n >= T{0} ? n : -n;
}

/**
 * Given a number n, computes (e, o), such that n=2^e*o.
 * 
 * @param n The number to decompose.
 * @return A pair with e and o. 
 */
template<typename T>
[[nodiscard]] constexpr std::pair<T,T> odd_part(T n) noexcept {
  T e{0};
  while (n != T{0} && is_even(n)) {
    n /= T{2};
    ++e;
  }
  return std::make_pair(e, n);
}

/**
 * Computes the greatest common divisor of a and b.
 * Uses the Euclidean Algorithm.
 * Runtime: O(log a + log b).
 *
 * The greatest common divisor of two numbers a and b (not both zero) is the
 * smallest positive number that divides both a and b.
 *
 * @param a The first number.
 * @param b The second number.
 * @return The greatest common divisor of a and b.
 */
template<typename T>
[[nodiscard]] constexpr T gcd(T a, T b) noexcept {
  assert(!(a == T{0} && b == T{0}));
  return b == T{0} ? abs(a) : gcd(b, a % b);
}

/**
 * Computes the least common multiple of a and b.
 * Runtime: O(log a + log b)
 *
 * The least common multiple of two non-zero integers a and b is the
 * smallest positive integer that can be divided by both a and b.
 *
 * @param a The first number.
 * @param b The second number.
 * @return The least common multiple of a and b.
 */
template<typename T>
[[nodiscard]] constexpr T lcm(T a, T b) noexcept {
  assert(a != T{0});
  assert(b != T{0});
  return abs(a) * (abs(b) / gcd(a, b));
}

/**
 * Extended Euclidean algorithm.
 * Finds whole number solutions for a*x + b*y = gcd(a,b).
 * 
 * @param a Parameter a
 * @param b Parameter b
 * @return Tuple (gcd(a,b), x, y).
 */
template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr std::tuple<T, S, S> extended_euclid(T a, T b) noexcept {
  assert(!(a == T{0} && b == T{0}));

  // Extended Euclidean algorithm for non-negative values.
  const std::function<std::tuple<T, S, S>(T, T)>
      extended_euclid_non_negative = [&extended_euclid_non_negative](T a, T b) {
    if (a == S{0}) { return std::make_tuple(b, S{0}, S{1}); }
    auto [gcd, xx, yy] = extended_euclid_non_negative(b % a, a);

    S x{yy - static_cast<S>(b / a) * xx};
    S y{xx};
    return std::make_tuple(gcd, x, y);
  };

  auto [gcd, x, y] = extended_euclid_non_negative(abs(a), abs(b));
  if (a < T{0}) x = -x;
  if (b < T{0}) y = -y;
  return std::make_tuple(gcd, x, y);
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
template<typename A, typename B>
[[nodiscard]] constexpr A pow(A a, B b, A unit = A{1}) noexcept {
  assert(!(a == A{0} && b == B{0}));
  assert(b >= B{0});

  if (b == B{0}) { return unit; }
  else if (b == B{1}) { return a; }
  else if (is_odd(b)) { return pow(a, b - B{1}, unit) * a; }
  else { return pow(a * a, b / B{2}, unit); }
}

/**
 * Computes the integer part of the binary logarithm.
 *
 * @param n The number to compute the ceiling of the binary logarithm for.
 * @return floor(log2(n))
 */
template<typename T>
[[nodiscard]] constexpr T ilog2(T n) noexcept {
  assert(n > T{0});

  constexpr T length = sizeof(T) * CHAR_BIT;
  if constexpr (std::is_same_v<T, unsigned int> ||
      std::is_same_v<T, int>) {
    return length - __builtin_clz(static_cast<unsigned int>(n)) - 1;
  } else if constexpr (std::is_same_v<T, unsigned long> ||
      std::is_same_v<T, long>) {
    return length - __builtin_clzl(static_cast<unsigned long>(n)) - 1;
  } else if constexpr (std::is_same_v<T, unsigned long long> ||
      std::is_same_v<T, long long>) {
    return length - __builtin_clzll(static_cast<unsigned long long>(n)) - 1;
  } else {
    T result{0};
    while (n >>= T{1}) ++result;
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
[[nodiscard]] constexpr T isqrt(T n) noexcept {
  assert(n >= T{0});

  if constexpr ((std::is_integral_v<T> && sizeof(T) <= 4) ||
      std::is_same_v<T, double>) {
    return floor(sqrt(n));
  } else if constexpr (std::is_same_v<T, float>) {
    return floorf(sqrtf(n));
  } else if constexpr (std::is_same_v<T, long double>) {
    return floorl(sqrtl(n));
  } else {
    // Checks without overflows that `a*a <= b`.
    const auto square_atmost = [](auto a, auto b) {
      return a <= b / a;
    };

    T result{0}, summand{1};
    while (square_atmost(result + summand, n)) {
      while (square_atmost(result + T{2} * summand, n)) { summand *= T{2}; }
      result += summand;
      summand = T{1};
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
[[nodiscard]] constexpr bool is_square(T n) noexcept {
  if (n < T{0}) { return false; }
  if (n == T{0}) { return true; }

  T last_digit = n % T{10};
  T second_last_digit = n / T{10} % T{10};
  T third_last_digit = n / T{100} % T{10};

  // If n is a multiple of four, we can look at n/4 instead.
  while (n && (n & T{3}) == T{0}) n >>= 2;
  if (n == T{0}) { return true; }

  // If n is not divisible by four (it is not by above test), its binary
  // representation must end with 001.
  if (!((n & T{7}) == T{1})) { return false; }

  // All squares end in the numbers 0, 1, 4, 5, 6, or 9.
  if (last_digit == T{2} || last_digit == T{3} || last_digit == T{7} ||
      last_digit == T{8}) {
    return false;
  }

  // The last two digits cannot both be odd.
  if (is_odd(last_digit) && is_odd(second_last_digit)) { return false; }

  // If the last digit is 1 or 9, the two digits befor must be a multiple of 4.
  if ((last_digit == T{1} || last_digit == T{9}) &&
      ((third_last_digit * T{10} + second_last_digit) % T{4} != T{0})) {
    return false;
  }

  // If the last digit is 4, the digit before it must be even.
  if (last_digit == T{4} && is_odd(second_last_digit)) { return false; }

  // If the last digit is 6, the digit before it must be odd.
  if (last_digit == T{6} && is_even(second_last_digit)) { return false; }

  // If the last digit is 5, the digit before it must be 2.
  if (last_digit == T{5} && second_last_digit != T{2}) { return false; }

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
[[nodiscard]] constexpr T factorial(T n) noexcept{
  assert(n >= T{0});

  T result{1};
  for (T i{2}; i <= n; ++i) { result *= i; }
  return result;
}

}
