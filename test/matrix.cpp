#include <gtest/gtest.h>

#include <cstdint>
#include <string>

import base;
import matrix;
import mod_int;

TEST(ArraySubscript, LValue) {
  constexpr int32_t ROWS = 10;
  constexpr int32_t COLUMNS = 20;
  ntlib::matrix<ROWS, COLUMNS, int32_t> mat;
  for (int32_t r = 0; r < ROWS; ++r) {
    for (int32_t c = 0; c < COLUMNS; ++c) {
      mat[r, c] = r * c;
      EXPECT_EQ((mat[r, c]), r * c);
    }
  }
}

TEST(Output, StringRepresentation) {
  ntlib::matrix<2, 3, int32_t> mat({{1,2,3},{4,5,6}});
  std::string rep = to_string(mat);
  EXPECT_EQ("{{1,2,3},{4,5,6}}", rep);
}

TEST(Arithmetic, Addition) {
  ntlib::matrix<2, 3, int32_t> m1({{1,2,3},{4,5,6}});
  auto m2 = m1 + m1;
  ntlib::matrix<2, 3, int32_t> sum({{2,4,6},{8,10,12}});
  EXPECT_EQ(m2, sum);
  m1 += m1;
  EXPECT_EQ(m1, sum);
}

TEST(Arithmetic, Subtraction) {
  ntlib::matrix<2, 3, int32_t> m1({{2,4,6},{8,10,12}});
  ntlib::matrix<2, 3, int32_t> m2({{1,2,3},{4,5,6}});
  auto m3 = m1 - m2;
  EXPECT_EQ(m2, m3);
  auto m4 = m2 - m1;
  EXPECT_EQ(m2, -m4);
  m1 -= m2;
  EXPECT_EQ(m1, m3);
}

TEST(Arithmetic, ScalarMultiplication) {
  ntlib::matrix<2, 3, int32_t> m1({{1,2,3},{4,5,6}});
  ntlib::matrix<2, 3, int32_t> m2({{2,4,6},{8,10,12}});
  EXPECT_EQ(2 * m1, m2);
  EXPECT_EQ(m1 * 2, m2);
  m1 *= 2;
  EXPECT_EQ(m1, m2);
}

TEST(Arithmetic, ScalarDivision) {
  ntlib::matrix<2, 3, int32_t> m1({{1,2,3},{4,5,6}});
  ntlib::matrix<2, 3, int32_t> m2({{2,4,6},{8,10,12}});
  EXPECT_EQ(m2 / 2, m1);
  m2 /= 2;
  EXPECT_EQ(m1, m2);
}

TEST(Arithmetic, Modulo) {
  ntlib::matrix<2, 3, int32_t> m1({{1,2,3},{4,5,6}});
  ntlib::matrix<2, 3, int32_t> m2({{1,2,0},{1,2,0}});
  EXPECT_EQ(m1 % 3, m2);
  m1 %= 3;
  EXPECT_EQ(m1, m2);
}

TEST(Arithmetic, Multiplication) {
  ntlib::matrix<2, 2, int32_t> m1({{1,2},{3,4}});
  ntlib::matrix<2, 2, int32_t> m2({{4,3},{2,1}});
  ntlib::matrix<2, 2, int32_t> m3({{8,5},{20,13}});
  EXPECT_EQ(m1 * m2, m3);
  m1 *= m2;
  EXPECT_EQ(m1, m3);
}

TEST(Arithmetic, Exponentiation) {
  ntlib::matrix<2, 2, int32_t> m({{1,1},{1,0}});
  ntlib::matrix<2, 1, int32_t> f({{1},{0}});
  m = ntlib::pow(m, 10);
  auto fib = m * f;
  EXPECT_EQ((fib[0, 0]), 89); // 11th Fibonacci number
  EXPECT_EQ((fib[1, 0]), 55); // 10th Fibonacci number
}

TEST(UnaryOperators, Minus) {
  ntlib::matrix<2, 3, int32_t> m1({{1,2,3},{4,5,6}});
  ntlib::matrix<2, 3, int32_t> m2({{-1,-2,-3},{-4,-5,-6}});
  EXPECT_EQ(m1, -m2);
  EXPECT_EQ(-m1, m2);
  EXPECT_EQ(-(-m1), m1);
}

TEST(UnaryOperators, Plus) {
  ntlib::matrix<2, 3, int32_t> m1({{1,2,3},{4,5,6}});
  EXPECT_EQ(m1, +m1);
  EXPECT_EQ(m1, +(+m1));
}

TEST(Zero, SmallValues) {
  const auto id1 = ntlib::matrix<1, 1, int32_t>::get_zero();
  EXPECT_EQ((id1[0,0]), 0);

  const auto id2 = ntlib::matrix<2, 2, int32_t>::get_zero();
  EXPECT_EQ((id2[0,0]), 0);
  EXPECT_EQ((id2[0,1]), 0);
  EXPECT_EQ((id2[1,0]), 0);
  EXPECT_EQ((id2[1,1]), 0);
}

TEST(Identity, SmallValues) {
  const auto id1 = ntlib::matrix<1, 1, int32_t>::get_identity();
  EXPECT_EQ((id1[0,0]), 1);

  const auto id2 = ntlib::matrix<2, 2, int32_t>::get_identity();
  EXPECT_EQ((id2[0,0]), 1);
  EXPECT_EQ((id2[0,1]), 0);
  EXPECT_EQ((id2[1,0]), 0);
  EXPECT_EQ((id2[1,1]), 1);
}

TEST(AdditiveNeutral, SmallValues) {
  const auto n1 = ntlib::zero<ntlib::matrix<2, 2, int32_t>>();
  EXPECT_EQ(n1, (ntlib::matrix<2, 2, int32_t>::get_zero()));
}

TEST(MultiplicativeNeutral, SmallValues) {
  const auto n1 = ntlib::one<ntlib::matrix<2, 2, int32_t>>();
  EXPECT_EQ(n1, (ntlib::matrix<2, 2, int32_t>::get_identity()));
}
