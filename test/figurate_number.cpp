#include <gtest/gtest.h>

#include <array>
#include <cstdint>

import figurate_number;

TEST(TriangleNumbers, SmallValues) {
  // https://oeis.org/A000217
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 3, 6, 10, 15, 21, 28,
      36, 45, 55, 66, 78, 91, 105, 120, 136, 153, 171, 190, 210, 231, 253, 276,
      300, 325, 351, 378, 406, 435, 465, 496, 528, 561, 595, 630, 666, 703, 741,
      780, 820, 861, 903, 946, 990, 1035, 1081, 1128, 1176, 1225, 1275, 1326,
      1378, 1431});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::triangle_number(i), truth[i]);
  }
}

TEST(SquareNumbers, SmallValues) {
  for (uint32_t i = 0; i < 100; ++i) {
    EXPECT_EQ(ntlib::square_number(i), i * i);
  }
}

TEST(PentagonalNumbers, SmallValues) {
  // https://oeis.org/A000326
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 5, 12, 22, 35, 51, 70,
      92, 117, 145, 176, 210, 247, 287, 330, 376, 425, 477, 532, 590, 651, 715,
      782, 852, 925, 1001, 1080, 1162, 1247, 1335, 1426, 1520, 1617, 1717, 1820,
      1926, 2035, 2147, 2262, 2380, 2501, 2625, 2752, 2882, 3015, 3151});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::pentagonal_number(i), truth[i]);
  }
}

TEST(HexagonalNumbers, SmallValues) {
  // https://oeis.org/A000384
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 6, 15, 28, 45, 66, 91,
      120, 153, 190, 231, 276, 325, 378, 435, 496, 561, 630, 703, 780, 861, 946,
      1035, 1128, 1225, 1326, 1431, 1540, 1653, 1770, 1891, 2016, 2145, 2278,
      2415, 2556, 2701, 2850, 3003, 3160, 3321, 3486, 3655, 3828, 4005, 4186,
      4371, 4560});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::hexagonal_number(i), truth[i]);
  }
}

TEST(HeptagonalNumbers, SmallValues) {
  // https://oeis.org/A000566
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 7, 18, 34, 55, 81, 112,
      148, 189, 235, 286, 342, 403, 469, 540, 616, 697, 783, 874, 970, 1071,
      1177, 1288, 1404, 1525, 1651, 1782, 1918, 2059, 2205, 2356, 2512, 2673,
      2839, 3010, 3186, 3367, 3553, 3744, 3940, 4141, 4347, 4558, 4774, 4995,
      5221, 5452, 5688});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::heptagonal_number(i), truth[i]);
  }
}

TEST(OctagonalNumbers, SmallValues) {
  // https://oeis.org/A000567
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 8, 21, 40, 65, 96, 133,
      176, 225, 280, 341, 408, 481, 560, 645, 736, 833, 936, 1045, 1160, 1281,
      1408, 1541, 1680, 1825, 1976, 2133, 2296, 2465, 2640, 2821, 3008, 3201,
      3400, 3605, 3816, 4033, 4256, 4485, 4720, 4961, 5208, 5461});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::octagonal_number(i), truth[i]);
  }
}

TEST(NonagonalNumbers, SmallValues) {
  // https://oeis.org/A001106
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 9, 24, 46, 75, 111, 154,
      204, 261, 325, 396, 474, 559, 651, 750, 856, 969, 1089, 1216, 1350, 1491,
      1639, 1794, 1956, 2125, 2301, 2484, 2674, 2871, 3075, 3286, 3504, 3729,
      3961, 4200, 4446, 4699, 4959, 5226, 5500, 5781, 6069, 6364});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::nonagonal_number(i), truth[i]);
  }
}

TEST(DecagonalNumbers, SmallValues) {
  // https://oeis.org/A001107
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 10, 27, 52, 85, 126,
      175, 232, 297, 370, 451, 540, 637, 742, 855, 976, 1105, 1242, 1387, 1540,
      1701, 1870, 2047, 2232, 2425, 2626, 2835, 3052, 3277, 3510, 3751, 4000,
      4257, 4522, 4795, 5076, 5365, 5662, 5967, 6280, 6601, 6930, 7267, 7612,
      7965, 8326});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::decagonal_number(i), truth[i]);
  }
}

TEST(TetrahedralNumbers, SmallValues) {
  // https://oeis.org/A000292
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 4, 10, 20, 35, 56, 84,
      120, 165, 220, 286, 364, 455, 560, 680, 816, 969, 1140, 1330, 1540, 1771,
      2024, 2300, 2600, 2925, 3276, 3654, 4060, 4495, 4960, 5456, 5984, 6545,
      7140, 7770, 8436, 9139, 9880, 10660, 11480, 12341, 13244, 14190, 15180});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::tetrahedral_number(i), truth[i]);
  }
}

TEST(CubeNumbers, SmallValues) {
  for (uint32_t i = 0; i < 100; ++i) {
    EXPECT_EQ(ntlib::cube_number(i), i * i * i);
  }
}

TEST(OctahedralNumbers, SmallValues) {
  // https://oeis.org/A005900
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 6, 19, 44, 85, 146, 231,
      344, 489, 670, 891, 1156, 1469, 1834, 2255, 2736, 3281, 3894, 4579, 5340,
      6181, 7106, 8119, 9224, 10425, 11726, 13131, 14644, 16269, 18010, 19871,
      21856, 23969, 26214, 28595, 31116, 33781, 36594, 39559, 42680});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::octahedral_number(i), truth[i]) << i;
  }
}

TEST(SquarePyramidalNumbers, SmallValues) {
  // https://oeis.org/A000330
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 5, 14, 30, 55, 91, 140,
      204, 285, 385, 506, 650, 819, 1015, 1240, 1496, 1785, 2109, 2470, 2870,
      3311, 3795, 4324, 4900, 5525, 6201, 6930, 7714, 8555, 9455, 10416, 11440,
      12529, 13685, 14910, 16206, 17575, 19019, 20540, 22140, 23821, 25585,
      27434, 29370});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::square_pyramidal_number(i), truth[i]);
  }
}

TEST(CenteredSquareNumbers, SmallValues) {
  // https://oeis.org/A001844
  constexpr auto truth = std::to_array<uint32_t>({1, 5, 13, 25, 41, 61, 85, 113,
      145, 181, 221, 265, 313, 365, 421, 481, 545, 613, 685, 761, 841, 925,
      1013, 1105, 1201, 1301, 1405, 1513, 1625, 1741, 1861, 1985, 2113, 2245,
      2381, 2521, 2665, 2813, 2965, 3121, 3281, 3445, 3613, 3785, 3961, 4141,
      4325, 4513});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::centered_square_number(i), truth[i]);
  }
}


