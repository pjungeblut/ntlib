/**
 * @file
 * @brief Primary module interface unit for module `int128`.
 */
module;

#include <type_traits>

/**
 * @module int128
 * @brief 128-bit integers.
 * 
 * Currently, these are just wrappers around the (non-standard) built-in 128-bit
 * integers supported by Clang and GCC.
 */
export module int128;

namespace ntlib {

/**
 * @brief Wrapper around (non-standard) built-in 128-bit signed integer.
 */
export using i128 = __int128;

/**
 * @brief Wrapper around (non-standard) built-in 128-bit unsigned integer.
 */
export using u128 = unsigned __int128;

/**
 * @brief Concept for signed and unsigned NTLib 128 bit integers.
 * 
 * @tparam T An integer-like type.
 */
export template<typename T>
concept Int128 = std::is_same_v<T, i128> || std::is_same_v<T, u128>;

/**
 * Specialization for `get_multiplicative_neutral()`.
 * 
 * @return The multiplicative neutral element (`1`).
 */
export template<Int128 T>
[[nodiscard]] constexpr
T get_multiplicative_neutral(T) noexcept {
  return T{1};
}

}