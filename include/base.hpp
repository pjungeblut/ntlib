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
static constexpr auto SMALL_PRIMES = std::to_array<uint32_t>({
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97});

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
 * Checks whether a number is eve.
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

/**
 * Returns the quotient of a/b rounded down.
 *
 * @param a The dividend.
 * @param b The divisor.
 * @return The quotient, rounded down.
 */
template<typename T>
[[nodiscard]] constexpr T floor_div(T a, T b) noexcept {
  assert(b != T{0});

  T quotient = a / b;
  if (((a < T{0}) ^ (b < T{0})) && a % b != T{0}) --quotient;
  return quotient;
}

/**
 * Returns the quotient of a/b rounded up.
 *
 * @param a The dividend.
 * @param m The divisor.
 * @return The quotient, rounded up.
 */
template<typename T>
[[nodiscard]] constexpr T ceil_div(T a, T b) noexcept {
  assert(b != T{0});

  T quotient = a / b;
  if (((a > T{0} && b > T{0}) || (a < T{0} && b < T{0})) && a % b != T{0}) {
    ++quotient;
  }
  return quotient;
}

/**
 * The mathematical modulo operation.
 *
 * @param a The number to take modulo.
 * @param m The modulus.
 * @return a mod m in the mathematical sense.
 *         If m is positive, then so is the result.
 */
template<typename T>
[[nodiscard]] constexpr T mod(T a, T m) noexcept {
  return a - m * floor_div(a, m);
}

/**
 * Computes a^b mod m using binary exponentation.
 * Runtime: O(log b)
 *
 * Caution: m^2 must be small enough to fit into type T.
 *
 * @param a The base.
 * @param b The exponent, must be non-negative.
 * @param m The modulus, must be positive.
 * @param unit The unit element of the group.
 * @return a^b (mod m), in particular the return value is in [0,m-1].
 */
template<typename A, typename B>
[[nodiscard]] constexpr A mod_pow(A a, B b, A m, A unit = A{1}) noexcept {
  assert(!(a == A{0} && b == B{0}));
  assert(b >= B{0});
  assert(m > A{0});

  if (b == B{0}) {
    return unit;
  } else if (b == B{1}) {
    return mod(a, m);
  } else if (is_odd(b)) {
    return mod(mod_pow(a, b - B{1}, m, unit) * a, m);
  } else {
    return mod_pow(mod(a * a, m), b / B{2}, m, unit);
  }
}

/**
 * Computes the multiplicative inverse of a (mod m).
 * Only exists, if gcd(a, m) = 1.
 *
 * @param a The number to invert.
 * @param m The order of the group.
 * @return The multiplicative inverse of a (mod m).
 */
template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr T mod_mult_inv(T a, T m) noexcept {
  assert(m > T{0});

  auto [gcd, x, y] = extended_euclid<T, S>(a, m);
  assert(gcd == T{1});

  return x >= S{0}
      ? mod(static_cast<T>(x), m)
      : m - (mod(static_cast<T>(-x), m));
}

/**
 * Tests, if a is a quadratic residue modulo p.
 * Uses Euler's criterion to compute Legendre Symbol (a/p).
 *
 * @param a The number to test.
 * @param p The modulus. Must be prime.
 * @return True, if and only if there is an x, such that x^2 = a (mod p).
 */
template<typename T>
[[nodiscard]] constexpr bool mod_is_square(T a, T p) noexcept {
  if (a == T{0}) { return true; }
  if (p == T{2}) { return true; }
  return mod_pow(a, (p - T{1}) / T{2}, p) == T{1};
}

/**
 * Computes square roots modular a prime number, that is an integer solution for
 * x in the equation x^2 = a mod p.
 * Uses the Tonelli-Shankes algorithm.
 *
 * @param a Parameter a. 0 <= a < p.
 * @param p Odd prime number p > 2.
 * @return 0 <= x < p such that x^2 = a mod p.
 *         There are two solutions, the other one is p-x.
 *         Returns the smaller one.
 */
template<typename T>
[[nodiscard]] constexpr T mod_sqrt(T n, T p) noexcept {
  // Find q, s with p-1 = q*2^s.
  T q{p - T{1}};
  T s{0};
  while (is_even(q)) {
    q /= T{2};
    ++s;
  }

  // If and only if s == 1, we have p = 3 (mod 4).
  // In this case we can compute root x directly.
  if (s == T{1}) { return mod_pow(n, (p + T{1}) / T{4}, p); }

  // Find a quadratic non-residue z.
  // Half the numbers in 1, ..., p-1 are, so we randomly guess.
  // On average, two tries are necessary.
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<T> dis(T{1}, p - T{1});
  T z{dis(gen)};
  while (mod_is_square(z, p)) z = dis(gen);

  T c = mod_pow(z, q, p);
  T x = mod_pow(n, (q + T{1}) / T{2}, p);
  T t = mod_pow(n, q, p);
  T m = s;

  while (t % p != T{1}) {
    // Find lowest 0 < i < m such that t^2^i = 1 (mod p).
    T i{0};
    T test{t};
    while (test != T{1}) {
      test = test * test % p;
      ++i;
    }

    T cexp = T{1} << (m - i - T{1});
    T b = mod_pow(c, cexp, p);
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
template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr S legendre(T a, T p) noexcept {
  assert(p != T{2});

  T rem = mod_pow(a, (p - T{1}) / T{2}, p);
  return rem <= T{1} ? static_cast<S>(rem) : S{-1};
}

/**
 * Computes the Jacobi Symbol (a/b).
 *
 * @param a The "numerator".
 * @param b The "denominator".
 * @return The Jacobi Symbol (a/b).
 */
template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr S jacobi(T a, T b) noexcept {
  assert(b > T{0});
  assert(is_odd(b));

  a = mod(a, b);
  // From here, both a and b are non-negative.
  S t{1};
  while (a != T{0}) {
    T s(0);
    while (is_even(a)) {
      a /= T{2};
      ++s;
    }
    if (is_odd(s) && (b % T{8} == T{3} || b % T{8} == T{5})) { t = -t; }

    std::swap(a, b);
    if (a % T{4} == T{3} && b % T{4} == T{3}) { t = -t; }
    a = mod(a, b);
  }
  if (b == T{1}) return t;
  else return S{0};
}

}
