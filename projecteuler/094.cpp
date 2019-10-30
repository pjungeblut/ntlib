#include <cstdint>
#include <cstdio>

#include "include/base.hpp"
#include "include/pell_equation.hpp"

int main() {
  const uint64_t max_perimeter = 1'000'000'000;
  const uint64_t d = 3;
  ntlib::tuple<uint64_t, uint64_t> initial = ntlib::min_pell_solution(d);
  ntlib::tuple<uint64_t, uint64_t> current = initial;
  uint64_t sum = 0;
  while (true) {
    // Case 1: (a, a, a - 1)
    uint64_t a3 = 2 * current.a - 1;
    if (a3 - 1 > max_perimeter) break;
    uint64_t area3 = current.b * (a3 - 3) / 2;
    if (a3 > 0 && area3 > 0 && a3 % 3 == 0 && area3 % 3 == 0) sum += a3 - 1;

    // Case 2: (a, a, a + 1)
    a3 = 2 * current.a + 1;
    if (a3 + 1 > max_perimeter) break;
    area3 = current.b * (a3 + 3) / 2;
    if (a3 > 0 && area3 > 0 && a3 % 3 == 0 && area3 % 3 == 0) sum += a3 + 1;

    current = ntlib::next_pell_solution(d, initial, current);
  }
  printf("%ld\n", sum);
  return 0;
}
