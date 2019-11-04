#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

namespace ntlib {

/**
 * Represents a natural number whose size is only limited by the computers
 * memory.
 */
class big_unsigned {
private:
  /**
   * Integral type used for each digit.
   * Must be big enoug to contain twice the biggest possible digit.
   */
  using digit_type = uint64_t;

  /**
   * The base in which the numbers are stored is a power of ten for easier
   * printing.
   */
  constexpr static std::size_t LOG_BASE = 18;
  constexpr static std::size_t BASE = 1'000'000'000'000'000'000uLL;

public:
  /**
   * Default constructor.
   */
  big_unsigned() {}

  /**
   * Constructor to build big_unsigned from a simple unsigned long long.
   *
   * @param n The value to initialize with.
   */
  big_unsigned(unsigned long long n) {
    digits.assign(1, n);
  }

  /**
   * Constructor to build big_unsigned from a string giving a base 10
   * representation of the value.
   *
   * @param n The value to initialize with.
   */
  big_unsigned(const std::string &n) {
    std::size_t i = 0;
    digit_type digit = 0;
    digit_type factor = 1;
    for (auto base_10_digit = n.rbegin(); base_10_digit != n.rend();
        ++base_10_digit) {
      digit += factor * (*base_10_digit - '0');
      factor *= 10;
      ++i;
      if (i == LOG_BASE) {
        digits.push_back(digit);
        i = 0;
        digit = 0;
        factor = 1;
      }
    }
    if (digit != 0) digits.push_back(digit);
  }

  /**
   * Convert value into a string.
   *
   * @return The value as a string in base 10.
   */
  std::string to_string() const {
    // Special case: value = 0.
    if (digits.size() == 0) {
      return "0";
    }

    // Create result string and reserve enough space.
    std::string s;
    s.reserve(LOG_BASE * digits.size());

    // Write digit by digit.
    // The most significant digit can be printed "as is".
    // All other ones need to be padded with leading zeros.
    for (auto digit = digits.rbegin(); digit != digits.rend(); ++digit) {
      to_string_digit(*digit, s, digit != digits.rbegin());
    }

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
   * Addition for big_unsigned.
   *
   * @param a The first summand.
   * @param b The second summand.
   * @return The sum of a and b.
   */
  friend big_unsigned operator+(const big_unsigned &a, const big_unsigned &b) {
    std::size_t da = a.digits.size();
    std::size_t db = b.digits.size();

    // Initialize result and reserve enough memeory to avoud further copying.
    big_unsigned sum;
    sum.digits.reserve(std::max(da, db) + 1);

    // Common positions of a and b.
    digit_type carry = 0;
    std::size_t i = 0;
    for (; i < std::min(da, db); ++i) {
      digit_type digit_sum = a.digits[i] + b.digits[i] + carry;
      sum.digits.push_back(digit_sum % BASE);
      carry = digit_sum / BASE;
    }

    // Take the longer summand and add remaining digits.
    auto *longer = da > db ? &a : &b;
    for (; i < std::max(da, db); ++i) {
      digit_type digit_sum = longer->digits[i] + carry;
      sum.digits.push_back(digit_sum % BASE);
      carry = digit_sum / BASE;
    }

    // If the carry is non-zero we need another digit.
    if (carry != 0) sum.digits.push_back(carry);

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
    std::size_t da = digits.size();
    std::size_t db = other.digits.size();

    // Reserve enough space for the sum and zero initialize the minimum amount
    // of needed digits.
    digits.reserve(std::max(da, db) + 1);
    digits.resize(std::max(da, db));

    // Common positions of a and b.
    digit_type carry = 0;
    std::size_t i = 0;
    for (; i < std::min(da, db); ++i) {
      digit_type digit_sum = digits[i] + other.digits[i] + carry;
      digits[i] = digit_sum % BASE;
      carry = digit_sum / BASE;
    }

    // Take the longer summand and add remaining digits.
    auto *longer = da > db ? this : &other;
    for (; i < std::max(da, db); ++i) {
      digit_type digit_sum = longer->digits[i] + carry;
      digits[i] = digit_sum % BASE;
      carry = digit_sum / BASE;
    }

    // If the carry is non-zero we need another digit.
    if (carry != 0) digits.push_back(carry);

    return *this;
  }

private:
  /**
   * An unbounded array containing the digits.
   * Elements are stroed in reversed order, so element i corresponds to the
   * value digits[i] * BASE^i.
   */
  std::vector<digit_type> digits;

  /**
   * Appends a base 10 representation of a single digit to a string.
   *
   * @param digit The digit to be written.
   * @param s The string to append this digit to.
   * @param leading_zeros Whether leading zeros should be printed.
   */
  void to_string_digit(digit_type digit, std::string &s,
      bool leading_zeros = true) const {
    char base_10_digits[LOG_BASE];
    for (std::size_t i = 0; i < LOG_BASE; ++i) {
      base_10_digits[LOG_BASE - i - 1] = (digit % 10) + '0';
      digit /= 10;
    }
    if (leading_zeros) {
      for (std::size_t i = 0; i < LOG_BASE; ++i) {
        s.push_back(base_10_digits[i]);
      }
    } else {
      bool non_zero = false;
      for (std::size_t i = 0; i < LOG_BASE; ++i) {
        if (base_10_digits[i] == '0') {
          if (non_zero) s.push_back(base_10_digits[i]);
        } else {
          s.push_back(base_10_digits[i]);
          non_zero = true;
        }
      }
    }
  }
};

}
