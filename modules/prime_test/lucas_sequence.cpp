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
    matrix<S> mat({{P, -Q}, {S{1}, S{0}}});
    mat = ntlib::pow(mat, n - 1);
    matrix<S> u({{S{1}}, {S{0}}});
    matrix<S> v({{P}, {S{2}}});
    u = mat * u;
    v = mat * v;
    return std::make_pair(u[0][0], v[0][0]);
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
    return std::make_pair(S{0}, mod(S{2}, m));
  } else if (n == N{1}) {
    return std::make_pair(S{1}, mod(P, m));
  } else {
    matrix<S> mat({{mod(P, m), mod(-Q, m)}, {S{1}, S{0}}});

    const auto component_mod_m = [m](const matrix<S> &x) {
      return exec_each_element(x, [m](S y) { return mod(y, m); });
    };
    mat = ntlib::mod_pow(mat, n - 1, component_mod_m,
        matrix<S>::get_identity(2));

    matrix<S> u({{S{1}}, {S{0}}});
    matrix<S> v({{mod(P, m)}, {S{mod(S{2}, m)}}});
    u = mat * u;
    v = mat * v;
    return std::make_pair(mod(u[0][0], m), mod(v[0][0], m));
  }
}

}