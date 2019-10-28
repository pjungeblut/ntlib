#pragma once

#include <type_traits>

namespace ntlib {

/**
 * Simple concept for integral data types.
 */
template<typename T>
concept bool Integral() {
  return std::is_integral<T>::value;
}

}
