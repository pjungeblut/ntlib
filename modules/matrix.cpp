module;

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

export module matrix;

import base;

namespace ntlib {

/**
 * Represents a RxC-matrix.
 */
export template<typename T,
    template <typename> typename Allocator = std::allocator>
class matrix {
public:
  /**
   * Default construction for an empty matrix.
   */
  matrix() : rows(0), columns(0) {};

  /**
   * Constructs a 1x1 matrix from a single value.
   *
   * @param val The single value.
   */
  explicit matrix(T val) : rows(1), columns(1) {
    mat.resize(rows, std::vector<T, Allocator<T>>(columns));
    mat[0][0] = val;
  }

  /**
   * Constructs a new matrix of the given dimensions with the values default
   * initialized.
   *
   * @param rows The number of rows.
   * @param columns The number of columns.
   */
  matrix(std::size_t rows, std::size_t columns) : rows(rows), columns(columns) {
    mat.resize(rows, std::vector<T, Allocator<T>>(columns));
  }

  /**
   * Constructs a new matrix from a given 2D initializer list.
   * Elements are given line by line.
   *
   * @param elements The elements of the matrix.
   */
  matrix(std::initializer_list<std::initializer_list<T>> elements) {
    rows = elements.size();
    columns = rows ? elements.begin()->size() : 0;
    mat.resize(rows, std::vector<T, Allocator<T>>(columns));

    std::size_t r = 0;
    for (const auto &line : elements) {
      std::size_t c = 0;
      for (const auto &e : line) {
        mat[r][c] = e;
        ++c;
      }
      ++r;
    }
  }

  /**
   * Returns the number of rows.
   *
   * @return The number of rows.
   */
  std::size_t get_rows() const noexcept {
    return rows;
  }

  /**
   * Returns the number of columns.
   *
   * @return The number of columns.
   */
  std::size_t get_columns() const noexcept {
    return columns;
  }

  /**
   * A proxy class to provide an lvalue that can be returned from operator[]
   * so that values can be changed but for example dimensions cannot.
   */
  class reference {
    friend class matrix;

    /**
     * Constant pointer to the queried row.
     */
    std::vector<T, Allocator<T>>* const row;

    /**
     * Constructs the reference.
     *
     * @param row_idx The index of the row to access.
     */
    reference(std::vector<T, Allocator<T>>* const row) : row(row) {};

  public:
    /**
     * Array subscript operator to access the row.
     *
     * @param col_idx The index of the column to access.
     * @return The value in row `row_idx` and column `col_idx`.
     */
    [[nodiscard]]
    T& operator[](std::size_t col_idx) {
      return (*row)[col_idx];
    }
  };

  /**
   * Array subscript operator.
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
   * Constant array subscript operator.
   *
   * @param row_idx The index of the row to access.
   * @return A constant reference to the vector containing the entries of the
   *         queried row.
   */
  const std::vector<T, Allocator<T>>& operator[](std::size_t row_idx) const {
    return mat[row_idx];
  }

  /**
   * Converts matrix into a string representation.
   *
   * @return String representation.
   */
  std::string to_string() const {
    using std::to_string;

    std::string rep = "";
    rep += "{";
    for (std::size_t r = 0; r < get_rows(); ++r) {
      if (r) rep += ",";
      rep += "{";
      for (std::size_t c = 0; c < get_columns(); ++c) {
        if (c) rep += ",";
        rep += to_string(mat[r][c]);
      }
      rep += "}";
    }
    rep += "}";
    return rep;
  }

  /**
   * Writes a text representation of the matrix to the given out stream.
   * Output is formatted line by line.
   *
   * @param os The out stream to write to.
   * @param obj
   * @return Reference to the out stream.
   */
  friend std::ostream& operator<<(std::ostream &os, const matrix &m) {
    os << m.to_string();
    return os;
  }

  /**
   * Matrix addition.
   *
   * @param lhs The first summand.
   * @param rhs The second summand.
   * @return The sum of `lhs` and `rhs`.
   */
  friend matrix operator+(const matrix &lhs, const matrix &rhs) {
    matrix sum(lhs.get_rows(), lhs.get_columns());
    add(lhs, rhs, sum);
    return sum;
  }

  /**
   * Compound matrix addition.
   *
   * @param rhs The other matrix to add to the current one.
   * @return Reference to this matrix.
   */
  matrix& operator+=(const matrix &rhs) {
    add(*this, rhs, *this);
    return *this;
  }

  /**
   * Matrix subtraction.
   *
   * @param lhs The minuend.
   * @param rhs The subtrahend.
   * @return The difference of `lhs` and `rhs`.
   */
  friend matrix operator-(const matrix &lhs, const matrix &rhs) {
    matrix difference(lhs.get_rows(), lhs.get_columns());
    subtract(lhs, rhs, difference);
    return difference;
  }

  /**
   * Compound matrix subtraction.
   *
   * @param rhs The subtrahend to subtract from the current matrix.
   * @return Reference to this matrix.
   */
  matrix& operator-=(const matrix &rhs) {
    subtract(*this, rhs, *this);
    return *this;
  }

