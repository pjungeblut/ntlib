/**
 * @file
 * @brief Primary module interface unit for module `mod_int`.
 */
module;

#include <cassert>
#include <istream>
#include <ostream>
#include <string>
#include <type_traits>

/**
 * @module mod_int
 * @brief Represents an element of a residue class.
 * 
 * For a given natural number \f$n \in \mathbb{N}\f$, represents an element of
 * the residue class \f$\mathbb{Z}/n\mathbb{Z}\f$.
 * The modulus \f$n\f$, i.e., the order of the group, can either be provided at
 * run-time or at compile-time.
 */
export module mod_int;

import modulo;

namespace ntlib {

/**
 * Class to represent an element of the residue class
 * \f$\mathbb{Z}/n\mathbb{Z}\f$.
 * 
 * The non-type template parameter `MOD` can be used if the modulus is known at
 * compile-time. In that case, the compiler can replace the costly division
 * operations needed for `mod()`-calls by cheaper multiplications.
 * 
 * Setting `MOD = 0` denotes that the modulus must be provided to the
 * constructor at run-time.
 * 
 * The underlying (unsigned) datatype `T` needs to be big enough to hold
 * `modulus^2` to work correctly.
 * 
 * @tparam T An integer-like type.
 * @tparam MOD The compile-type modulos (or `0` for run-time modulus).
 * @tparam S The signed type corresponding to `T`.
 */
export template <typename T, T MOD = 0, typename S = std::make_signed_t<T>>
class mod_int {
public:
  /**
   * @brief Constructor for compile-time modulus.
   * 
   * @param value The initial value.
   */
  mod_int(T value = 0) noexcept
      requires(MOD > 0) :
          modulus(MOD), value(mod(value, modulus)) {}

  /**
   * @brief Default constructor.
   * 
   * This allows to create instances with `value` equal to `0` (independent
   * of the modulus). Setting `modulus` to `1` makes sure that these instances
   * will never have a value other than `0`. 
   * 
   * @param value Must be zero.
   */
  explicit mod_int(T value = 0) noexcept
      requires(MOD == 0) :
          modulus(T{1}), value(value) {
    assert(value == 0);
  }

  /**
   * @brief Constructor for run-time modulus.
   * 
   * @param value The initial value.
   * @param modulus The modulus.
   */
  mod_int(T value, T modulus) noexcept
      requires(MOD == 0) :
          modulus(modulus), value(mod(value, modulus)) {
    assert(modulus > 0);
  }

  /**
   * @brief Returns the value.
   * 
   * @return The value in range `[0,modulus)`.
   */
  [[nodiscard]]
  T get() const noexcept {
    return value;
  }

  /**
   * @brief Returns the modulus.
   * 
   * @return The modulus.
   */
  [[nodiscard]]
  T get_modulus() const noexcept {
    return modulus;
  }

  /**
   * @brief Sets the value.
   * 
   * @param v The value.
   */
  void set(T v) noexcept {
    value = mod(v, modulus);
  }

  /**
   * @brief Prefix increment.
   * 
   * @return Reference to the result (with the new value).
   */
  mod_int& operator++() {
    value = mod(value + 1, modulus);
    return *this;
  }

  /**
   * @brief Postfix increment.
   * 
   * @return A `mod_int` with the old value.
   */
  mod_int operator++(int) {
    mod_int old = *this;
    operator++();
    return old;
  }

  /**
   * @brief Prefix decrement.
   * 
   * @return Reference to the result (with the new value).
   */
  mod_int& operator--() {
    value = value ? value - 1 : modulus - 1;
    return *this;
  }

  /**
   * @brief Postfix decrement.
   * 
   * @return A `mod_int` with the old value.
   */
  mod_int operator--(int) {
    mod_int old = *this;
    operator--();
    return old;
  }

  /**
   * @brief Compound plus operator for addition.
   * 
   * @param rhs The second summand.
   * @return Reference to the result.
   */
  mod_int& operator+=(mod_int rhs) {
    value = mod(value + rhs.value, modulus);
    return *this;
  }

  /**
   * @brief Plus operator for addition.
   * 
   * @param lhs The first summand.
   * @param rhs The second summand.
   * @return The sum of `lhs` and `rhs`.
   */
  friend mod_int operator+(mod_int lhs, mod_int rhs) {
    lhs += rhs;
    return lhs;
  }

  /**
   * @brief Compound minus operator for subtraction.
   * 
   * @param rhs The subtrahend.
   * @return Reference to the result.
   */
  mod_int& operator-=(mod_int rhs) {
    // Adding `modulus` to avoid negative intermediate values.
    value = mod(value + modulus - rhs.value, modulus);
    return *this;
  }
  
