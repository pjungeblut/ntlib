#pragma once

#include <cassert>
#include <random>
#include <type_traits>

#include "base.hpp"

namespace ntlib {

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
  if constexpr (std::is_unsigned_v<T>) { return a % m; }
  else { return a - m * floor_div(a, m); }
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
template<typename A, typename B, typename MF>
[[nodiscard]] constexpr
A mod_pow(A a, B b, MF mod_func, A unit = A{1}) noexcept {
  assert(!(a == A{0} && b == B{0}));
  assert(b >= B{0});

  if (b == B{0}) {
    return unit;
  } else if (b == B{1}) {
    return mod_func(a);
  } else if (is_odd(b)) {
    // TODO: Check why this cannot be in one line.
    auto no_mod = mod_pow(a, b - B{1}, mod_func, unit) * a;
    return mod_func(no_mod);
  } else {
    // TODO: Check why this cannot be in one line.
    auto a_squared = a * a;
    return mod_pow(mod_func(a_squared), b / B{2}, mod_func, unit);
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
  const auto mod_p = [p](T n) { return mod(n, p); };
  return mod_pow(a, (p - T{1}) / T{2}, mod_p) == T{1};
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
  const auto mod_p = [p](T x) { return mod(x, p); };

  // Find q, s with p-1 = q*2^s.
  auto [s, q] = odd_part(p - T{1});

  // If and only if s == 1, we have p = 3 (mod 4).
  // In this case we can compute root x directly.
  if (s == T{1}) { return mod_pow(n, (p + T{1}) / T{4}, mod_p); }

  // Find a quadratic non-residue z.
  // Half the numbers in 1, ..., p-1 are, so we randomly guess.
  // On average, two tries are necessary.
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<T> dis(T{1}, p - T{1});
  T z{dis(gen)};
  while (mod_is_square(z, p)) z = dis(gen);

  T c = mod_pow(z, q, mod_p);
  T x = mod_pow(n, (q + T{1}) / T{2}, mod_p);
  T t = mod_pow(n, q, mod_p);
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
    T b = mod_pow(c, cexp, mod_p);
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

  const auto mod_p = [&p](T n) { return mod(n, p); };
  T rem = mod_pow(a, (p - T{1}) / T{2}, mod_p);
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
  S t{1};
  while (a != T{0}) {
    auto [s, aa] = odd_part(a);
    a = aa;

    if (is_odd(s) && (b % T{8} == T{3} || b % T{8} == T{5})) { t = -t; }

    std::swap(a, b);
    if (a % T{4} == T{3} && b % T{4} == T{3}) { t = -t; }
    a = mod(a, b);
  }
  if (b == T{1}) return t;
  else return S{0};
}

}