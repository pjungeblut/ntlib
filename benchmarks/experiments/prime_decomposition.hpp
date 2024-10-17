#pragma once

#include "../../include/prime_list.hpp"
#include "../../include/prime_test.hpp"

import base;
import int128;
import prime_decomposition;

namespace ntlib {
namespace experiments {

template<typename T, typename F>
[[nodiscard]] constexpr
std::optional<T> find_factor_pollard_rho(T n, F f, T x) {
  assert(!is_prime(n));

  T y = x;
  T g{1};

  while (g == T{1}) {
    x = f(x);
    y = f(f(y));
    g = gcd(difference(x, y), n);
  }

  if (g == n) { return std::optional<T>(); }
  else { return g; }
}

template<typename T, typename F>
[[nodiscard]] constexpr
std::optional<T> find_factor_pollard_rho_brent(T n, F f, T x) {
  T y = x;
  T g{1};

  std::size_t i = 1;
  std::size_t k = 2;
  while (g == T{1}) {
    ++i;
    x = f(x);
    g = gcd(difference(x, y), n);
    if (i == k) {
      y = x;
      k *= 2;
    }
  }

  if (g == n) { return std::optional<T>(); }
  else { return g; }
}

template<typename T, typename F>
[[nodiscard]] constexpr
std::optional<T> find_factor_pollard_rho_cpalgos(T n, F f, T x) {
  T g{1};
  T q{1};
  T xs, y;

  std::size_t m = 128;
  std::size_t l = 1;

  while (g == T{1}) {
    y = x;
    for (std::size_t i = 1; i < l; i++) { x = f(x); }
    std::size_t k = 0;
    while (k < l && g == T{1}) {
      xs = x;
      for (std::size_t i = 0; i < m && i < l - k; ++i) {
        x = f(x);
        q = u128{q} * difference(x, y) % n;
      }
      g = gcd(q, n);
      k += m;
    }
    l *= 2;
  }

  if (g == n) {
    do {
      xs = f(xs);
      g = gcd(difference(xs, y), n);
    } while (g == T{1});
  }

  if (g == n) { return std::optional<T>(); }
  else { return g; }
}

}
}
