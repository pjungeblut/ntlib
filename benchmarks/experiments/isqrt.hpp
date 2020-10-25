#include <cmath>

namespace ntlib {
namespace experiments {

int isqrt_int_floor_sqrt(int n) {
  return floor(sqrt(n));
}

// Taken from:
// https://web.archive.org/web/20120306040058/http://medialab.freaknet.org/martin/src/sqrt/sqrt.c
int isqrt_int_abacus(int n) {
  int res = 0;
  int one = 1 << 30;
  while (one > n) one /= 4;
  while (one != 0) {
    if (n >= res + one) {
      n -= res + one;
      res += 2 * one;
    }
    res /= 2;
    one /= 4;
  }
  return res;
}

// Taken from:
// https://en.wikipedia.org/wiki/Integer_square_root
int isqrt_int_newton(int n) {
  if (n < 4) return (n + 1) / 2;
  unsigned int l = (ceil_log2(n) + 1) / 2;
  unsigned int r = (1U << (l - 1)) + (n >> (l + 1));
  unsigned int r_0 = 0;
  for (int i = 0; i < 3 && abs(r - r_0) > 1; ++i) {
    r_0 = r;
    r = (r + n / r) / 2;
  }
  return r;
}

int isqrt_int_binsearch(int n) {
  int u = 1;
  do u *= 2;
  while (u * u <= n);
  int l = u / 2;

  const int THRESHOLD = 15;
  while (u - l > THRESHOLD) {
    int m = l + (u - l) / 2;
    if (m * m <= n) l = m;
    else u = m;
  }
  while (l * l <= n) ++l;
  return l - 1;
}

}
}
