#include <gtest/gtest.h>

#include <array>
#include <cstdint>

import figurate_number;

TEST(PolygonalNumbers, Triangular) {
  // https://oeis.org/A000217
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 3, 6, 10, 15, 21, 28,
      36, 45, 55, 66, 78, 91, 105, 120, 136, 153, 171, 190, 210, 231, 253, 276,
      300, 325, 351, 378, 406, 435, 465, 496, 528, 561, 595, 630, 666, 703, 741,
      780, 820, 861, 903, 946, 990, 1035, 1081, 1128, 1176, 1225, 1275, 1326,
      1378, 1431});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::k_gonal_number(3u, i), truth[i]);
  }
}

TEST(PolygonalNumbers, Square) {
  for (uint32_t i = 0; i < 100; ++i) {
    EXPECT_EQ(ntlib::k_gonal_number(4u, i), i * i);
  }
}

TEST(PolygonalNumbers, Pentagonal) {
  // https://oeis.org/A000326
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 5, 12, 22, 35, 51, 70,
      92, 117, 145, 176, 210, 247, 287, 330, 376, 425, 477, 532, 590, 651, 715,
      782, 852, 925, 1001, 1080, 1162, 1247, 1335, 1426, 1520, 1617, 1717, 1820,
      1926, 2035, 2147, 2262, 2380, 2501, 2625, 2752, 2882, 3015, 3151});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::k_gonal_number(5u, i), truth[i]);
  }
}

TEST(PolygonalNumbers, Hexagonal) {
  // https://oeis.org/A000384
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 6, 15, 28, 45, 66, 91,
      120, 153, 190, 231, 276, 325, 378, 435, 496, 561, 630, 703, 780, 861, 946,
      1035, 1128, 1225, 1326, 1431, 1540, 1653, 1770, 1891, 2016, 2145, 2278,
      2415, 2556, 2701, 2850, 3003, 3160, 3321, 3486, 3655, 3828, 4005, 4186,
      4371, 4560});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::k_gonal_number(6u, i), truth[i]);
  }
}

TEST(PolygonalNumbers, Heptagonal) {
  // https://oeis.org/A000566
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 7, 18, 34, 55, 81, 112,
      148, 189, 235, 286, 342, 403, 469, 540, 616, 697, 783, 874, 970, 1071,
      1177, 1288, 1404, 1525, 1651, 1782, 1918, 2059, 2205, 2356, 2512, 2673,
      2839, 3010, 3186, 3367, 3553, 3744, 3940, 4141, 4347, 4558, 4774, 4995,
      5221, 5452, 5688});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::k_gonal_number(7u, i), truth[i]);
  }
}

TEST(PolygonalNumbers, Octagonal) {
  // https://oeis.org/A000567
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 8, 21, 40, 65, 96, 133,
      176, 225, 280, 341, 408, 481, 560, 645, 736, 833, 936, 1045, 1160, 1281,
      1408, 1541, 1680, 1825, 1976, 2133, 2296, 2465, 2640, 2821, 3008, 3201,
      3400, 3605, 3816, 4033, 4256, 4485, 4720, 4961, 5208, 5461});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::k_gonal_number(8u, i), truth[i]);
  }
}

TEST(PolygonalNumbers, Nonagonal) {
  // https://oeis.org/A001106
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 9, 24, 46, 75, 111, 154,
      204, 261, 325, 396, 474, 559, 651, 750, 856, 969, 1089, 1216, 1350, 1491,
      1639, 1794, 1956, 2125, 2301, 2484, 2674, 2871, 3075, 3286, 3504, 3729,
      3961, 4200, 4446, 4699, 4959, 5226, 5500, 5781, 6069, 6364});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::k_gonal_number(9u, i), truth[i]);
  }
}

TEST(PolygonalNumbers, Decagonal) {
  // https://oeis.org/A001107
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 10, 27, 52, 85, 126,
      175, 232, 297, 370, 451, 540, 637, 742, 855, 976, 1105, 1242, 1387, 1540,
      1701, 1870, 2047, 2232, 2425, 2626, 2835, 3052, 3277, 3510, 3751, 4000,
      4257, 4522, 4795, 5076, 5365, 5662, 5967, 6280, 6601, 6930, 7267, 7612,
      7965, 8326});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::k_gonal_number(10u, i), truth[i]);
  }
}

