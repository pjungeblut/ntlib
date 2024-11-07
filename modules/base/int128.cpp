module;

#include <type_traits>

export module int128;

namespace ntlib {

// Wrapper around 128 bit integers as these are non-standard.
export using i128 = __int128;
export using u128 = unsigned __int128;

/**
 * Concept for 128 bit integers.
 */
export template<typename T>
concept Int128 = std::is_same_v<T, i128> || std::is_same_v<T, u128>;

/**
 * Specialization for `get_multiplicative_neutral()`.
 * 
 * @param _ Element of the type.
 * @return The multiplicative neutral element (`1`).
 */
export template<Int128 T>
[[nodiscard]] constexpr
T get_multiplicative_neutral(T) noexcept {
  return T{1};
}

}