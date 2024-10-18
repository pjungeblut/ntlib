module;

#include <vector>

export module euler_totient;

namespace ntlib {

/**
 * Computes the value of Euler's totient function phi(i) for all numbers n
 * in [1..N].
 * Runtime: O(N log log N)
 *
 * @param N The number until which all values should be computed.
 * @param sieve The sieve to put the values into.
 */
export template<typename T>
void sieve_phi(T N, std::vector<T> &sieve) {
  sieve.resize(N + 1);
  for (T i = 1; i <= N; ++i) sieve[i] = i;
  for (T i = 2; i <= N; ++i) {
    if (sieve[i] == i) {
      for (T j = i; j <= N; j += i) {
        sieve[j] /= i;
        sieve[j] *= i - 1;
      }
    }
  }
}

}