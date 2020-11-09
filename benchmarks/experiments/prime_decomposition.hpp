#pragma once

#include "base.hpp"
#include "../../include/prime_test.hpp"

namespace ntlib {
namespace experiments {

/**
 * Pollard's Rho algorithm to compute the prime decomposition of a given number.
 *
 * @param n The number to decompose.
 * @param factors The prime factors.
 */
template<typename T>
void pollard_rho(T n, std::map<T, T> &factors) {
  const std::function<T(T)> rho = [&rho](T k) {
    if (~k & 1) return static_cast<T>(2);
    T x = rand() % k;
    T y = x;
    T d = 1;
    while (d == 1) {
      x = (x * x + 1) % k;
      y = (y * y + 1) % k;
      y = (y * y + 1) % k;
      d = ntlib::gcd(x - y, k);
    }
    return d == k ? rho(k) : d;
  };

  if (n <= 1) return;
  if (is_prime(n)) {
    ++factors[n];
    return;
  }
  T f = rho(n);
  pollard_rho(n / f, factors);
  pollard_rho(f, factors);
}

}
}
