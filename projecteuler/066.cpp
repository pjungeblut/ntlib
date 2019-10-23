#include <cstdint>
#include <cstdio>
#include <vector>

#include "include/base.hpp"
#include "include/continued_fraction.hpp"
#include "include/rational.hpp"

int main() {
  __uint128_t max_d = 0;
  __uint128_t max_x = 0;
  for (__uint128_t d = 2; d <= 1'000; ++d) {
    if (ntlib::is_square(d)) continue;

    std::vector<__uint128_t> cf;
    __uint128_t period = ntlib::quadratic_irrational_cf(d, cf);
    __uint128_t n = period & 1 ? 2 * period - 1 : period - 1;
    ntlib::rational<__uint128_t> pq = ntlib::nth_convergent_quadratic_irrational_cf(n, cf);
    if (pq.numerator > max_x) {
      max_x = pq.numerator;
      max_d = d;
    }
  }
  printf("%d\n", static_cast<uint32_t>(max_d));

  return 0;
}
