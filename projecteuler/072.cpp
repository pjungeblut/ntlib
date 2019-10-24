#include <cstdint>
#include <cstdio>
#include <vector>

#include "include/euler_totient.hpp"

int main() {
  uint32_t N = 1'000'000;
  std::vector<uint32_t> phi;
  ntlib::sieve_phi(N, phi);

  uint64_t sum = 0;
  for (uint32_t i = 2; i <= N; ++i) sum += phi[i];
  printf("%ld\n", sum);
  return 0;
}
