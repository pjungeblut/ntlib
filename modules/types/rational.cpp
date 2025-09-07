/**
 * @file
 * @brief Primary module interface unit for module `rational`.
 */
module;

#include <cassert>
#include <compare>
#include <ostream>
#include <string>
#include <type_traits>

/**
 * @module rational
 * @brief Provides a class template for rational numbers.
 */
export module rational;

import base;

namespace ntlib {

/**
 * @brief Represents a rational number.
 * 
 * @tparam T An integer-like type.
 */
export template<Integer T>
class rational {
public:
  /**
   * @brief Construct rational from a numerator and a denominator.
   *
   * This constructor allows implicit conversion from elements of type `T`.
   * 
   * @param n The numerator.
   * @param d The denominator. Defaults to 1.
   */
  rational(T n, T d = 1) noexcept {
    assert(d != T{0});

    T f = ntlib::gcd(n, d);
    numerator = n / f;
    denominator = d / f;

    if constexpr (std::is_signed_v<T>) {
      if (denominator < T{0}) {
        denominator *= T{-1};
        numerator *= T{-1};
      }
      assert(denominator > 0);
    }
  }

  /**
   * @brief Returns the numerator.
   *
   * @return The numerator.
   */
  [[nodiscard]]
  T get_numerator() const noexcept {
    return numerator;
  }

  /**
   * @brief Returns the denominator.
   *
   * @return The denominator.
   */
  [[nodiscard]]
  T get_denominator() const noexcept {
    return denominator;
  }

  /**
   * @brief Compound assignment addition operator.
   * 
   * @param rhs The second summand.
   * @return Reference to the result.
   */
  rational &operator+=(const rational &rhs) noexcept {
    T l = ntlib::lcm(denominator, rhs.denominator);
    T f_lhs = l / denominator;
    T f_rhs = l / rhs.denominator;
    T new_numerator = f_lhs * numerator + f_rhs * rhs.numerator;
    T new_denominator = l;

    T g = ntlib::gcd(new_numerator, new_denominator);
    if (g != T{1}) {
      new_numerator /= g;
      new_denominator /= g;
    }
    numerator = new_numerator;
    denominator = new_denominator;
    return *this;
  }

  /**
   * @brief Addition operator.
   *
   * @param lhs The first summand.
   * @param rhs The second summand.
   * @return The sum of `lhs` and `rhs`.
   */
  [[nodiscard]]
  friend rational operator+(rational lhs, const rational &rhs) noexcept {
    lhs += rhs;
    return lhs;
  }

  /**
   * @brief Compound assignment subtraction operator.
   *
   * @param rhs The subtrahend.
   * @return Reference to the result.
   */
  rational &operator-=(const rational &rhs) noexcept {
    T l = ntlib::lcm(denominator, rhs.denominator);
    T f_lhs = l / denominator;
    T f_rhs = l / rhs.denominator;
    T new_numerator = f_lhs * numerator - f_rhs * rhs.numerator;
    T new_denominator = l;

    T g = ntlib::gcd(new_numerator, new_denominator);
    if (g != T{1}) {
      new_numerator /= g;
      new_denominator /= g;
    }
    numerator = new_numerator;
    denominator = new_denominator;
    return *this;
  }

  /**
   * @brief Subtraction operator.
   *
   * @param lhs The minuend.
   * @param rhs The subtrahend.
   * @return The difference of `lhs` and `rhs`.
   */
  [[nodiscard]]
  friend rational operator-(rational lhs, const rational &rhs) noexcept {
    lhs -= rhs;
    return lhs;
  }

   /**
   * @brief Compound assignment multiplication operator.
   *
   * @param rhs The second factor.
   * @return Reference to the result.
   */
  rational &operator*=(const rational &rhs) noexcept {
    numerator *= rhs.numerator;
    denominator *= rhs.denominator;

    T g = ntlib::gcd(numerator, denominator);
    if (g != T{1}) {
      numerator /= g;
      denominator /= g;
    }
    return *this;
  }

  /**
   * @brief Multiplication operator.
   *
   * @param lhs The first factor.
   * @param rhs The second factor.
   * @return The product of `lhs` and `rhs`.
   */
  [[nodiscard]]
  friend rational operator*(rational lhs, const rational &rhs) noexcept {
    lhs *= rhs;
    return lhs;
  }

