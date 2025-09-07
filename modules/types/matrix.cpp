/**
 * @file
 * @brief Primary module interface unit for module `matrix`.
 */
module;

#include <cassert>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <ostream>
#include <string>

/**
 * @module matrix
 * @brief Represents a matrix whose dimensions are compile time constants.
 */
export module matrix;

import base;

namespace ntlib {

/**
 * @brief Represents a matrix with a given number of rows and colums.
 * 
 * @tparam ROWS The number of rows.
 * @tparam COLUMS The number of colums.
 * @tparam T The element type.
 */
export template<std::size_t ROWS, std::size_t COLUMNS, std::regular T>
class matrix {
public:
  /**
   * @brief Creates new matrix.
   * 
   * Elements will be default initialized.
   */
  matrix() = default;

  /**
   * @brief Constructs a new matrix from a given 2D initializer list.
   * 
   * The 2D initializer list shall be a list of lines.
   * 
   * @param elements The elements of the matrix.
   */
  matrix(const std::initializer_list<std::initializer_list<T>> &elements) {
    assert(elements.size() == ROWS);

    std::size_t r = 0;
    for (const auto &line : elements) {
      assert(line.size() == COLUMNS);
      std::size_t c = 0;
      for (const auto &element : line) {
        mat[r][c] = element;
        ++c;
      }
      ++r;
    }
  }

  /**
   * @brief Two dimensional array subscript operator.
   * 
   * @param self Explicity object parameter.
   * @param r The row.
   * @param c The column.
   * @return (Constant) reference to the element at the given position.
   */
  decltype(auto) operator[](this auto& self, std::size_t r, std::size_t c) {
    return self.mat[r][c];
  }

  /**
   * @brief Compound plus operator for matrix addition.
   * 
   * @param rhs The second summand.
   * @return Reference to the result.
   */
  matrix& operator+=(const matrix &rhs) {
    for (std::size_t r = 0; r < ROWS; ++r) {
      for (std::size_t c = 0; c < COLUMNS; ++c) {
        mat[r][c] += rhs[r, c];
      }
    }
    return *this;
  }

  /**
   * @brief Compound minus operator for matrix subtraction.
   *
   * @param rhs The subtrahend.
   * @return Reference to the result.
   */
  matrix& operator-=(const matrix &rhs) {
    for (std::size_t r = 0; r < ROWS; ++r) {
      for (std::size_t c = 0; c < COLUMNS; ++c) {
        mat[r][c] -= rhs[r, c];
      }
    }
    return *this;
  }

  /**
   * @brief Compound times operator for right scalar multiplication.
   *
   * @param rhs The second factor. A scalar.
   * @return Reference to the result.
   */
  template<typename S>
  matrix& operator*=(S rhs) {
    for (std::size_t r = 0; r < ROWS; ++r) {
      for (std::size_t c = 0; c < COLUMNS; ++c) {
        mat[r][c] *= rhs;
      }
    }
    return *this;
  }

  /**
   * @brief Compound divides operator for scalar division.
   *
   * @param rhs The divisor. A scalar.
   * @return Reference to this matrix.
   */
  template<typename S>
  matrix& operator/=(S rhs) {
    assert(rhs != S{0});

    for (std::size_t r = 0; r < ROWS; ++r) {
      for (std::size_t c = 0; c < COLUMNS; ++c) {
        mat[r][c] /= rhs;
      }
    }
    return *this;
  }

  /**
   * @brief Compound modulo operator for elementwise modulo by a scalar.
   * 
   * @param rhs The divisor. A scalar.
   * @return Reference to the result.
   */
  template<typename S>
  matrix& operator%=(S rhs) {
    assert(rhs != S{0});

    for (std::size_t r = 0; r < ROWS; ++r) {
      for (std::size_t c = 0; c < COLUMNS; ++c) {
        mat[r][c] %= rhs;
      }
    }
    return *this;
  }

