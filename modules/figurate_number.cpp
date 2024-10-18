/**
 * Implementations to generate figurate numbers.
 */

export module figurate_number;

namespace ntlib {

/**
 * Computes the `n`-th triangle number.
 *
 * @param n Which triangle number to generate.
 * @return The `n`-th triangle number.
 */
export template<typename T>
[[nodiscard]] constexpr
T triangle_number(T n) noexcept {
  return n * (n + 1) / 2;
}

/**
 * Computes the `n`-th square number.
 *
 * @param n Which square number to generate.
 * @return The `n`-th square number.
 */
export template<typename T>
[[nodiscard]] constexpr
T square_number(T n) noexcept {
  return n * n;
}

/**
 * Computes the `n`-th pentagonal number.
 *
 * @param n Which pentagonal number to generate.
 * @return The `n`-th pentagonal number.
 */
export template<typename T>
[[nodiscard]] constexpr
T pentagonal_number(T n) noexcept {
  return n * (3 * n - 1) / 2;
}

/**
 * Computes the `n`-th hexagonal number.
 *
 * @param n Which hexagonal number to generate.
 * @return The `n`-th hexagonal number.
 */
export template<typename T>
[[nodiscard]] constexpr
T hexagonal_number(T n) noexcept {
  return n * (2 * n - 1);
}

/**
 * Computes the `n`-th heptagonal number.
 *
 * @param n Which heptagonal number to generate.
 * @return The `n`-th heptagonal number.
 */
export template<typename T>
[[nodiscard]] constexpr
T heptagonal_number(T n) noexcept {
  return n * (5 * n - 3) / 2;
}

/**
 * Computes the `n`-th octagonal number.
 *
 * @param n Which octagonal number to generate.
 * @return The `n`-th octagonal number.
 */
export template<typename T>
[[nodiscard]] constexpr
T octagonal_number(T n) noexcept {
  return n * (3 * n - 2);
}

/**
 * Computes the `n`-th nonagonal number.
 *
 * @param n Which nonagonal number to generate.
 * @return The `n`-th nonagonal number.
 */
export template<typename T>
[[nodiscard]] constexpr
T nonagonal_number(T n) noexcept {
  return (7 * n * n - 5 * n) / 2;
}

/**
 * Computes the `n`-th decagonal number.
 *
 * @param n Which decagonal number to generate.
 * @return The `n`-th decagonal number.
 */
export template<typename T>
[[nodiscard]] constexpr
T decagonal_number(T n) noexcept {
  return 4 * n * n - 3 * n;
}

/**
 * Computes the `n`-th centered square number.
 *
 * @param n Which centered square number to gnerate.
 * @return The `n`-th centered square number.
 */
export template<typename T>
[[nodiscard]] constexpr
T centered_square_number(T n) noexcept {
  return square_number(n + 1) + square_number(n);
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
 * Computes the `n`-th tetrahedral number.
 *
 * @param n Which tetrahedral number to generate.
 * @return The `n`-th tetrahedral number.
 */
export template<typename T>
[[nodiscard]] constexpr
T tetrahedral_number(T n) noexcept {
  return n * (n + 1) / 2 * (n + 2) / 3;
}

/**
 * Computes the `n`-th square pyramid number.
 *
 * @param n Which square pyramid number to gnerate.
 * @return The `n`-th square pyramid number.
 */
export template<typename T>
[[nodiscard]] constexpr
T square_pyramidal_number(T n) noexcept {
  return n * (n + 1) / 2 * (2 * n + 1) / 3;
}

}


