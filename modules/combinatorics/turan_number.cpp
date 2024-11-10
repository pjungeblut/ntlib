module;

#include <cassert>

export module turan_number;

namespace ntlib {

/**
 * Computes the number of edges in the Turan graph `T(n,k)`, that is a complete
 * `k`-partite graph on `n` vertices with almost equal parts.
 *
 * @param n The number of vertices.
 * @param k The number of bipartition sets.
 * @return The number of edges in `T(n,k)`.
 */
export template<typename T>
[[nodiscard]] constexpr
T turan_number(T n, T k) noexcept {
  assert(n >= T{0});
  assert(k > T{0});

  const T s = n % k;
  return ((k - T{1}) * (n * n - s * s)) / (T{2} * k) + s * (s - T{1}) / T{2};
}

}