module;

#include <algorithm>
#include <numeric>
#include <vector>

export module euler_totient;

import base;
import prime_decomposition;

namespace ntlib {

/**
 * Computes Euler's totient function `phi(n)` for a given number `n`, i.e., the
 * number of integers up to `n` coprime to `n`.
 * 
 * @param factors The prime decomposition of `n`.
 * @return Euler's totient function `phi(n)`.
 */
export template<typename T>
[[nodiscard]] constexpr
T euler_totient(const std::vector<prime_power<T>> &factors) {
  return std::ranges::fold_left(factors, T{1}, [](T prod, prime_power<T> pp) {
    return prod * pow(pp.p, pp.e - 1) * (pp.p - 1);
  });
}

/**
 * Computes Euler's totient function `phi(n)` for a given number `n`, i.e., the
 * number of integers up to `n` coprime to `n`.
 * 
 * @param n The given number `n`.
 * @return Euler's totient function `phi(n)`.
 */
export template<typename T>
[[nodiscard]] constexpr
T euler_totient(T n) {
  return euler_totient(prime_decomposition(n));
}

/**
 * Computes Euler's totient function `phi(n)` for all integers smaller than a
 * given number `N`.
 * 
 * Runtime: O(N log log N)
 *
 * @param N The number until which all values should be computed.
 * @return A `std::vector<T>` containing all values. The element at index `0`
 *     will be set to `0`.
 */
export template<typename T>
[[nodiscard]]
std::vector<T> euler_totient_sieve(T N) noexcept {
  std::vector<T> sieve(N + 1);
  std::ranges::iota(sieve, 0);
  for (T i{2}; i <= N; ++i) {
    if (sieve[i] == i) {
      for (T j = i; j <= N; j += i) {
        sieve[j] /= i;
        sieve[j] *= i - 1;
      }
    }
  }
  return sieve;
}

}