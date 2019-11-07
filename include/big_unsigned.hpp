#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace ntlib {

/**
 * Represents a natural number whose size is only limited by the computers
 * memory.
 *
 * TODO: Write copy constructor, copy assignment operator, move constructor and
 *       move assignment operator.
 * TODO: Add missing increment/decrement operators.
 */
class big_unsigned {
private:
  /**
   * Integral type used for each digit.
   */
  using digit_type = uint32_t;

  /**
   * Type big enough to contain the sum/product of any two digits.
   */
  using double_digit_type = uint64_t;

  /**
   * The base in which the numbers are stored is a power of two for optimal
   * memory usage.
   */
  constexpr static uint8_t LOG_BASE = sizeof(digit_type) * 8;
  constexpr static double_digit_type BASE = 1LL << LOG_BASE;

public:
  /**
   * Default constructor.
   */
  big_unsigned() {}

  /**
   * Constructor to build big_unsigned from a simple integer.
   *
   * @param n The value to initialize with.
   */
  big_unsigned(uint32_t n) {
    if (n > 0) digits.assign(1, n);
  }

  /**
   * Constructor to build big_unsigned from a string representation of the
   * value. Can be given in any base b.
   *
   * @param n The value to initialize with.
   * @param base The base. 2 <= base <= 16. Default is base = 10.
   */
  big_unsigned(const std::string &n, uint8_t base = 10) {
    if (n == "0") return;

    // Create big_unsigned with value 0.
    // Correct value will added in later.
    big_unsigned(0);

    // Read digit by digit.
    for (char digit : n) {
      digit_multiply(*this, base);
      // TODO: This would only need a possibly faster digit_add.
      digit_add(*this, char_to_value(digit, base));
      // add(*this, char_to_value(digit, base), *this);
    }
  }

  /**
   * Convert value into a string.
   *
   * @param base The base to print in. 2 <= base <= 16.
   * @return The value as a string in the given base.
   */
  std::string to_string(uint8_t base = 10) const {
    // Special case: value = 0.
    if (digits.size() == 0) return "0";

    // Recursively find the digits in the requested base.
    // Must work on a copy, since the value must be repeatedly divided by base.
    std::string s;
    big_unsigned value = *this;
    to_string_recursive(s, value, base);
    return s;
  }

