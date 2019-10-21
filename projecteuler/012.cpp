#include <cstdint>
#include <cstdio>
#include <vector>

#include "include/divisor_function.hpp"
#include "include/prime_decomposition.hpp"
#include "include/sieve_eratosthenes.hpp"

int main() {
  std::vector<uint32_t> primes;
  ntlib::sieve_eratosthenes_generate_segmented(1000, primes);

  uint32_t i = 2;
  while (true) {
    std::map<uint32_t, uint32_t> factors_current, factors_next;
    ntlib::prime_decomposition_list(i, factors_current, primes);
    ntlib::prime_decomposition_list(i + 1, factors_next, primes);

    uint32_t divisors = 1;
    if (i & 1) factors_next[2]--;
    else factors_current[2]--;
    divisors *= ntlib::count_divisors(factors_current);
    divisors *= ntlib::count_divisors(factors_next);

    if (divisors > 500) {
      printf("%d\n", i * (i + 1) / 2);
      break;
    }

    ++i;
  }
  return 0;
}
