#include <cstdint>
#include <cstdio>

#include "include/base.hpp"

int main() {
  __uint128_t sum = 0;
  for (__uint128_t a = 2; 3 * a - 1 <= 1'000'000'000; ++a) {
    __uint128_t s2 = 3 * a - 1;
    __uint128_t inner = s2 * (s2 - 2 * a) * (s2 - 2 * a) * (s2 - 2 * (a - 1));
    if (ntlib::is_square(inner)) {
      sum += 3 * a - 1;
    }

    s2 = 3 * a + 1;
    inner = s2 * (s2 - 2 * a) * (s2 - 2 * a) * (s2 - 2 * (a + 1));
    if (ntlib::is_square(inner)) {
      sum += 3 * a + 1;
    }
  }
  printf("%ld\n", static_cast<uint64_t>(sum));
  return 0;
}
