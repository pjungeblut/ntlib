#pragma once

/**
 * Chinese remainder theorem.
 */

#include <numeric>
#include <type_traits>
#include <vector>

#include "modulo.hpp"

namespace ntlib {

/**
 * Represents a single congruence `x = a mod m`.
 */
template<typename T>
struct crt_congruence {
  T a;
  T m;
};

/**
 * Chinese remainder theorem for the special case that all moduli are coprime.
 * 
 * @param congruences The list of congruences.
 * @return The unique solution (must exists).
 */
template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr
T crt_coprime(const std::vector<crt_congruence<T>> &congurences) noexcept {
  T M = std::accumulate(congurences.begin(), congurences.end(), T{1},
      [](T prod, crt_congruence<T> c) {
    return prod * c.m;
  });
  
  T res = 0;
  for (const auto &c : congurences) {
    T M_i = M / c.m;
    T N_i = mod_mult_inv<T,S>(M_i, c.m);
    res = mod(res + mod(c.a * M_i, M) * N_i, M);
  }
  return res;
}

};