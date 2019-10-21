#include <cstdio>
#include <cstdint>

#include "include/base.hpp"

int main() {
  using NumberType = uint64_t;

  NumberType lcm = 1;
  for (NumberType n = 1; n <= 20; ++n) {
    lcm = ntlib::lcm(lcm, n);
  }
  printf("LCM(1..20) = %ld\n", lcm);

  return 0;
}
