#include <gtest/gtest.h>

import rational;

TEST(Initialization, ConstructionByValue) {
  ntlib::rational r1(5);
  EXPECT_EQ(r1.get_numerator(), 5);
  EXPECT_EQ(r1.get_denominator(), 1);

  ntlib::rational r2(6, 2);
  EXPECT_EQ(r2.get_numerator(), 3);
  EXPECT_EQ(r2.get_denominator(), 1);

  ntlib::rational r3(4, -2);
  EXPECT_EQ(r3.get_numerator(), -2);
  EXPECT_EQ(r3.get_denominator(), 1);
}

TEST(Output, StringRepresentation) {
  ntlib::rational a(5, 2);
  EXPECT_EQ(a.to_string(), "5/2");
  ntlib::rational b(10, 2);
  EXPECT_EQ(b.to_string(), "5/1");
}

TEST(Operators, Comparison) {
  ntlib::rational a(1, 2);
  ntlib::rational b(2, 1);
  ntlib::rational c(2, 4);

  EXPECT_TRUE(a == a);
  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a == c);
  EXPECT_FALSE(b == c);

  EXPECT_FALSE(a != a);
  EXPECT_TRUE(a != b);
  EXPECT_FALSE(a != c);
  EXPECT_FALSE(b != b);

  EXPECT_TRUE(a < b);
  EXPECT_FALSE(a < a);

  EXPECT_TRUE(b > a);
  EXPECT_FALSE(a > a);

  EXPECT_TRUE(a <= a);
  EXPECT_TRUE(a <= b);

  EXPECT_TRUE(a >= a);
  EXPECT_FALSE(a >= b);
}

TEST(Arithmetic, Addition) {
  ntlib::rational a(5, 2);
  ntlib::rational b(7, 2);
  ntlib::rational z(0, 2);

  EXPECT_EQ(a + z, a);
  EXPECT_EQ(a + b, ntlib::rational{6});
  EXPECT_EQ(b + a, ntlib::rational{6});
  a += b;
  EXPECT_EQ(a, ntlib::rational{6});
  a += 5;
  EXPECT_EQ(a, ntlib::rational{11});
  ntlib::rational bb = b;
  b += z;
  EXPECT_EQ(bb, b);
}

TEST(Arithmetic, Subtraction) {
  ntlib::rational a(5, 2);
  ntlib::rational b(7, 2);
  ntlib::rational z(0, 2);

  EXPECT_EQ(a - z, a);
  EXPECT_EQ(a - b, ntlib::rational{-1});
  EXPECT_EQ(b - a, ntlib::rational{1});
  a -= b;
  EXPECT_EQ(a, ntlib::rational{-1});
  a -= 5;
  EXPECT_EQ(a, ntlib::rational{-6});
  ntlib::rational bb = b;
  b -= z;
  EXPECT_EQ(bb, b);
}

TEST(Arithmetic, Multiplication) {
  ntlib::rational a(5, 2);
  ntlib::rational b(4, 5);
  ntlib::rational z(0, 2);

  EXPECT_EQ(a * z, ntlib::rational{0});
  EXPECT_EQ(a * b, b * a);
  EXPECT_EQ(a * b, ntlib::rational{2});
  a *= b;
  EXPECT_EQ(a, ntlib::rational{2});
  b *= 10;
  EXPECT_EQ(b, ntlib::rational{8});
  b *= z;
  EXPECT_EQ(b, z);
}

TEST(Arithmetic, Division) {
  ntlib::rational a(5, 2);
  ntlib::rational b(4, 5);
  ntlib::rational z(0, 2);

  EXPECT_EQ(a / b, ntlib::rational(25, 8));
  EXPECT_EQ(b / a, ntlib::rational(8, 25));
  a /= b;
  EXPECT_EQ(a, ntlib::rational(25, 8));
  b /= 10;
  EXPECT_EQ(b, ntlib::rational(2, 25));
}

TEST(MultiplicativeNeutral, Simple) {
  ntlib::rational a(5, 3);
  auto na = ntlib::get_multiplicative_neutral(a);
  EXPECT_EQ(na, ntlib::rational{1});
}
