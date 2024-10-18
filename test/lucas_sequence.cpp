#include <gtest/gtest.h>

import int128;
import lucas_sequence;
import modulo;

static constexpr int64_t MOD = 1'000'000'007; 

TEST(Lucas, BaseCases) {
  for (int p = -10; p < 10; ++p) {
    for (int q = -10; q < 10; ++q) {
      const auto [u0, v0] = ntlib::lucas_nth_term(0, p, q);
      EXPECT_EQ(u0, 0);
      EXPECT_EQ(v0, 2);

      const auto [u1, v1] = ntlib::lucas_nth_term(1, p, q);
      EXPECT_EQ(u1, 1);
      EXPECT_EQ(v1, p);

      const auto [um0, vm0] = ntlib::mod_lucas_nth_term(0, p, q, 11);
      EXPECT_EQ(um0, 0);
      EXPECT_EQ(vm0, 2);

      const auto [um1, vm1] = ntlib::mod_lucas_nth_term(1, p, q, 11);
      EXPECT_EQ(um1, 1);
      EXPECT_EQ(vm1, ntlib::mod(p, 11));
    }
  }
}

TEST(Lucas, PQ_P1_N1) {
  constexpr int64_t P = 1;
  constexpr int64_t Q = -1;
  // https://oeis.org/A000045
  constexpr auto u = std::to_array<int64_t>({0, 1, 1, 2, 3, 5, 8, 13, 21, 34,
      55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711,
      28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269,
      2178309, 3524578, 5702887, 9227465, 14930352, 24157817, 39088169});
  // https://oeis.org/A000032
  constexpr auto v = std::to_array<int64_t>({2, 1, 3, 4, 7, 11, 18, 29, 47,
      76, 123, 199, 322, 521, 843, 1364, 2207, 3571, 5778, 9349, 15127, 24476,
      39603, 64079, 103682, 167761, 271443, 439204, 710647, 1149851, 1860498,
      3010349, 4870847, 7881196, 12752043, 20633239, 33385282, 54018521,
      87403803});
  static_assert(u.size() == v.size());

  for (std::size_t n = 0; n < u.size(); ++n) {
    const auto [un, vn] = ntlib::lucas_nth_term(n, P, Q);
    EXPECT_EQ(un, u[n]);
    EXPECT_EQ(vn, v[n]);

    const auto [um, vm] = ntlib::mod_lucas_nth_term(n, P, Q, MOD);
    EXPECT_EQ(um, ntlib::mod(u[n], MOD));
    EXPECT_EQ(vm, ntlib::mod(v[n], MOD));
  }
}

TEST(Lucas, PQ_P4_N3) {
  constexpr int64_t P = 4;
  constexpr int64_t Q = -3;
  // https://oeis.org/A015530
  constexpr auto u = std::to_array<int64_t>({0, 1, 4, 19, 88, 409, 1900,
      8827, 41008, 190513, 885076, 4111843, 19102600, 88745929, 412291516,
      1915403851, 8898489952, 41340171361, 192056155300, 892245135283,
      4145149007032, 19257331433977});
  // https://oeis.org/A080042
  const auto v = std::to_array<int64_t>({2, 4, 22, 100, 466, 2164, 10054,
      46708, 216994, 1008100, 4683382, 21757828, 101081458, 469599316,
      2181641638, 10135364500, 47086382914, 218751625156, 1016265649366,
      4721317472932, 21934066839826, 101900219778100});
  static_assert(u.size() == v.size());

  for (std::size_t n = 0; n < u.size(); ++n) {
    const auto [un, vn] = ntlib::lucas_nth_term(n, P, Q);
    EXPECT_EQ(un, u[n]);
    EXPECT_EQ(vn, v[n]);

    const auto [um, vm] = ntlib::mod_lucas_nth_term(n, P, Q, MOD);
    EXPECT_EQ(um, ntlib::mod(u[n], MOD));
    EXPECT_EQ(vm, ntlib::mod(v[n], MOD));
  }
}

