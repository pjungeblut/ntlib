/**
 * @file
 * @brief Primary module interface unit for module `figurate_number`.
 */

/**
 * @module figurate_number
 * @brief Computes figurate numbers in two and three dimensions.
 */
export module figurate_number;

namespace ntlib {

/**
 * @brief Computes the `n`-th `k`-gonal number.
 * 
 * @tparam T An integer-like type.
 * @param k The number of sides of the polygon.
 * @param n Which `k`-gonal number to generate.
 * @return The `n`-th `k`-gonal number.
 */
export template<typename T>
[[nodiscard]] constexpr
T k_gonal_number(T k, T n) noexcept {
  return ((k - 2) * n * n - (k - 4) * n) / 2;
}

/**
 * @brief Computes the `n`-th centered `k`-gonal number.
 * 
 * @tparam T An integer-like type.
 * @param k The number of sides of the polygon.
 * @param n Which centered `k`-gonal number to generate.
 * @return The `n`-th centered `k`-gonal number.
 */
export template<typename T>
[[nodiscard]] constexpr
T centered_k_gonal_number(T k, T n) noexcept {
  return k * n * (n + 1) / 2 + 1;
}

/**
 * @brief Computes the `n`-th cube number.
 *
 * @tparam T An integer-like type.
 * @param n Which cube number to generate.
 * @return The `n`-th cube number.
 */
export template<typename T>
[[nodiscard]] constexpr
T cube_number(T n) noexcept {
  return n * n * n;
}

/**
 * @brief Computes the `n`-th octahedral number.
 *
 * @tparam T An integer-like type.
 * @param n Which octahedral number to generate.
 * @return The `n`-th octahedral number.
 */
export template<typename T>
[[nodiscard]] constexpr
T octahedral_number(T n) noexcept {
  return n * (2 * n * n + 1) / 3;
}

/**
 * @brief Computes the `n`-th docecahedral number.
 *
 * @tparam T An integer-like type.
 * @param n Which docecahedral number to generate.
 * @return The `n`-th docecahedral number.
 */
export template<typename T>
[[nodiscard]] constexpr
T dodecahedral_number(T n) noexcept {
  return n * (3 * n - 1) * (3 * n - 2) / 2;
}

/**
 * @brief Computes the `n`-th icosahedral number.
 *
 * @tparam T An integer-like type.
 * @param n Which icosahedral number to generate.
 * @return The `n`-th icosahedral number.
 */
export template<typename T>
[[nodiscard]] constexpr
T icosahedral_number(T n) noexcept {
  return n * (5 * n * n - 5 * n + 2) / 2;
}

/**
 * @brief Computes the `n`-th centered tetrahedral number.
 *
 * @tparam T An integer-like type.
 * @param n Which centered tetrahedral number to generate.
 * @return The `n`-th centered tetrahedral number.
 */
export template<typename T>
[[nodiscard]] constexpr
T centered_tetrahedral_number(T n) noexcept {
  return (2 * n + 1) * (n * n + n + 3) / 3;
}

/**
 * @brief Computes the `n`-th centered cube number.
 *
 * @tparam T An integer-like type.
 * @param n Which centered cube number to generate.
 * @return The `n`-th centered cube number.
 */
export template<typename T>
[[nodiscard]] constexpr
T centered_cube_number(T n) noexcept {
  return (2 * n + 1) * (n * n + n + 1);
}

/**
 * @brief Computes the `n`-th centered octahedral number.
 *
 * @tparam T An integer-like type.
 * @param n Which centered octahedral number to generate.
 * @return The `n`-th centered octahedral number.
 */
export template<typename T>
[[nodiscard]] constexpr
T centered_octahedral_number(T n) noexcept {
  return (2 * n + 1) * (2 * n * n + 2 * n + 3) / 3;
}

/**
 * @brief Computes the `n`-th centered dodecahedral number.
 *
 * @tparam T An integer-like type.
 * @param n Which centered dodecahedral number to generate.
 * @return The `n`-th centered dodecahedral number.
 */
export template<typename T>
[[nodiscard]] constexpr
T centered_dodecahedral_number(T n) noexcept {
  return (2 * n + 1) * (5 * n * n + 5 * n + 1);
}

/**
 * @brief Computes the `n`-th centered icosahedral number.
 *
 * @tparam T An integer-like type.
 * @param n Which centered icosahedral number to generate.
 * @return The `n`-th centered icosahedral number.
 */
export template<typename T>
[[nodiscard]] constexpr
T centered_icosahedral_number(T n) noexcept {
  return (2 * n + 1) * (5 * n * n + 5 * n + 3) / 3;
}

/**
 * @brief Computes the `n`-th `k`-gonal pyramidal number.
 * 
 * @tparam T An integer-like type.
 * @param k The number of sides of the polygonal base.
 * @param n Which `k`-gonal pyramidal number to generate.
 * @return The `n`-th `k`-gonal pyramidal number.
 */
export template<typename T>
[[nodiscard]] constexpr
T k_gonal_pyramidal_number(T k, T n) noexcept {
  return (3 * n * n + n * n * n * (k - 2) - n * (k - 5)) / 6;
}

}