TEST(CenteredPolygonalNumbers, Triangular) {
  // https://oeis.org/A005448
  constexpr auto truth = std::to_array<uint32_t>({1, 4, 10, 19, 31, 46, 64,
      85, 109, 136, 166, 199, 235, 274, 316, 361, 409, 460, 514, 571, 631, 694,
      760, 829, 901, 976, 1054, 1135, 1219, 1306, 1396, 1489, 1585, 1684, 1786,
      1891, 1999, 2110, 2224, 2341, 2461, 2584, 2710, 2839, 2971, 3106, 3244,
      3385, 3529});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::centered_k_gonal_number(3u, i), truth[i]);
  }
}

TEST(CenteredPolygonalNumbers, Square) {
  // https://oeis.org/A001844
  constexpr auto truth = std::to_array<uint32_t>({1, 5, 13, 25, 41, 61, 85, 113,
      145, 181, 221, 265, 313, 365, 421, 481, 545, 613, 685, 761, 841, 925,
      1013, 1105, 1201, 1301, 1405, 1513, 1625, 1741, 1861, 1985, 2113, 2245,
      2381, 2521, 2665, 2813, 2965, 3121, 3281, 3445, 3613, 3785, 3961, 4141,
      4325, 4513});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::centered_k_gonal_number(4u, i), truth[i]);
  }
}

TEST(CenteredPolygonalNumbers, Pentagonal) {
  // https://oeis.org/A005891
  constexpr auto truth = std::to_array<uint32_t>({1, 6, 16, 31, 51, 76, 106,
      141, 181, 226, 276, 331, 391, 456, 526, 601, 681, 766, 856, 951, 1051,
      1156, 1266, 1381, 1501, 1626, 1756, 1891, 2031, 2176, 2326, 2481, 2641,
      2806, 2976, 3151, 3331, 3516, 3706, 3901, 4101, 4306, 4516, 4731, 4951,
      5176, 5406});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::centered_k_gonal_number(5u, i), truth[i]);
  }
}

TEST(CenteredPolygonalNumbers, Hexagonal) {
  // https://oeis.org/A003215
  constexpr auto truth = std::to_array<uint32_t>({1, 7, 19, 37, 61, 91, 127,
      169, 217, 271, 331, 397, 469, 547, 631, 721, 817, 919, 1027, 1141, 1261,
      1387, 1519, 1657, 1801, 1951, 2107, 2269, 2437, 2611, 2791, 2977, 3169,
      3367, 3571, 3781, 3997, 4219, 4447, 4681, 4921, 5167, 5419, 5677, 5941,
      6211, 6487, 6769});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::centered_k_gonal_number(6u, i), truth[i]);
  }
}

TEST(CenteredPolygonalNumbers, Heptagonal) {
  // https://oeis.org/A069099
  constexpr auto truth = std::to_array<uint32_t>({1, 8, 22, 43, 71, 106, 148,
      197, 253, 316, 386, 463, 547, 638, 736, 841, 953, 1072, 1198, 1331, 1471,
      1618, 1772, 1933, 2101, 2276, 2458, 2647, 2843, 3046, 3256, 3473, 3697,
      3928, 4166, 4411, 4663, 4922, 5188, 5461, 5741, 6028, 6322, 6623, 6931,
      7246});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::centered_k_gonal_number(7u, i), truth[i]);
  }
}

TEST(CenteredPolygonalNumbers, Octagonal) {
  // https://oeis.org/A016754
  constexpr auto truth = std::to_array<uint32_t>({1, 9, 25, 49, 81, 121, 169,
      225, 289, 361, 441, 529, 625, 729, 841, 961, 1089, 1225, 1369, 1521, 1681,
      1849, 2025, 2209, 2401, 2601, 2809, 3025, 3249, 3481, 3721, 3969, 4225,
      4489, 4761, 5041, 5329, 5625, 5929, 6241, 6561, 6889, 7225, 7569, 7921,
      8281, 8649, 9025});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::centered_k_gonal_number(8u, i), truth[i]);
  }
}

TEST(CenteredPolygonalNumbers, Nonagonal) {
  // https://oeis.org/A060544
  constexpr auto truth = std::to_array<uint32_t>({1, 10, 28, 55, 91, 136, 190,
      253, 325, 406, 496, 595, 703, 820, 946, 1081, 1225, 1378, 1540, 1711,
      1891, 2080, 2278, 2485, 2701, 2926, 3160, 3403, 3655, 3916, 4186, 4465,
      4753, 5050, 5356, 5671, 5995, 6328, 6670, 7021, 7381, 7750, 8128, 8515,
      8911, 9316});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::centered_k_gonal_number(9u, i), truth[i]);
  }
}

