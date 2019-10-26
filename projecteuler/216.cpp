#include <cstdint>
#include <cstdio>

#include "include/prime_test.hpp"

int main() {
  uint32_t counter = 0;
  for (uint64_t n = 2; n <= 50'000'000; ++n) {
    uint64_t test = 2 * n * n - 1;
    if (ntlib::is_prime_miller_rabin(test)) counter++;
  }
  printf("%d\n", counter);
  return 0;
}
