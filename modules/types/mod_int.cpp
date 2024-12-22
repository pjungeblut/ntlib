/**
 * @file
 * @brief Primary module interface unit for module `mod_int`.
 */
module;

#include <algorithm>
#include <cassert>
#include <tuple>
#include <type_traits>

/**
 * @module mod_int
 * @brief Represents an element of a residue class modulo a given number.
 * 
 * Let \f$m \in \mathbb{N}\f$. This class represents an element of the residue
 * class \f$\mathbb{Z}/m\mathbb{Z}\f$.
 * 
 * There are two classes in this module.
 * - `rt_mod_int` takes the modulus \f$m\f$ at run-time.
 * - `ct_mod_int` takes the modulus \f$m\f$ at compile-time.
 */
export module mod_int;

import base;
import modulo;

namespace ntlib {

/**
 * Forward declarations.
 */
export template<typename T> class rt_mod_int;

/**
 * @brief Base class for CRTP design of `rt_mod_int` and `ct_mod_int`.
 * 
 * @note As this base class does not store the modulus \f$n\f$, it makes no
 *   sense to create instances of it. For this reason, all constructors and
 *   assignment operators are protected.
 * 
 * @tparam T An integer-like type.
 * @tparam DERIVED Class derived from this one.
 */
template<typename T, typename DERIVED>
class mod_int {
public:
  /**
   * @brief Returns the value.
   * 
   * @return The value in the range \f$[0, m)\f$.
   */
  explicit operator T() const noexcept {
    return value;
  }

  /**
   * @brief Prefix increment operator.
   * 
   * @return Reference to the result (with new value).
   */
  DERIVED& operator++() noexcept {
    value = mod(value + 1);
    return static_cast<DERIVED&>(*this);
  }

  /**
   * @brief Postfix increment operator.
   * 
   * @return The old value.
   */
  DERIVED operator++(int) noexcept {
    const auto old_value = static_cast<DERIVED&>(*this);
    operator++();
    return old_value;
  }

  /**
   * @brief Prefix decrement operator.
   * 
   * @return Reference to the result (with the new value).
   */
  DERIVED& operator--() noexcept {
    const T m = static_cast<DERIVED*>(this)->get_modulus();
    value = mod(value + m - 1);
    return static_cast<DERIVED&>(*this);
  }

  /**
   * @brief Postfix decrement operator.
   * 
   * @return The old value.
   */
  DERIVED operator--(int) noexcept {
    const auto old_value = static_cast<DERIVED&>(*this);
    operator--();
    return old_value;
  }

  /**
   * @brief Compound plus operator for modular addition.
   * 
   * @param rhs The second summand.
   * @return Reference to the result.
   */
  DERIVED& operator+=(DERIVED rhs) noexcept {
    value = mod(value + rhs.value);
    return static_cast<DERIVED&>(*this);
  }

  /**
   * @brief Plus operator for modular addition.
   * 
   * @param lhs The first summand.
   * @param rhs The second summand.
   * @return The sum of `lhs` and `rhs`.
   */
  friend DERIVED operator+(DERIVED lhs, DERIVED rhs) noexcept {
    lhs += rhs;
    return lhs;
  }

  /**
   * @brief Compound minus operator for modular subtraction.
   * 
   * @param rhs The subtrahend.
   * @return Reference to the result.
   */
  DERIVED& operator-=(DERIVED rhs) noexcept {
    const T m = static_cast<DERIVED*>(this)->get_modulus();
    value = mod(value + m - rhs.value);
    return static_cast<DERIVED&>(*this);
  }

  /**
   * @brief Minus operator for modular subtraction.
   * 
   * @param lhs The minuend.
   * @param rhs The subtrahend.
   * @return The difference of `lhs` and `rhs`.
   */
  friend DERIVED operator-(DERIVED lhs, DERIVED rhs) noexcept {
    lhs -= rhs;
    return lhs;
  }

  /**
   * @brief Compound times operator for modular multiplication.
   * 
   * @param rhs The second factor.
   * @return Reference to the result.
   */
  DERIVED& operator*=(DERIVED rhs) noexcept {
    value = mod(value * rhs.value);
    return static_cast<DERIVED&>(*this);
  }

  /**
   * @brief Times operator for modular multiplication.
   * 
   * @param lhs The first factor.
   * @param rhs The second factor.
   * @return The product of `lhs` and `rhs`.
   */
  friend DERIVED operator*(DERIVED lhs, DERIVED rhs) noexcept {
    lhs *= rhs;
    return lhs;
  }

  /**
   * @brief Unary plus operator. Does nothing.
   * 
   * @return The same instance as before.
   */
  DERIVED operator+() const noexcept {
    return static_cast<const DERIVED&>(*this);
  }

  /**
   * @brief Inverts the value.
   * 
   * @note This requires that the current value is coprime to the modulus.
   */
  template<typename S = std::make_signed_t<T>>
  void invert() {
    const T m = static_cast<DERIVED*>(this)->get_modulus();
    assert(ntlib::gcd(value, m) == 1);
    value = ntlib::mod_mult_inv<T,S>(value, m);
  }

protected:
  /**
   * @brief Default constructor.
   */
  mod_int() noexcept = default;