TEST(CenteredPolygonalNumbers, Decagonal) {
  // https://oeis.org/A062786
  constexpr auto truth = std::to_array<uint32_t>({1, 11, 31, 61, 101, 151, 211,
      281, 361, 451, 551, 661, 781, 911, 1051, 1201, 1361, 1531, 1711, 1901,
      2101, 2311, 2531, 2761, 3001, 3251, 3511, 3781, 4061, 4351, 4651, 4961,
      5281, 5611, 5951, 6301, 6661, 7031, 7411, 7801, 8201, 8611, 9031, 9461,
      9901, 10351, 10811});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::centered_k_gonal_number(10u, i), truth[i]);
  }
}

TEST(PyramidalNumbers, Triangular) {
  // https://oeis.org/A000292
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 4, 10, 20, 35, 56, 84,
      120, 165, 220, 286, 364, 455, 560, 680, 816, 969, 1140, 1330, 1540, 1771,
      2024, 2300, 2600, 2925, 3276, 3654, 4060, 4495, 4960, 5456, 5984, 6545,
      7140, 7770, 8436, 9139, 9880, 10660, 11480, 12341, 13244, 14190, 15180});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::k_gonal_pyramidal_number(3u, i), truth[i]);
  }
}

TEST(PyramidalNumbers, Square) {
  // https://oeis.org/A000330
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 5, 14, 30, 55, 91, 140,
      204, 285, 385, 506, 650, 819, 1015, 1240, 1496, 1785, 2109, 2470, 2870,
      3311, 3795, 4324, 4900, 5525, 6201, 6930, 7714, 8555, 9455, 10416, 11440,
      12529, 13685, 14910, 16206, 17575, 19019, 20540, 22140, 23821, 25585,
      27434, 29370});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::k_gonal_pyramidal_number(4u, i), truth[i]);
  }
}

TEST(PyramidalNumbers, Pentagonal) {
  // https://oeis.org/A002411
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 6, 18, 40, 75, 126, 196,
      288, 405, 550, 726, 936, 1183, 1470, 1800, 2176, 2601, 3078, 3610, 4200,
      4851, 5566, 6348, 7200, 8125, 9126, 10206, 11368, 12615, 13950, 15376,
      16896, 18513, 20230, 22050, 23976, 26011, 28158, 30420, 32800, 35301,
      37926, 40678});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::k_gonal_pyramidal_number(5u, i), truth[i]);
  }
}

TEST(PyramidalNumbers, Hexagonal) {
  // https://oeis.org/A002412
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 7, 22, 50, 95, 161, 252,
      372, 525, 715, 946, 1222, 1547, 1925, 2360, 2856, 3417, 4047, 4750, 5530,
      6391, 7337, 8372, 9500, 10725, 12051, 13482, 15022, 16675, 18445, 20336,
      22352, 24497, 26775, 29190, 31746, 34447, 37297, 40300});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::k_gonal_pyramidal_number(6u, i), truth[i]);
  }
}

TEST(PyramidalNumbers, Heptagonal) {
  // https://oeis.org/A002413
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 8, 26, 60, 115, 196,
      308, 456, 645, 880, 1166, 1508, 1911, 2380, 2920, 3536, 4233, 5016, 5890,
      6860, 7931, 9108, 10396, 11800, 13325, 14976, 16758, 18676, 20735, 22940,
      25296, 27808, 30481, 33320, 36330, 39516, 42883, 46436, 50180, 54120});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::k_gonal_pyramidal_number(7u, i), truth[i]);
  }
}

TEST(PolyhedralNumbers, Cubes) {
  for (uint32_t i = 0; i < 100; ++i) {
    EXPECT_EQ(ntlib::cube_number(i), i * i * i);
  }
}

TEST(PolyhedralNumbers, Octahedral) {
  // https://oeis.org/A005900
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 6, 19, 44, 85, 146, 231,
      344, 489, 670, 891, 1156, 1469, 1834, 2255, 2736, 3281, 3894, 4579, 5340,
      6181, 7106, 8119, 9224, 10425, 11726, 13131, 14644, 16269, 18010, 19871,
      21856, 23969, 26214, 28595, 31116, 33781, 36594, 39559, 42680});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::octahedral_number(i), truth[i]);
  }
}

