#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <vector>

#include "include/pythagorean_triple.hpp"

int main() {
  const uint32_t maxi = 1000;
  std::vector<ntlib::triple<uint32_t, uint32_t, uint32_t>> primitives;
  ntlib::primitive_pythagorean_triples(maxi, primitives);
  std::vector<uint32_t> counter(1001, 0);
  for (auto &[a, b, c] : primitives) {
    uint32_t f = 1;
    while (f * (a + b + c) <= 1000) {
      counter[f * (a + b + c)]++;
      f++;
    }
  }
  printf("%ld\n", std::max_element(counter.begin(), counter.end()) - counter.begin());
  return 0;
}
