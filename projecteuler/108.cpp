#include <cstdint>
#include <cstdio>
#include <map>
#include <vector>

#include "include/prime_decomposition.hpp"
#include "include/sieve_eratosthenes.hpp"

int main() {
  const uint64_t N = 100'000;
  std::vector<uint64_t> primes;
  ntlib::sieve_eratosthenes_list_segmented(N, primes);

  std::vector<uint64_t> distinct_factors(2);
  distinct_factors[0] = 0;
  distinct_factors[1] = 0;

  for (uint64_t n = 2; ; ++n) {
    // Iinitialize with one, since the last possible decomposition will not be
    // found.
    uint32_t solutions = 1;

    std::map<uint64_t, uint64_t> factors;
    ntlib::prime_decomposition_list(n, factors, primes);
    distinct_factors.push_back(factors.size());

    std::vector<uint64_t> divisors;
    ntlib::enumerate_divisors(factors, divisors);

    for (uint64_t d : divisors) solutions += 1uL << (distinct_factors[n/d] - 1);

    if (solutions >= 1'000) {
      printf("%ld\n", n);
      break;
    }
  }
  return 0;
}
