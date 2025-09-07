/**
 * @file
 * @brief Primary module interface unit for module `pythagorean_triple`.
 */
module;

#include <algorithm>
#include <tuple>
#include <vector>

/**
 * @brief Generate primitive Pythagorean triples.
 * 
 * Implementations to generate Pythagorean triples, i.e., triples \f$(a,b,c)\f$
 * such that \f$a^2 + b^2 = c^2\f$.
 * A triple is called primitive, if \f$\mathrm{gcd}(a,b,c) = 1\f$.
 *
 * For all integers \f$u > v > 1\f$ we get a Pythagorean triple by setting
 * \f[
 * \begin{align}
 * a &\colon= u^2 - v^2 \\
 * b &\colon= 2uv \\
 * c &\colon= u^2 + v^2`
 * \end{align}
 * \f]
 * The smalles primitive triple is \f$(3,4,5)\f$, obtained by choosing
 * \f$u = 2\f$ and \f$v = 1\f$.
 *
 * It holds that \f$(a,b,c)\f$ is primitive, if and only if
 * \f$\mathrm{gcd}(u,v) = 1\f$ and at least one of them is even. Every primitive
 * Pythagorean triple an be constructed by above formula.
 */
export module pythagorean_triple;

import base;

namespace ntlib {

/**
 * @brief Creates all primitive pythagorean triples \f$(a,b,c)\f$ with
 * \f$a \leq b \leq c \leq N\f$.
 *
 * @tparam T An integer-like type.
 * @param N The maximum value of any component.
 * @return triples A `std::vector` containing the triples.
 */
export template<Integer T>
[[nodiscard]] constexpr
std::vector<std::tuple<T,T,T>> primitive_pythagorean_triples(T N) {
  std::vector<std::tuple<T,T,T>> triples;
  for (T v{1}; T{2} * v * v <= N; ++v) {
    for (T u = v + T{1}; u * u + v * v <= N; ++u) {
      if (!(ntlib::is_odd(u) && ntlib::is_odd(v)) && ntlib::gcd(u, v) == T{1}) {
        T a = u * u - v * v;
        T b = T{2} * u * v;
        T c = u * u + v * v;
        if (a > b) { std::swap(a, b); }
        triples.push_back(std::make_tuple(a, b, c));
      }
    }
  }
  return triples;
}

}
