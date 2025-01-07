/**
 * @file
 * @brief Primary module interface unit for module `binomial_coefficient`.
 */
module;

#include <algorithm>
#include <cassert>
#include <type_traits>
#include <vector>

/**
 * @module binomial_coefficient
 * @brief Compute binomial coefficients.
 * 
 * Compute the binomial coefficient \f${n \choose k}\f$ for
 * \f$0 \leq k \leq n\f$, possibly modulo some number \f$m\f$.
 * The binomial coeffient counts the number of \f$k\f$-element subsets of an
 * \f$n\f$-element universe.
 * 
 * Code for one-shot computation modulo a number \f$m\f$ is based on
 * https://cp-algorithms.com/combinatorics/binomial-coefficients.html.
 */
export module binomial_coefficient;

import base;
import chinese_remainder;
import modulo;
import prime_decomposition;
import prime_test;

namespace ntlib {

/**
 * @brief Computes a single binomial coefficient.
 *
 * Intermediate results are at most \f$k\f$ times as big as the result.
 * 
 * Runtime: \f$O(k)\f$
 *
 * @tparam T An integer-like type.
 * @param n Size of universe.
 * @param k Size of the subsets.
 * @return The binomial coefficient \f${n \choose k}\f$.
 */
export template<typename T>
[[nodiscard]] constexpr
T binom(T n, T k) noexcept {
  assert(n >= T{0});

  if (k < T{0} || k > n) { return T{0}; }

  T res{1};
  for (T i = T{1}; i <= k; ++i) {
    res *= n--;
    res /= i;
  }
  return res;
}

/**
 * @brief Compute a single binomial coefficient module a sufficiently large
 * prime.
 * 
 * To compute \f${n \choose k} \mod p\f$ we need \f$p > \max\{k, n-k\}\f$.
 * 
 * @tparam T An integer-like type.
 * @tparam S The signed type corresponding to `T`.
 * @param n Size of the universe.
 * @param k Size of the subsets.
 * @param p The modulus. Must be prime and sufficiently large.
 * @return The binomial coefficient \f${n \choose k} \mod p\f$.
 */
export template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr
T mod_p_binom(T n, T k, T p) noexcept {
  assert(n >= T{0});

  // Base cases.
  if (k < T{0} || k > n) { return T{0}; }

  assert(p > std::max(k, n - k));

  T res = mod_factorial(n, p);
  res = ntlib::mod(
      res * ntlib::mod_mult_inv<T,S>(ntlib::mod_factorial(k, p), p), p);
  res = ntlib::mod(
      res * ntlib::mod_mult_inv<T,S>(ntlib::mod_factorial(n - k, p), p), p);
  return res;
}

/**
 * @brief Computes a single binomial coefficient modulo a prime power.
 * 
 * For a prime \f$p\f$ and a positive integer \f$e\f$ this computes
 * \f${n \choose k} \mod p^e\f$.
 *
 * @tparam T An integer-like type.
 * @tparam S The signed type corresponding to `T`.
 * @param n Size of universe.
 * @param k Size of the subsets.
 * @param p A prime number.
 * @param e The exponent of the prime.
 * @return The binomial coefficient \f${n \choose k} \mod p^e\f$.
 */
export template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr
T mod_pp_binom(T n, T k, T p, T e) {
  assert(n >= T{0});

  // Compute the modulus `pp`.
  const T pp = ntlib::pow(p, e);

  // Base cases.
  if (k < T{0} || k > n) { return T{0}; }

  // If `pp` is large enough, all required modular inverses exist.
  if (p > std::max(k, n - k)) { return ntlib::mod_p_binom(n, k, pp); }

  // TODO: This should be in its own function to be testable.
  //
  // Compute `c(x)` and `g(x) mod pp` for all `x \in [0,n]`.
  // Uses dynamic programming.
  // `c(x)` is the largest `c` such that `p^c` divides `x!`.
  // `g(x)` is `x!/p^c(x)`.
  std::vector<T> c(n + 1);
  std::vector<T> g(n + 1);
  g[0] = 1;
  for (T i{1}; i <= n; ++i) {
    c[i] = c[i - 1];
    g[i] = g[i - 1];

    T ii = i;
    while (ntlib::mod(ii, p) == T{0}) {
      ++c[i];
      ii /= p;
    }
    g[i] = ntlib::mod(g[i] * ii, pp);
  }

  // Compute the binomial coefficient.
  T res = g[n];
  const auto mod_pp = [pp](T x) { return ntlib::mod(x, pp); };
  res = ntlib::mod(res * ntlib::mod_pow(p, c[n] - c[k] - c[n - k], mod_pp), pp);
  res = ntlib::mod(res * ntlib::mod_mult_inv<T,S>(g[k], pp), pp);
  res = ntlib::mod(res * ntlib::mod_mult_inv<T,S>(g[n - k], pp), pp);
  return res;
}

/**
 * @brief Computes a single binomial coefficient modulo an arbitrary number.
 * 
 * @tparam T An integer-like type.
 * @tparam S The signed type corresponding to `T`.
 * @param n The size of the universe.
 * @param k The size of the subsets.
 * @param m The modulus.
 * @return The binomial coefficient \f${n \choose k} \mod m\f$.
 */
export template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr
T mod_binom(T n, T k, T m) {
  // Find prime decomposition of modulus.
  const prime_factors<T> factors = ntlib::prime_decomposition(m);

  // Compute for each prime power individually.
  std::vector<crt_congruence<T>> congruences;
  congruences.reserve(factors.size());
  for (const auto [p, e] : factors) {
    const T pp = ntlib::pow(p, e);
    const T res_mod_pp = ntlib::mod_pp_binom<T,S>(n, k, p, e);
    congruences.push_back(crt_congruence {res_mod_pp, pp});
  }

  // Use Chinese remainder theorem to get the result.
  return ntlib::crt_coprime<T,S>(congruences).a;
}

/**
 * @brief Computes the first \f$N\f$ rows of Pascal's triangle.
 * 
 * Computes a table of all binomial coefficiets \f${i \choose j}\f$ with
 * \f$0 \leq i,j \leq N\f$.
 * 
 * Intermediate results do not exceed the final results.
 * 
 * Runtime: \f$O(N^2)\f$
 *
 * @tparam T An integer-like type.
 * @param N The number of rows to compute.
 * @return Two-dimensional `std::vector` containing the binomial coefficients.
 */
export template<typename T>
[[nodiscard]] constexpr
std::vector<std::vector<T>> binom_table(T N) {
  assert(N >= T{0});

  std::vector<std::vector<T>> binoms(N + 1, std::vector<T>(N + 1, T{0}));
  binoms[0][0] = T{1};
  for (T n{1}; n <= N; ++n) {
    binoms[n][0] = T{1};
    for (T k{1}; k <= n; ++k) {
      binoms[n][k] = binoms[n - 1][k - 1] + binoms[n - 1][k];
    }
  }
  return binoms;
}

/**
 * @brief Computes the first \f$N\f$ rows of Pascal's triangle module some
 * number \f$m\f$.
 * 
 * Computes a table of all binomial coefficiets \f${i \choose j} \mod m\f$ with
 * \f$0 \leq i,j \leq N\f$.
 * 
 * Runtime: \f$O(N^2)\f$
 *
 * @tparam T An integer-like type.
 * @param N The number of rows to compute.
 * @param m The modulus.
 * @return Two-dimensional `std::vector` containing the binomial coefficients.
 */
export template<typename T>
[[nodiscard]] constexpr
std::vector<std::vector<T>> mod_binom_table(std::size_t N, T m) {
  assert(N >= 0);
  assert(m > T{0});

  std::vector<std::vector<T>> binoms(N + 1, std::vector<T>(N + 1, T{0}));
  binoms[0][0] = ntlib::mod(T{1}, m);
  for (std::size_t n = 1; n <= N; ++n) {
    binoms[n][0] = ntlib::mod(T{1}, m);
    for (std::size_t k = 1; k <= n; ++k) {
      binoms[n][k] = ntlib::mod(binoms[n - 1][k - 1] + binoms[n - 1][k], m);
    }
  }
  return binoms;
}

};