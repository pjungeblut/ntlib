#include <cstdint>
#include <cstdio>

#include "include/base.hpp"
#include "include/pythagorean_triple.hpp"

int main() {
  const uint32_t maxi = 100'000'000;
  const uint32_t max_perimeter = 100'000'000;
  std::vector<ntlib::triple<uint32_t, uint32_t, uint32_t>> primitives;
  ntlib::primitive_pythagorean_triples(maxi, primitives);

  uint32_t result = 0;
  for (auto &p : primitives) {
    uint32_t f = 1;
    while (f++ * (p.a + p.b + p.c) <= max_perimeter) {
      if (p.c % (p.b - p.a) == 0) result++;
    }
  }
  printf("%d\n", result);
  return 0;
}
