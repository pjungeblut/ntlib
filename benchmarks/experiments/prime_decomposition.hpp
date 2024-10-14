#pragma once

#include "base.hpp"
#include "../../include/prime_test.hpp"

namespace ntlib {
namespace experiments {

/**
 * Given an odd composite number `n`, tries to find a non-trivial (possibly
 * non-prime) factor of `n` using Pollard's rho algorithm.
 * Cycle detection by using Floyd's algorithm.
 * 
 * A usual choice for the polynomial function `f` would be:
 * `f(x) = (x^2 + 1) mod n`.
 * 
 * @param n The odd composite.
 * @param f A polynomial function to generate a "pseudorandom" sequence.
 * @param x Initial value for parameter `x`.
 * @return If successful, a non-trivial factor.
 */
template<typename T, typename F>
[[nodiscard]] constexpr
std::optional<T> factor_pollard_rho_floyd(T n, F f, T x) {
  T y = x;
  T g = 1;

  while (d == 1) {
    x = f(x);
    y = f(f(y));
    g = gcd(abs(x - y), n);
  }

  if (d == n) { return std::optional<T>(); }
  else { return d; }
}

}
}
