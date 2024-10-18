module;

#include <cassert>
#include <compare>
#include <ostream>
#include <string>
#include <type_traits>

export module rational;

import base;

namespace ntlib {

/**
 * Represents a rational number.
 */
export template<typename T>
class rational {
public:
  /**
   * Construct rational from a numerator and a denominator.
   *
   * @param n The numerator.
   * @param d The denominator. Defaults to 1.
   */
  rational(T n = 0, T d = 1) noexcept {
    assert(d != T{0});

    T f = gcd(n, d);
    numerator = n / f;
    denominator = d / f;

    if constexpr (std::is_signed_v<T>) {
      if (denominator < T{0}) {
        denominator *= T{-1};
        numerator *= T{-1};
      }
    }
  }

  /**
   * Returns the numerator.
   *
   * @return The numerator.
   */
  [[nodiscard]]
  T get_numerator() const noexcept {
    return numerator;
  }

  /**
   * Returns the denominator.
   *
   * @return The denominator.
   */
  [[nodiscard]]
  T get_denominator() const noexcept {
    return denominator;
  }

  /**
   * Creates a string "a/b" with reduced a and b.
   *
   * @return A string representation.
   */
  [[nodiscard]]
  std::string to_string() const noexcept {
    using std::to_string;
    return to_string(numerator) + "/" + to_string(denominator);
  }

  /**
   * Addition for rationals.
   *
   * @param a The first summand.
   * @param b The second summand.
   * @return The sum of a and b.
   */
  [[nodiscard]]
  friend rational operator+(const rational &a, const rational &b) noexcept {
    rational sum(0);
    add(a, b, sum);
    return sum;
  }

  /**
   * Adds another rational to the current one.
   *
   * @param other The other summand.
   * @return Reference to the current rational containing the sum of its
   *         previous value and other.
   */
  rational &operator+=(const rational &other) noexcept {
    add(*this, other, *this);
    return *this;
  }

  /**
   * Subtraction for rationals.
   *
   * @param a The minuend.
   * @param b The subtrahend.
   * @return The difference of a and b.
   */
  [[nodiscard]]
  friend rational operator-(const rational &a, const rational &b) noexcept {
    rational difference;
    subtract(a, b, difference);
    return difference;
  }

  /**
   * Subtracts another rational from the current one.
   *
   * @param other The subtrahend.
   * @return Reference to the current rational containing the difference of its
   *         previous value and other.
   */
  rational &operator-=(const rational &other) noexcept {
    subtract(*this, other, *this);
    return *this;
  }

  /**
   * Multiplication for rationals.
   *
   * @param a The first factor.
   * @param b The second factor.
   * @return The product of a and b.
   */
  [[nodiscard]]
  friend rational operator*(const rational &a, const rational &b) noexcept {
    rational product;
    multiply(a, b, product);
    return product;
  }

  /**
   * Multiplies another rational to the current one.
   *
   * @param other The other factor.
   * @return Reference to the current rational containing the product of its
   *         previous value and other.
   */
  rational &operator*=(const rational &other) noexcept {
    multiply(*this, other, *this);
    return *this;
  }

  /**
   * Division for rationals.
   *
   * @param a The dividend.
   * @param b The divisor.
   * @return The quotient of a and b.
   */
  [[nodiscard]]
  friend rational operator/(const rational &a, const rational &b) noexcept {
    assert(b != rational{0});

    rational quotient;
    divide(a, b, quotient);
    return quotient;
  }

  /**
   * Divides another rational from the current one.
   *
   * @param other The divisor.
   * @return Reference to the current rational containing the quotient of its
   *         previous value and other.
   */
  rational &operator/=(const rational &other) noexcept {
    assert(other != rational{0});

    divide(*this, other, *this);
    return *this;
  }

  /**
   * Cast to bool.
   */
  [[nodiscard]]
  explicit operator bool() const noexcept {
    return numerator != 0;
  }

private:
  /**
   * Numerator and denominator.
   */
  T numerator;
  T denominator;

