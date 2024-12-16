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
#include <vector>

/**
 * @module matrix
 * @brief Represents a matrix with runtime provided dimensions.
 */
export module matrix;

import base;

namespace ntlib {

/**
 * @brief Represents a matrix with a given number of rows and colums.
 * 
 * The dimensions of the matrix are set at runtime.
 * 
 * @tparam T The element type.
 */
export template<std::regular T>
class matrix {
public:
  /**
   * @brief Default constructor. Creates 0x0-matrix.
   */
  matrix() : rows(0), columns(0) {};

  /**
   * @brief Constructs a 1x1 matrix from a given value.
   *
   * @param val The given value.
   */
  explicit matrix(T val) : rows(1), columns(1) {
    mat.resize(rows, std::vector<T>(columns));
    mat[0][0] = val;
  }

  /**
   * @brief Constructs a new matrix of the given dimensions,
   * 
   * Elements will be default initialized.
   *
   * @param rows The number of rows.
   * @param columns The number of columns.
   */
  matrix(std::size_t rows, std::size_t columns) : rows(rows), columns(columns) {
    mat.resize(rows, std::vector<T>(columns));
  }

  /**
   * @brief Constructs a new matrix from a given 2D initializer list.
   * 
   * The 2D initializer list shall be a list of lines.
   *
   * @param elements The elements of the matrix.
   */
  matrix(std::initializer_list<std::initializer_list<T>> elements) {
    rows = elements.size();
    if (rows == 0) { return; }

    columns = elements.begin()->size();
    mat.resize(rows, std::vector<T>(columns));

    std::size_t r = 0;
    for (const auto &line : elements) {
      assert(line.size() == columns);
      mat[r] = line;
      ++r;
    }
  }

  /**
   * @brief Returns the number of rows.
   *
   * @return The number of rows.
   */
  std::size_t get_rows() const noexcept {
    return rows;
  }

  /**
   * @brief Returns the number of columns.
   *
   * @return The number of columns.
   */
  std::size_t get_columns() const noexcept {
    return columns;
  }

  /**
   * @brief A proxy class to provide an lvalue that can be returned from
   * `matrix<T>::operator[]`.
   * 
   * This is necessary so that values can be changed but for example dimensions
   * cannot.
   */
  class reference {
    friend class matrix;

    /**
     * @brief Constant pointer to the queried row.
     */
    std::vector<T>* const row;

    /**
     * @brief Constructs the reference.
     *
     * @param row Pointer to the accessed row.
     */
    reference(std::vector<T>* const row) : row(row) {};

  public:
    /**
     * @brief Array subscript operator to access the column.
     *
     * @param col_idx The index of the column to access.
     * @return The value in the referenced row and column `col_idx`.
     */
    [[nodiscard]]
    T& operator[](std::size_t col_idx) {
      return (*row)[col_idx];
    }

    /**
     * @brief Constant array subscript operator to access the column.
     * 
     * @param col_idx The index of the column to access.
     * @return The value in the referenced tow and column `col_idx`.
     */
    [[nodiscard]]
    const T& operator[](std::size_t col_idx) const {
      return (*row)[col_idx];
    }
  };

  /**
   * @brief Array subscript operator.
   *
   * @param row_idx The index of the row to access.
   * @return A reference to the vector containing the entries of the queried
   *         row.
   */
  [[nodiscard]]
  reference operator[](std::size_t row_idx) {
    return reference(&mat[row_idx]);
  }

  /**
   * @brief Constant array subscript operator.
   *
   * @param row_idx The index of the row to access.
   * @return A constant reference to the vector containing the entries of the
   *         queried row.
   */
  const std::vector<T>& operator[](std::size_t row_idx) const {
    return mat[row_idx];
  }

