#include <cstdint>
#include <cstdio>
#include <vector>

#include "include/base.hpp"
#include "include/pell_equation.hpp"

int main() {
  __uint128_t max_d = 0;
  __uint128_t max_x = 0;
  for (__uint128_t d = 2; d <= 1'000; ++d) {
    if (ntlib::is_square(d)) continue;

    auto sol = ntlib::min_pell_solution(d);
    if (sol.a > max_x) {
      max_x = sol.a;
      max_d = d;
    }
  }
  printf("%d\n", static_cast<uint32_t>(max_d));

  return 0;
}
