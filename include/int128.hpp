#pragma once

// Wrapper around 128 bit integers as these are non-standard.
using i128 = __int128;
using u128 = unsigned __int128;

namespace ntlib {

/**
 * Specialization for `get_multiplicative_neutral()`.
 * 
 * @param _ Element of the type.
 * @return The multiplicative neutral element (`1`).
 */
template<typename T>
[[nodiscard]] constexpr
T get_multiplicative_neutral(T)
    requires(std::is_same_v<T, i128> || std::is_same_v<T, u128>) {
  return T{1};
}

}
