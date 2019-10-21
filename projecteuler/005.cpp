#include <cstdio>
#include <cstdint>

#include "include/base.hpp"

int main() {uint32_t lcm = 1;
  for (uint32_t n = 1; n <= 20; ++n) lcm = ntlib::lcm(lcm, n);
  printf("%d\n", lcm);

  return 0;
}