TEST(PolyhedralNumbers, Dodecahedral) {
  // https://oeis.org/A006566
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 20, 84, 220, 455, 816,
      1330, 2024, 2925, 4060, 5456, 7140, 9139, 11480, 14190, 17296, 20825,
      24804, 29260, 34220, 39711, 45760, 52394, 59640, 67525, 76076, 85320,
      95284, 105995, 117480, 129766, 142880, 156849, 171700, 187460, 204156});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::dodecahedral_number(i), truth[i]);
  }
}

TEST(PolyhedralNumbers, Icosahedral) {
  // https://oeis.org/A006564
  constexpr auto truth = std::to_array<uint32_t>({0, 1, 12, 48, 124, 255, 456,
      742, 1128, 1629, 2260, 3036, 3972, 5083, 6384, 7890, 9616, 11577, 13788,
      16264, 19020, 22071, 25432, 29118, 33144, 37525, 42276, 47412, 52948,
      58899, 65280, 72106, 79392, 87153, 95404, 104160, 113436, 123247,
      133608});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::icosahedral_number(i), truth[i]);
  }
}

TEST(CenteredPolyhedralNumbers, Tetrahedral) {
  // https://oeis.org/A005894
  constexpr auto truth = std::to_array<uint32_t>({1, 5, 15, 35, 69, 121, 195,
      295, 425, 589, 791, 1035, 1325, 1665, 2059, 2511, 3025, 3605, 4255, 4979,
      5781, 6665, 7635, 8695, 9849, 11101, 12455, 13915, 15485, 17169, 18971,
      20895, 22945, 25125, 27439, 29891, 32485, 35225, 38115});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::centered_tetrahedral_number(i), truth[i]);
  }
}

TEST(CenteredPolyhedralNumbers, Cubes) {
  // https://oeis.org/A005898
  constexpr auto truth = std::to_array<uint32_t>({1, 9, 35, 91, 189, 341, 559,
      855, 1241, 1729, 2331, 3059, 3925, 4941, 6119, 7471, 9009, 10745, 12691,
      14859, 17261, 19909, 22815, 25991, 29449, 33201, 37259, 41635, 46341,
      51389, 56791, 62559, 68705, 75241, 82179, 89531, 97309, 105525, 114191,
      123319, 132921});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::centered_cube_number(i), truth[i]);
  }
}

TEST(CenteredPolyhedralNumbers, Octahedral) {
  // https://oeis.org/A001845
  constexpr auto truth = std::to_array<uint32_t>({1, 7, 25, 63, 129, 231, 377,
      575, 833, 1159, 1561, 2047, 2625, 3303, 4089, 4991, 6017, 7175, 8473,
      9919, 11521, 13287, 15225, 17343, 19649, 22151, 24857, 27775, 30913,
      34279, 37881, 41727, 45825, 50183, 54809, 59711, 64897, 70375, 76153,
      82239});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::centered_octahedral_number(i), truth[i]);
  }
}

TEST(CenteredPolyhedralNumbers, Dodecahedral) {
  // https://oeis.org/A005904
  constexpr auto truth = std::to_array<uint32_t>({1, 33, 155, 427, 909, 1661,
      2743, 4215, 6137, 8569, 11571, 15203, 19525, 24597, 30479, 37231, 44913,
      53585, 63307, 74139, 86141, 99373, 113895, 129767, 147049, 165801, 186083,
      207955, 231477, 256709, 283711, 312543, 343265, 375937, 410619, 447371,
      486253, 527325});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::centered_dodecahedral_number(i), truth[i]);
  }
}

TEST(CenteredPolyhedralNumbers, Icosahedral) {
  // https://oeis.org/A005902
  constexpr auto truth = std::to_array<uint32_t>({1, 13, 55, 147, 309, 561, 923,
      1415, 2057, 2869, 3871, 5083, 6525, 8217, 10179, 12431, 14993, 17885,
      21127, 24739, 28741, 33153, 37995, 43287, 49049, 55301, 62063, 69355,
      77197, 85609, 94611, 104223, 114465, 125357, 136919, 149171, 162133,
      175825, 190267, 205479});
  for (uint32_t i = 0; i < truth.size(); ++i) {
    EXPECT_EQ(ntlib::centered_icosahedral_number(i), truth[i]);
  }
}
