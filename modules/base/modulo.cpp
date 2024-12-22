/**
 * @file
 * @brief Primary module interface unit for module `modulo`.
 */
module;

#include <cassert>
#include <random>
#include <type_traits>

/**
 * @module modulo
 * @brief Function templates for modular arithmetic.
 */
export module modulo;

import base;

namespace ntlib {

/**
 * @brief Returns the rounded down quotient of two given numbers.
 *
 * @tparam T An integer-like type.
 * @param a The dividend.
 * @param b The divisor.
 * @return The quotient, rounded down.
 */
export template<typename T>
[[nodiscard]] constexpr T floor_div(T a, T b) noexcept {
  assert(b != T{0});

  T quotient = a / b;
  if (((a < T{0}) ^ (b < T{0})) && a % b != T{0}) { --quotient; }
  return quotient;
}

/**
 * @brief Returns the rounded up quotient of two given numbers.
 *
 * @tparam T An integer-like type.
 * @param a The dividend.
 * @param b The divisor.
 * @return The quotient, rounded up.
 */
export template<typename T>
[[nodiscard]] constexpr T ceil_div(T a, T b) noexcept {
  assert(b != T{0});

  T quotient = a / b;
  if (((a > T{0} && b > T{0}) || (a < T{0} && b < T{0})) && a % b != T{0}) {
    ++quotient;
  }
  return quotient;
}

/**
 * @brief The mathematical modulo operation.
 * 
 * If the modulus \f$m\f$ is positive, then so is the result.
 *
 * @tparam T An integer-like type.
 * @param a The number to take modulo \f$m\f$.
 * @param m The modulus.
 * @return \f$a \mod m\f$ in the mathematical sense.
 */
export template<typename T>
[[nodiscard]] constexpr T mod(T a, T m) noexcept {
  if constexpr (std::is_unsigned_v<T>) {
    return a % m;
  } else {
    return a - m * ntlib::floor_div(a, m);
  }
}

/**
 * @brief Binary exponentiation with a custom mod-function.
 * 
 * Let \f$\mathrm{mod\_func}\f$ be a mod-function.
 * Computes \f$\mathrm{mod\_func}(a^b)\f$ using binary exponentation.
 * 
 * Runtime: \f$O(\log(b))\f$ calls to \f$\mathrm{mod\_func}\f$.
 *
 * @tparam A Multiplicative monoid.
 * @tparam B An integer-like type.
 * @tparam MF Function object.
 * @param a The base.
 * @param b The exponent, must be non-negative.
 * @param mod_func The mod-function.
 * @param unit The multiplicative unit of `A`.
 * @return The result of \f$\mathrm{mod\_func}\f$ applied to the power
 *     \f$a^b\f$.
 */
export template<typename A, typename B, typename MF>
[[nodiscard]] constexpr
A mod_pow(A a, B b, MF mod_func, A unit = A{1}) noexcept {
  assert(!(a == A{0} && b == B{0}));
  assert(b >= B{0});

  if (b == B{0}) { return unit; }
  else if (b == B{1}) { return mod_func(a); }
  else if (ntlib::is_odd(b)) {
    return mod_func(ntlib::mod_pow(a, b - B{1}, mod_func, unit) * a);
  } else {
    return ntlib::mod_pow(mod_func(a * a), b / B{2}, mod_func, unit);
  }
}

/**
 * @brief Computes the multiplicative inverse.
 * 
 * Let \f$m \in \mathbb{N}\f$ and \f$a \in \mathbb{Z}/m\mathbb{Z}\f$ such that
 * \f$\mathrm{gcd}(a, m) = 1\f$.
 * Computes the multiplicative inverse of \f$a \mod m\f$.
 *
 * @tparam T A integer-like type.
 * @tparam S The signed type corresponding to `T`
 * @param a The number to invert.
 * @param m The order of the group \f$a \in \mathbb{Z}/m\mathbb{Z}\f$.
 * @return The multiplicative inverse of \f$a\f$ modulo \f$m\f$.
 */
export template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr T mod_mult_inv(T a, T m) noexcept {
  assert(m > T{0});

  const auto [gcd, x, y] = ntlib::extended_euclid<T, S>(a, m);
  assert(gcd == T{1});

  return x >= S{0}
      ? ntlib::mod(static_cast<T>(x), m)
      : m - (ntlib::mod(static_cast<T>(-x), m));
}

/**
 * @brief Test whether a given number is a quadratic residue module a prime.
 * 
 * For \f$a, p \in \mathbb{N}\f$ where \f$p\f$ is prime, checks whether \f$a\f$
 * is a quadratic residue modulo \f$p\f$, i.e., if there is an \f$x\f$ such that
 * \f$x^2 = a \mod p\f$.
 *
 * @tparam T An integer-like type.
 * @param a The number to test.
 * @param p The modulus. Must be prime.
 * @return Whether \f$a\f$ is a quadratic residue modulo \f$p\f$.
 */
export template<typename T>
[[nodiscard]] constexpr bool mod_is_square(T a, T p) noexcept {
  if (a == T{0}) { return true; }
  if (p == T{2}) { return true; }
  const auto mod_p = [p](T n) { return ntlib::mod(n, p); };
  return ntlib::mod_pow(a, (p - T{1}) / T{2}, mod_p) == T{1};
}

/**
 * @brief Computes square roots moduolo an odd prime.
 * 
 * Given \f$n \in \mathbb{N}\f$ and an odd prime \f$p\f$, uses the Tonelli-Shankes
 * algorithm to compute an \f$x\f$ such that \f$x^2 = n \mod p\f$.
 * 
 * For every solution \f$0 \leq x < p\f$, there is a second solution
 * \f$p - x\f$. Returns the smaller one.
 *
 * @tparam T An integer-like type.
 * @param n Parameter \f$n\f$. Must be \f$0 \leq n < p\f$.
 * @param p Odd prime number.
 * @return The smaller of two solutions \f$x\f$ to \f$x^2 = n \mod p\f$.
 */
export template<typename T>
[[nodiscard]] constexpr T mod_sqrt(T n, T p) noexcept {
  const auto mod_p = [p](T x) { return ntlib::mod(x, p); };

  // Find q, s with p-1 = q*2^s.
  const auto [s, q] = ntlib::odd_part(p - T{1});

  // If and only if s == 1, we have p = 3 (mod 4).
  // In this case we can compute root x directly.
  if (s == T{1}) { return ntlib::mod_pow(n, (p + T{1}) / T{4}, mod_p); }

  // Find a quadratic non-residue z.
  // Half the numbers in 1, ..., p-1 are, so we randomly guess.
  // On average, two tries are necessary.
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<T> dis(T{1}, p - T{1});
  T z{dis(gen)};
  while (ntlib::mod_is_square(z, p)) z = dis(gen);

  T c = ntlib::mod_pow(z, q, mod_p);
  T x = ntlib::mod_pow(n, (q + T{1}) / T{2}, mod_p);
  T t = ntlib::mod_pow(n, q, mod_p);
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
    T b = ntlib::mod_pow(c, cexp, mod_p);
    x = x * b % p;
    t = t * b % p * b % p;
    c = b * b % p;
    m = i;
  }

