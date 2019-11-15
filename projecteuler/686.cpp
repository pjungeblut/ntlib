#include <cmath>
#include <cstdint>
#include <cstdio>

int main() {
  const uint32_t n = 678910;
  uint32_t counter = 0;
  for (uint32_t i = 1; ; ++i) {
    double log = i / log2(10);
    double frac = log - floor(log);
    uint32_t digits = pow(10, frac + 2);
    if (digits == 123) {
      ++counter;
      if (counter == n) {
        printf("%d\n", i);
        return 0;
      }
    }
  }
  return 0;
}
