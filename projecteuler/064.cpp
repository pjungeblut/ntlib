#include <cstdint>
#include <cstdio>
#include <vector>

#include "include/base.hpp"
#include "include/continued_fraction.hpp"

int main() {
  uint32_t res = 0;
  for (uint32_t i = 2; i <= 10'000; ++i) {
    if (ntlib::is_square(i)) continue;
    std::vector<uint32_t> cf;
    uint32_t period = ntlib::quadratic_irrational_cf(i, cf);
    if (period & 1) ++res;
  }
  printf("%d\n", res);
  return 0;
}