TEST(Lucas, PQ_P3_N5) {
  constexpr int64_t P = 3;
  constexpr int64_t Q = -5;
  // https://oeis.org/A015523
  constexpr auto u = std::to_array<int64_t>({0, 1, 3, 14, 57, 241, 1008,
      4229, 17727, 74326, 311613, 1306469, 5477472, 22964761, 96281643,
      403668734, 1692414417, 7095586921, 29748832848, 124724433149,
      522917463687, 2192374556806, 9191710988853, 38537005750589});
  // https://oeis.org/A072263
  constexpr auto v = std::to_array<int64_t>({2, 3, 19, 72, 311, 1293, 5434,
      22767, 95471, 400248, 1678099, 7035537, 29497106, 123669003, 518492539,
      2173822632, 9113930591, 38210904933, 160202367754, 671661627927,
      2815996722551, 11806298307288, 49498878534619, 207528127140297});
  static_assert(u.size() == v.size());

  for (std::size_t n = 0; n < u.size(); ++n) {
    const auto [un, vn] = ntlib::lucas_nth_term(n, P, Q);
    EXPECT_EQ(un, u[n]);
    EXPECT_EQ(vn, v[n]);

    const auto [um, vm] = ntlib::mod_lucas_nth_term(n, P, Q, MOD);
    EXPECT_EQ(um, ntlib::mod(u[n], MOD));
    EXPECT_EQ(vm, ntlib::mod(v[n], MOD));
  }
}


TEST(Lucas, PQ_P6_P1) {
  constexpr int64_t P = 6;
  constexpr int64_t Q = 1;
  // https://oeis.org/A001109
  constexpr auto u = std::to_array<int64_t>({0, 1, 6, 35, 204, 1189, 6930,
      40391, 235416, 1372105, 7997214, 46611179, 271669860, 1583407981,
      9228778026, 53789260175, 313506783024, 1827251437969, 10650001844790,
      62072759630771, 361786555939836, 2108646576008245, 12290092900109634});
  // https://oeis.org/A003499
  constexpr auto v = std::to_array<int64_t>({2, 6, 34, 198, 1154, 6726,
      39202, 228486, 1331714, 7761798, 45239074, 263672646, 1536796802,
      8957108166, 52205852194, 304278004998, 1773462177794, 10336495061766,
      60245508192802, 351136554095046, 2046573816377474, 11928306344169798,
      69523264248641314});
  static_assert(u.size() == v.size());

  for (std::size_t n = 0; n < u.size(); ++n) {
    const auto [un, vn] = ntlib::lucas_nth_term(n, P, Q);
    EXPECT_EQ(un, u[n]);
    EXPECT_EQ(vn, v[n]);

    const auto [um, vm] = ntlib::mod_lucas_nth_term(n, P, Q, MOD);
    EXPECT_EQ(um, ntlib::mod(u[n], MOD));
    EXPECT_EQ(vm, ntlib::mod(v[n], MOD));
  }
}

TEST(Lucas, PQ_P1_P2) {
  constexpr int64_t P = 1;
  constexpr int64_t Q = 2;
  // https://oeis.org/A107920
  constexpr auto u = std::to_array<int64_t>({0, 1, 1, -1, -3, -1, 5, 7, -3, -17,
      -11, 23, 45, -1, -91, -89, 93, 271, 85, -457, -627, 287, 1541, 967, -2115,
      -4049, 181, 8279, 7917, -8641, -24475, -7193, 41757, 56143, -27371,
      -139657, -84915, 194399, 364229, -24569});
  // https://oeis.org/A002249
  constexpr auto v = std::to_array<int64_t>({2, 1, -3, -5, 1, 11, 9, -13, -31,
      -5, 57, 67, -47, -181, -87, 275, 449, -101, -999, -797, 1201, 2795, 393,
      -5197, -5983, 4411, 16377, 7555, -25199, -40309, 10089, 90707, 70529,
      -110885, -251943, -30173, 473713, 534059, -413367, -1481485});
  static_assert(u.size() == v.size());

  for (std::size_t n = 0; n < u.size(); ++n) {
    const auto [un, vn] = ntlib::lucas_nth_term(n, P, Q);
    EXPECT_EQ(un, u[n]);
    EXPECT_EQ(vn, v[n]);

    const auto [um, vm] = ntlib::mod_lucas_nth_term(n, P, Q, MOD);
    EXPECT_EQ(um, ntlib::mod(u[n], MOD));
    EXPECT_EQ(vm, ntlib::mod(v[n], MOD));
  }
}
