#include <gtest/gtest.h>

#include <cstdint>

#include "divisors.hpp"
#include "prime_decomposition.hpp"

// All sigma sequences below have been prefixed with a `0` element to make them
// 1-indexed.
// https://oeis.org/A000005
constexpr auto sigma_0 = std::to_array<uint32_t>({0, 1, 2, 2, 3, 2, 4, 2, 4, 3,
    4, 2, 6, 2, 4, 4, 5, 2, 6, 2, 6, 4, 4, 2, 8, 3, 4, 4, 6, 2, 8, 2, 6, 4, 4,
    4, 9, 2, 4, 4, 8, 2, 8, 2, 6, 6, 4, 2, 10, 3, 6, 4, 6, 2, 8, 4, 8, 4, 4, 2,
    12, 2, 4, 6, 7, 4, 8, 2, 6, 4, 8, 2, 12, 2, 4, 6, 6, 4, 8, 2, 10, 5, 4, 2,
    12, 4, 4, 4, 8, 2, 12, 4, 6, 4, 4, 4, 12, 2, 6, 6, 9, 2, 8, 2, 8});
// https://oeis.org/A000203
constexpr auto sigma_1 = std::to_array<uint32_t>({0, 1, 3, 4, 7, 6, 12, 8, 15,
    13, 18, 12, 28, 14, 24, 24, 31, 18, 39, 20, 42, 32, 36, 24, 60, 31, 42, 40,
    56, 30, 72, 32, 63, 48, 54, 48, 91, 38, 60, 56, 90, 42, 96, 44, 84, 78, 72,
    48, 124, 57, 93, 72, 98, 54, 120, 72, 120, 80, 90, 60, 168, 62, 96, 104,
    127, 84, 144, 68, 126, 96, 144});
// https://oeis.org/A001157
constexpr auto sigma_2 = std::to_array<uint32_t>({0, 1, 5, 10, 21, 26, 50, 50,
    85, 91, 130, 122, 210, 170, 250, 260, 341, 290, 455, 362, 546, 500, 610,
    530, 850, 651, 850, 820, 1050, 842, 1300, 962, 1365, 1220, 1450, 1300, 1911,
    1370, 1810, 1700, 2210, 1682, 2500, 1850, 2562, 2366, 2650, 2210, 3410,
    2451, 3255});
// https://oeis.org/A001158
constexpr auto sigma_3 = std::to_array<uint32_t>({0, 1, 9, 28, 73, 126, 252,
    344, 585, 757, 1134, 1332, 2044, 2198, 3096, 3528, 4681, 4914, 6813, 6860,
    9198, 9632, 11988, 12168, 16380, 15751, 19782, 20440, 25112, 24390, 31752,
    29792, 37449, 37296, 44226, 43344, 55261, 50654, 61740, 61544, 73710, 68922,
    86688});
// https://oeis.org/A001159
constexpr auto sigma_4 = std::to_array<uint32_t>({0, 1, 17, 82, 273, 626, 1394,
    2402, 4369, 6643, 10642, 14642, 22386, 28562, 40834, 51332, 69905, 83522,
    112931, 130322, 170898, 196964, 248914, 279842, 358258, 391251, 485554,
    538084, 655746, 707282, 872644, 923522, 1118481, 1200644});

TEST(CountDivisors, SmallValues) {
  for (uint32_t i = 1; i < sigma_0.size(); ++i) {
    const auto pd = ntlib::prime_decomposition(i);
    EXPECT_EQ(ntlib::count_divisors(pd), sigma_0[i]);
  }
}

TEST(DivisorFunction_E0, SmallValues) {
  for (uint32_t i = 1; i < sigma_0.size(); ++i) {
    const auto pd = ntlib::prime_decomposition(i);
    EXPECT_EQ(ntlib::divisor_function(pd, 0u), sigma_0[i]);
  }
}

TEST(DivisorFunction_E1, SmallValues) {
  for (uint32_t i = 1; i < sigma_1.size(); ++i) {
    const auto pd = ntlib::prime_decomposition(i);
    EXPECT_EQ(ntlib::divisor_function(pd, 1u), sigma_1[i]);
  }
}

TEST(DivisorFunction_E2, SmallValues) {
  for (uint32_t i = 1; i < sigma_2.size(); ++i) {
    const auto pd = ntlib::prime_decomposition(i);
    EXPECT_EQ(ntlib::divisor_function(pd, 2u), sigma_2[i]);
  }
}

TEST(DivisorFunction_E3, SmallValues) {
  for (uint32_t i = 1; i < sigma_3.size(); ++i) {
    const auto pd = ntlib::prime_decomposition(i);
    EXPECT_EQ(ntlib::divisor_function(pd, 3u), sigma_3[i]);
  }
}

TEST(DivisorFunction_E4, SmallValues) {
  for (uint32_t i = 1; i < sigma_4.size(); ++i) {
    const auto pd = ntlib::prime_decomposition(i);
    EXPECT_EQ(ntlib::divisor_function(pd, 4u), sigma_4[i]);
  }
}