  /**
   * @brief Compound assignment division operator.
   *
   * @param rhs The divisor.
   * @return Reference to the result.
   */
  rational &operator/=(const rational &rhs) noexcept {
    assert(rhs != rational{0});

    numerator *= rhs.denominator;
    denominator *= rhs.numerator;

    T g = ntlib::gcd(numerator, denominator);
    if (g != T{1}) {
      numerator /= g;
      denominator /= g;
    }

    if constexpr (std::is_signed_v<T>) {
      if (denominator < T{0}) {
        numerator *= T{-1};
        denominator *= T{-1};
      }
      assert(denominator > 0);
    }

    return *this;
  }

  /**
   * @brief Division operator.
   *
   * @param lhs The dividend.
   * @param rhs The divisor.
   * @return The quotient of `lhs` and `rhs`.
   */
  [[nodiscard]]
  friend rational operator/(rational lhs, const rational &rhs) noexcept {
    lhs /= rhs;
    return lhs;
  }

  /**
   * @brief Cast to bool.
   * 
   * @return Whether the rational is not equal to zero.
   */
  [[nodiscard]]
  explicit operator bool() const noexcept {
    return numerator != 0;
  }

private:
  /// @brief The numerator.
  T numerator;

  /// @brief The denominator.
  T denominator;
};

/**
 * @brief Creates a string "a/b" where `a` and `b` are coprime.
 *
 * @tparam T An integer-like type.
 * @param r A rational.
 * @return A string representation.
 */
export template<Integer T>
[[nodiscard]]
std::string to_string(const rational<T> &r) {
  using std::to_string;
  return to_string(r.get_numerator()) + "/" + to_string(r.get_denominator());
}

/**
 * @brief Stream insertion operator.
 *
 * @tparam T An integer-like type.
 * @param os The output stream.
 * @param r The rational.
 * @return The output stream.
 */
export template<Integer T>
std::ostream &operator<<(std::ostream &os, const rational<T> &r) {
  os << to_string(r);
  return os;
}

/**
 * @brief Equality operator.
 *
 * @param lhs The left hand side to compare.
 * @param rhs The right hand side to compare.
 * @return True, if and only if `lhs` is equal to `rhs`.
 */
export template<Integer T>
[[nodiscard]]
bool operator==(const rational<T> &lhs, const rational<T> &rhs) {
  return lhs.get_numerator() == rhs.get_numerator() &&
         lhs.get_denominator() == rhs.get_denominator();
}

/**
 * @brief Three-way comparison operator.
 *
 * @tparam T An integer-like type.
 * @param lhs The left hand side to compare.
 * @param rhs The right hand side to compare.
 * @retval <0 If `lhs < rhs`.
 * @retval 0 If `lhs == rhs`.
 * @retval >0 If `lhs > rhs`.
 */
export template<Integer T>
[[nodiscard]]
std::strong_ordering operator<=>(
    const rational<T> &lhs, const rational<T> &rhs) {
  return lhs.get_numerator() * rhs.get_denominator() <=>
         rhs.get_numerator() * lhs.get_denominator();
};

/**
 * @brief Unary minus operator.
 *
 * @tparam T An integer-like type.
 * @param r The rational.
 * @return The rational with the same absolute value but opposite sign.
 */
export template<Integer T>
[[nodiscard]]
rational<T> operator-(const rational<T> &r) {
  static_assert(std::is_signed_v<T>);

  rational<T> neg{T{-1} * r.get_numerator(), r.get_denominator()};
  return neg;
}

/**
 * @brief Specialization of `ntlib::algebra_traits` for `rational`.
 * 
 * @tparam T An integer-like type.
 */
export template<Integer T>
class algebra_traits<rational<T>> {
public:
  /**
   * @brief Returns the additive neutral element of `rational<T>`.
   * 
   * @return The additive neutral element, i.e., \f$0/1\f$.
   */
  [[nodiscard]] static constexpr rational<T> get_zero() noexcept {
    return rational<T>{T{0}, T{1}};
  }

  /**
   * @brief Returns the multiplicative neutral element of `rational<T>`.
   * 
   * @return The multiplicative neutral element, i.e., \f$1/1\f$.
   */
  [[nodiscard]] static constexpr rational<T> get_one() noexcept {
    return rational<T>{T{1}, T{1}};
  }

  /// @brief Rational addition is commutative.
  static constexpr bool is_additive_commutative = true;

  /// @brief Rational multiplication is commutative.
  static constexpr bool is_multiplicative_commutative = true;
};

} // namespace ntlib
