/**
 * @file
 * @brief Primary module interface unit for modlue `lucas_sequence`.
 */
module;

#include <cassert>
#include <utility>

/**
 * @module lucas_sequence
 * @brief Compute terms of Lucas sequences.
 */
export module lucas_sequence;

import base;
import matrix;
import modulo;

namespace ntlib {

/**
 * @brief Compute the \f$n\f$-th term of the Lucas sequences of the first and
 * second kind.
 * 
 * Runtime: \f$O(\log(n))\f$
 *
 * @tparam N An integer-like type.
 * @tparam S A signed integer-like type.
 * @param n Which term to compute.
 * @param P Parameter \f$P\f$.
 * @param Q Parameter \f$Q\f$.
 * @return A `std::pair<S,S>` containing \f$U_n(P,Q)\f$ as its first element and
 *     \f$V_n(P,Q)\f$ as its second element.
 */
export template <typename N, typename S>
[[nodiscard]] constexpr
std::pair<S,S> lucas_nth_term(N n, S P, S Q) {
  if (n == N{0}) {
    return std::make_pair(S{0}, S{2});
  } else if (n == N{1}) {
    return std::make_pair(S{1}, P);
  } else {
    matrix<2, 2, S> mat({{P, -Q}, {S{1}, S{0}}});
    mat = ntlib::pow(mat, n - 1);
    matrix<2, 1, S> u({{S{1}}, {S{0}}});
    matrix<2, 1, S> v({{P}, {S{2}}});
    u = mat * u;
    v = mat * v;
    return std::make_pair(u[0, 0], v[0, 0]);
  }
}

/**
 * @brief Compute the \f$n\f$-th term of the Lucas sequences of the first and
 * second kind modulo some number.
 * 
 * Runtime: \f$O(\log(n))\f$
 *
 * @tparam N An integer-like type.
 * @tparam S A signed integer-like type.
 * @param n Which term to compute.
 * @param P Parameter \f$P\f$.
 * @param Q Parameter \f$Q\f$.
 * @param m The modulus.
 * @return A `std::pair<T,T>` containing \f$U_n(P,Q) \mod m\f$ as its first
 *     element and \f$V_n(P,Q) \mod m\f$ as its second element.
 */
export template <typename N, typename S>
[[nodiscard]] constexpr
std::pair<S,S> mod_lucas_nth_term(N n, S P, S Q, S m) {
  // Check that `S` is a signed type.
  static_assert(S{-1} < S{1});
  assert(m > S{0});

  if (n == N{0}) {
    return std::make_pair(S{0}, ntlib::mod(S{2}, m));
  } else if (n == N{1}) {
    return std::make_pair(S{1}, ntlib::mod(P, m));
  } else {
    matrix<2, 2, S> mat({{ntlib::mod(P, m), ntlib::mod(-Q, m)}, {S{1}, S{0}}});

    const auto component_mod_m = [](const matrix<2, 2, S> &x, S m) {
      return exec_each_element(x, [m](S y) { return ntlib::mod(y, m); });
    };
    mat = ntlib::mod_pow(mat, n - 1, m, component_mod_m);

    matrix<2, 1, S> u({{S{1}}, {S{0}}});
    matrix<2, 1, S> v({{ntlib::mod(P, m)}, {S{ntlib::mod(S{2}, m)}}});
    u = mat * u;
    v = mat * v;
    return std::make_pair(ntlib::mod(u[0, 0], m), ntlib::mod(v[0, 0], m));
  }
}

} // namespace ntlib
