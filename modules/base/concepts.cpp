/**
 * @file
 * @brief Module interface unit for module `base`, partition `concepts`.
 */
module;

#include <concepts>

export module base:concepts;

namespace ntlib {

/**
 * @brief Traits class for algebraic types.
 * 
 * This empty class is the primary template definition. Concepts defined by
 * NTLib use this traits class to determine algebraic properties of types.
 * 
 * All algebraic types defined by NTLib specialize this class appropriately.
 * To be able to use a user-defined type `T` with NTLib, you need to provide a
 * specialization of `ntlib::algebra_traits<T>`.
 * 
 * A specialization must contain the following static member functions:
 *   - `T get_zero()`: Returns the additive neutral element.
 *   - `T get_one()`: Returns the multiplicative neutral element.
 * 
 * A specialization must contain the following static member constants:
 *   - `bool is_additive_commutative`: Whether the addition operation is
 *     commutative.
 *   - `bool is_multiplicative_commutative`: Whether the multiplication
 *     operation is commutative.
 */
export template<typename>
class algebra_traits {};

/**
 * @brief Specialization of `ntlib::algebra_traits` for integral types.
 * 
 * @tparam T An integral type.
 */
export template<std::integral T>
class algebra_traits<T> {
public:
  /**
   * @brief Returns the additive neutral element of `T`.
   * 
   * @return The additive neutral element, i.e., \f$0\f$.
   */
  [[nodiscard]] static constexpr T get_zero() noexcept {
    return T{0};
  }

  /**
   * @brief Returns the multiplicative neutral element of `T`.
   * 
   * @return The multiplicative neutral element, i.e., \f$1\f$.
   */
  [[nodiscard]] static constexpr T get_one() noexcept {
    return T{1};
  }

  /// @brief Integral addition is commutative.
  static constexpr bool is_additive_commutative = true;

