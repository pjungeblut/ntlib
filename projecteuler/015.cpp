#include <cstdint>
#include <cstdio>

#include "include/binomial_coefficient.hpp"

int main() {
  uint64_t n = 40;
  uint64_t k = 20;
  printf("%ld\n", ntlib::binom(n, k));
  return 0;
}
