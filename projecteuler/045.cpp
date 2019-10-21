#include <cstdint>
#include <cstdio>

#include "include/polygonal_number.hpp"

int main() {
  uint64_t t = 286;
  uint64_t p = 166;
  uint64_t h = 144;
  while (true) {
    uint64_t tn = ntlib::triangle_number(t);
    uint64_t pn = ntlib::pentagonal_number(p);
    uint64_t hn = ntlib::hexagonal_number(h);

    if (tn == pn && tn == hn) {
      printf("%ld\n", tn);
      return 0;
    }

    if (tn <= pn && tn <= hn) ++t;
    else if (pn <= tn && pn <= hn) ++p;
    else if (hn <= tn && hn <= pn) ++h;
  }
  return 0;
}
