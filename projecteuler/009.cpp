#include <cstdint>
#include <cstdio>
#include <vector>

#include "include/pythagorean_triple.hpp"

int main() {
  const uint32_t maxi = 500;
  std::vector<ntlib::triple<uint32_t, uint32_t, uint32_t>> primitives;
  ntlib::primitive_pythagorean_triples(maxi, primitives);
  for (auto &t : primitives) {
    uint32_t f = 1;
    uint32_t sum;
    do {
      sum = f * t.a + f * t.b + f * t.c;
      if (sum == 1000) {
        printf("%d\n", f * t.a * f * t.b * f * t.c);
      }
      ++f;
    } while (sum <= 1000);
  }
  return 0;
}
