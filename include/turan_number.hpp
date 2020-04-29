#pragma once

namespace ntlib {

/**
 * Computes the number of edges in the Turan Graph T(n,k), that is a complete
 * k-partite graph with almost equal parts.
 *
 * @param n The number of vertices.
 * @param k The number of bipartition sets.
 * @return The number of edges in T(n,k).
 */
template<Integral I>
I turan(I n, I k) {
  I s = n % k;
  return ((k - 1) * (n * n - s * s)) / (2 * k) + s * (s - 1) / 2;
}

}
