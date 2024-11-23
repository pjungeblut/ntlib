/**
 * @file
 * @brief Primary module interface unit for module `turan_number`.
 */
module;

#include <cassert>

/**
 * @module turan_number
 * @brief Compute the number of edges in a Turan graph.
 * 
 * The Turan graph \f$T(n,k)\f$ is a complete \f$k\f$-partite graph on \f$n\f$
 * vertices with almost equal parts.
 * By Turan's theorem, the Turan graph \f$T(n,k)\f$ is the unique graph with the
 * maximum number of edges not containing \f$K_{k+1}\f$ as a subgraph.
 */
export module turan_number;

namespace ntlib {

/**
 * @brief Computes the number of edges in the Turan graph \f$T(n,k)\f$.
 *
 * @tparam T An integer-like type.
 * @param n The number of vertices.
 * @param k The number of bipartition sets.
 * @return The number of edges in \f$T(n,k)\f$.
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