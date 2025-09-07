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

import base;

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
concept Int128 =
    std::is_same_v<std::remove_cvref_t<T>, i128> ||
    std::is_same_v<std::remove_cvref_t<T>, u128>;

} // namespace ntlib