  /**
   * Hidden friend for stream output.
   *
   * @param os The out stream.
   * @param bu The big_unsigned to print.
   * @return The out stream.
   */
  friend std::ostream& operator<<(std::ostream &os, const big_unsigned &bu) {
    os << bu.to_string();
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
  friend bool operator<(const big_unsigned &a, const big_unsigned &b) {
    // If they have different length, we can just compare their lengths.
    std::size_t da = a.digits.size();
    std::size_t db = b.digits.size();
    if (da != db) return da < db;

    // They have the same length. Find the highest position where their digits
    // are different.
    for (auto ait = a.digits.rbegin(), bit = b.digits.rbegin();
        ait != a.digits.rend(); ++ait, ++bit) {
      if (*ait != *bit) return *ait < *bit;
    }
    // They must be equal.
    return false;
  }

  /**
   * Greater than operator.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a > b.
   */
  friend bool operator>(const big_unsigned &a, const big_unsigned &b) {
    return b < a;
  }

  /**
   * Equal operator.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a == b.
   */
  friend bool operator==(const big_unsigned &a, const big_unsigned &b) {
    return a.digits == b.digits;
  }

  /**
   * Unequal operator.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a != b.
   */
  friend bool operator!=(const big_unsigned &a, const big_unsigned &b) {
    return a.digits != b.digits;
  }

  /**
   * Less than or equal operator.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a <= b.
   */
  friend bool operator<=(const big_unsigned &a, const big_unsigned &b) {
    // If they have different length, we can just compare their lengths.
    std::size_t da = a.digits.size();
    std::size_t db = b.digits.size();
    if (da != db) return da < db;

    // They have the same length. Find the highest position where their digits
    // are different.
    for (auto ait = a.digits.rbegin(), bit = b.digits.rbegin();
        ait != a.digits.rend(); ++ait, ++bit) {
      if (*ait != *bit) return *ait < *bit;
    }
    // They must be equal.
    return true;
  }

  /**
   * Greater than or equal operator.
   *
   * @param a The left hand side to compare.
   * @param b The right hand side to compare.
   * @return True, if and only if a >= b.
   */
  friend bool operator>=(const big_unsigned &a, const big_unsigned &b) {
    return b <= a;
  }

  /**
   * Addition for big_unsigned.
   *
   * @param a The first summand.
   * @param b The second summand.
   * @return The sum of a and b.
   */
  friend big_unsigned operator+(const big_unsigned &a, const big_unsigned &b) {
    big_unsigned sum;
    add(a, b, sum);
    return sum;
  }

  /**
   * Adds another big_unsigned to the current one.
   *
   * @param other The other summand.
   * @return Reference to the current big_unsigned containing the sum of its
   *         previous value and other.
   */
  big_unsigned& operator+=(const big_unsigned &other) {
    add(*this, other, *this);
    return *this;
  }

  /**
   * Adds a single digit into the current one.
   *
   * @param other The other summand.
   * @return  Reference to the current big_unsigned containing the sum of its
   *          previous value and other.
   */
   big_unsigned& operator+=(digit_type other) {
     digit_add(*this, other);
     return *this;
   }

  // /**
  //  * Pre-increment.
  //  *
  //  * TODO: Add more efficient implementation. Does not need general big_unsigned
  //  *       addition. Digits addition is enough.
  //  *
  //  * @return Reference to current (incremented object).
  //  */
  // big_unsigned& operator++() {
  //   add(*this, 1, *this);
  //   return *this;
  // }

  /**
   * Subtraction for big_unsigned.
   * It must be a >= b.
   *
   * @param a The minuend.
   * @param b The subtrahend.
   * @return The difference a - b.
   */
  friend big_unsigned operator-(const big_unsigned &a, const big_unsigned &b) {
    big_unsigned difference;
    subtract(a, b, difference);
    return difference;
  }

  /**
   * Subtracts another big_unsigned from the current one.
   *
   * @param other The subtrahend. Must be at least as big as current value.
   * @return Reference to the current big_unsigned containing the diffrence of
   *         its previous value and other.
   */
  big_unsigned& operator-=(const big_unsigned &other) {
    subtract(*this, other, *this);
    return *this;
  }

  /**
   * Multiplication for big_unsigned.
   *
   * @param a The first factor.
   * @param b The second factor.
   * @return The product a * b.
   */
  friend big_unsigned operator*(const big_unsigned &a, const big_unsigned &b) {
    big_unsigned product;
    mulitply(a, b, product);
    return product;
  }

  /**
   * Multiplies another big_unsigned to the current one.
   *
   * @param other The other factor.
   * @return Reference to the current big_unsigned containing the product of its
   *         previous value and other.
   */
  big_unsigned& operator*=(const big_unsigned &other) {
    // TODO: This creates an intermediate big_unsigned. Find a way to do the
    //       multiplication in-place.
    big_unsigned product;
    mulitply(*this, other, product);
    digits = std::move(product.digits);
    return *this;
  }

  /**
   * Divides a by b.
   *
   * @param a The dividend.
   * @param b The divisor.
   * @return The quotient of a div b.
   */
  friend big_unsigned operator/(const big_unsigned &a, const big_unsigned &b) {
    big_unsigned quotient, remainder;
    divide_with_remainder(a, b, quotient, remainder);
    return quotient;
  }

  /**
   * Divides another big_unsigned from the current one.
   *
   * @param other The divisor.
   * @return Reference to the current big_unsigned containing the quotient of
   *         its previous value and other.
   */
  big_unsigned& operator/=(const big_unsigned &other) {
    big_unsigned quotient, remainder;
    divide_with_remainder(*this, other, quotient, remainder);
    digits = std::move(quotient.digits);
    return *this;
  }

  /**
   * Computes the remainder of a and b.
   *
   * @param a The dividend.
   * @param b The divisor.
   * @return The remainder of a mod b.
   */
  friend big_unsigned operator%(const big_unsigned &a, const big_unsigned &b) {
    big_unsigned quotient, remainder;
    divide_with_remainder(a, b, quotient, remainder);
    return remainder;
  }

  /**
   * Computes the remainder of another big_unsigned and the current one.
   *
   * @param other The divisor.
   * @return Reference to the current big_unsigned containing the remainder of
   *         its previous value and other.
   */
  big_unsigned& operator%=(const big_unsigned &other) {
    big_unsigned quotient, remainder;
    divide_with_remainder(*this, other, quotient, remainder);
    digits = std::move(remainder.digits);
    return *this;
  }

  /**
   * Divides a by b and outputs the quotient and the remainder.
   *
   * TODO: Uses binary search to find the quotient. This is far from optimal.
   *       Investigate for a better algorithm.
   *
   * @param a The dividend.
   * @param b The divisor.
   * @param quotient The quotient a div b.
   * @param remainder The remainder a mod b.
   */
  static void divide_with_remainder(const big_unsigned &a, const big_unsigned &b,
      big_unsigned &quotient, big_unsigned &remainder) {
    // Base case, equality.
    if (a == b) {
      quotient = 1;
      remainder = 0;
      return;
    }

    // Base case, a < b.
    if (a < b) {
      quotient = 0;
      remainder = a;
      return;
    }

    // Binary search for the quotient.
    quotient = 0;
    remainder = a - quotient * b;
    while (remainder >= b) {
      big_unsigned increase = 1;
      while ((quotient + increase * 2) * b <= a) increase *= 2;
      quotient += increase;
      remainder = a - quotient * b;
    }
  }

  /**
   * Cast to digit_type.
   * Only returns the last digit.
   *
   * @return The value of the least significant digit.
   */
  explicit operator digit_type() const {
    return digits.size() ? digits[0] : 0;
  }

private:
  /**
   * An unbounded array containing the digits.
   * Elements are stroed in reversed order, so element i corresponds to the
   * value digits[i] * BASE^i.
   */
  std::vector<digit_type> digits;

  /**
   * Converts a character into its numerical value.
   *
   * @param c The character.
   * @param base The base.
   * @return The numerical value of the character.
   */
  static digit_type char_to_value(char c, uint8_t base) {
    digit_type value = 0;
    if ('0' <= c && c <= '9') value = c - '0';
    else if ('A' <= c && c <= 'F') value = c - 'A' + 10;
    else throw std::invalid_argument("Digits can only be written by 0..9A..F.");

    if (value >= base) {
      throw std::invalid_argument("Invalid digit in given base.");
    }
    return value;
  }

  static char value_to_char(digit_type value, uint8_t base) {
    if (value >= base) {
      throw std::invalid_argument("Value is too big for the given base.");
    }

    if (value <= 9) return value + '0';
    return value - 10 + 'A';
  }

  /**
   * Recursively computes the digits in the required base.
   * The result is in reverse order.
   *
   * @param s The string to write the result into.
   * @param value The current value of the remaining big_unsigned.
   * @param base The base to print in. 2 <= base <= 16.
   */
  void to_string_recursive(std::string &s, big_unsigned &value, uint8_t base)
      const {
    // Base case for value 0.
    if (value == 0) return;

    // Compute last digit, recurse and then push it to the result.
    digit_type remainder;
    digit_divide_with_remainder(value, base, value, remainder);
    to_string_recursive(s, value, base);
    s.push_back(value_to_char(remainder, base));
  }

  /**
   * Removes leading zeros.
   */
  void remove_leading_zeros() {
    while (digits.size() && digits.back() == 0) digits.pop_back();
  }

  /**
   * Adds digit b to big_unsigned a.
   *
   * @param a The big_unsigned to add into.
   * @param b The digit to add.
   */
  static void digit_add(big_unsigned &a, digit_type b) {
    std::size_t da = a.digits.size();

    // Base case: a == 0
    if (da == 0) {
      a.digits.push_back(b);
      return;
    }

    // Add to each digit.
    digit_type carry = b;
    for (std::size_t i = 0; i < da; ++i) {
      // Cast is important to avoid overflows.
      double_digit_type digit_sum =
          static_cast<double_digit_type>(a.digits[i]) + carry;
      a.digits[i] = digit_sum % BASE;
      carry = digit_sum / BASE;
    }

    // If the carry is not zero, a has another digit.
    if (carry != 0) a.digits.push_back(carry);
  }

  /**
   * Adds two big_unsigneds into a third.
   * Parameters a and c may be the same, for a += b.
   *
   * @param a The first summand.
   * @param b The second summand.
   * @param c The sum a + b.
   */
  static void add(const big_unsigned &a, const big_unsigned &b,
      big_unsigned &c) {
    std::size_t da = a.digits.size();
    std::size_t db = b.digits.size();

    // Reserve enough space for the sum and zero initialize the minimum amount
    // of needed digits.
    c.digits.reserve(std::max(da, db) + 1);
    c.digits.resize(std::max(da, db));

    // Common positions of a and b.
    digit_type carry = 0;
    std::size_t i = 0;
    for (; i < std::min(da, db); ++i) {
      // Cast is important to avoid overflows.
      double_digit_type digit_sum =
          static_cast<double_digit_type>(a.digits[i]) + b.digits[i] + carry;
      c.digits[i] = digit_sum % BASE;
      carry = digit_sum / BASE;
    }

    // Take the longer summand and add remaining digits.
    auto *longer = da > db ? &a : &b;
    for (; i < std::max(da, db); ++i) {
      // Cast is important to avoid overflows.
      double_digit_type digit_sum =
          static_cast<double_digit_type>(longer->digits[i]) + carry;
      c.digits[i] = digit_sum % BASE;
      carry = digit_sum / BASE;
    }

    // If the carry is non-zero we need another digit.
    if (carry != 0) c.digits.push_back(carry);
  }

  /**
   * Subtracts two big_unsigneds into a third.
   * Parameters a and c may be the same, for a -= b.
   * It must be a >= b.
   *
   * @param a The minuend.
   * @param b The subtrahend.
   * @param c The difference a - b.
   */
  static void subtract(const big_unsigned &a, const big_unsigned &b,
      big_unsigned &c) {
    std::size_t da = a.digits.size();
    std::size_t db = b.digits.size();

    // Initialize result and resize to the maximum number of digits.
    c.digits.resize(da);

    // Since a >= b it must be da >= db.
    // First go through b's digits.
    digit_type carry = 0;
    std::size_t i = 0;
    for (; i < db; ++i) {
      digit_type lower = b.digits[i] + carry;
      digit_type upper = a.digits[i];
      if (lower > upper) {
        upper += BASE;
        carry = 1;
      } else carry = 0;
      c.digits[i] = upper - lower;
    }

    // Now go through the remaining digits of a.
    for (; i < da; ++i) {
      if (carry > a.digits[i]) {
        c.digits[i] = BASE - 1;
        carry = 1;
      } else {
        c.digits[i] = a.digits[i] - carry;
        carry = 0;
      }
    }

    // Leading digits of the difference might be 0. Remove those.
    c.remove_leading_zeros();
  }

  /**
   * In place mulitplication of a by digit b.
   *
   * @param a The first factor with an arbitrary number of digits.
   * @param b The second factor, a single digit.
   */
  static void digit_multiply(big_unsigned &a, const digit_type b) {
    std::size_t da = a.digits.size();

    // Go through the digits of a.
    digit_type product_carry = 0;
    for (std::size_t i = 0; i < da; ++i) {
      // Cast is important to avoid overflows.
      double_digit_type digit_product =
          static_cast<double_digit_type>(a.digits[i]) * b + product_carry;
      product_carry = digit_product / BASE;
      a.digits[i] = digit_product % BASE;
    }

    // If the product_carry is non-zero, it is the most significant digit of the
    // product. Add it into the result array.
    if (product_carry != 0) a.digits.push_back(product_carry);
  }

  /**
   * Performs a digit multiplication.
   *
   * Simultaneously computes each digit of the product and adds it into the
   * partial result.
   *
   * @param a The first factor with an arbitrary number of digits.
   * @param b The second factor, a single digit.
   * @param c The big_unsigned the result is added into.
   * @param d The offset used when writing the result into c. This allows to not
   *          just add the result but mulitply it with a factor BASE^i
   *          beforehand, thus aligning the partial sums.
   */
  static void digit_multiply_offset(const big_unsigned &a, const digit_type b,
      big_unsigned &c, std::size_t d = 0) {
    std::size_t da = a.digits.size();

    // Go through the digits of a.
    digit_type product_carry = 0;
    digit_type sum_carry = 0;
    std::size_t i = 0;
    for (; i < da; ++i) {
      // Cast is important to avoid overflows.
      double_digit_type digit_product =
          static_cast<double_digit_type>(a.digits[i]) * b + product_carry;
      product_carry = digit_product / BASE;
      // Cast is important to avoid overflows.
      double_digit_type digit_sum =
          static_cast<double_digit_type>(c.digits[d + i]) +
          digit_product % BASE +
          sum_carry;
      c.digits[d + i] = digit_sum % BASE;
      sum_carry = digit_sum / BASE;
    }

    // If the product_carry is non-zero, it is the most significant digit of the
    // product. Add it into the result array.
    // Further there might still be a sum_carry.
    if (product_carry != 0) {
      // Cast is important to avoid overflows.
      double_digit_type digit_sum =
          static_cast<double_digit_type>(c.digits[d + i]) +
          product_carry +
          sum_carry;
      c.digits[d + i] = digit_sum % BASE;
      sum_carry = digit_sum / BASE;
    }

    // Now product_carry is guaranteed to be zero, but we might still have a
    // carry from the addition.
    while (sum_carry != 0) {
      ++i;
      // Cast is important to avoid overflows.
      double_digit_type digit_sum =
          static_cast<double_digit_type>(c.digits[d + i]) + sum_carry;
      c.digits[d + i] = digit_sum % BASE;
      sum_carry = digit_sum / BASE;
    }
  }

  /**
   * Multiplies two big_unsigneds into a third.
   * Parameters a and b must be different to c, as c's content gets erased.
   *
   * @param a The first factor.
   * @param b The second factor.
   * @param c The product a * b.
   */
  static void mulitply(const big_unsigned &a, const big_unsigned &b,
      big_unsigned &c) {
    std::size_t da = a.digits.size();
    std::size_t db = b.digits.size();

    // Reserve enough space for the result and zero initialize.
    c.digits.assign(da + db, 0);

    // Go through the digits of the shorter number and perform digit
    // multiplications adding the shifted results.
    if (da <= db) {
      for (std::size_t i = 0; i < da; ++i) {
        digit_multiply_offset(b, a.digits[i], c, i);
      }
    } else {
      for (std::size_t i = 0; i < db; ++i) {
        digit_multiply_offset(a, b.digits[i], c, i);
      }
    }

    // Result might not need all reserved digits.
    c.remove_leading_zeros();
  }

  /**
   * Divides the big_unsigned a by a single digit b.
   * Parameters a and quotient may be the same.
   *
   * @param a The dividend.
   * @param b The divisor. A single digit.
   * @param quotient The quotient of the division.
   * @param remainder The remainder of the division.
   */
   static void digit_divide_with_remainder(const big_unsigned &a, digit_type b,
      big_unsigned &quotient, digit_type &remainder) {
    std::size_t da = a.digits.size();

    // Is the first digit of a at least b?
    // If so, the quotient has the same length as a.
    // Otherwise it is exactly one digit shorter.
    std::size_t dc = da;
    if (a.digits.back() < b) --dc;

    // Resize quotient to its exact size.
    quotient.digits.resize(da);

    // Divide by school method.
    remainder = 0;
    for (std::size_t i = 0; i < da; ++i) {
      std::size_t ia = da - i - 1;

      // Cast is important to avoid overflows.
      double_digit_type double_remainder =
          static_cast<double_digit_type>(remainder) * BASE + a.digits[ia];
      quotient.digits[ia] = double_remainder / b;
      remainder = double_remainder % b;
    }

    // If dc < da, then the result is shifted.
    // Bring it back into place.
    if (dc < da) quotient.digits.pop_back();
  }
};

}
