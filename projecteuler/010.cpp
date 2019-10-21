#include <cstdint>
#include <cstdio>

#include "include/sieve_eratosthenes.hpp"

int main() {
  std::vector<uint32_t> primes;
  ntlib::sieve_eratosthenes_generate_segmented(2'000'000, primes);
  uint64_t sum = 0;
  for (uint32_t p : primes) sum += p;
  printf("%ld\n", sum);
  return 0;
}
