#include <gtest/gtest.h>

#include <cstdint>

#include "matrix.hpp"

TEST(Construction, Default) {
  ntlib::matrix<int32_t> mat;
  EXPECT_EQ(mat.get_rows(), 0);
  EXPECT_EQ(mat.get_columns(), 0);
}

TEST(Construction, ByDimensions) {
  const int32_t rows = 3;
  const int32_t columns = 5;
  ntlib::matrix<int32_t> mat(rows, columns);
  EXPECT_EQ(mat.get_rows(), rows);
  EXPECT_EQ(mat.get_columns(), columns);
  for (int32_t r = 0; r < rows; ++r) {
    for (int32_t c = 0; c < columns; ++c) {
      EXPECT_EQ(mat[r][c], 0);
    }
  }
}

TEST(ArraySubscript, LValue) {
  const int32_t rows = 10;
  const int32_t columns = 20;
  ntlib::matrix<int32_t> mat(rows, columns);
  EXPECT_EQ(mat.get_rows(), rows);
  EXPECT_EQ(mat.get_columns(), columns);
  for (int32_t r = 0; r < rows; ++r) {
    for (int32_t c = 0; c < columns; ++c) {
      mat[r][c] = r * c;
      EXPECT_EQ(mat[r][c], r * c);
    }
  }
}

TEST(Output, StringRepresentation) {
  ntlib::matrix<int32_t> mat({{1,2,3},{4,5,6}});
  std::string rep = mat.to_string();
  EXPECT_EQ("{{1,2,3},{4,5,6}}", rep);
}

TEST(Arithmetic, Addition) {
  ntlib::matrix<int32_t> m1({{1,2,3},{4,5,6}});
  ntlib::matrix<int32_t> m2 = m1 + m1;
  ntlib::matrix<int32_t> sum({{2,4,6},{8,10,12}});
  EXPECT_EQ(m2, sum);
  m1 += m1;
  EXPECT_EQ(m1, sum);
}

TEST(Arithmetic, Subtraction) {
  ntlib::matrix<int32_t> m1({{2,4,6},{8,10,12}});
  ntlib::matrix<int32_t> m2({{1,2,3},{4,5,6}});
  ntlib::matrix<int32_t> m3 = m1 - m2;
  EXPECT_EQ(m2, m3);
  ntlib::matrix<int32_t> m4 = m2 - m1;
  EXPECT_EQ(m2, -m4);
  m1 -= m2;
  EXPECT_EQ(m1, m3);
}

TEST(Arithmetic, ScalarMultiplication) {
  ntlib::matrix<int32_t> m1({{1,2,3},{4,5,6}});
  ntlib::matrix<int32_t> m2({{2,4,6},{8,10,12}});
  EXPECT_EQ(2 * m1, m2);
  EXPECT_EQ(m1 * 2, m2);
  m1 *= 2;
  EXPECT_EQ(m1, m2);
}

TEST(Arithmetic, ScalarDivision) {
  ntlib::matrix<int32_t> m1({{1,2,3},{4,5,6}});
  ntlib::matrix<int32_t> m2({{2,4,6},{8,10,12}});
  EXPECT_EQ(m2 / 2, m1);
  m2 /= 2;
  EXPECT_EQ(m1, m2);
}

TEST(Arithmetic, Modulo) {
  ntlib::matrix<int32_t> m1({{1,2,3},{4,5,6}});
  ntlib::matrix<int32_t> m2({{1,2,0},{1,2,0}});
  EXPECT_EQ(m1 % 3, m2);
  m1 %= 3;
  EXPECT_EQ(m1, m2);
}

TEST(Arithmetic, Multiplication) {
  ntlib::matrix<int32_t> m1({{1,2,3},{4,5,6}});
  ntlib::matrix<int32_t> m2({{1,2},{3,4},{5,6}});
  ntlib::matrix<int32_t> m3({{22,28},{49,64}});
  EXPECT_EQ(m1 * m2, m3);
  m1 *= m2;
  EXPECT_EQ(m1, m3);
}

TEST(UnaryOperators, Minus) {
  ntlib::matrix<int32_t> m1({{1,2,3},{4,5,6}});
  ntlib::matrix<int32_t> m2({{-1,-2,-3},{-4,-5,-6}});
  EXPECT_EQ(m1, -m2);
  EXPECT_EQ(-m1, m2);
  EXPECT_EQ(-(-m1), m1);
}

TEST(UnaryOperators, Plus) {
  ntlib::matrix<int32_t> m1({{1,2,3},{4,5,6}});
  EXPECT_EQ(m1, +m1);
  EXPECT_EQ(m1, +(+m1));
}

TEST(Identity, SmallValues) {
  const auto is_id = [](auto m) {
    for (std::size_t r = 0; r < m.get_rows(); ++r) {
      for (std::size_t c = 0; c < m.get_columns(); ++c) {
        if (m[r][c] != (r == c)) return false;
      }
    }
    return true;
  };

  for (int32_t i = 0; i < 10; ++i) {
    const auto id = ntlib::matrix<int32_t>::get_identity(i);
    EXPECT_TRUE(is_id(id));
  }
}