  /**
   * Adds two rationals into a third.
   * Parameters a and c may be the same for a += b.
   *
   * @param a The first summand.
   * @param b The second summand.
   * @param c The sum a + b.
   */
  static void add(const rational &a, const rational &b, rational &c) {
    T lcd = lcm(a.denominator, b.denominator);
    T fa = lcd / a.denominator;
    T fb = lcd / b.denominator;
    T numerator = fa * a.numerator + fb * b.numerator;
    T denominator = lcd;
    T g = gcd(numerator, denominator);
    if (g != T{1}) {
      numerator /= g;
      denominator /= g;
    }
    c.numerator = numerator;
    c.denominator = denominator;
  }

  /**
   * Subtracts two rationals into a third.
   * Parameters a and c may be the same for a -= b.
   *
   * @param a The minuend.
   * @param b The subtrahend.
   * @param c The difference a - b.
   */
  static void subtract(const rational &a, const rational &b, rational &c) {
    T lcd = lcm(a.denominator, b.denominator);
    T fa = lcd / a.denominator;
    T fb = lcd / b.denominator;
    T numerator = fa * a.numerator - fb * b.numerator;
    T denominator = lcd;
    T g = gcd(numerator, denominator);
    if (g != T{1}) {
      numerator /= g;
      denominator /= g;
    }
    c.numerator = numerator;
    c.denominator = denominator;
  }

  /**
   * Multiplies two rationals into a third.
   * Parameters a and c may be the same for a *= b.
   *
   * @param a The first factor.
   * @param b The second factor.
   * @param c The product a * b.
   */
  static void multiply(const rational &a, const rational &b, rational &c) {
    T numerator = a.numerator * b.numerator;
    T denominator = a.denominator * b.denominator;
    T g = gcd(numerator, denominator);
    if (g != T{1}) {
      numerator /= g;
      denominator /= g;
    }
    c.numerator = numerator;
    c.denominator = denominator;
  }

  /**
   * Divides two rationals into a third.
   * Parameters a and c may be the same for a /= b.
   *
   * @param a The dividend.
   * @param b The divisor. Must not be zero.
   * @param c The quotient a / b.
   */
  static void divide(const rational &a, const rational &b, rational &c) {
    T numerator = a.numerator * b.denominator;
    T denominator = a.denominator * b.numerator;
    T g = gcd(numerator, denominator);
    if (g != T{1}) {
      numerator /= g;
      denominator /= g;
    }
    if constexpr (std::is_signed_v<T>) {
      if (denominator < T{0}) {
        numerator *= T{-1};
        denominator *= T{-1};
      }
    }
    c.numerator = numerator;
    c.denominator = denominator;
  }
};

/**
 * Stream output.
 *
 * @param os The out stream.
 * @param bu The rational to print.
 * @return The out stream.
 */
export template<typename T>
std::ostream &operator<<(std::ostream &os, const rational<T> &r) {
  os << r.to_string();
  return os;
}

/**
 * Equal operator.
 *
 * @param a The left hand side to compare.
 * @param b The right hand side to compare.
 * @return True, if and only if a == b.
 */
export template<typename T>
[[nodiscard]]
bool operator==(const rational<T> &a, const rational<T> &b) {
  return a.get_numerator() == b.get_numerator() &&
         a.get_denominator() == b.get_denominator();
}

/**
 * Three-way comparison.
 *
 * @param a The left hand side to compare.
 * @param b The right hand side to compare.
 * @return True, if and only if a <=> b.
 */
export template<typename T>
[[nodiscard]]
std::strong_ordering operator<=>(const rational<T> &a, const rational<T> &b) {
  return a.get_numerator() * b.get_denominator() <=>
         b.get_numerator() * a.get_denominator();
};

/**
 * Unary minus to negate value.
 *
 * @param a The rational.
 * @return The rational with the same absolute value but opposite sign.
 */
export template<typename T>
[[nodiscard]]
rational<T> operator-(const rational<T> &a) {
  rational<T> neg{-1 * a.get_numerator(), a.get_denominator()};
  return neg;
}

/**
 * Specialization for `get_multiplicative_neutral()`.
 * 
 * @param r Instance of the type.
 * @return The multiplicative neutral `1/1`.
 */
export template<typename T>
[[nodiscard]] constexpr
rational<T> get_multiplicative_neutral(rational<T> r) {
  return rational{get_multiplicative_neutral(r.get_numerator())};
}

}
