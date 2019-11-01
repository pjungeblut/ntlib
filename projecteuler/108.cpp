#include <cstdint>
#include <cstdio>
#include <map>

#include "include/prime_decomposition.hpp"
#include "include/sieve_eratosthenes.hpp"

int main() {
  std::vector<uint64_t> primes = {2, 3, 5, 7, 11, 13, 17, 19};

  for (uint64_t n = 2; ; ++n) {
    std::map<uint64_t, uint64_t> factors;
    ntlib::prime_decomposition_list(n, factors, primes);

    uint64_t solutions = 1;
    for (auto &[f, m] : factors) solutions *= 2 * m + 1;
    solutions /= 2;

    if (solutions >= 1'000) {
      printf("%ld\n", n);
      return 0;
    }
  }
  return 0;
}
