#include <cstdint>
#include <cstdio>
#include <map>
#include <vector>

#include "include/divisor_function.hpp"
#include "include/prime_decomposition.hpp"
#include "include/sieve_eratosthenes.hpp"

int main() {
  const uint32_t N = 28123;

  std::vector<uint32_t> primes;
  ntlib::sieve_eratosthenes_list_segmented(N, primes);

  std::vector<uint32_t> abundant;
  for (uint32_t i = 1; i <= N; ++i) {
    std::map<uint32_t, uint32_t> factors;
    ntlib::prime_decomposition_list(i, factors, primes);
    uint32_t sum_of_divisors =
        ntlib::divisor_function(factors, static_cast<uint32_t>(1)) - i;
    if (sum_of_divisors > i) abundant.push_back(i);
  }

  std::vector<bool> possible_sum(N + 1);
  for (uint32_t a : abundant) {
    for (uint32_t b : abundant) {
      if (a + b <= N) possible_sum[a + b] = true;
    }
  }

  uint32_t sum = 0;
  for (uint32_t i = 0; i <= N; ++i) {
    if (!possible_sum[i]) sum += i;
  }
  printf("%d\n", sum);

  return 0;
}
