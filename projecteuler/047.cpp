#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <map>

#include "include/prime_decomposition.hpp"
#include "include/sieve_eratosthenes.hpp"

int main() {
  const uint32_t N = 400;
  std::vector<uint32_t> primes;
  ntlib::sieve_eratosthenes_list_segmented(N, primes);

  uint32_t distinct_factors[4];
  for (uint32_t i = 1; ; ++i) {
    std::map<uint32_t, uint32_t> factors;
    ntlib::prime_decomposition_list(i, factors, primes);
    distinct_factors[i % 4] = factors.size();

    if (i >= 4) {
      if (std::all_of(distinct_factors, distinct_factors + 4, [](uint32_t f) {
        return f >= 4;
      })) {
        printf("%d\n", i - 3);
        return 0;
      }
    }
  }
  return 0;
}
