#pragma once

#include <type_traits>

namespace ntlib {

/**
 * Simple concept for integral data types.
 */
 template <typename T>
 concept Integral = std::is_integral<T>::value ||
    std::is_same<T, __int128_t>::value ||
    std::is_same<T, __uint128_t>::value;
}
