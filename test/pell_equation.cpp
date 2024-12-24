#include <gtest/gtest.h>

#include <cstdint>
#include <functional>
#include <ranges>
#include <tuple>

import base;
import int128;
import pell_equation;

class PellFundamentalSolutionTable :
    public testing::TestWithParam<std::tuple<uint64_t, uint64_t, uint64_t>> {};

TEST_P(PellFundamentalSolutionTable, WolframAlpha) {
  // See: https://mathworld.wolfram.com/PellEquation.html
  const auto [D, true_x, true_y] = GetParam();
  const auto [x, y] = ntlib::pell_fundamental_solution(D);
  EXPECT_EQ(x, true_x);
  EXPECT_EQ(y, true_y);
}
// Values: {D, true_x, true_y}
// The following solutions are all known to be fundamental.
INSTANTIATE_TEST_SUITE_P(CorrectnessMinimality, PellFundamentalSolutionTable,
    testing::Values(
      std::make_tuple(2, 3, 2),
      std::make_tuple(3, 2, 1),
      std::make_tuple(5, 9, 4),
      std::make_tuple(6, 5, 2),
      std::make_tuple(7, 8, 3),
      std::make_tuple(8, 3, 1),
      std::make_tuple(10, 19, 6),
      std::make_tuple(11, 10, 3),
      std::make_tuple(12, 7, 2),
      std::make_tuple(13, 649, 180),
      std::make_tuple(14, 15, 4),
      std::make_tuple(15, 4, 1),
      std::make_tuple(17, 33, 8),
      std::make_tuple(18, 17, 4),
      std::make_tuple(19, 170, 39),
      std::make_tuple(20, 9, 2),
      std::make_tuple(21, 55, 12),
      std::make_tuple(22, 197, 42),
      std::make_tuple(23, 24, 5),
      std::make_tuple(24, 5, 1),
      std::make_tuple(29, 9801, 1820),
      std::make_tuple(46, 24335, 3588),
      std::make_tuple(53, 66249, 9100),
      std::make_tuple(61, 1766319049, 226153980)
));

class PellFundamentalSolution : public testing::TestWithParam<ntlib::u128> {};

TEST_P(PellFundamentalSolution, Correctness) {
  const auto D = GetParam();
  const auto [x, y] = ntlib::pell_fundamental_solution(D);
  EXPECT_EQ(x * x - D * y * y, ntlib::u128 {1});
}
INSTANTIATE_TEST_SUITE_P(First500, PellFundamentalSolution, testing::ValuesIn(
    std::views::iota(1, 500)
        | std::views::filter(std::not_fn(ntlib::is_square<ntlib::u128>))
        | std::ranges::to<std::vector<ntlib::u128>>()
));

class PellNextSolution : public testing::TestWithParam<ntlib::u128> {};

TEST_P(PellNextSolution, CorrectnessNext10) {
  const auto D = GetParam();
  const auto fundamental = ntlib::pell_fundamental_solution(D);
  const auto [x0, y0] = fundamental;
  EXPECT_EQ(x0 * x0 - D * y0 * y0, ntlib::u128 {1});

  auto current = fundamental;
  for (std::size_t i = 0; i < 10; ++i) {
    const auto next = ntlib::pell_next_solution(D, fundamental, current);
    const auto [xi, yi] = next;
    EXPECT_EQ(xi * xi - D * yi * yi, ntlib::u128 {1});
    current = next;
  }
}
INSTANTIATE_TEST_SUITE_P(First100, PellNextSolution, testing::ValuesIn(
    std::views::iota(1, 100)
        | std::views::filter(std::not_fn(ntlib::is_square<ntlib::u128>))
        | std::ranges::to<std::vector<ntlib::u128>>()
));
