#include <cstdint>
#include <cstdio>
#include <vector>

#include "include/sieve_eratosthenes.hpp"

int main() {
  const uint32_t N = 1'000'000;

  std::vector<bool> sieve(N + 1, 1);
  ntlib::sieve_eratosthenes_classic(N, sieve);

  int32_t maxn = 0;
  int32_t res = 0;
  for (int32_t a = -999; a <= 999; ++a) {
    for (int32_t b = -1000; b <= 1000; ++b) {
      for (int32_t n = 0; ; ++n) {
        int32_t val = n * n + a * n + b;
        if (2 <= val && val <= 1000 && sieve[val]) {
          if (n > maxn) {
            maxn = n;
            res = a * b;
          }
        } else break;
      }
    }
  }
  printf("%d\n", res);

  return 0;
}
