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
   * Integral type used internally to contain the product of two digits.
   */
  using prod_type = __uint128_t;

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

  /**
   * Removes leading zeros.
   */
  void remove_leading_zeros() {
    while (digits.size() && digits.back() == 0) digits.pop_back();
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
      digit_type digit_sum = a.digits[i] + b.digits[i] + carry;
      c.digits[i] = digit_sum % BASE;
      carry = digit_sum / BASE;
    }

    // Take the longer summand and add remaining digits.
    auto *longer = da > db ? &a : &b;
    for (; i < std::max(da, db); ++i) {
      digit_type digit_sum = longer->digits[i] + carry;
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
  static void digit_multiply(const big_unsigned &a, const digit_type b,
      big_unsigned &c, std::size_t d) {
    std::size_t da = a.digits.size();

    // Go through the digits of a.
    digit_type product_carry = 0;
    digit_type sum_carry = 0;
    std::size_t i = 0;
    for (; i < da; ++i) {
      prod_type digit_product =
          static_cast<prod_type>(a.digits[i]) * b + product_carry;
      product_carry = digit_product / BASE;
      digit_type digit_sum = c.digits[d + i] + digit_product % BASE + sum_carry;
      c.digits[d + i] = digit_sum % BASE;
      sum_carry = digit_sum / BASE;
    }

    // If the product_carry is non-zero, it is the most significant digit of the
    // product. Add it into the result array.
    // Further there might still be a sum_carry.
    if (product_carry != 0) {
      digit_type digit_sum = c.digits[d + i] + product_carry + sum_carry;
      c.digits[d + i] = digit_sum % BASE;
      sum_carry = digit_sum / BASE;
    }

    // Now product_carry is guaranteed to be zero, but we might still have a
    // carry from the addition.
    while (sum_carry != 0) {
      ++i;
      digit_type digit_sum = c.digits[d + i] + sum_carry;
      c.digits[d + i] = digit_sum % BASE;
      sum_carry = digit_sum / BASE;
    }
  }

  /**
   * Multiplies two big_unsigneds into a third.
   * Parameters a and c may NOT be the same, as the content of c gets erased.
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

    // Go through the digits of b and perform digit multiplications adding the
    // shifted results.
    // TODO: Investigate whether there is a significant speedup when comparing
    //       a and b first and going throug the smaller one.
    for (std::size_t i = 0; i < db; ++i) {
      digit_multiply(a, b.digits[i], c, i);
    }

    // Result might not need all reserved digits.
    c.remove_leading_zeros();
  }
};

}
