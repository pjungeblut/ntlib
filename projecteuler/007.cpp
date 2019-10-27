#include <cstdint>
#include <cstdio>

#include "include/sieve_eratosthenes.hpp"

int main() {
  std::vector<uint32_t> primes;
  ntlib::sieve_eratosthenes_list_segmented(105000u, primes);
  printf("%d\n", primes[10000]);
  return 0;
}
