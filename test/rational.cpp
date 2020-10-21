#include <gtest/gtest.h>

#include "rational.hpp"

TEST(Initialization, ConstructionByValue) {
  // General tests.
  EXPECT_EQ(ntlib::rational(5), 5);
  EXPECT_EQ(ntlib::rational(6, 2), 3);

  // Negative denominator.
  EXPECT_EQ(ntlib::rational(4, -2), -2);
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
  EXPECT_EQ(a + b, 6);
  EXPECT_EQ(b + a, 6);
  a += b;
  EXPECT_EQ(a, 6);
  a += 5;
  EXPECT_EQ(a, 11);
  b += z;
  EXPECT_EQ(b, b);
}

TEST(Arithmetic, Subtraction) {
  ntlib::rational a(5, 2);
  ntlib::rational b(7, 2);
  ntlib::rational z(0, 2);

  EXPECT_EQ(a - z, a);
  EXPECT_EQ(a - b, -1);
  EXPECT_EQ(b - a, 1);
  a -= b;
  EXPECT_EQ(a, -1);
  a -= 5;
  EXPECT_EQ(a, -6);
  b -= z;
  EXPECT_EQ(b, b);
}

TEST(Arithmetic, Multiplication) {
  ntlib::rational a(5, 2);
  ntlib::rational b(4, 5);
  ntlib::rational z(0, 2);

  EXPECT_EQ(a * z, 0);
  EXPECT_EQ(a * b, b * a);
  EXPECT_EQ(a * b, 2);
  a *= b;
  EXPECT_EQ(a, 2);
  b *= 10;
  EXPECT_EQ(b, 8);
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
