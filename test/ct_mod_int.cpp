#include <gtest/gtest.h>

#include <cstdint>
#include <sstream>
#include <string>

import base;
import mod_int;

// Must be at least 1.
static constexpr uint32_t MOD = 10;

TEST(Check, CompileTimeContants) {
  static_assert(MOD > 0);
}

TEST(Construction, Default) {
  ntlib::mod_int<uint32_t, MOD> a;
  EXPECT_EQ(a.get(), 0);
}

TEST(Construction, ByValue) {
  ntlib::mod_int<uint32_t, MOD> a(0);
  EXPECT_EQ(a.get(), 0);

  ntlib::mod_int<uint32_t, MOD> b(MOD - 1);
  EXPECT_EQ(b.get(), MOD - 1);

  ntlib::mod_int<uint32_t, MOD> c(MOD);
  EXPECT_EQ(c.get(), 0);

  ntlib::mod_int<int32_t, MOD> d(-MOD);
  EXPECT_EQ(d.get(), 0);

  ntlib::mod_int<int32_t, MOD> e(1 - MOD);
  EXPECT_EQ(e.get(), 1 % MOD);
}

TEST(Streaming, Insertion) {
  ntlib::mod_int<uint32_t, MOD> a(5), b(MOD - 1), c(MOD);
  std::string result =
      std::to_string(5 % MOD) + " " + std::to_string(MOD - 1) + " 0";
  std::ostringstream out;
  out << a << " " << b << " " << c;
  EXPECT_EQ(out.str(), result);
}

TEST(Streaming, Extraction) {
  std::istringstream in(
      "5 " + std::to_string(MOD - 1) + " " + std::to_string(MOD));
  ntlib::mod_int<uint32_t, MOD> a, b, c;
  in >> a >> b >> c;
  EXPECT_EQ(a.get(), 5 % MOD);
  EXPECT_EQ(b.get(), MOD - 1);
  EXPECT_EQ(c.get(), 0);
}

TEST(Increment, Prefix) {
  ntlib::mod_int<uint32_t, MOD> a(0);
  EXPECT_EQ((++a).get(), 1 % MOD);
  EXPECT_EQ(a.get(), 1 % MOD);

  ntlib::mod_int<uint32_t, MOD> b(MOD - 1);
  EXPECT_EQ((++b).get(), 0);
  EXPECT_EQ(b.get(), 0);
}

TEST(Increment, Postfix) {
  ntlib::mod_int<uint32_t, MOD> a(0);
  EXPECT_EQ((a++).get(), 0);
  EXPECT_EQ(a.get(), 1);

  ntlib::mod_int<uint32_t, MOD> b(MOD - 1);
  EXPECT_EQ((b++).get(), MOD - 1);
  EXPECT_EQ(b.get(), 0);
}

TEST(Decrement, Prefix) {
  ntlib::mod_int<uint32_t, MOD> a(0);
  EXPECT_EQ((--a).get(), MOD - 1);
  EXPECT_EQ(a.get(), MOD - 1);

  ntlib::mod_int<uint32_t, MOD> b(1);
  EXPECT_EQ((--b).get(), 0);
  EXPECT_EQ(b.get(), 0);
}

TEST(Decrement, Postfix) {
  ntlib::mod_int<uint32_t, MOD> a(0);
  EXPECT_EQ((a--).get(), 0);
  EXPECT_EQ(a.get(), MOD - 1);

  ntlib::mod_int<uint32_t, MOD> b(1);
  EXPECT_EQ((b--).get(), 1 % MOD);
  EXPECT_EQ(b.get(), 0);
}

TEST(Addition, Compound) {
  ntlib::mod_int<uint32_t, MOD> a(0), b(1);
  EXPECT_EQ((a += b).get(), 1 % MOD);
  EXPECT_EQ(a.get(), 1 % MOD);
}

