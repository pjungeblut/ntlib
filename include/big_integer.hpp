#pragma once

#include "include/big_unsigned.hpp"

namespace ntlib {

/**
 * Represents a whole number whose size is only limited by the computers
 * memory.
 */
class big_integer {
private:
  /**
   * The magnitude of the big_integer.
   */
  big_unsigned magnitude;

  /**
   * The sign.
   */
  enum struct sign_type {
    MINUS = -1,
    PLUS = 1
  };
  sign_type sign;

public:
  /**
   * Default constructor.
   */
  big_integer() : magnitude(0), sign(sign_type::PLUS) {}

  /**
   * Constructor to build big_integer from a simple long long.
   *
   * @param n The value to initialize with.
   */
  big_integer(long long n) {
    if (n >= 0) {
      magnitude = big_unsigned(n);
      sign = sign_type::PLUS;
    } else {
      magnitude = big_unsigned(-n);
      sign = sign_type::MINUS;
    }
  }

  /**
   * Constructor to build big_integer from a string giving a base 10
   * representation of the value.
   *
   * @param n The value to initialize with.
   */
  big_integer(const std::string &n) {
    if (n.size() && n[0] == '-') {
      magnitude = big_unsigned(n.substr(1));
      sign = sign_type::MINUS;
    } else {
      magnitude = big_unsigned(n);
      sign = sign_type::PLUS;
    }
  }

  /**
   * Convert value into a string.
   *
   * @return The value as a string in base 10.
   */
  std::string to_string() const {
    if (sign == sign_type::MINUS) return "-" + magnitude.to_string();
    return magnitude.to_string();
  }

  /**
   * Hidden friend for stream output.
   *
   * @param os The out stream.
   * @param bu The big_integer to print.
   * @return The out stream.
   */
  friend std::ostream& operator<<(std::ostream &os, const big_integer &bi) {
    os << bi.to_string();
    return os;
  }

  /**
   * Less than operator.
   *
   * TODO: Replace this by the three way comparison (spaceship) operator once
   *       it gets implemented for g++.
   *       As soon as this is done, remove the six classic comparison operators.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a < b.
   */
  friend bool operator<(const big_integer &a, const big_integer &b) {
    // If they have different sign, we can compare their sign.
    if (a.sign != b.sign) return a.sign < b.sign;

    // Otherwise check their absolute values.
    if (a.sign == sign_type::PLUS) return a.magnitude < b.magnitude;
    return a.magnitude > b.magnitude;
  }

  /**
   * Greater than operator.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a > b.
   */
  friend bool operator>(const big_integer &a, const big_integer &b) {
    return b < a;
  }

  /**
   * Equal operator.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a == b.
   */
  friend bool operator==(const big_integer &a, const big_integer &b) {
    return a.sign == b.sign && a.magnitude == b.magnitude;
  }

  /**
   * Unequal operator.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a != b.
   */
  friend bool operator!=(const big_integer &a, const big_integer &b) {
    return !(a == b);
  }

  /**
   * Less than or equal operator.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a <= b.
   */
  friend bool operator<=(const big_integer &a, const big_integer &b) {
    // If they have different sign, we can compare their sign.
    if (a.sign != b.sign) return a.sign < b.sign;

    // Otherwise check their absolute values.
    if (a.sign == sign_type::PLUS) return a.magnitude <= b.magnitude;
    return a.magnitude >= b.magnitude;
  }

  /**
   * Greater than or equal operator.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a >= b.
   */
  friend bool operator>=(const big_integer &a, const big_integer &b) {
    return b <= a;
  }

  /**
   * Addition for big_integer.
   *
   * @param a The first summand.
   * @param b The second summand.
   * @return The sum of a and b.
   */
  friend big_integer operator+(const big_integer &a, const big_integer &b) {
    big_integer sum;
    add(a, b, sum);
    return sum;
  }

  /**
   * Adds another big_integer to the current one.
   *
   * @param other The other summand.
   * @return Reference to the current big_integer containing the sum of its
   *         previous value and other.
   */
  big_integer& operator+=(const big_integer &other) {
    add(*this, other, *this);
    return *this;
  }

  /**
   * Unary minus to negate value.
   *
   * @return The big integer with the same value but opposite sign.
   */
  friend big_integer operator-(const big_integer &other) {
    big_integer negation;
    negation.sign = other.sign == sign_type::PLUS
        ? sign_type::MINUS : sign_type::PLUS;
    negation.magnitude = other.magnitude;
    return negation;
  }

  /**
   * Subtraction for big_integer.
   *
   * @param a The minuend.
   * @param b The subtrahend.
   * @return The difference a - b.
   */
  friend big_integer operator-(const big_integer &a, const big_integer &b) {
    big_integer difference;
    subtract(a, b, difference);
    return difference;
  }

