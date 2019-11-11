#include <cstddef>
#include <cstdio>
#include <string>

#include "include/big_unsigned.hpp"

bool begins_pandigital(std::string &s) {
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
  ntlib::big_unsigned f[3];

  f[1] = 1;
  f[2] = 1;
  for (std::size_t i = 3; ; ++i) {
    std::size_t idx = i % 3;
    f[idx] = f[(i - 1) % 3] + f[(i - 2) % 3];

    std::string last_nine = (f[idx] % 1'000'000'000).to_string();
    if (begins_pandigital(last_nine)) {
      std::string all = f[idx].to_string();

      if (begins_pandigital(all)) {
        printf("%ld\n", i);
        return 0;
      }
    }
  }
  return 0;
}