  /**
   * Scalar multiplicatoin.
   *
   * @param lhs The first factor. A matrix.
   * @param rhs The second factor. A scalar.
   * @return The product.
   */
  template<typename S>
  friend matrix operator*(const matrix &lhs, S rhs) {
    matrix product(lhs.get_rows(), lhs.get_columns());
    scalar_multiply(lhs, rhs, product);
    return product;
  }

  /**
   * Scalar multiplicatoin.
   *
   * @param lhs The first factor. A scalar.
   * @param rhs The second factor. A matrix.
   * @return The product.
   */
  template<typename S>
  friend matrix operator*(S lhs, const matrix &rhs) {
    matrix product(rhs.get_rows(), rhs.get_columns());
    scalar_multiply(rhs, lhs, product);
    return product;
  }

  /**
   * Compound scalar multiplication.
   *
   * @param rhs The second factor. A scalar.
   * @return Reference to this matrix.
   */
  template<typename S>
  matrix& operator*=(S rhs) {
    scalar_multiply(*this, rhs, *this);
    return *this;
  }

  /**
   * Division by scalar.
   *
   * @param lhs The dividend. A matrix.
   * @param rhs The divisor. A scalar.
   * @return The quotient matrix.
   */
  template<typename S>
  friend matrix operator/(const matrix &lhs, S rhs) {
    matrix quotient(lhs.get_rows(), lhs.get_columns());
    scalar_divide(lhs, rhs, quotient);
    return quotient;
  }

  /**
   * Compound division by a scalar.
   *
   * @param rhs The divisor. A scalar.
   * @return Reference to this matrix.
   */
  template<typename S>
  matrix& operator/=(S rhs) {
    scalar_divide(*this, rhs, *this);
    return *this;
  }

  /**
   * Elementwise modulo.
   *
   * @param lhs The dividend. A matrix.
   * @param rhs The divisor. A scalar.
   * @return A matrix with every element being the remainder of the
   *         corresponding element in `lhs` after divison by `rhs`.
   */
  template<typename S>
  friend matrix operator%(const matrix &lhs, S rhs) {
    matrix remainder(lhs.get_rows(), lhs.get_columns());
    modulo(lhs, rhs, remainder);
    return remainder;
  }

  /**
   * Compound elementwise modulo.
   *
   * @param rhs The divisor. A scalar.
   * @return Reference to this matrix.
   */
  template<typename S>
  matrix& operator%=(S rhs) {
    modulo(*this, rhs, *this);
    return *this;
  }

  /**
   * Matrix multiplication.
   *
   * @param lhs The first factor.
   * @param rhs The second factor.
   * @return The product.
   */
  friend matrix operator*(const matrix &lhs, const matrix &rhs) {
    matrix product(lhs.get_rows(), rhs.get_columns());
    multiply(lhs, rhs, product);
    return product;
  }

  /**
   * Compound matrix multiplication.
   *
   * @param rhs The second factor.
   * @return Reference to this matrix.
   */
  matrix& operator*=(const matrix &rhs) {
    matrix product(get_rows(), rhs.get_columns());
    multiply(*this, rhs, product);
    *this = std::move(product);
    return *this;
  }

  /**
   * Unary plus operator. Does nothing.
   *
   * @param rhs The matrix to do nothing on.
   * @return A copy of the old matrix.
   */
  friend matrix operator+(const matrix &rhs) {
    return rhs;
  }

  /**
   * Unary minus operator.
   *
   * @param rhs The matrix to negate.
   * @return A matrix with all elements negated.
   */
  friend matrix operator-(const matrix &rhs) {
    matrix negated(rhs.get_rows(), rhs.get_columns());
    for (std::size_t r = 0; r < rhs.get_rows(); ++r) {
      for (std::size_t c = 0; c < rhs.get_columns(); ++c) {
        negated[r][c] = -rhs[r][c];
      }
    }
    return negated;
  }

  /**
   * Default equality comparison.
   *
   * @return Whether two matrices are equal.
   */
  bool operator==(const matrix&) const = default;

  /**
   * Returns a square identity matrix with the given dimensions.
   *
   * @param dim The dimension.
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
   * Number of rows and columns.
   */
  std::size_t rows;
  std::size_t columns;

  /**
   * The entries of the matrix.
   */
  std::vector<
      std::vector<T, Allocator<T>>,
      Allocator<std::vector<T, Allocator<T>>>> mat;

  /**
   * Adds two matrices into a third.
   * The dimensions of all three parameters must be the same.
   *
   * @param lhs The first summand.
   * @param rhs The second summand.
   * @param sum The sum. May be the same as `lhs`.
   */
  static void add(const matrix &lhs, const matrix &rhs, matrix &sum) {
    assert(lhs.get_rows() == rhs.get_rows());
    assert(lhs.get_rows() == sum.get_rows());
    assert(lhs.get_columns() == rhs.get_columns());
    assert(lhs.get_columns() == sum.get_columns());

    for (std::size_t r = 0; r < lhs.get_rows(); ++r) {
      for (std::size_t c = 0; c < lhs.get_columns(); ++c) {
        sum[r][c] = lhs[r][c] + rhs[r][c];
      }
    }
  }

