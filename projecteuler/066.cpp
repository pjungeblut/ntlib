#include <cstdint>
#include <cstdio>
#include <vector>

#include "include/base.hpp"
#include "include/big_integer.hpp"
#include "include/pell_equation.hpp"

int main() {
  ntlib::big_integer max_d = 0;
  ntlib::big_integer max_x = 0;
  for (ntlib::big_integer d = 2; d <= 1'000; ++d) {
    if (ntlib::is_square(d)) continue;

    auto sol = ntlib::min_pell_solution(d);
    if (sol.a > max_x) {
      max_x = sol.a;
      max_d = d;
    }
  }
  printf("%s\n", max_d.to_string().c_str());

  return 0;
}
