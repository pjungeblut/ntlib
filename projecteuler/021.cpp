#include <cstdint>
#include <cstdio>
#include <vector>
#include <map>

#include "include/divisor_function.hpp"
#include "include/prime_decomposition.hpp"
#include "include/sieve_eratosthenes.hpp"

int main() {
  const uint32_t N = 10000;

  std::vector<uint32_t> primes;
  ntlib::sieve_eratosthenes_generate_segmented(N, primes);

  std::vector<uint32_t> sum_of_divisors(N + 1);
  for (uint32_t i = 1; i <= N; ++i) {
    std::map<uint32_t, uint32_t> factors;
    ntlib::prime_decomposition_list(i, factors, primes);
    sum_of_divisors[i] =
        ntlib::divisor_function(factors, static_cast<uint32_t>(1)) - i;
  }

  uint32_t amicable_sum = 0;
  for (uint32_t i = 1; i <= N; ++i) {
    if (i == sum_of_divisors[i]) continue;
    if (sum_of_divisors[sum_of_divisors[i]] == i) {
      amicable_sum += i;
    }
  }
  printf("%d\n", amicable_sum);
  return 0;
}
