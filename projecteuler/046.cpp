#include <cstdint>
#include <cstdio>
#include <vector>

#include "include/base.hpp"
#include "include/sieve_eratosthenes.hpp"

int main() {
  const uint32_t N = 10000;
  std::vector<bool> sieve(N + 1, true);
  std::vector<uint32_t> primes;
  ntlib::sieve_eratosthenes_generate(N, sieve, primes);

  for (uint32_t i = 9; ; i += 2) {
    for (uint32_t p : primes) {
      if (p > i) {
        printf("%d\n", i);
        return 0;
      }
      if (!((i - p) & 1) && ntlib::is_square((i - p) / 2)) break;
    }
  }
  return 0;
}
