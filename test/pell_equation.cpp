#include <gtest/gtest.h>

#include <cstdint>
#include <tuple>

#include "base.hpp"
#include "int128.hpp"
#include "pell_equation.hpp"

TEST(MinPellSolutions, SmallValues) {
  const uint32_t values[][3] = {
    {2, 3, 2},
    {3, 2, 1},
    {5, 9, 4},
    {6, 5, 2},
    {7, 8, 3}
  };
  for (const auto &v : values) {
    const auto [x, y] = ntlib::min_pell_solution(v[0]);
    EXPECT_EQ(x, v[1]);
    EXPECT_EQ(y, v[2]);
  }
}

TEST(MinPellSolutions, BiggerValues) {
  const ntlib::u128 N = 1'000;
  for (ntlib::u128 d = 2; d <= N; ++d) {
    if (ntlib::is_square(d)) continue;
    const auto [x, y] = ntlib::min_pell_solution(d);
    EXPECT_EQ(x * x - d * y * y, 1);
  }
}

TEST(NextPellSolution, SmallValues) {
  const ntlib::u128 N = 100;
  const ntlib::u128 M = 10;
  for (ntlib::u128 d = 2; d <= N; ++d) {
    if (ntlib::is_square(d)) continue;
    const auto s0 = ntlib::min_pell_solution(d);
    const auto [x0, y0] = s0;
    EXPECT_EQ(x0 * x0 - d * y0 * y0, 1);

    auto last = s0;
    for (ntlib::u128 i = 0; i < M; ++i) {
      const auto si = ntlib::next_pell_solution(d, s0, last);
      const auto [xi, yi] = si;
      EXPECT_EQ(xi * xi - d * yi * yi, 1);
      last = si;
    }
  }
}
