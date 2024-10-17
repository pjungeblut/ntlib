#pragma once

import base;

namespace ntlib {
namespace experiments {

template<typename T>
bool is_square_newton(T n) {
  T x = 1, last = 0;
  while (x != last) {
    last = x;
    x = x - (x * x - n) / (2 * x);
  }
  return x * x == n;
}

template<typename T>
bool is_square_isqrt(T n) {
  const T iroot = isqrt(n);
  return iroot * iroot == n;
}

}
}
