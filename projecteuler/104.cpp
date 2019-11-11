#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>

bool pandigital(const std::string &s) {
  if (s.size() < 9) return false;

  bool found[10] = {false};
  for (std::size_t i = 0; i < 9; ++i) {
    found[s[i] - '0'] = true;
  }
  for (std::size_t i = 1; i <= 9; ++i) {
    if (!found[i]) return false;
  }
  return true;
}

int main() {
  uint64_t f[3];
  f[1] = 1;
  f[2] = 1;
  const uint64_t MOD = 1'000'000'000uLL;
  const double log_phi = log10((1 + sqrt(5)) / 2.0);
  const double log_sqrt5 = log10(sqrt(5));

  for (std::size_t i = 3; ; ++i) {
    f[i % 3] = (f[(i - 1) % 3] + f[(i - 2) % 3]) % MOD;

    if (pandigital(std::to_string(f[i % 3]))) {
      double t = i * log_phi - log_sqrt5;
      uint64_t first = pow(10, t - (uint64_t)t + 8);
      if (pandigital(std::to_string(first))) {
        printf("%ld\n", i);
        return 0;
      }
    }
  }

  return 0;
}
