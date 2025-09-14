/**
 * @file
 * @brief Primary module interface unit for module `chinese_remainder`.
 */
module;

#include <algorithm>
#include <optional>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <vector>

/**
 * @module chinese_remainder
 * @brief Solve systems of modular congruences using the Chinese remainder
 * theorem.
 * 
 * Consider the following system of \f$k\f$ modular congruences:
 * \f[
 * \begin{cases}
 * a = a_1 \mod m_1 \\
 * a = a_2 \mod m_2 \\
 * \hphantom{a}~\vdots \\
 * a = a_k \mod m_k
 * \end{cases}
 * \f]
 * 
 * Assuming that the \f$m_i\f$ are pairwise coprime, the Chinese remainder
 * theorem states that there is a unique solution \f$x\f$ modulo
 * \f$M = \Pi_{i = 1}^k m_i\f$ solving all congruences simulatenously.
 * 
 * If the \f$m_i\f$ are not pairwise coprime, there is either no solution or
 * a unique solution modulo \f$M = \mathrm{lcm}(m_1, \ldots, m_k)\f$.
 */
export module chinese_remainder;

import base;
import modulo;
import prime_decomposition;
import prime_generation;

namespace ntlib {

/**
 * @brief Represents a single congruence `x = a mod m`.
 * 
 * @tparam T An integer-like type.
 */
export template<Integer T>
struct crt_congruence {
  /**
   * @brief The remainder.
   */
  T a;

  /**
   * @brief The modulus.
   */
  T m;
};

/**
 * @brief Solve a system of modular congruences for pairwise coprime moduli.
 * 
 * Uses the Chinese remainder theorem in its classical form.
 * A solution always exists and is unique modulo \f$M = \Pi_i m_i\f$.
 * 
 * @tparam T An integer-like type.
 * @param congruences The list of congruences.
 * @return The unique solution as a `crt_congruence<T>`.
 */
export template<Integer T>
    requires std::numeric_limits<T>::is_signed
[[nodiscard]] constexpr
crt_congruence<T> crt_coprime(
    const std::vector<crt_congruence<T>> &congruences) noexcept {
  const T M = std::ranges::fold_left(congruences, T{1},
      [](T prod, crt_congruence<T> c) {
    return prod * c.m;
  });

  const T x = std::ranges::fold_left(congruences, T{0},
      [M](T sum, crt_congruence<T> c) {
    const T M_i = M / c.m;
    const T N_i = ntlib::mod_mult_inv<T>(M_i, c.m);
    return ntlib::mod(sum + ntlib::mod(c.a * M_i, M) * N_i, M);
  });

  return crt_congruence {x, M};
}

/**
 * @brief Solve a general system of modular congruences.
 * 
 * Uses a generalization of the Chinese remainder theorem to handle non-coprime
 * moduli.
 * Either there is no solution, or there is a unique solution modulo
 * \f$M = \mathrm{lcm}(m_1, \ldots, m_k)\f$. 
 * 
 * @tparam T An integer-like type.
 * @param congruences The list of congruences.
 * @return A `std::optional` containing the unique solution if it exists.
 */
export template<Integer T>
    requires std::numeric_limits<T>::is_signed
[[nodiscard]] constexpr
std::optional<crt_congruence<T>> crt(
    const std::vector<crt_congruence<T>> &congruences) {
  // Generate all primes necessary to efficiently factorize all moduli.
  const auto ms = congruences |
      std::ranges::views::transform(&crt_congruence<T>::m);
  const T max_mod = *std::ranges::max_element(ms);
  const T iroot = ntlib::isqrt(max_mod);
  std::vector<T> primes;
  ntlib::prime_sieve(iroot, primes);

  // Represents a single congruence `x = a mod p^e`, where `p^e` is a prime
  // power.
  struct pp_crt_congruence {
    T a;
    T p;
    T e;
  };

  // Transform congruences into equivalent set of congruences modulo prime
  // powers.
  std::vector<pp_crt_congruence> pp_congruences;
  for (const auto c : congruences) {
    const auto factors = ntlib::prime_decomposition_list(c.m, primes);
    for (const auto [p, e] : factors) {
      pp_congruences.push_back(
          pp_crt_congruence {ntlib::mod(c.a, ntlib::pow(p, e)), p, e});
    }
  }

  // Sort ascending by primes (primary), descending by exponent (secondary) and
  // ascending by value (ternary).
  std::ranges::sort(pp_congruences,
      [](pp_crt_congruence c1, pp_crt_congruence c2) {
    T neg_e1 = -c1.e;
    T neg_e2 = -c2.e;
    return std::tie(c1.p, neg_e1, c1.a) < std::tie(c2.p, neg_e2, c2.a);
  });

  // Find potential inconsistencies in congruences and build consistent list of
  // congruences.
  std::vector<crt_congruence<T>> flattened;
  T last_prime{1};
  for (const pp_crt_congruence pp_c : pp_congruences) {
    const T pp = ntlib::pow(pp_c.p, pp_c.e);

    if (last_prime != pp_c.p) {
      // First congruence with this prime (and with the highest power).
      last_prime = pp_c.p;
      flattened.push_back(crt_congruence {pp_c.a, pp});
    } else {
      // Dominated congruence. Check for inconsistency.
      if (ntlib::mod(flattened.back().a, pp) != pp_c.a) {
        return std::optional<crt_congruence<T>>();
      }
    }
  }

  // Use coprime case on normalized congruences.
  return ntlib::crt_coprime(flattened);
}

}