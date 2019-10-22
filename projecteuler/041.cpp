#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <numeric>
#include <vector>

#include "include/sieve_eratosthenes.hpp"

int main() {
  uint32_t N = 1'000'000'000;
  std::vector<bool> sieve(N + 1, true);
  ntlib::sieve_eratosthenes(N, sieve);

  uint32_t n = 9;
  while (true) {
    std::vector<uint32_t> digits(n);
    std::iota(digits.rbegin(), digits.rend(), 1);

    do {
      uint32_t val = 0;
      for (auto it = digits.begin(); it != digits.end(); ++it) val = 10 * val + *it;
      if (sieve[val]) {
        printf("%d\n", val);
        return 0;
      }
    } while (std::prev_permutation(digits.begin(), digits.end()));

    n--;
  }
  return 0;
}
