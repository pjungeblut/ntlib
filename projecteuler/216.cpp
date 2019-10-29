#include <cstdint>
#include <cstdio>

#include "include/prime_test.hpp"

const uint64_t N = 50'000'000;
uint64_t candidates[N + 1];

int main() {
  uint32_t counter = 0;
  for (uint64_t i = 1; i <= N; ++i) candidates[i] = 2 * i * i - 1;
  for (uint64_t i = 2; i <= N; ++i) {
    uint64_t p = candidates[i];
    if (p == 1) continue;
    if (p == 2 * i * i - 1) ++counter;
    for (uint64_t j = 0; j * p + i <= N; ++j) {
      uint64_t m = j * p + i;
      while (candidates[m] % p == 0) candidates[m] /= p;
    }
    for (uint64_t j = 1; j * p - i <= N; ++j) {
      uint64_t m = j * p - i;
      while (candidates[m] % p == 0) candidates[m] /= p;
    }
  }
  printf("%d\n", counter);
  return 0;
}