  /**
   * @brief Compound times operator for matrix multiplication.
   *
   * @tparam RHS_ROWS The number of rows of the second factor.
   * @tparam RHS_COLUMNS The number of columns of the second factor.
   * @param rhs The second factor.
   * @return Reference to the result.
   */
  template<std::size_t RHS_ROWS, std::size_t RHS_COLUMNS>
      requires (ROWS == COLUMNS && ROWS == RHS_ROWS && COLUMNS == RHS_COLUMNS)
  matrix& operator*=(const matrix<RHS_ROWS, RHS_COLUMNS, T> &rhs) {
    static_assert(COLUMNS == RHS_ROWS,
        "Incompatible dimensions for matrix multiplication.");

    *this = *this * rhs;
    return *this;
  }

  /**
   * @brief Default equality operator.
   *
   * @return Whether two matrices are equal.
   */
  friend bool operator==(const matrix&, const matrix&) = default;

  /**
   * @brief Returns a zero matrix.
   * 
   * @return A zero matrix.
   */
  static matrix get_zero() {
    matrix zero_mat;
    for (std::size_t i = 0; i < ROWS; ++i) {
      for (std::size_t j = 0; j < COLUMNS; ++j) {
        zero_mat[i, j] = ntlib::zero<T>();
      }
    }
    return zero_mat;
  }

