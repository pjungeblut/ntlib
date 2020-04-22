#pragma once

/**
 * Implementations to easily work with rational numbers.
 */

#include <cassert>
#include <string>

#include "base.hpp"
#include "integral.hpp"

namespace ntlib {

/**
 * Represents a rational number.
 */
template<Integral I>
struct rational {
  /**
   * Construct rational from a numerator and a denominator.
   *
   * @param n The numerator.
   * @param d The denominator. Defaults to 1.
   */
  rational(I n = 0, I d = 1) {
    assert(d != 0);

    I f = gcd(n, d);
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
  I get_numerator() {
    return numerator;
  }

  /**
   * Returns the denominator.
   *
   * @return The denominator.
   */
  I get_denominator() {
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
  friend bool operator==(const rational &a, const rational &b) {
    return a.numerator == b.numerator && a.denominator == b.denominator;
  }

  /**
   * Unequal operator.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a != b.
   */
  friend bool operator!=(const rational &a, const rational &b) {
    return !(a == b);
  }

  /**
   * Smaller than operator.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a < b.
   */
  friend bool operator<(const rational &a, const rational &b) {
    return a.numerator * b.denominator < b.numerator * a.denominator;
  }

  /**
   * Greater than operator.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a > b.
   */
  friend bool operator>(const rational &a, const rational &b) {
    return b < a;
  }

  /**
   * Less than or equal operator.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a <= b.
   */
  friend bool operator<=(const rational &a, const rational &b) {
    return a < b || a == b;
  }

  /**
   * Greather than or equal operator.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a >= b.
   */
  friend bool operator>=(const rational &a, const rational &b) {
    return a > b || a == b;
  }

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
  I numerator;
  I denominator;

  /**
   * Adds two rationals into a third.
   * Parameters a and c may be the same for a += b.
   *
   * @param a The first summand.
   * @param b The second summand.
   * @param c The sum a + b.
   */
  static void add(const rational &a, const rational &b, rational &c) {
    I lcd = lcm(a.denominator, b.denominator);
    I fa = lcd / a.denominator;
    I fb = lcd / b.denominator;
    I numerator = fa * a.numerator + fb * b.numerator;
    I denominator = lcd;
    I g = gcd(numerator, denominator);
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
    I lcd = lcm(a.denominator, b.denominator);
    I fa = lcd / a.denominator;
    I fb = lcd / b.denominator;
    I numerator = fa * a.numerator - fb * b.numerator;
    I denominator = lcd;
    I g = gcd(numerator, denominator);
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
    I numerator = a.numerator * b.numerator;
    I denominator = a.denominator * b.denominator;
    I g = gcd(numerator, denominator);
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
    I numerator = a.numerator * b.denominator;
    I denominator = a.denominator * b.numerator;
    I g = gcd(numerator, denominator);
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
