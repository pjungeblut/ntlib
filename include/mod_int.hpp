#pragma once

#include <istream>
#include <ostream>
#include <type_traits>

#include "modulo.hpp"

namespace ntlib {

inline constexpr uint32_t runtime_modulus = 0;

/**
 * Class to represent an element of residue class Z/modulusZ.
 * 
 * The non-type template parameter `MOD` can be used if the modulus is known at
 * compile-time. In that case, the compiler can replace the costly division
 * operations needed for `mod()`-calls by cheaper multiplication.
 * 
 * Setting `MOD = 0` denotes that the modulus must be provided to the
 * constructor at run-time.
 * 
 * The underlying (unsigned) datatype `U` needs to be big enough to hold
 * `modulus^2` to work correctly.
 */
template <typename U, U MOD = 0, typename S = std::make_signed_t<U>>
class mod_int {
public:
  /**
   * Constructor for compile-time modulus.
   * 
   * @param value The initial value.
   */
  mod_int(U value = 0) requires(MOD != 0) :
      modulus(MOD), value(mod(value, modulus)) {
    static_assert(MOD > 0);
  }

  /**
   * Constructor for run-time modulus.
   * 
   * @param value The initial value.
   * @param modulus The modulus.
   */
  mod_int(U value, U modulus) requires(MOD == 0) :
      modulus(modulus), value(mod(value, modulus)) {
    assert(modulus > 0);
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
   * Returns the modulus.
   * 
   * @return The modulus.
   */
  [[nodiscard]]
  U get_modulus() const {
    return modulus;
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
   * @return Reference to this `mod_int` (with the new value).
   */
  mod_int& operator++() {
    value = mod(value + 1, modulus);
    return *this;
  }

  /**
   * Postfix increment.
   * 
   * @param _ To indicate postfix version.
   * @return A `mod_int` with the old value.
   */
  mod_int operator++(int) {
    mod_int old = *this;
    operator++();
    return old;
  }

  /**
   * Prefix decrement.
   * 
   * @return Reference to this `mod_int` (with the new value).
   */
  mod_int& operator--() {
    value = value ? value - 1 : modulus - 1;
    return *this;
  }

  /**
   * Postfix decrement.
   * 
   * @param _ To indicate postfix version.
   * @return A `mod_int` with the old value.
   */
  mod_int operator--(int) {
    mod_int old = *this;
    operator--();
    return old;
  }

  /**
   * Compound addition.
   * 
   * @param rhs The summand.
   * @return Reference to this `mod_int`.
   */
  mod_int& operator+=(mod_int rhs) {
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
  friend mod_int operator+(mod_int lhs, mod_int rhs) {
    lhs += rhs;
    return lhs;
  }

  /**
   * Compound subtraction.
   * 
   * @param rhs The subtrahend.
   * @return Reference to this `mod_int`.
   */
  mod_int& operator-=(mod_int rhs) {
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
  friend mod_int operator-(mod_int lhs, mod_int rhs) {
    lhs -= rhs;
    return lhs;
  }

  /**
   * Compound multiplication.
   * 
   * @param rhs The second factor.
   * @return Reference to this `mod_int`.
   */
  mod_int& operator*=(mod_int rhs) {
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
  friend mod_int operator*(mod_int lhs, mod_int rhs) {
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
  friend bool operator==(mod_int lhs, mod_int rhs) = default;

  /**
   * Finds the multiplicative inverse.
   * 
   * @return The multiplicative inverse in the range `[0,modulus)`.
   */
  [[nodiscard]]
  mod_int invert() const {
    auto result = mod_mult_inv<U,S>(value, modulus);
    if constexpr (MOD == 0) {
      return mod_int(result, modulus);
    } else {
      return mod_int(result);
    }
  }

private:
  /**
   * The modulus.
   */
  const U modulus;

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
 * @param obj Instance of a `mod_int`.
 * @return Reference to the output stream.
 */
template<typename U, U MOD = 0, typename S = std::make_signed_t<U>>
std::ostream& operator<<(std::ostream &os, mod_int<U, MOD, S> obj) {
  os << obj.get();
  return os;
}

/**
 * Stream extraction operator.
 * Only available for compile-time modulus version.
 * 
 * @param is Reference to the input stream.
 * @param obj Reference to the `ct_mod_int` to store the read value.
 * @return Reference to the input stream.
 */
template<typename U, U MOD = 0, typename S = std::make_signed_t<U>>
std::istream& operator>>(std::istream &is, mod_int<U, MOD, S> &obj)
    requires(MOD != 0) {
  U tmp;
  is >> tmp;
  obj.set(tmp);
  return is;
}

}