  return std::min(x, p - x);
}

/**
 * @brief Computes a factorial modulo a given number.
 * 
 * For \f$n, m \in \mathbb{N}\f$, computes \f$n! \mod m\f$.
 * 
 * @tparam T An integer-like type.
 * @param n The given number.
 * @param m The modulus.
 * @return The factorial \f$n!\f$ modulo \f$m\f$.
 */
export template<typename T>
[[nodiscard]] constexpr
T mod_factorial(T n, T m) {
  T res = ntlib::mod(T{1}, m);
  while (n > 1) {
    res = ntlib::mod(res * n--, m);
  }
  return res;
}

/**
 * @brief Computes the Legendre Symbol.
 * 
 * For an integer \f$a\f$ and an odd prime \f$p\f$, the Legendre Symbol
 * \f$\left(\frac{a}{p}\right)\f$ determines whether \f$a\f$ is a quadratic
 * residue modulo \f$p\f$.
 *
 * @tparam T An integer-like type.
 * @tparam S The signed type corresponding to `T`.
 * @param a An integer.
 * @param p An odd prime.
 * @return The Legendre Symbol \f$\left(\frac{a}{p}\right)\f$.
 */
export template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr S legendre(T a, T p) noexcept {
  assert(p != T{2});

  const auto mod_p = [&p](T n) { return ntlib::mod(n, p); };
  T rem = ntlib::mod_pow(a, (p - T{1}) / T{2}, mod_p);
  return rem <= T{1} ? static_cast<S>(rem) : S{-1};
}

/**
 * @brief Computes the Jacobi Symbol \f$\left(\frac{a}{p}\right)\f$.
 *
 * @tparam T An integer-like type.
 * @tparam S The signed type corresponding to `T`.
 * @param a The "numerator".
 * @param b The "denominator".
 * @return The Jacobi Symbol \f$\left(\frac{a}{p}\right)\f$.
 */
export template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr S jacobi(T a, T b) noexcept {
  assert(b > T{0});
  assert(ntlib::is_odd(b));

  using std::swap;

  a = ntlib::mod(a, b);
  S t{1};
  while (a != T{0}) {
    auto [s, aa] = ntlib::odd_part(a);
    a = aa;

    if (ntlib::is_odd(s) && (b % T{8} == T{3} || b % T{8} == T{5})) { t = -t; }

    swap(a, b);
    if (a % T{4} == T{3} && b % T{4} == T{3}) { t = -t; }
    a = ntlib::mod(a, b);
  }
  if (b == T{1}) return t;
  else return S{0};
}

}