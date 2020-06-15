#pragma once

/**
 * Implementations to easily work with rational numbers.
 */

#include <cassert>
#include <compare>
#include <string>

#include "base.hpp"

namespace ntlib {

/**
 * Represents a rational number.
 */
template<typename T>
struct rational {
  /**
   * Construct rational from a numerator and a denominator.
   *
   * @param n The numerator.
   * @param d The denominator. Defaults to 1.
   */
  rational(T n = 0, T d = 1) {
    assert(d != 0);

    T f = gcd(n, d);
    numerator = n / f;
    denominator = d / f;

    if (denominator < 0) {
      denominator *= -1;
      numerator *= -1;
    }
  }

  /**
   * Returns the numerator.
   *
   * @return The numerator.
   */
  T get_numerator() const {
    return numerator;
  }

  /**
   * Returns the denominator.
   *
   * @return The denominator.
   */
  T get_denominator() const {
    return denominator;
  }

  /**
   * Creates a string "a/b" with reduced a and b.
   *
   * @return A string representation.
   */
  std::string to_string() const {
    return std::to_string(numerator) + "/" + std::to_string(denominator);
  }

  /**
   * Stream output.
   *
   * @param os The out stream.
   * @param bu The rational to print.
   * @return The out stream.
   */
  friend std::ostream& operator<<(std::ostream &os, const rational &r) {
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
  bool operator==(const rational &other) const {
    return numerator == other.numerator && denominator == other.denominator;
  }

  /**
   * Three-way comparison.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a <=> b.
   */
  std::strong_ordering operator<=>(const rational &other) const {
    return numerator * other.denominator <=> other.numerator * denominator;
  };

  /**
   * Unary minus to negate value.
   *
   * @return The rational with the same absolute value but opposite sign.
   */
  friend rational operator-(const rational &a) {
    rational neg = a;
    a.numerator *= -1;
    return neg;
  }

  /**
   * Addition for rationals.
   *
   * @param a The first summand.
   * @param b The second summand.
   * @return The sum of a and b.
   */
  friend rational operator+(const rational &a, const rational &b) {
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
  rational& operator+=(const rational &other) {
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
  friend rational operator-(const rational &a, const rational &b) {
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
  rational& operator-=(const rational &other) {
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
  friend rational operator*(const rational &a, const rational &b) {
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
  rational& operator*=(const rational &other) {
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
  friend rational operator/(const rational &a, const rational &b) {
    assert(b != 0);
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
  rational& operator/=(const rational &other) {
    assert(other != 0);
    divide(*this, other, *this);
    return *this;
  }

  /**
   * Cast to bool.
   */
  explicit operator bool() const {
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
    if (g != 1) {
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
    if (g != 1) {
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
    if (g != 1) {
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
    if (g != 1) {
      numerator /= g;
      denominator /= g;
    }
    if (denominator < 0) {
      numerator *= -1;
      denominator *= -1;
    }
    c.numerator = numerator;
    c.denominator = denominator;
  }
};

}