  /**
   * @brief Compound plus operator for matrix addition.
   * 
   * @param rhs The second summand.
   * @return Reference to the result.
   */
  matrix& operator+=(const matrix &rhs) {
    assert(rows == rhs.rows);
    assert(columns == rhs.columns);

    for (std::size_t r = 0; r < rows; ++r) {
      for (std::size_t c = 0; c < columns; ++c) {
        mat[r][c] += rhs[r][c];
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
    assert(rows == rhs.rows);
    assert(columns == rhs.columns);

    for (std::size_t r = 0; r < rows; ++r) {
      for (std::size_t c = 0; c < columns; ++c) {
        mat[r][c] -= rhs[r][c];
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
    for (std::size_t r = 0; r < rows; ++r) {
      for (std::size_t c = 0; c < columns; ++c) {
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

    for (std::size_t r = 0; r < rows; ++r) {
      for (std::size_t c = 0; c < columns; ++c) {
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

    for (std::size_t r = 0; r < rows; ++r) {
      for (std::size_t c = 0; c < columns; ++c) {
        mat[r][c] %= rhs;
      }
    }
    return *this;
  }

  /**
   * @brief Compound times operator for matrix multiplication.
   *
   * @param rhs The second factor.
   * @return Reference to the result.
   */
  matrix& operator*=(const matrix &rhs) {
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
   * @brief Returns a square identity matrix with the given dimensions.
   *
   * @param dim The dimensions, i.e., the number of rows and columns.
   * @param multiplicative_neutral The multiplicative neutral element to use for
   *     the main diagonal.
   * @return An identity matrix.
   */
  static matrix get_identity(std::size_t dim, T multiplicative_neutral = T{1}) {
    assert(dim > 0);

    matrix id(dim, dim);
    for (std::size_t i = 0; i < dim; ++i) {
      id[i][i] = multiplicative_neutral;
    }
    return id;
  }

private:
  /**
   * @brief Number of rows.
   */
  std::size_t rows;

  /**
   * @brief Number of columns.
   */
  std::size_t columns;

  /**
   * @brief The entries of the matrix.
   */
  std::vector<std::vector<T>> mat;
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
 * @tparam T The element type.
 * @param mat The given matrix.
 * @return A string representation of the given matrix.
 */
export template<std::regular T>
std::string to_string(matrix<T> mat) {
  using std::to_string;

  std::string rep = "";
  rep += "{";
  for (std::size_t r = 0; r < mat.get_rows(); ++r) {
    if (r) rep += ",";
    rep += "{";
    for (std::size_t c = 0; c < mat.get_columns(); ++c) {
      if (c) rep += ",";
      rep += to_string(mat[r][c]);
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
 * @tparam T The element type.
 * @param os The given output stream.
 * @param m The given matrix.
 * @return Reference to the output stream.
 */
export template<std::regular T>
std::ostream& operator<<(std::ostream &os, const matrix<T> &m) {
  os << ntlib::to_string(m);
  return os;
}

/**
 * @brief Plus operator for matrix addition.
 *
 * @tparam T The element type.
 * @param lhs The first summand.
 * @param rhs The second summand.
 * @return The sum of `lhs` and `rhs`.
 */
export template<std::regular T>
[[nodiscard]]
matrix<T> operator+(matrix<T> lhs, const matrix<T> &rhs) {
  lhs += rhs;
  return lhs;
}

/**
 * @brief Minus operator for matrix subtraction.
 *
 * @tparam T The element type.
 * @param lhs The minuend.
 * @param rhs The subtrahend.
 * @return The difference of `lhs` and `rhs`.
 */
export template<std::regular T>
matrix<T> operator-(matrix<T> lhs, const matrix<T> &rhs) {
  lhs -= rhs;
  return lhs;
}

/**
 * @brief Times operator for right scalar multiplicatoin.
 *
 * @tparam T The element type.
 * @tparam S The scalar type.
 * @param lhs The first factor, a matrix.
 * @param rhs The second factor, a scalar.
 * @return The product of `lhs` and `rhs`.
 */
export template<std::regular T, typename S>
matrix<T> operator*(matrix<T> lhs, S rhs) {
  lhs *= rhs;
  return lhs;
}

/**
 * @brief Times operator for left scalar multiplicatoin.
 *
 * @tparam T The element type.
 * @tparam S The scalar type.
 * @param lhs The first factor, a scalar.
 * @param rhs The second factor, a matrix.
 * @return The product of `lhs` and `rhs`.
 */
export template<std::regular T, typename S>
matrix<T> operator*(S lhs, matrix<T> rhs) {
  // This uses commutativiy of scalar multiplication.
  // Depending on T this might not be true.
  rhs *= lhs;
  return rhs;
}

/**
 * @brief Divides operator for scalar division.
 *
 * @tparam T The element type.
 * @tparam S The scalar type.
 * @param lhs The dividend. A matrix.
 * @param rhs The divisor. A scalar.
 * @return The quotient matrix.
 */
export template<std::regular T, typename S>
matrix<T> operator/(matrix<T> lhs, S rhs) {
  lhs /= rhs;
  return lhs;
}

/**
 * @brief Modulo operator for elementwise modulo by a scalar.
 *
 * @tparam T The element type.
 * @tparam S The scalar type.
 * @param lhs The dividend. A matrix.
 * @param rhs The divisor. A scalar.
 * @return A matrix with every element being the remainder of the
 *     corresponding element in `lhs` after divison by `rhs`.
 */
export template<std::regular T, typename S>
matrix<T> operator%(matrix<T> lhs, S rhs) {
  lhs %= rhs;
  return lhs;
}

/**
 * @brief Unary plus operator. Does nothing.
 *
 * @param rhs The matrix..
 * @return A copy of the old matrix.
 */
export template<std::regular T>
matrix<T> operator+(const matrix<T> &rhs) {
  return rhs;
}

/**
 * @brief Unary minus operator.
 *
 * @param rhs The matrix.
 * @return A matrix with all elements negated.
 */
export template<std::regular T>
matrix<T> operator-(const matrix<T> &rhs) {
  matrix<T> negated(rhs.get_rows(), rhs.get_columns());
  for (std::size_t r = 0; r < rhs.get_rows(); ++r) {
    for (std::size_t c = 0; c < rhs.get_columns(); ++c) {
      negated[r][c] = -rhs[r][c];
    }
  }
  return negated;
}

/**
 * @brief Times operator for matrix multiplication.
 * 
 * @param lhs The first factor.
 * @param rhs The second factor.
 * @return The matrix product of `lhs` and `rhs`.
 */
export template<std::regular T>
matrix<T> operator*(const matrix<T> &lhs, const matrix<T> &rhs) {
  assert(lhs.get_columns() == rhs.get_rows());

  matrix<T> product(lhs.get_rows(), rhs.get_columns());
  for (std::size_t r = 0; r < lhs.get_rows(); ++r) {
    for (std::size_t k = 0; k < lhs.get_columns(); ++k) {
      for (std::size_t c = 0; c < rhs.get_columns(); ++c) {
        product[r][c] += lhs[r][k] * rhs[k][c];
      }
    }
  }
  return product;
}

/**
 * @brief Executes a function an each element.
 * 
 * @tparam T The element type.
 * @tparam F A function object.
 * @param m The matrix.
 * @param func The function to execute.
 * @return The new matrix.
 */
export template<typename T, typename F>
matrix<T> exec_each_element(const matrix<T> &m, const F &func) {
  matrix<T> res(m.get_rows(), m.get_columns());
  for (std::size_t r = 0; r < m.get_rows(); ++r) {
    for (std::size_t c = 0; c < m.get_columns(); ++c) {
      res[r][c] = func(m[r][c]);
    }
  }
  return res;
}

/**
 * @brief Specialization for `get_multiplicative_neutral()`.
 * 
 * @tparam T The element type.
 * @param m Instance of the type.
 * @return Identity matrix of same dimension.
 */
export template<typename T>
[[nodiscard]] constexpr
matrix<T> get_multiplicative_neutral(const matrix<T> &m) {
  assert(m.get_rows() > 0);
  assert(m.get_columns() > 0);

  return matrix<T>::get_identity(
      m.get_rows(), get_multiplicative_neutral(m[0][0]));
}

}