  /**
   * @brief Construct from a given value.
   * 
   * @param n The given value.
   */
  explicit mod_int(T n) noexcept : value(n) {}

private:
  /**
   * @brief Computes the remainder of a given number modulo \f$m\f$.
   * 
   * @note Helper function for the CRTP design to get the modulus \f$m\f$ from
   *   the respective base class.
   * 
   * @param n The given number.
   * @return The remainder in the interval \f$[0, m)\f$.
   */
  [[nodiscard]]
  T mod(T n) const noexcept {
    return ntlib::mod(n, static_cast<const DERIVED&>(*this).get_modulus());
  }

  /**
   * @brief The current value.
   */
  T value;
};

/**
 * @brief Represents an element of the residue class
 * \f$\mathbb{Z}/m\mathbb{Z}\f$, where the modulus \f$m\f$ is provided at the
 * time of construction (i.e., at run-time).
 * 
 * @tparam An integer-like type. 
 */
export template<typename T>
class rt_mod_int : public mod_int<T, rt_mod_int<T>> {
public:
  /**
   * @brief Default constructor.
   * 
   * @note Trying to read the value or modulus of a default constructed
   *   `rt_mod_int` is undefined behavior.
   */
  rt_mod_int() noexcept = default;

  /**
   * @brief Construct from a given value and a given modulus.
   * 
   * @param n The given value.
   * @param m The given modulus.
   */
  rt_mod_int(T n, T m) noexcept :
      mod_int<T, rt_mod_int<T>>(ntlib::mod(n, m)),
      modulus(m) {}

  /**
   * @brief Returns the modulus \f$m\f$.
   * 
   * @return The modulus.
   */
  [[nodiscard]]
  T get_modulus() const noexcept {
    return modulus;
  }

  /**
   * @brief Equality comparison operator.
   * 
   * @param lhs The first instance.
   * @param rhs The second instance.
   * @return Whether `lhs` and `rhs` are equal.
   */
  friend bool operator==(rt_mod_int lhs, rt_mod_int rhs) noexcept {
    return static_cast<T>(lhs) == static_cast<T>(rhs) &&
        lhs.get_modulus() == rhs.get_modulus();
  }

  /**
   * @brief Unary minus operator to negate value.
   * 
   * @return The negated instance.
   */
  rt_mod_int operator-() const noexcept {
    return rt_mod_int {
        static_cast<T>(modulus - static_cast<T>(*this)), modulus};
  }

private:
  /**
   * The modulus \f$m\f$.
   */
  T modulus;
};

/**
 * @brief Represents an element of the residue class
 * \f$\mathbb{Z}/m\mathbb{Z}\f$, where the modulus \f$m\f$ is provided at
 * compile-time.
 * 
 * @tparam An integer-like type. 
 * @tparam The modulus \f$m\f$.
 */
export template<typename T, T m>
class ct_mod_int : public mod_int<T, ct_mod_int<T, m>> {
public:
  /**
   * @brief Default constructor.
   * 
   * @note Trying to read the value of a default constructed `ct_mod_int` is
   *   undefined behavior.
   */
  ct_mod_int() noexcept = default;

  /**
   * @brief Construct from a given value.
   * 
   * @param n The given value.
   */
  ct_mod_int(T n) noexcept :
      mod_int<T, ct_mod_int<T, m>>(ntlib::mod(n, m)) {}

  /**
   * @brief Returns the modulus \f$m\f$.
   * 
   * @return The modulus.
   */
  [[nodiscard]]
  T get_modulus() const noexcept {
    return m;
  }

  /**
   * @brief Equality comparison operator.
   * 
   * @param lhs The first instance.
   * @param rhs The second instance.
   * @return Whether `lhs` and `rhs` are equal.
   */
  friend bool operator==(ct_mod_int lhs, ct_mod_int rhs) noexcept {
    return static_cast<T>(lhs) == static_cast<T>(rhs);
  }

  /**
   * @brief Unary minus operator to negate value.
   * 
   * @return The negated instance.
   */
  ct_mod_int operator-() const noexcept {
    return ct_mod_int {static_cast<T>(m - static_cast<T>(*this))};
  }
};

/**
 * @brief Specialization for `get_multiplicative_neutral()`.
 * 
 * @tparam T An integer-like type.
 * @param n Instance of the type.
 * @return The multiplicative neutral `1`.
 */
export template<typename T>
[[nodiscard]] constexpr
rt_mod_int<T> get_multiplicative_neutral(rt_mod_int<T> n) {
  return rt_mod_int<T> {1, n.get_modulus()};
}

/**
 * @brief Specialization for `get_multiplicative_neutral()`.
 * 
 * @tparam T An integer-like type.
 * @tparam m The modulus.
 * @return The multiplicative neutral `1`.
 */
export template<typename T, T m>
[[nodiscard]] constexpr
ct_mod_int<T, m> get_multiplicative_neutral(ct_mod_int<T, m>) {
  return ct_mod_int<T, m> {1};
}

}