  /// @brief Integral multiplication is commutative.
  static constexpr bool is_multiplicative_commutative = true;
};

/**
 * @brief Returns the additive neutral element of a given type.
 * 
 * @tparam T The type. Must have a specialization of `ntlib::algebra_traits`
 *     with a static `get_zero` method.
 * @return The additive neutral element of type `T`.
 */
export template<typename T>
    requires requires { algebra_traits<T>::get_zero(); }
[[nodiscard]] constexpr
T zero() noexcept {
  return algebra_traits<T>::get_zero();
}

/**
 * @brief Returns the multiplicative neutral element of a given type.
 * 
 * @tparam T The type. Must have a specialization of `ntlib::algebra_traits`
 *     with a static `get_one` method.
 * @return The multiplicative neutral element of type `T`.
 */
export template<typename T>
    requires requires { algebra_traits<T>::get_one(); }
[[nodiscard]] constexpr
T one() noexcept {
  return algebra_traits<T>::get_one();
}

/**
 * @concept HasAdditiveNeutral
 * @brief Restricts to types with an additive neutral element.
 * 
 * @note The least restrictive (additive) concept in NTLib requiring an additive
 *     neutral element is `AdditiveMonoid`. Therefore, the following can be
 *     assumed to hold:
 *     - The addtive neutral element is unique.
 *     - The unique additive neutral element is left- and right-neutral.
 * 
 * @tparam T A set with an inner addition operation.
 */
export template<typename T>
concept HasAdditiveNeutral = requires(T n) {
  // Existence.
  { ntlib::zero<T>() } -> std::convertible_to<T>;
};

/**
 * @concept HasMultiplicativeNeutral
 * @brief Restricts to types with a multiplicative neutral element.
 * 
 * @note The least restrictive (multiplicative) concept in NTLib requiring a
 *     multiplicative neutral element is `MultiplicativeMonoid`. Therefore, the
 *     following can be assumed to hold:
 *     - The multiplicative neutral element is unique.
 *     - The unique multiplicative neutral element is left- and right-neutral.
 * 
 * @tparam T A set with an inner multiplication operation.
 */
export template<typename T>
concept HasMultiplicativeNeutral = requires(T n) {
  // Existence.
  { ntlib::one<T>() } -> std::convertible_to<T>;
};

/**
 * @concept AdditiveSemigroup
 * @brief Restricts to types modeling an additive semigroup.
 * 
 * @tparam T A set with an associative inner addition operation.
 */
export template<typename T>
concept AdditiveSemigroup = requires(T a, T b) {
  // Require that `operator+(T,T)` is overloaded.
  { a + b } -> std::convertible_to<T>;
};

/**
 * @concept MultiplicativeSemigroup
 * @brief Restricts to types modeling a multiplication semigroup
 * 
 * @tparam T A set with an associative inner multiplication operation.
 */
export template<typename T>
concept MultiplicativeSemigroup = requires(T a, T b) {
  // Require that `operator*(T,T)` is overloaded.
  { a * b } -> std::convertible_to<T>;
};

/**
 * @concept AdditiveMonoid
 * @brief Restricts to types modeling an additive monoid.
 * 
 * @tparam T A set with an associative inner addition operation and an
 *     additive neutral element.
 */
export template<typename T>
concept AdditiveMonoid = HasAdditiveNeutral<T> && AdditiveSemigroup<T>;

/**
 * @concept MultiplicativeMonoid
 * @brief Restricts to types modeling a multiplicative monoid.
 * 
 * @tparam T A set with an associative inner multiplication operation and a
 *     multiplicative neutral element.
 */
export template<typename T>
concept MultiplicativeMonoid =
    HasMultiplicativeNeutral<T> && MultiplicativeSemigroup<T>;

/**
 * @concept HasAdditiveInverse
 * @brief Restricts to types with additive inverse elements.
 * 
 * @note The least restrictive (additive) concept in NTLib requiring a additive
 *     inverse elements is `AdditiveGroup`. Therefore, the following can be
 *     assumed to hold:
 *     - The addtive inverse element are unique.
 *     - The unique additive inverse elements are left- and right-inverse.
 * 
 * @tparam T A set with an inner additive operation.
 */
export template<typename T>
concept HasAdditiveInverses = requires(T n) {
  // Require that `operator-(T)` is overloaded.
  { -n } -> std::convertible_to<T>;

  // Require that `operator-(T,T)` is overloaded.
  { n - n } -> std::convertible_to<T>;
};

/**
 * @concept HasMultiplicativeInverses
 * @brief Restricts to types with multiplicative inverse elements.
 * 
 * @note The least restrictive (multiplicative) concept in NTLib requiring
 *     multiplicative inverse elements is `MultiplicativeGroup`. Therefore, the
 *     following can be assumed to hold:
 *     - The multiplicative inverse elements are unique.
 *     - The unique multiplicative inverse elements are left- and right-inverse.
 *
 * @tparam T A set with an inner multiplication operation.
 */
export template<typename T>
concept HasMultiplicativeInverses = requires(T a, T b) {
  // Require that `operator/(T,T)` is overloaded.
  { a / b } -> std::convertible_to<T>;
};

/**
 * @concept AdditiveGroup
 * @brief Restricts to types modeling an additive group.
 * 
 * @tparam T A set with an associative inner addition operation, an additive
 *     neutral element, and additive inverse elements.
 */
export template<typename T>
concept AdditiveGroup = AdditiveMonoid<T> && HasAdditiveInverses<T>;

/**
 * @concept MultiplicativeGroup
 * @brief Restricts to types modeling a multiplicative group.
 * 
 * @tparam T A set with an associative inner multiplication operation, a
 *     multiplicative neutral element, and multiplicative inverse elements (for
 *     all non-zero elements).
 */
export template<typename T>
concept MultiplicativeGroup =
    MultiplicativeMonoid<T> && HasMultiplicativeInverses<T>;

/**
 * @concept IsAdditiveCommutative
 * @brief Restricts to types with a commutative addition operation.
 * 
 * @tparam T A set with a commutative inner addition operation.
 */
export template<typename T>
concept IsAdditiveCommutative = requires() {
  // Commutativity.
  // axiom(T a, T b) {
  //   a + b == b + a;
  // }
  ntlib::algebra_traits<T>::is_additive_commutative;
};

/**
 * @concept IsMultiplicativeCommutative
 * @brief Restricts to types with a commutative multiplication operation.
 * 
 * @tparam T A set with a commutative inner multiplication operation.
 */
export template<typename T>
concept IsMultiplicativeCommutative = requires() {
  // Commutativity.
  // axiom(T a, T b) {
  //   a * b == b * a;
  // }
  ntlib::algebra_traits<T>::is_multiplicative_commutative;
};

/**
 * @concept AdditiveCommutativeGroup
 * @brief Restricts to types modeling an additive commutative group.
 * 
 * @tparam T A set with an associative and commutative inner addition operation,
 *     an additive neutral element, and a additive inverse elements.
 */
export template<typename T>
concept AdditiveCommutativeGroup = AdditiveGroup<T> && IsAdditiveCommutative<T>;

/**
 * @concept MultiplicativeCommutativeGroup
 * @brief Restricts to types modeling a multiplicative commutative group.
 * 
 * @tparam T A set with an associative and commutative inner multiplication
 *     operation, a multiplicative neutral element, and multiplicative inverse
 *     elements (for all non-zero elements).
 */
export template<typename T>
concept MultiplicativeCommutativeGroup =
    MultiplicativeGroup<T> && IsMultiplicativeCommutative<T>;

/**
 * @concept Ring
 * @brief Restricts to types modeling a ring.
 * 
 * @note In NTLib, rings do always have a multiplicative neutral element.
 * 
 * @tparam T A set that acts as an additive commutative group and a
 *     multiplicative monoid, and where multiplication is distributive over
 *     addition.
 */
export template<typename T>
concept Ring = AdditiveCommutativeGroup<T> && MultiplicativeMonoid<T>;

/**
 * @concept CommutativeRing
 * @brief Restricts to types modeling a commutative ring.
 * 
 * @tparam T A set that acts as a ring and where multiplication is commutative.
 */
export template<typename T>
concept CommutativeRing = Ring<T> && requires {
  ntlib::algebra_traits<T>::is_multiplicative_commutative;
};

/**
 * @concept Field
 * @brief Restricts to types modeling a field.
 * 
 * @tparam T A set that acts as a commutative ring and a multiplicative
 *     commutative group (for all non-zero elements).
 */
export template<typename T>
concept Field =
    AdditiveCommutativeGroup<T> && MultiplicativeCommutativeGroup<T>;

/**
 * @concept HasDivisionWithRemainder
 * @brief Restricts to types supporting division with remainder.
 * 
 * @tparam T A set that supports division with remainder.
 */
export template<typename T>
concept HasDivisionWithRemainder = requires(T a, T b) {
  { a / b } -> std::convertible_to<T>;
  { a % b } -> std::convertible_to<T>;
};

/**
 * @concept Integer
 * @brief Restricts to types modeling the integers.
 * 
 * @note Strictly speaking, this concept restricts only to ordered Euclidean
 *     rings as there is currently no need for NTLib to distinguish those and
 *     integers. If this changes in the future, then new intermediate concepts
 *     could be added while this concept can be refined.
 *
 * @note To better model the integers, the following could be considered:
 *     - Exclude rings with nonzero zero-divisors. This would require another
 *       Boolean customization point in `ntlib::algebra_traits`.
 *       Then, this concept would restrict to integral domains.
 *
 * @tparam T A set that acts as an ordered Euclidean ring..
 */
export template<typename T>
concept Integer =
    Ring<T> && HasDivisionWithRemainder<T> && std::totally_ordered<T>;

} // namespace ntlib
