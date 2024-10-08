#pragma once

#include <cassert>
#include <cstddef>
#include <utility>

#include "base.hpp"
#include "matrix.hpp"
#include "modulo.hpp"

namespace ntlib {

/**
 * Computes the n-th terms of the Lucas sequences U(p,q) and V(p,q) of the first
 * and second kind.
 * Runtime: O(log n)
 *
 * @param n Which term to compute.
 * @param p Parameter p.
 * @param q Parameter q.
 * @return A pair with the U_n(p,q) as the first element and V_n(p,q) as the
 *         second element.
 */
template <typename T>
[[nodiscard]] constexpr
std::pair<T, T> lucas_nth_term(std::size_t n, T p, T q) noexcept {
  if (n == 0) {
    return std::make_pair(T{0}, T{2});
  } else if (n == 1) {
    return std::make_pair(T{1}, p);
  } else {
    matrix<T> mat({{p, -q}, {T{1}, T{0}}});
    mat = ntlib::pow(mat, n - 1);
    matrix<T> u({{T{1}}, {T{0}}});
    matrix<T> v({{p}, {T{2}}});
    u = mat * u;
    v = mat * v;
    return std::make_pair(u[0][0], v[0][0]);
  }
}

/**
 * Computes the n-th terms of the Lucas sequences U(p,q) and V(p,q) of the first
 * and second kind modulo a fixed number.
 * Runtime: O(log n)
 *
 * @param n Which term to compute.
 * @param p Parameter p.
 * @param q Parameter q.
 * @param m The modulus.
 * @return A pair with the U_n(p,q) as the first element and V_n(p,q) as the
 *         second element modulo m.
 */
template <typename T>
[[nodiscard]] constexpr
std::pair<T, T> lucas_nth_term_mod(std::size_t n, T p, T q, T m) noexcept {
  // Check that T is a signed type.
  static_assert(T{-1} < T{1});
  assert(m > T{0});

  if (n == 0) {
    return std::make_pair(T{0}, mod(T{2}, m));
  } else if (n == 1) {
    return std::make_pair(T{1}, mod(p, m));
  } else {
    matrix<T> mat({{mod(p, m), mod(-q, m)}, {T{1}, T{0}}});

    const auto component_mod_m = [m](const matrix<T> &x) {
      return exec_each_element(x, [m](T y) { return mod(y, m); });
    };
    mat = ntlib::mod_pow(mat, n - 1, component_mod_m,
        matrix<T>::get_identity(2));

    matrix<T> u({{T{1}}, {T{0}}});
    matrix<T> v({{mod(p, m)}, {T{mod(T{2}, m)}}});
    u = mat * u;
    v = mat * v;
    return std::make_pair(mod(u[0][0], m), mod(v[0][0], m));
  }
}

}