  /**
   * @brief Minus operator for subtraction.
   * 
   * @param lhs The minuend.
   * @param rhs The subtrahend.
   * @return The difference of `lhs` and `rhs`.
   */
  friend mod_int operator-(mod_int lhs, mod_int rhs) {
    lhs -= rhs;
    return lhs;
  }

  /**
   * @brief Compound times operator for multiplication.
   * 
   * @param rhs The second factor.
   * @return Reference to the result.
   */
  mod_int& operator*=(mod_int rhs) {
    value = mod(value * rhs.value, modulus);
    return *this;
  }

  /**
   * @brief Times operator for multiplication.
   * 
   * @param lhs The first factor.
   * @param rhs The second factor.
   * @return The product of `lhs` and `rhs`.
   */
  friend mod_int operator*(mod_int lhs, mod_int rhs) {
    lhs *= rhs;
    return lhs;
  }

  /**
   * @brief Unary minus operator.
   * 
   * @return The "negative" value, in the range `[0,modulus)`.
   */
  mod_int operator-() const {
    return modulus - value;
  }

  /**
   * @brief Default equality comparison.
   * 
   * @param lhs The left operand.
   * @param rhs The right operand.
   * @return Whether `lhs` and `rhs` are equal.
   */
  friend bool operator==(mod_int lhs, mod_int rhs) = default;

  /**
   * @brief Computes the multiplicative inverse.
   * 
   * @return The multiplicative inverse in the range `[0,modulus)`.
   */
  [[nodiscard]]
  mod_int invert() const {
    auto result = mod_mult_inv<T,S>(value, modulus);
    if constexpr (MOD == 0) {
      return mod_int(result, modulus);
    } else {
      return mod_int(result);
    }
  }

private:
  /**
   * @brief The modulus.
   */
  T modulus;

  /**
   * @brief The value in the range `[0,modulus)`.
   */
  T value;
};

/**
 * Stream insertion operator.
 * Just prints the value in the range `[0,modulus)`.
 * 
 * @tparam T An integer-like type.
 * @tparam MOD The compile-type modulos (or `0` for run-time modulus).
 * @tparam S The signed type corresponding to `T`.
 * @param os Reference to the output stream.
 * @param obj Instance of a `mod_int`.
 * @return Reference to the output stream.
 */
export template<typename T, T MOD = 0, typename S = std::make_signed_t<T>>
std::ostream& operator<<(std::ostream &os, mod_int<T, MOD, S> obj) {
  os << obj.get();
  return os;
}

/**
 * Stream extraction operator.
 * Only available for compile-time modulus version.
 * 
 * @tparam T An integer-like type.
 * @tparam MOD The compile-type modulos (or `0` for run-time modulus).
 * @tparam S The signed type corresponding to `T`.
 * @param is Reference to the input stream.
 * @param obj Reference to the `ct_mod_int` to store the read value.
 * @return Reference to the input stream.
 */
export template<typename T, T MOD = 0, typename S = std::make_signed_t<T>>
std::istream& operator>>(std::istream &is, mod_int<T, MOD, S> &obj)
    requires(MOD != 0) {
  T tmp;
  is >> tmp;
  obj.set(tmp);
  return is;
}

/**
 * Specialization `get_multiplicative_neutral()`.
 * 
 * @tparam T An integer-like type.
 * @tparam MOD The compile-type modulos (or `0` for run-time modulus).
 * @tparam S The signed type corresponding to `T`.
 * @param obj An instance of `mod_int`.
 * @return The multiplicative unit. `1` whenever the modulus is greater than
 *         `1`, and `0` if the modulus is `1`.
 */
export template<typename T, T MOD = 0, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr
mod_int<T, MOD, S> get_multiplicative_neutral(mod_int<T, MOD, S> obj) {
  if constexpr(MOD == 0) {
    if (obj.get_modulus() > 1) {
      return mod_int<T, MOD, S>(1, obj.get_modulus());
    } else { return mod_int<T, MOD, S>{0}; }
  } else {
    if constexpr (MOD > 1) { return mod_int<T, MOD, S>{1}; }
    else { return mod_int<T, MOD, S>{0}; }
  }
}

/**
 * Returns a string representation.
 *
 * @tparam T An integer-like type.
 * @tparam MOD The compile-type modulos (or `0` for run-time modulus).
 * @tparam S The signed type corresponding to `T`.
 * @param obj An instance of `mod_int`.
 * @return String representation.
 */
export template<typename T, T MOD = 0, typename S = std::make_signed_t<T>>
std::string to_string(mod_int<T, MOD, S> obj) {
  using std::to_string;

  if constexpr(MOD == 0) {
    return to_string(obj.get()) + " (mod " + to_string(obj.get_modulus()) + ")";
  } else {
    return to_string(obj.get()) + " (mod " + to_string(MOD) + ")";
  }
}

}