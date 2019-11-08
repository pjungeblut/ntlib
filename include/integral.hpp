#pragma once

#include <type_traits>

namespace ntlib {

/**
 * Concept for integral data types.
 * These can be used as whole numbers.
 *
 * Correctly define when g++ supports concepts.
 */
template<typename T>
concept Integral = true;

}
