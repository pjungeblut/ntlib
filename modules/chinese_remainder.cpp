/**
 * Chinese remainder theorem.
 */

module;

#include <algorithm>
#include <numeric>
#include <optional>
#include <tuple>
#include <type_traits>
#include <vector>

#include "prime_decomposition.hpp"
#include "prime_generation.hpp"

export module chinese_remainder;

import base;
import modulo;

namespace ntlib {

/**
 * Represents a single congruence `x = a mod m`.
 */
export template<typename T>
struct crt_congruence {
  T a;
  T m;
};

/**
 * Chinese remainder theorem for the special case that all moduli are coprime.
 * 
 * A solution always exists and is unique modulo `M = \Pi_i m_i`.
 * 
 * @param congruences The list of congruences.
 * @return The unique solution.
 */
export template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr
crt_congruence<T> crt_coprime(
    const std::vector<crt_congruence<T>> &congurences) noexcept {
  const T M = std::accumulate(congurences.begin(), congurences.end(), T{1},
      [](T prod, crt_congruence<T> c) {
    return prod * c.m;
  });
  
  T res = 0;
  for (const auto &c : congurences) {
    const T M_i = M / c.m;
    const T N_i = mod_mult_inv<T,S>(M_i, c.m);
    res = mod(res + mod(c.a * M_i, M) * N_i, M);
  }
  return crt_congruence {res, M};
}

/**
 * Chinese remainder theorem for the general case in which the moduli do not
 * need to be coprime.
 * 
 * It is possible that no solution exists.
 * If a solution exists, it is unique modulo `M = \lcm_i m_i`. 
 * 
 * @param congruences The list of congruences.
 * @return The unique solution if it exists.
 */
export template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr
std::optional<crt_congruence<T>> crt(
    const std::vector<crt_congruence<T>> &congruences) {
  // Build list of primes, large enough to factor all moduli.
  const T max_mod = std::max_element(congruences.begin(), congruences.end(),
      [](crt_congruence<T> c1, crt_congruence<T> c2) {
    return std::tie(c1.m, c1.a) < std::tie(c2.m, c2.a);
  })->m;
  const T iroot = isqrt(max_mod);
  std::vector<T> primes;
  prime_sieve(iroot, primes);

  struct pp_crt_congruence {
    T a;
    T p;
    T e;
  };

  // Transform congruences into equivalent set of congruences modulo prime
  // powers.
  std::vector<pp_crt_congruence> pp_congruences;
  for (const auto c : congruences) {
    const auto factors = prime_decomposition_complete_list(c.m, primes);
    for (const auto [p, e] : factors) {
      pp_congruences.push_back(pp_crt_congruence {mod(c.a, pow(p, e)), p, e});
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
    const T pp = pow(pp_c.p, pp_c.e);

    if (last_prime != pp_c.p) {
      // First congruence with this prime (and with the highest power).
      last_prime = pp_c.p;
      flattened.push_back(crt_congruence {pp_c.a, pp});
    } else {
      // Dominated congruence. Check for inconsistency.
      if (mod(flattened.back().a, pp) != pp_c.a) {
        return std::optional<crt_congruence<T>>();
      }
    }
  }

  // Use coprime case on normalized congruences.
  return crt_coprime(flattened);
}

}