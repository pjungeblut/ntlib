module;

#include <cassert>
#include <utility>

#include "matrix.hpp"
#include "modulo.hpp"

export module lucas_sequence;

import base;

namespace ntlib {

/**
 * Computes the `n`-th terms of the Lucas sequences `U(P,Q)` and `V(P,Q)` of the
 * first and second kind.
 * Runtime: `O(log n)`
 *
 * @param n Which term to compute.
 * @param P Parameter `P`.
 * @param Q Parameter `Q`.
 * @return A pair with `U_n(P,Q)` as the first element and `V_n(P,Q)` as the
 *     second element.
 */
export template <typename N, typename T>
[[nodiscard]] constexpr
std::pair<T, T> lucas_nth_term(N n, T P, T Q) {
  if (n == N{0}) {
    return std::make_pair(T{0}, T{2});
  } else if (n == N{1}) {
    return std::make_pair(T{1}, P);
  } else {
    matrix<T> mat({{P, -Q}, {T{1}, T{0}}});
    mat = ntlib::pow(mat, n - 1);
    matrix<T> u({{T{1}}, {T{0}}});
    matrix<T> v({{P}, {T{2}}});
    u = mat * u;
    v = mat * v;
    return std::make_pair(u[0][0], v[0][0]);
  }
}

/**
 * Computes the `n`-th terms of the Lucas sequences `U(P,Q)` and `V(P,Q)` of the
 * first and second kind modulo a fixed number `m`.
 * Runtime: O(log n)
 *
 * @param n Which term to compute.
 * @param P Parameter `P`.
 * @param Q Parameter `Q`.
 * @param m The modulus.
 * @return A pair with `U_n(P,Q)` as the first element and `V_n(P,Q)` as the
 *         second element, both modulo `m`.
 */
export template <typename N, typename T>
[[nodiscard]] constexpr
std::pair<T, T> mod_lucas_nth_term(N n, T P, T Q, T m) {
  // Check that T is a signed type.
  static_assert(T{-1} < T{1});
  assert(m > T{0});

  if (n == N{0}) {
    return std::make_pair(T{0}, mod(T{2}, m));
  } else if (n == N{1}) {
    return std::make_pair(T{1}, mod(P, m));
  } else {
    matrix<T> mat({{mod(P, m), mod(-Q, m)}, {T{1}, T{0}}});

    const auto component_mod_m = [m](const matrix<T> &x) {
      return exec_each_element(x, [m](T y) { return mod(y, m); });
    };
    mat = ntlib::mod_pow(mat, n - 1, component_mod_m,
        matrix<T>::get_identity(2));

    matrix<T> u({{T{1}}, {T{0}}});
    matrix<T> v({{mod(P, m)}, {T{mod(T{2}, m)}}});
    u = mat * u;
    v = mat * v;
    return std::make_pair(mod(u[0][0], m), mod(v[0][0], m));
  }
}

}