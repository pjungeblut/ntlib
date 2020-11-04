#pragma once

#include <vector>

#include "base.hpp"
#include "../../include/prime_generation.hpp"

namespace ntlib {
namespace experiments {

/**
 * Prime number test, trial division with prime list.
 *
 * @param n The number to test for primality.
 * @return Whether `n` is prime.
 */
template<typename T>
bool is_prime_list(const T &n) {
  std::vector<T> primes;
  ntlib::eratosthenes_list(ntlib::isqrt(n) + 1, primes);
  for (const T &p : primes) {
    if (n == p) return true;
    else if (n % p == 0) return false;
  }
  return true;
}

}
}
