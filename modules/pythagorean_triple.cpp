/**
 * Implementations to generate pythagorean triples `(a,b,c)`, such that
 * `a^2 + b^2 = c^2`.
 * A triple is called primitive, if `gcd(a,b,c) = 1`.
 *
 * For any natural `u > v > 1` we get a pythagorean triple by setting
 * `a := u^2 - v^2`
 * `b := 2uv`
 * `c := u^2 + v^2`.
 * The smalles primitive triple is `(3,4,5)`, created by `u = 2` and `v = 1`.
 *
 * `(a,b,c)` is primitive, if and only if `gcd(u,v) = 1` and at least one of
 * them is even. Every primitive triple an be constructed by above formula.
 */

module;

#include <algorithm>
#include <tuple>
#include <vector>

export module pythagorean_triple;

import base;

namespace ntlib {

/**
 * Creates all primitive pythagorean triples `(a,b,c)` with `a <= b <= c <= N`.
 *
 * @param N The maximum value of any component.
 * @return triples The vector to push the triples into.
 */
export template<typename T>
[[nodiscard]] constexpr
std::vector<std::tuple<T,T,T>> primitive_pythagorean_triples(T N) {
  std::vector<std::tuple<T,T,T>> triples;
  for (T v{1}; T{2} * v * v <= N; ++v) {
    for (T u = v + T{1}; u * u + v * v <= N; ++u) {
      if (!(is_odd(u) && is_odd(v)) && gcd(u, v) == T{1}) {
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