TEST(Addition, Symmetric) {
  ntlib::mod_int<uint32_t, MOD> a(1), b(2);
  EXPECT_EQ((a + b).get(), (b + a).get());
}

TEST(Addition, Chained) {
  ntlib::mod_int<uint32_t, MOD> a(1), b(2), c(3);
  EXPECT_EQ((a + b + c).get(), 6 % MOD);
}

TEST(Addition, WithInts) {
  ntlib::mod_int<uint32_t, MOD> a(1);
  EXPECT_EQ((a + 4).get(), 5 % MOD);
  EXPECT_EQ((4 + a).get(), 5 % MOD);
}

TEST(Subtraction, Compound) {
  ntlib::mod_int<uint32_t, MOD> a(0), b(1);
  EXPECT_EQ((a -= b).get(), MOD - 1);
  EXPECT_EQ(a.get(), MOD - 1);

  ntlib::mod_int<uint32_t, MOD> c(MOD - 1), d(1);
  EXPECT_EQ((c -= d).get(), (2 * MOD - 2) % MOD);
  EXPECT_EQ(c.get(), (2 * MOD - 2) % MOD);
}

TEST(Subtraction, Basic) {
  ntlib::mod_int<uint32_t, MOD> a(1), b(2);
  EXPECT_EQ((a - b).get(), MOD - 1);
  EXPECT_EQ((b - a).get(), 1 % MOD);
}

TEST(Subtraction, Chained) {
  static_assert(MOD > 6);
  ntlib::mod_int<uint32_t, MOD> a(1), b(2), c(3);
  EXPECT_EQ((a - b - c).get(), 6);
}

TEST(Subtraction, WithInts) {
  static_assert(MOD > 7);
  ntlib::mod_int<uint32_t, MOD> a(1);
  EXPECT_EQ((a - 4).get(), 7);
  EXPECT_EQ((4 - a).get(), 3 % MOD);
}

TEST(Multiplication, Compound) {
  ntlib::mod_int<uint32_t, MOD> a(2), b(3);
  EXPECT_EQ((a *= b).get(), 6 % MOD);
  EXPECT_EQ(a.get(), 6 % MOD);
}

TEST(Multiplication, Symmetric) {
  ntlib::mod_int<uint32_t, MOD> a(2), b(3);
  EXPECT_EQ((a * b).get(), (b * a).get());
}

TEST(Multiplication, Chained) {
  ntlib::mod_int<uint32_t, MOD> a(2), b(3), c(4);
  EXPECT_EQ((a * b * c).get(), 24 % MOD);
}

TEST(Multiplication, WithInts) {
  ntlib::mod_int<uint32_t, MOD> a(2);
  EXPECT_EQ((a * 3).get(), 6 % MOD);
  EXPECT_EQ((3 * a).get(), 6 % MOD);
}

TEST(Comparison, Equality) {
  static_assert(MOD > 2);
  ntlib::mod_int<uint32_t, MOD> a(1), b(2);
  EXPECT_TRUE(a == a);
  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a + a == b);
}

TEST(Comparison, Inequality) {
  static_assert(MOD > 2);
  ntlib::mod_int<uint32_t, MOD> a(1), b(2);
  EXPECT_FALSE(a != a);
  EXPECT_TRUE(a != b);
  EXPECT_FALSE(a + a != b);
}

TEST(Inversion, All) {
  for (uint32_t i = 0; i < MOD; ++i) {
    ntlib::mod_int<uint32_t, MOD> a(i);
    if (ntlib::gcd(i, MOD) != 1) { continue; }
    EXPECT_EQ(a * a.invert(), 1 % MOD);
  }
}

TEST(MultiplicativeNeutral, Compiletime) {
  ntlib::mod_int<uint32_t, 10> a(5);
  EXPECT_EQ(ntlib::get_multiplicative_neutral(a).get(), 1);

  ntlib::mod_int<uint32_t, 1> b(5);
  EXPECT_EQ(ntlib::get_multiplicative_neutral(b).get(), 0);
}