  /**
   * @brief Returns a square identity matrix.
   * 
   * @return An identity matrix.
   */
  template<typename = void>
      requires (ROWS == COLUMNS)
  static matrix get_identity() {
    matrix id = get_zero();
    for (std::size_t i = 0; i < ROWS; ++i) {
      id[i, i] = ntlib::one<T>();
    }
    return id;
  }

private:
  /// @brief The entries of the matrix.
  T mat[ROWS][COLUMNS];
};

/**
 * @brief Converts a given matrix into a human readable string representation.
 * 
 * For example, the matrix
 * \f[
 * \begin{pmatrix}
 * a & b \\
 * c & d
 * \end{pmatrix}
 * \f]
 * will be converted into "{{a,b},{c,d}}".
 *
 * @tparam ROWS The number of rows.
 * @tparam COLUMNS The number of columns.
 * @tparam T The element type.
 * @param mat The given matrix.
 * @return A string representation of the given matrix.
 */
export template<std::size_t ROWS, std::size_t COLUMNS, std::regular T>
std::string to_string(matrix<ROWS, COLUMNS, T> mat) {
  using std::to_string;

  std::string rep = "";
  rep += "{";
  for (std::size_t r = 0; r < ROWS; ++r) {
    if (r) rep += ",";
    rep += "{";
    for (std::size_t c = 0; c < COLUMNS; ++c) {
      if (c) rep += ",";
      rep += to_string(mat[r, c]);
    }
    rep += "}";
  }
  rep += "}";

  return rep;
}

/**
 * @brief Writes a string representation of a given matrix to a given output
 * stream.
 *
 * @tparam ROWS The number of rows.
 * @tparam COLUMS The number of colums.
 * @tparam T The element type.
 * @param os The given output stream.
 * @param m The given matrix.
 * @return Reference to the output stream.
 */
export template<std::size_t ROWS, std::size_t COLUMNS, std::regular T>
std::ostream& operator<<(std::ostream &os, const matrix<ROWS, COLUMNS, T> &m) {
  os << ntlib::to_string(m);
  return os;
}

/**
 * @brief Plus operator for matrix addition.
 *
 * @tparam ROWS The number of rows.
 * @tparam COLUMNS The number of columns.
 * @tparam T The element type.
 * @param lhs The first summand.
 * @param rhs The second summand.
 * @return The sum of `lhs` and `rhs`.
 */
export template<std::size_t ROWS, std::size_t COLUMNS, std::regular T>
[[nodiscard]]
matrix<ROWS, COLUMNS, T> operator+(
    matrix<ROWS, COLUMNS, T> lhs, const matrix<ROWS, COLUMNS, T> &rhs) {
  lhs += rhs;
  return lhs;
}

/**
 * @brief Minus operator for matrix subtraction.
 *
 * @tparam ROWS The number of rows.
 * @tparam COLUMNS The number of columns.
 * @tparam T The element type.
 * @param lhs The minuend.
 * @param rhs The subtrahend.
 * @return The difference of `lhs` and `rhs`.
 */
export template<std::size_t ROWS, std::size_t COLUMNS, std::regular T>
matrix<ROWS, COLUMNS, T> operator-(
    matrix<ROWS, COLUMNS, T> lhs, const matrix<ROWS, COLUMNS, T> &rhs) {
  lhs -= rhs;
  return lhs;
}

/**
 * @brief Times operator for right scalar multiplicatoin.
 *
 * @tparam ROWS The number of rows.
 * @tparam COLUMNS The number of columns.
 * @tparam T The element type.
 * @tparam S The scalar type.
 * @param lhs The first factor, a matrix.
 * @param rhs The second factor, a scalar.
 * @return The product of `lhs` and `rhs`.
 */
export template<
    std::size_t ROWS, std::size_t COLUMNS, std::regular T, typename S>
matrix<ROWS, COLUMNS, T> operator*(matrix<ROWS, COLUMNS, T> lhs, S rhs) {
  lhs *= rhs;
  return lhs;
}

/**
 * @brief Times operator for left scalar multiplicatoin.
 *
 * @tparam ROWS The number of rows.
 * @tparam COLUMNS The number of columns.
 * @tparam T The element type.
 * @tparam S The scalar type.
 * @param lhs The first factor, a scalar.
 * @param rhs The second factor, a matrix.
 * @return The product of `lhs` and `rhs`.
 */
export template<
    std::size_t ROWS, std::size_t COLUMNS, std::regular T, typename S>
matrix<ROWS, COLUMNS, T> operator*(S lhs, matrix<ROWS, COLUMNS, T> rhs) {
  // TODO (pjungeblut): Fix this!
  // This uses commutativiy of scalar multiplication.
  // Depending on T this might not be true!
  rhs *= lhs;
  return rhs;
}

/**
 * @brief Divides operator for scalar division.
 *
 * @tparam ROWS The number of rows.
 * @tparam COLUMNS The number of columns.
 * @tparam T The element type.
 * @tparam S The scalar type.
 * @param lhs The dividend. A matrix.
 * @param rhs The divisor. A scalar.
 * @return The quotient matrix.
 */
export template<
    std::size_t ROWS, std::size_t COLUMNS, std::regular T, typename S>
matrix<ROWS, COLUMNS, T> operator/(matrix<ROWS, COLUMNS, T> lhs, S rhs) {
  lhs /= rhs;
  return lhs;
}

/**
 * @brief Modulo operator for elementwise modulo by a scalar.
 * 
 * @tparam ROWS The number of rows.
 * @tparam COLUMNS The number of columns.
 * @tparam T The element type.
 * @tparam S The scalar type.
 * @param lhs The dividend. A matrix.
 * @param rhs The divisor. A scalar.
 * @return A matrix with every element being the remainder of the
 *     corresponding element in `lhs` after divison by `rhs`.
 */
export template<
    std::size_t ROWS, std::size_t COLUMNS, std::regular T, typename S>
matrix<ROWS, COLUMNS, T> operator%(matrix<ROWS, COLUMNS, T> lhs, S rhs) {
  lhs %= rhs;
  return lhs;
}

/**
 * @brief Unary plus operator. Does nothing.
 *
 * @tparam ROWS The number of rows.
 * @tparam COLUMNS The number of columns.
 * @tparam T The element type.
 * @param rhs The matrix..
 * @return A copy of the old matrix.
 */
export template<std::size_t ROWS, std::size_t COLUMNS, std::regular T>
matrix<ROWS, COLUMNS, T> operator+(const matrix<ROWS, COLUMNS, T> &rhs) {
  return rhs;
}

/**
 * @brief Unary minus operator.
 *
 * @tparam ROWS The number of rows.
 * @tparam COLUMNS The number of columns.
 * @tparam T The element type.
 * @param rhs The matrix.
 * @return A matrix with all elements negated.
 */
export template<std::size_t ROWS, std::size_t COLUMNS, std::regular T>
matrix<ROWS, COLUMNS, T> operator-(const matrix<ROWS, COLUMNS, T> &rhs) {
  matrix<ROWS, COLUMNS, T> negated;
  for (std::size_t r = 0; r < ROWS; ++r) {
    for (std::size_t c = 0; c < COLUMNS; ++c) {
      negated[r, c] = -rhs[r, c];
    }
  }
  return negated;
}

/**
 * @brief Times operator for matrix multiplication.
 * 
 * @tparam ROWS_LHS The number of rows of the first factor.
 * @tparam COLUMNS_LHS The number of columns of the first factor.
 * @tparam ROWS_RHS The number of rows of the second factor.
 * @tparam COLUMNS_RHS The number of columns of the second factor.
 * @tparam T The element type.
 * @param lhs The first factor.
 * @param rhs The second factor.
 * @return The matrix product of `lhs` and `rhs`.
 */
export template<
    std::size_t ROWS_LHS,
    std::size_t COLUMNS_LHS,
    std::size_t ROWS_RHS,
    std::size_t COLUMNS_RHS,
    std::regular T>
    requires (COLUMNS_LHS == ROWS_RHS)
matrix<ROWS_LHS, COLUMNS_RHS, T> operator*(
    const matrix<ROWS_LHS, COLUMNS_LHS, T> &lhs,
    const matrix<ROWS_RHS, COLUMNS_RHS, T> &rhs) {

  auto product = matrix<ROWS_LHS, COLUMNS_RHS, T>::get_zero();
  for (std::size_t r = 0; r < ROWS_LHS; ++r) {
    for (std::size_t k = 0; k < COLUMNS_LHS; ++k) {
      for (std::size_t c = 0; c < COLUMNS_RHS; ++c) {
        product[r, c] += lhs[r, k] * rhs[k, c];
      }
    }
  }
  return product;
}

/**
 * @brief Executes a function an each element.
 * 
 * TODO (pjungeblut): Try to get rid of this function.
 * 
 * @tparam ROWS The number of rows.
 * @tparam COLUMNS The number of columns.
 * @tparam T The element type.
 * @tparam F A function object.
 * @param m The matrix.
 * @param func The function to execute.
 * @return The new matrix.
 */
export template<std::size_t ROWS, std::size_t COLUMNS, std::regular T, typename F>
matrix<ROWS, COLUMNS, T> exec_each_element(
    const matrix<ROWS, COLUMNS, T> &m, const F &func) {
  matrix<ROWS, COLUMNS, T> res;
  for (std::size_t r = 0; r < ROWS; ++r) {
    for (std::size_t c = 0; c < COLUMNS; ++c) {
      res[r, c] = func(m[r, c]);
    }
  }
  return res;
}

/**
 * @brief Specialization of `ntlib::zero_helper` for `matrix`.
 * 
 * @tparam ROWS The number of rows.
 * @tparam COLUMNS The number of columns.
 * @tparam T The element type.
 */
export template<std::size_t ROWS, std::size_t COLUMNS, std::regular T>
class zero_helper<matrix<ROWS, COLUMNS, T>> {
public:
  /**
   * @brief Returns the additive neutral element of `matrix<ROWS, COLUMNS, T>`.
   * 
   * @return The additive neutral element, i.e., the zero matrix.
   */
  [[nodiscard]] constexpr
  static matrix<ROWS, COLUMNS, T> get_zero() noexcept {
    return matrix<ROWS, COLUMNS, T>::get_zero();
  }
};

/**
 * @brief Specialization of `ntlib::one_helper` for `matrix`.
 * 
 * @tparam ROWS The number of rows.
 * @tparam COLUMNS The number of columns.
 * @tparam T The element type.
 */
export template<std::size_t ROWS, std::size_t COLUMNS, std::regular T>
class one_helper<matrix<ROWS, COLUMNS, T>> {
public:
  /**
   * @brief Returns the multiplicative neutral element of
   *     `matrix<ROWS, COLUMNS, T>`.
   * 
   * @return The multiplicative neutral element, i.e., the identity matrix.
   */
  [[nodiscard]] constexpr
  static matrix<ROWS, COLUMNS, T> get_one() noexcept {
    return matrix<ROWS, COLUMNS, T>::get_identity();
  }
};

} // namespace ntlib
