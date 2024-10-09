#pragma once

#include <utility>

#include "base.hpp"

namespace ntlib {
namespace experiments {

template<typename T>
[[nodiscard]] constexpr
std::pair<T,T> odd_part_naive(T n) noexcept {
  T e{0};
  while (n != T{0} && is_even(n)) {
    n /= T{2};
    ++e;
  }
  return std::make_pair(e, n);
}

}
}