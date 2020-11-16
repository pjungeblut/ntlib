#pragma once

#include <cassert>
#include <utility>

#include "base.hpp"
#include "matrix.hpp"

namespace ntlib {

/**
 * Computes the n-th terms of the Lucas sequences U(P,Q) and V(P,Q) of the first
 * and second kind.
 *
 * @param n Which term to compute.
 * @param P Parameter `P`.
 * @param Q Parameter `Q`.
 * @return A pair with the U_n(P,Q) as the first element and V_n(P,Q) as the
 *         second element.
 */
template <typename T>
std::pair<T, T> lucas_nth_term(T n, T P, T Q) {
  assert(n > 0);

  if (n == 0) return std::make_pair(0, 2);
  else if (n == 1) return std::make_pair(1, P);
  else {
    ntlib::matrix<T> m({{P, -Q},{1, 0}});
    m = ntlib::pow(m, n - 1, ntlib::matrix<T>::get_identity(2));
    ntlib::matrix<T> u({{1},{0}});
    ntlib::matrix<T> v({{P},{2}});
    u = m * u;
    v = m * v;
    return std::make_pair(u[0][0], v[0][0]);
  }
}

/**
 * Computes the n-th terms of the Lucas sequences U(P,Q) and V(P,Q) of the first
 * and second kind modulo a fixed number.
 *
 * TODO: This can be extremly dangerous with overflows. Try to fix this by
 *       having only signed overflows so that we always have signed behaviour.
 *
 * @param n Which term to compute.
 * @param P Parameter `P`.
 * @param Q Parameter `Q`.
 * @param mod The modulus.
 * @return A pair with the U_n(P,Q) as the first element and V_n(P,Q) as the
 *         second element.
 */
template <typename T>
std::pair<T, T> lucas_nth_term_mod(T n, T P, T Q, T m) {
  assert(n > 0);

  if (n == 0) return std::make_pair(0, mod(static_cast<T>(2), m));
  else if (n == 1) return std::make_pair(mod(static_cast<T>(1), m), mod(P, m));
  else {
    const T D = P * P - 4 * Q;
    if (n & 1) {
      const auto [uu, vv] = lucas_nth_term_mod(n - 1, P, Q, m);
      auto u = P * uu + vv;
      if (u & 1) u += m;
      u /= 2;
      auto v = D * uu + P * vv;
      if (v & 1) v += m;
      v /= 2;
      return std::make_pair(mod(u, m), mod(v, m));
    } else {
      const auto [uu, vv] = lucas_nth_term_mod(n / 2, P, Q, m);
      return std::make_pair(
          mod(uu * vv, m),
          mod((vv * vv + uu * uu * D) / 2, m)
      );
    }
  }
}

}
