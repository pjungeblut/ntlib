#include <cstdio>
#include <cstdint>
#include <typeinfo>

#include "include/base.hpp"

int main() {
  __int128_t a = 15;
  __int128_t b = 10;
  auto gcd = ntlib::gcd(a, b);
  printf("GCD(15,10) = %ld\n", static_cast<int64_t>(gcd));

  a = 15;
  b = -10;
  auto gcd2 = ntlib::gcd(a, b);
  printf("GCD(15,-10) = %ld\n", static_cast<int64_t>(gcd2));

  uint64_t lcm = 1;
  for (uint64_t n = 1; n <= 20; ++n) {
    lcm = ntlib::lcm(lcm, n);
  }
  printf("LCM(1..20) = %ld\n", lcm);

  int64_t lcm2 = 1;
  for (int64_t n = 3; n >= -1; --n) {
    if (n == 0) continue;
    lcm2 = ntlib::lcm(lcm2, n);
  }
  printf("LCM(-3,-2,-1,1,2,3) = %ld\n", lcm2);

  {
    int32_t a = -2;
    int32_t b = 3;
    auto lcm = ntlib::lcm(a, b);
    printf("LCM(%d,%d) = %u\n", a, b, lcm);
  }

  return 0;
}
