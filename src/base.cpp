#include <cstdio>
#include <cstdint>
#include <typeinfo>

#include "base.hpp"

int main() {
  {
    __int128_t a = 15;
    __int128_t b = 10;
    auto gcd = ntlib::gcd(a, b);
    printf("GCD(15,10) = %ld\n", static_cast<uint64_t>(gcd));
  }

  {
    int64_t a = 15;
    int64_t b = -10;
    auto gcd = ntlib::gcd(a, b);
    printf("GCD(15,-10) = %ld\n", gcd);
  }

  {
    uint64_t lcm = 1;
    for (uint64_t n = 1; n <= 20; ++n) {
      lcm = ntlib::lcm(lcm, n);
    }
    printf("LCM(1..20) = %ld\n", lcm);
  }

  {
    int64_t lcm = 1;
    for (int64_t n = 3; n >= -1; --n) {
      if (n == 0) continue;
      lcm = ntlib::lcm(lcm, n);
    }
    printf("LCM(-3,-2,-1,1,2,3) = %ld\n", lcm);
  }

  {
    int32_t a = -2;
    int32_t b = 3;
    auto lcm = ntlib::lcm(a, b);
    printf("LCM(%d,%d) = %u\n", a, b, lcm);
  }

  {
    uint32_t m = 500;
    for (uint32_t n = 1; n < m; ++n) {
      if (ntlib::gcd(n, m) == 1) {
        uint32_t inv = ntlib::mod_mult_inv(n, m);
        if (n * inv % m != 1) printf("Wrong multiplicative inverse for %d (mod %d). Got %d.\n", n, m, inv);
      }
    }
    printf("All multiplicative inverses in Z/500Z correct.\n");
  }

  {
    uint32_t m = 509; // Prime and 509 mod 4 = 1, so no shortcut.
    for (uint32_t n = 1; n < m; ++n) {
      if (ntlib::mod_is_square(n, m)) {
        uint32_t x = ntlib::mod_sqrt(n, m);
        if (x * x % m != n) printf("Wrong modular root for %d (mod %d). Got %d.\n", n, m, x);
      }
    }
    printf("All modular roots in Z/509Z correct.\n");
  }

  return 0;
}
