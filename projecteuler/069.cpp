#include <cstdint>
#include <cstdio>
#include <vector>

#include "include/euler_totient.hpp"

int main() {
  uint32_t N = 1'000'000;
  std::vector<uint32_t> totient;
  ntlib::sieve_phi(N, totient);

  double max_val = 0;
  uint32_t max_idx = 0;
  for (uint32_t i = 1; i <= N; ++i) {
    double val = (double)i / totient[i];
    if (val > max_val) {
      max_val = val;
      max_idx = i;
    }
  }
  printf("%d\n", max_idx);
  return 0;
}
