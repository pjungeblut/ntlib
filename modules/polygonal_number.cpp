/**
 * Implementations to generate polygonal numbers.
 */

export module polygonal_number;

namespace ntlib {

/**
 * Computes the n-th triangle number.
 *
 * @param n Which triangle number to generate.
 * @return The n-th triangle number.
 */
export template<typename T>
T triangle_number(T n) {
  return n * (n + 1) / 2;
}

/**
 * Computes the n-th square number.
 *
 * @param n Which square number to generate.
 * @return The n-th square number.
 */
export template<typename T>
T square_number(T n) {
  return n * n;
}

/**
 * Computes the n-th pentagonal number.
 *
 * @param n Which pentagonal number to generate.
 * @return The n-th pentagonal number.
 */
export template<typename T>
T pentagonal_number(T n) {
  return n * (3 * n - 1) / 2;
}

/**
 * Computes the n-th hexagonal number.
 *
 * @param n Which hexagonal number to generate.
 * @return The n-th hexagonal number.
 */
export template<typename T>
T hexagonal_number(T n) {
  return n * (2 * n - 1);
}

/**
 * Computes the n-th heptagonal number.
 *
 * @param n Which heptagonal number to generate.
 * @return The n-th heptagonal number.
 */
export template<typename T>
T heptagonal_number(T n) {
  return n * (5 * n - 3) / 2;
}

/**
 * Computes the n-th octagonal number.
 *
 * @param n Which octagonal number to generate.
 * @return The n-th octagonal number.
 */
export template<typename T>
T octagonal_number(T n) {
  return n * (3 * n - 2);
}

/**
 * Computes the n-th tetrahedral number.
 *
 * @param n Which tetrahedral number to generate.
 * @return The n-th tetrahedral number.
 */
export template<typename T>
T tetrahedral_number(T n) {
  return n * (n + 1) / 2 * (n + 2) / 3;
}

/**
 * Computes the n-th cube number.
 *
 * @param n Which cube number to generate.
 * @return The n-th cube number.
 */
export template<typename T>
T cube_number(T n) {
  return n * n * n;
}

/**
 * Computes the n-th octahedral number.
 *
 * @param n Which octahedral number to generate.
 * @return The n-th octahedral number.
 */
export template<typename T>
T octahedral_number(T n) {
  return n * (2 * n * n + 1) / 3;
}

/**
 * Computes the n-th square pyramid number.
 *
 * @param n Which square pyramid number to gnerate.
 * @return The n-th square pyramid number.
 */
export template<typename T>
T square_pyramid_number(T n) {
  return n * (n + 1) / 2 * (2 * n + 1) / 3;
}

/**
 * Computes the n-th centered square number.
 *
 * @param n Which centered square number to gnerate.
 * @return The n-th centered square number.
 */
export template<typename T>
T centered_square_number(T n) {
  return square_number(n) + square_number(n - 1);
}

}


