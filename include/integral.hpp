#pragma once

#include <type_traits>

namespace ntlib {

/**
 * Concept for integral data types.
 * These can be used as whole numbers.
 */
template<typename T>
concept Integral = std::is_integral<T>::value;

/**
 * Concept for unsigned integral data types.
 * These can be used as natural numbers.
 */
template<typename T>
concept UnsignedIntegral = std::is_integral<T>::value &&
    std::is_unsigned<T>::value;

}
