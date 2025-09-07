/**
 * @file
 * @brief Primary module interface unit for module `mod_int`.
 */
module;

#include <algorithm>
#include <cassert>
#include <limits>
#include <tuple>
#include <type_traits>

/**
 * @module mod_int
 * @brief Represents an integer modulo another integer.
 */
export module mod_int;

import base;
import modulo;

namespace ntlib {

/**
 * @concept ModIntAddable
 * @brief Concept to ensure that no overflow can occur during addition or
 *     subtraction.
 *
 * Checks that `T` can hold at least \f$2m\f$ which is required internally for
 * addition and subtraction. If this concept is not satisfied, a bigger integer
 * type must be used for `T`.
 */
template<typename T, T m>
concept ModIntAddable =
    !std::numeric_limits<T>::is_bounded ||
    std::numeric_limits<T>::max() / 2 >= m;

/**
 * @concept ModIntMultiplicable
 * @brief Concept to ensure that no overflow can occur during multiplication.
 * 
 * Checks that `T` can hold at least \f$m^2\f$ which is required internally for
 * multiplication. If this concept is not satisfied, a bigger integer type must
 * be used for `T`.
 */
template<typename T, T m>
concept ModIntMultiplicable =
    !std::numeric_limits<T>::is_bounded ||
    std::numeric_limits<T>::max() / m >= m;

/**
 * @brief Class representing an element of a ring of integers modulo another
 *     positive integer. The modulus is provided at compile-time.
 * 
 * @note To avoid overflows, the type `T` must be able to hold
 *     - \f$2m\f$ for addition and subtraction,
 *     - \f$m^2\f$ for multiplication.
 *     This is ensured at compile-time. For this to work, `T` must provide
 *    `std::numeric_limits<T>::max()`.
 * 
 * @tparam T An integer-like type.
 * @tparam m The modulus.
 */
export template<Integer T, T m>
    requires(m > 0)
class mod_int {
public:
  /**
   * @brief Default constructor. Initializes the value to zero.
   */
  mod_int() noexcept : value(0) {}

  /**
   * @brief Construct a new element from a given value.
   * 
   * @param n The given value.
   */
  explicit mod_int(T n) noexcept : value([](T n) {
    if constexpr (std::is_unsigned_v<T>) {
      return n % m;
    } else {
      return ((n % m) + m) % m;
    }
  }(n)) {}

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
  mod_int& operator++() noexcept {
    value = (value + 1) % m;
    return *this;
  }

  /**
   * @brief Postfix increment operator.
   * 
   * @return The old value.
   */
  mod_int operator++(int) noexcept {
    const mod_int old_value = *this;
    operator++();
    return old_value;
  }

  /**
   * @brief Prefix decrement operator.
   * 
   * @return Reference to the result (with the new value).
   */
  mod_int& operator--() noexcept {
    if (value == 0) { value = m - 1; }
    else { --value; }
    return *this;
  }

  /**
   * @brief Postfix decrement operator.
   * 
   * @return The old value.
   */
  mod_int operator--(int) noexcept {
    const mod_int old_value = *this;
    operator--();
    return old_value;
  }

  /**
   * @brief Compound plus operator for modular addition.
   * 
   * @note Contains a compile-time check to ensure that no overflow can occur.
   * 
   * @param rhs The second summand.
   * @return Reference to the result.
   */
  mod_int& operator+=(mod_int rhs) noexcept requires ModIntAddable<T, m> {
    value = (value + rhs.value) % m;
    return *this;
  }

  /**
   * @brief Plus operator for modular addition.
   * 
   * @param lhs The first summand.
   * @param rhs The second summand.
   * @return The sum of `lhs` and `rhs`.
   */
  friend mod_int operator+(mod_int lhs, mod_int rhs) noexcept {
    lhs += rhs;
    return lhs;
  }

  /**
   * @brief Compound minus operator for modular subtraction.
   * 
   * @note Contains a compile-time check to ensure that no overflow can occur.
   * 
   * @param rhs The subtrahend.
   * @return Reference to the result.
   */
  mod_int& operator-=(mod_int rhs) noexcept requires ModIntAddable<T, m> {
    value = (value + m - rhs.value) % m;
    return *this;
  }

  /**
   * @brief Minus operator for modular subtraction.
   * 
   * @param lhs The minuend.
   * @param rhs The subtrahend.
   * @return The difference of `lhs` and `rhs`.
   */
  friend mod_int operator-(mod_int lhs, mod_int rhs) noexcept {
    lhs -= rhs;
    return lhs;
  }

  /**
   * @brief Compound times operator for modular multiplication.
   * 
   * @note Contains a compile-time check to ensure that no overflow can occur.
   * 
   * @param rhs The second factor.
   * @return Reference to the result.
   */
  mod_int& operator*=(mod_int rhs) noexcept requires ModIntMultiplicable<T, m> {
    value = (value * rhs.value) % m;
    return *this;
  }

  /**
   * @brief Times operator for modular multiplication.
   * 
   * @param lhs The first factor.
   * @param rhs The second factor.
   * @return The product of `lhs` and `rhs`.
   */
  friend mod_int operator*(mod_int lhs, mod_int rhs) noexcept {
    lhs *= rhs;
    return lhs;
  }

  /**
   * @brief Unary plus operator. Does nothing.
   * 
   * @return The same instance as before.
   */
  mod_int operator+() const noexcept {
    return *this;
  }

  /**
   * @brief Unary minus operator to negate value.
   * 
   * @return The negated instance.
   */
  mod_int operator-() const noexcept {
    return mod_int(m - value);
  }

  /**
   * @brief Inverts the value.
   * 
   * @note This requires that the current value is coprime to the modulus.
   */
  void invert() requires (std::numeric_limits<T>::is_signed) {
    assert(ntlib::gcd(value, m) == 1);
    value = ntlib::mod_mult_inv<T,T>(value, m);
  }

  /**
   * @brief Equality comparison operator.
   * 
   * @param lhs The first instance.
   * @param rhs The second instance.
   * @return Whether `lhs` and `rhs` are equal.
   */
  friend bool operator==(mod_int lhs, mod_int rhs) noexcept {
    return static_cast<T>(lhs) == static_cast<T>(rhs);
  }

private:
  /// @brief The current value.
  T value;
};

/**
 * @brief Specialization of `ntlib::algebra_traits` for `mod_int`.
 * 
 * @tparam T An integer-like type.
 * @tparam m The modulus.
 */
export template<Integer T, T m>
class algebra_traits<mod_int<T, m>> {
public:
  /**
   * @brief Returns the additive neutral element of `mod_int<T, m>`.
   * 
   * @return The additive neutral element, i.e., \f$0 \mod m\f$.
   */
  [[nodiscard]] static constexpr mod_int<T, m> get_zero() noexcept {
    return mod_int<T, m> {0};
  }

  /**
   * @brief Returns the multiplicative neutral element of `mod_int<T, m>`.
   * 
   * @return The multiplicative neutral element, i.e., \f$1 \mod m\f$.
   */
  [[nodiscard]] static constexpr mod_int<T, m> get_one() noexcept {
    return mod_int<T, m> {1};
  }

  /// @brief Modular addition is commutative.
  static constexpr bool is_additive_commutative = true;

  /// @brief Modular multiplication is commutative.
  static constexpr bool is_multiplicative_commutative = true;
};

} // namespace ntlib
