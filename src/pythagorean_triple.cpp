#include <cstdint>
#include <cstdio>
#include <vector>

#include "include/pythagorean_triple.hpp"

int main() {
  using NumberType = uint64_t;

  // Project Euler, Problem 9:
  // https://projecteuler.net/problem=9
  const NumberType maxi = 500;
  std::vector<ntlib::triple<NumberType, NumberType, NumberType>> primitives;
  ntlib::primitive_pythagorean_triples(maxi, primitives);
  for (auto &t : primitives) {
    NumberType f = 1;
    NumberType sum;
    do {
      sum = f * t.a + f * t.b + f * t.c;
      if (sum == 1000) {
        printf("(%ld, %ld, %ld) = %ld * (%ld, %ld, %ld)\n",
            f * t.a, f * t.b, f * t.c, f, t.a, t.b, t.c);
      }
      ++f;
    } while (sum <= 1000);
  }
  return 0;
}
