#pragma once

#include <type_traits>

namespace ntlib {

// Wrapper around 128 bit integers as these are non-standard.
using i128 = __int128;
using u128 = unsigned __int128;

/**
 * Concept for 128 bit integers.
 */
template<typename T>
concept Int128 = std::is_same_v<T, i128> || std::is_same_v<T, u128>;

/**
 * Specialization for `get_multiplicative_neutral()`.
 * 
 * @param _ Element of the type.
 * @return The multiplicative neutral element (`1`).
 */
template<Int128 T>
[[nodiscard]] constexpr
T get_multiplicative_neutral(T) {
  return T{1};
}

}