  /**
   * Subtracts another big_integer from the current one.
   *
   * @param other The subtrahend. Must be at least as big as current value.
   * @return Reference to the current big_integer containing the diffrence of
   *         its previous value and other.
   */
  big_integer& operator-=(const big_integer &other) {
    subtract(*this, other, *this);
    return *this;
  }

  /**
   * Multiplication for big_integer.
   *
   * @param a The first factor.
   * @param b The second factor.
   * @return The product a * b.
   */
  friend big_integer operator*(const big_integer &a, const big_integer &b) {
    big_integer product;
    mulitply(a, b, product);
    return product;
  }

  /**
   * Multiplies another big_integer to the current one.
   *
   * @param other The other factor.
   * @return Reference to the current big_integer containing the product of its
   *         previous value and other.
   */
  big_integer& operator*=(const big_integer &other) {
    // TODO: This creates an intermediate big_integer. Find a way to do the
    //       multiplication in-place.
    big_integer product;
    mulitply(*this, other, product);
    sign = product.sign;
    magnitude = std::move(product.magnitude);
    return *this;
  }

  /**
   * Divides a by b.
   *
   * @param a The dividend.
   * @param b The divisor.
   * @return The quotient of a div b.
   */
  friend big_integer operator/(const big_integer &a, const big_integer &b) {
    big_integer quotient, remainder;
    divide(a, b, quotient, remainder);
    return quotient;
  }

  /**
   * Divides another big_integer from the current one.
   *
   * @param other The divisor.
   * @return Reference to the current big_integer containing the quotient of
   *         its previous value and other.
   */
  big_integer& operator/=(const big_integer &other) {
    big_integer quotient, remainder;
    divide(*this, other, quotient, remainder);
    sign = quotient.sign;
    magnitude = std::move(quotient.magnitude);
    return *this;
  }

private:
  /**
   * Adds two big_integers into a third.
   * Parameters a and c may be the same, for a += b.
   *
   * @param a The first summand.
   * @param b The second summand.
   * @param c The sum a + b.
   */
  static void add(const big_integer &a, const big_integer &b,
      big_integer &c) {
    if (a.sign == b.sign) {
      // Either both positive or both negative.
      c.sign = a.sign;
      c.magnitude = a.magnitude + b.magnitude;
    } else if (a.sign == sign_type::PLUS) {
      // a is positive, b is negative.
      if (a.magnitude >= b.magnitude) {
        c.sign = sign_type::PLUS;
        c.magnitude = a.magnitude - b.magnitude;
      } else {
        c.sign = sign_type::MINUS;
        c.magnitude = b.magnitude - a.magnitude;
      }
    } else {
      // a is negative, b is positive.
      if (a.magnitude > b.magnitude) {
        c.sign = sign_type::MINUS;
        c.magnitude = a.magnitude - b.magnitude;
      } else {
        c.sign = sign_type::PLUS;
        c.magnitude = b.magnitude - a.magnitude;
      }
    }
  }

  /**
   * Subtracts two big_integers into a third.
   * Parameters a and c may be the same, for a -= b.
   *
   * @param a The minuend.
   * @param b The subtrahend.
   * @param c The difference a - b.
   */
  static void subtract(const big_integer &a, const big_integer &b,
      big_integer &c) {
    if (b.sign == sign_type::MINUS) add(a, -b, c);
    else {
      if (a.sign == sign_type::PLUS) {
        if (a.magnitude >= b.magnitude) {
          c.sign = sign_type::PLUS;
          c.magnitude = a.magnitude - b.magnitude;
        } else {
          c.sign = sign_type::MINUS;
          c.magnitude = b.magnitude - a.magnitude;
        }
      } else {
        c.sign = sign_type::MINUS;
        c.magnitude = a.magnitude + b.magnitude;
      }
    }
  }

  /**
   * Multiplies two big_integers into a third.
   * Parameters a and c may NOT be the same, as the content of c gets erased.
   *
   * @param a The first factor.
   * @param b The second factor.
   * @param c The product a * b.
   */
  static void mulitply(const big_integer &a, const big_integer &b,
      big_integer &c) {
    c.magnitude = a.magnitude * b.magnitude;
    c.sign = a.sign == b.sign ? sign_type::PLUS : sign_type:: MINUS;
  }

  /**
   * Divides a by b and outputs the quotient and the remainder.
   *
   * @param a The dividend.
   * @param b The divisor.
   * @param quotient The quotient a div b.
   * @param remainder The remainder a mod b.
   */
  static void divide(const big_integer &a, const big_integer &b,
      big_integer &quotient, big_integer &remainder) {
    big_unsigned::divide_with_remainder(a.magnitude, b.magnitude,
        quotient.magnitude, remainder.magnitude);
    quotient.sign = a.sign == b.sign ? sign_type::PLUS : sign_type:: MINUS;
    remainder.sign = a.sign;
  }
};

}
