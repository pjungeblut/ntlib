#pragma once

#include <istream>
#include <ostream>
#include <type_traits>

#include "modulo.hpp"

namespace ntlib {

/**
 * Class to represent an element of residue class Z/modulusZ.
 * Here, `modulus` is a compile time constant.
 * 
 * The underlying (unsigned) datatype needs to be big enough to hold
 * `modulus^2` to work correctly.
 */
template <typename U, U modulus, typename S = std::make_signed_t<U>>
class ct_mod_int {
public:
  /**
   * Constructs an element of the residue class Z/modulusZ.
   * 
   * @param value The initial value.
   */
  ct_mod_int(U value = 0) : value(mod(value, modulus)) {
    static_assert(modulus > 0);
  }

  /**
   * Returns the value.
   * 
   * @return The value in range `[0,modulus)`.
   */
  [[nodiscard]]
  U get() const {
    return value;
  }

  /**
   * Sets a value.
   * 
   * @param v The value.
   */
  void set(U v) {
    value = mod(v, modulus);
  }

  /**
   * Prefix increment.
   * 
   * @return Reference to this `ct_mod_int` (with the new value).
   */
  ct_mod_int& operator++() {
    value = mod(value + 1, modulus);
    return *this;
  }

  /**
   * Postfix increment.
   * 
   * @param _ To indicate postfix version.
   * @return A `ct_mod_int` with the old value.
   */
  ct_mod_int operator++(int) {
    ct_mod_int old = *this;
    operator++();
    return old;
  }

  /**
   * Prefix decrement.
   * 
   * @return Reference to this `ct_mod_int` (with the new value).
   */
  ct_mod_int& operator--() {
    value = value ? value - 1 : modulus - 1;
    return *this;
  }

  /**
   * Postfix decrement.
   * 
   * @param _ To indicate postfix version.
   * @return A `ct_mod_int` with the old value.
   */
  ct_mod_int operator--(int) {
    ct_mod_int old = *this;
    operator--();
    return old;
  }

  /**
   * Compound addition.
   * 
   * @param rhs The summand.
   * @return Reference to this `ct_mod_int`.
   */
  ct_mod_int& operator+=(ct_mod_int rhs) {
    value = mod(value + rhs.value, modulus);
    return *this;
  }

  /**
   * Addition.
   * 
   * @param lhs The first summand.
   * @param rhs The second summand.
   * @return The sum.
   */
  friend ct_mod_int operator+(ct_mod_int lhs, ct_mod_int rhs) {
    lhs += rhs;
    return lhs;
  }

  /**
   * Compound subtraction.
   * 
   * @param rhs The subtrahend.
   * @return Reference to this `ct_mod_int`.
   */
  ct_mod_int& operator-=(ct_mod_int rhs) {
    // Adding `modulus` to avoid negative intermediate values.
    value = mod(value + modulus - rhs.value, modulus);
    return *this;
  }
  
  /**
   * Subtraction.
   * 
   * @param lhs The minuend.
   * @param rhs The subtrahend.
   * @return The difference.
   */
  friend ct_mod_int operator-(ct_mod_int lhs, ct_mod_int rhs) {
    lhs -= rhs;
    return lhs;
  }

  /**
   * Compound multiplication.
   * 
   * @param rhs The second factor.
   * @return Reference to this `ct_mod_int`.
   */
  ct_mod_int& operator*=(ct_mod_int rhs) {
    value = mod(value * rhs.value, modulus);
    return *this;
  }

  /**
   * Multiplication.
   * 
   * @param lhs The first factor.
   * @param rhs The second factor.
   * @return The product.
   */
  friend ct_mod_int operator*(ct_mod_int lhs, ct_mod_int rhs) {
    lhs *= rhs;
    return lhs;
  }

  /**
   * Equality comparison.
   * Compiler will automatically generate `operator!=`.
   * 
   * @param lhs The left operand.
   * @param rhs The right operand.
   * @return Whether they are equal.
   */
  friend bool operator==(ct_mod_int lhs, ct_mod_int rhs) = default;

  /**
   * Finds the multiplicative inverse.
   * 
   * @return The multiplicative inverse in the range `[0,modulus)`.
   */
  [[nodiscard]]
  ct_mod_int invert() const {
    return ct_mod_int{mod_mult_inv<U,S>(value, modulus)};
  }

private:
  /**
   * The value in the range `[0,modulus)`.
   */
  U value;
};

/**
 * Stream insertion operator.
 * Just prints the value in the range `[0,modulus)`.
 * 
 * @param os Reference to the output stream.
 * @param obj Instance of a `ct_mod_int`.
 * @return Reference to the output stream.
 */
template<typename U, U modulus, typename S = std::make_signed_t<U>>
std::ostream& operator<<(std::ostream &os, ct_mod_int<U, modulus, S> obj) {
  os << obj.get();
  return os;
}

/**
 * Stream extraction operator.
 * 
 * @param is Reference to the input stream.
 * @param obj Reference to the `ct_mod_int` to store the read value.
 * @return Reference to the input stream.
 */
template<typename U, U modulus, typename S = std::make_signed_t<U>>
std::istream& operator>>(std::istream &is, ct_mod_int<U, modulus, S> &obj) {
  U tmp;
  is >> tmp;
  obj.set(tmp);
  return is;
}

}