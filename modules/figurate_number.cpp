/**
 * Implementations to generate figurate numbers.
 */

export module figurate_number;

namespace ntlib {

/**
 * Computes the `n`-th `k`-gonal number.
 * 
 * @param n Which `k`-gonal number to generate.
 * @return The `n`-th `k`-gonal number.
 */
export template<typename T>
[[nodiscard]] constexpr
T k_gonal_number(T k, T n) noexcept {
  return ((k - 2) * n * n - (k - 4) * n) / 2;
}

/**
 * Computes the `n`-th centered `k`-gonal number.
 * 
 * @param n Which centered `k`-gonal number to generate.
 * @return The `n`-th centered `k`-gonal number.
 */
export template<typename T>
[[nodiscard]] constexpr
T centered_k_gonal_number(T k, T n) noexcept {
  return k * n * (n + 1) / 2 + 1;
}

/**
 * Computes the `n`-th cube number.
 *
 * @param n Which cube number to generate.
 * @return The `n`-th cube number.
 */
export template<typename T>
[[nodiscard]] constexpr
T cube_number(T n) noexcept {
  return n * n * n;
}

/**
 * Computes the `n`-th octahedral number.
 *
 * @param n Which octahedral number to generate.
 * @return The `n`-th octahedral number.
 */
export template<typename T>
[[nodiscard]] constexpr
T octahedral_number(T n) noexcept {
  return n * (2 * n * n + 1) / 3;
}

/**
 * Computes the `n`-th docecahedral number.
 *
 * @param n Which docecahedral number to generate.
 * @return The `n`-th docecahedral number.
 */
export template<typename T>
[[nodiscard]] constexpr
T dodecahedral_number(T n) noexcept {
  return n * (3 * n - 1) * (3 * n - 2) / 2;
}

/**
 * Computes the `n`-th icosahedral number.
 *
 * @param n Which icosahedral number to generate.
 * @return The `n`-th icosahedral number.
 */
export template<typename T>
[[nodiscard]] constexpr
T icosahedral_number(T n) noexcept {
  return n * (5 * n * n - 5 * n + 2) / 2;
}

/**
 * Computes the `n`-th centered tetrahedral number.
 *
 * @param n Which centered tetrahedral number to generate.
 * @return The `n`-th centered tetrahedral number.
 */
export template<typename T>
[[nodiscard]] constexpr
T centered_tetrahedral_number(T n) noexcept {
  return (2 * n + 1) * (n * n + n + 3) / 3;
}

/**
 * Computes the `n`-th centered cube number.
 *
 * @param n Which centered cube number to generate.
 * @return The `n`-th centered cube number.
 */
export template<typename T>
[[nodiscard]] constexpr
T centered_cube_number(T n) noexcept {
  return (2 * n + 1) * (n * n + n + 1);
}

/**
 * Computes the `n`-th centered octahedral number.
 *
 * @param n Which centered octahedral number to generate.
 * @return The `n`-th centered octahedral number.
 */
export template<typename T>
[[nodiscard]] constexpr
T centered_octahedral_number(T n) noexcept {
  return (2 * n + 1) * (2 * n * n + 2 * n + 3) / 3;
}

/**
 * Computes the `n`-th centered dodecahedral number.
 *
 * @param n Which centered dodecahedral number to generate.
 * @return The `n`-th centered dodecahedral number.
 */
export template<typename T>
[[nodiscard]] constexpr
T centered_dodecahedral_number(T n) noexcept {
  return (2 * n + 1) * (5 * n * n + 5 * n + 1);
}

/**
 * Computes the `n`-th centered icosahedral number.
 *
 * @param n Which centered icosahedral number to generate.
 * @return The `n`-th centered icosahedral number.
 */
export template<typename T>
[[nodiscard]] constexpr
T centered_icosahedral_number(T n) noexcept {
  return (2 * n + 1) * (5 * n * n + 5 * n + 3) / 3;
}

/**
 * Computes the `n`-th `k`-gonal pyramidal number.
 * 
 * @param n Which `k`-gonal pyramidal number to generate.
 * @return The `n`-th `k`-gonal pyramidal number.
 */
export template<typename T>
[[nodiscard]] constexpr
T k_gonal_pyramidal_number(T k, T n) noexcept {
  return (3 * n * n + n * n * n * (k - 2) - n * (k - 5)) / 6;
}

}


