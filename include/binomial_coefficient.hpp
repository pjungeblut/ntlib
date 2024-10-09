#pragma once

/**
 * Implementations to compute binomial coefficients.
 * 
 * Code for one-shot computation modulo a number `m` is based on:
 * https://cp-algorithms.com/combinatorics/binomial-coefficients.html
 */

#include <algorithm>
#include <cassert>
#include <map>
#include <type_traits>
#include <vector>

#include "chinese_remainder.hpp"
#include "modulo.hpp"
#include "prime_decomposition.hpp"
#include "prime_test.hpp"

namespace ntlib {

/**
 * Computes the binomial coefficient `binom(n,k)`, i.e., the number of
 * `k`-element subsets of an `n`-element universe.
 * Runtime: `O(k)`
 *
 * Intermediate results are at most `k` times as big as the result.
 *
 * @param n Size of universe.
 * @param k Size of the subsets.
 * @return The binomial coefficient `binom(n,k)`.
 */
template<typename T>
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
 * Computes the binomial coefficient `binom(n,k)` modulo a sufficiently large
 * prime number `p` (needs `p > max(k, n-k)`).
 * 
 * @param n Size of the universe.
 * @param k Size of the subsets.
 * @param p The modulus. Must be prime and sufficiently large.
 * @return The binomial coefficient `binom(n,k)` modulo `p`.
 */
template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr
T mod_p_binom(T n, T k, T p) noexcept {
  assert(n >= T{0});

  // Base cases.
  if (k < T{0} || k > n) { return T{0}; }

  assert(p > std::max(k, n - k));

  T res = mod_factorial(n, p);
  res = mod(res * mod_mult_inv<T,S>(mod_factorial(k, p), p), p);
  res = mod(res * mod_mult_inv<T,S>(mod_factorial(n - k, p), p), p);
  return res;
}

/**
 * Computes the binomial coefficient `binom(n,k)` modulo a prime power `pp`.
 * The prime power `pp` is given as `p^e`.
 *
 * @param n Size of universe.
 * @param k Size of the subsets.
 * @param p The prime.
 * @param e The exponent of the prime.
 * @return The binomial coefficient `binom(n,k)` modulo `pp`.
 */
template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr
T mod_pp_binom(T n, T k, T p, T e) {
  assert(n >= T{0});

  // Compute the modulus `pp`.
  const T pp = pow(p, e);

  // Base cases.
  if (k < T{0} || k > n) { return T{0}; }

  // If `pp` is large enough, all required modular inverses exist.
  if (p > std::max(k, n - k)) { return mod_p_binom(n, k, pp); }

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
    while (mod(ii, p) == 0) {
      ++c[i];
      ii /= p;
    }
    g[i] = mod(g[i] * ii, pp);
  }

  // Compute the binomial coefficient.
  T res = g[n];
  const auto mod_pp = [pp](T x) { return mod(x, pp); };
  res = mod(res * mod_pow(p, c[n] - c[k] - c[n - k], mod_pp), pp);
  res = mod(res * mod_mult_inv<T,S>(g[k], pp), pp);
  res = mod(res * mod_mult_inv<T,S>(g[n - k], pp), pp);
  return res;
}

/**
 * Computes the binomial coefficient `binom(n,k)` modulo an arbitrary
 * number `m`.
 * 
 * @param n The size of the universe.
 * @param k The size of the subsets.
 * @param m The modulus.
 * @return The binomial coefficient `binom(n,k)` modulo `m`.
 */
template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr
T mod_binom(T n, T k, T m) {
  // Find prime decomposition of modulus.
  std::map<T,T> factors;
  prime_decomposition(m, factors);

  // Compute for each prime power individually.
  std::vector<crt_congruence<T>> congruences;
  congruences.reserve(factors.size());
  for (const auto [p, e] : factors) {
    const T pp = pow(p, e);
    const T res_mod_pp = mod_pp_binom<T,S>(n, k, p, e);
    congruences.push_back(crt_congruence {res_mod_pp, pp});
  }

  // Use Chinese remainder theorem to get the result.
  return crt_coprime<T,S>(congruences);
}

/**
 * Computes a table of all binomial coefficiets `binom(i,j)`
 * with `0 <= i,j <= N`.
 * 
 * Intermediate results do not exceed results.
 * Runtime: O(N^2)
 *
 * @param N The parameter N.
 * @return Two-dimensional vector containing the binomial coefficients.
 */
template<typename T>
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
 * Computes a table of all binomial coefficiets `binom(i,j)`
 * with `0 <= i,j <= N` modulo `m`.
 * 
 * Runtime: O(N^2)
 *
 * @param N The parameter N.
 * @param m The modulus.
 * @return Two-dimensional vector containing the binomial coefficients.
 */
template<typename T>
[[nodiscard]] constexpr
std::vector<std::vector<T>> mod_binom_table(T N, T m) {
  assert(N >= T{0});
  assert(m > T{0});

  std::vector<std::vector<T>> binoms(N + 1, std::vector<T>(N + 1, T{0}));
  binoms[0][0] = mod(T{1}, m);
  for (T n{1}; n <= N; ++n) {
    binoms[n][0] = mod(T{1}, m);
    for (T k{1}; k <= n; ++k) {
      binoms[n][k] = mod(binoms[n - 1][k - 1] + binoms[n - 1][k], m);
    }
  }
  return binoms;
}

}
