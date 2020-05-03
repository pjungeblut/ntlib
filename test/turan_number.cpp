#include <gtest/gtest.h>

#include <cstdint>

#include "turan_number.hpp"

TEST(TuranNumber, SingleValue) {
  // Invalid arguments.
  EXPECT_DEATH(ntlib::turan(0, 0), "");
  EXPECT_DEATH(ntlib::turan(1, 0), "");
  EXPECT_DEATH(ntlib::turan(-1, 1), "");

  // Small values.
  uint32_t values[] = {0, 0, 1, 0, 2, 3, 0, 4, 5, 6, 0, 6, 8, 9, 10, 0, 9, 12, 13, 14, 15};
  uint32_t idx = 0;
  for (uint32_t n = 1; n <= 6; ++n) {
    for (uint32_t k = 1; k <= n; ++k) {
      EXPECT_EQ(ntlib::turan(n, k), values[idx++]);
    }
  }
}