  /**
   * Subtracts two matrices into a thrid.
   *
   * @param lhs The minuend.
   * @param rhs The subtrahend.
   * @param difference The difference. May be the same as `lhs`.
   */
  static void subtract(const matrix &lhs, const matrix &rhs, matrix &difference) {
    assert(lhs.get_rows() == rhs.get_rows());
    assert(lhs.get_rows() == difference.get_rows());
    assert(lhs.get_columns() == rhs.get_columns());
    assert(lhs.get_columns() == difference.get_columns());

    for (std::size_t r = 0; r < lhs.get_rows(); ++r) {
      for (std::size_t c = 0; c < lhs.get_columns(); ++c) {
        difference[r][c] = lhs[r][c] - rhs[r][c];
      }
    }
  }

  /**
   * Scalar multiplication.
   *
   * @param lhs The first factor. A matrix.
   * @param rhs The second factor. A scalar.
   * @param product The product. May be the same a `lhs`.
   */
  template<typename S>
  static void scalar_multiply(const matrix &lhs, S rhs, matrix &product) {
    assert(lhs.get_rows() == product.get_rows());
    assert(lhs.get_columns() == product.get_columns());

    for (std::size_t r = 0; r < lhs.get_rows(); ++r) {
      for (std::size_t c = 0; c < lhs.get_columns(); ++c) {
        product[r][c] = lhs[r][c] * rhs;
      }
    }
  }

  /**
   * Scalar division.
   *
   * @param lhs The dividend. A matrix.
   * @param rhs The divisor. A scalar.
   * @param quotient The quotient. May be the same as `lhs`.
   */
  template<typename S>
  static void scalar_divide(const matrix &lhs, S rhs, matrix &quotient) {
    assert(rhs != 0);
    assert(lhs.get_rows() == quotient.get_rows());
    assert(lhs.get_columns() == quotient.get_columns());

    for (std::size_t r = 0; r < lhs.get_rows(); ++r) {
      for (std::size_t c = 0; c < lhs.get_columns(); ++c) {
        quotient[r][c] = lhs[r][c] / rhs;
      }
    }
  }

  /**
   * Elementwise modulo.
   *
   * @param lhs The dividend. A matrix.
   * @param rhs The divisor. A scalar.
   * @param remainder The remainder. May be the same as `lhs`.
   */
  template<typename S>
  static void modulo(const matrix &lhs, S rhs, matrix &remainder) {
    assert(rhs != 0);
    assert(lhs.get_rows() == remainder.get_rows());
    assert(lhs.get_columns() == remainder.get_columns());

    for (std::size_t r = 0; r < lhs.get_rows(); ++r) {
      for (std::size_t c = 0; c < lhs.get_columns(); ++c) {
        remainder[r][c] = lhs[r][c] % rhs;
      }
    }
  }

  /**
   * Matrix multiplication.
   *
   * @param lhs The first factor.
   * @param rhs The second factor.
   * @param product The product. May not be the same as `lhs` or `rhs`.
   */
  static void multiply(const matrix &lhs, const matrix &rhs, matrix &product) {
    assert(lhs.get_columns() == rhs.get_rows());
    assert(lhs.get_rows() == product.get_rows());
    assert(rhs.get_columns() == product.get_columns());

    for (std::size_t r = 0; r < lhs.get_rows(); ++r) {
      for (std::size_t c = 0; c < rhs.get_columns(); ++c) {
        product[r][c] = T{0};
      }
    }

    for (std::size_t r = 0; r < lhs.get_rows(); ++r) {
      for (std::size_t k = 0; k < lhs.get_columns(); ++k) {
        for (std::size_t c = 0; c < rhs.get_columns(); ++c) {
          product[r][c] += lhs[r][k] * rhs[k][c];
        }
      }
    }
  }
};

/**
 * Executes a function an each element.
 * 
 * @param m The matrix.
 * @param func The function to execute.
 * @return The new matrix.
 */
export template<typename T, typename F>
matrix<T> exec_each_element(matrix<T> m, const F &func) {
  for (std::size_t r = 0; r < m.get_rows(); ++r) {
    for (std::size_t c = 0; c < m.get_columns(); ++c) {
      m[r][c] = func(m[r][c]);
    }
  }
  return m;
}

/**
 * Specialization for `get_multiplicative_neutral()`.
 * 
 * @param _ Instance of the type.
 * @return Identity matrix of same dimension.
 */
export template<typename T>
[[nodiscard]] constexpr
matrix<T> get_multiplicative_neutral(matrix<T> m) {
  assert(m.get_rows() == m.get_columns());
  assert(m.get_rows() > 0);

  return matrix<T>::get_identity(
      m.get_rows(), get_multiplicative_neutral(m[0][0]));
}

}