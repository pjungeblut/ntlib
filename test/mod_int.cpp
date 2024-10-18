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

TEST(Construction, Simple) {
  ntlib::mod_int<uint32_t> a(0, MOD);
  EXPECT_EQ(a.get(), 0);
  EXPECT_EQ(a.get_modulus(), MOD);
}

TEST(Construction, WithoutModulus) {
  ntlib::mod_int<uint32_t> a(0);
  EXPECT_EQ(a.get(), 0);
  EXPECT_EQ(a.get_modulus(), 1);

  #ifndef NDEBUG
  EXPECT_DEATH({
    std::cerr << "Inside death test." << std::endl;
    ntlib::mod_int<uint32_t> b(1);
  }, "");
  #endif
}

TEST(Construction, ByValue) {
  ntlib::mod_int<uint32_t> a(0, MOD);
  EXPECT_EQ(a.get(), 0);

  ntlib::mod_int<uint32_t> b(MOD - 1, MOD);
  EXPECT_EQ(b.get(), MOD - 1);

  ntlib::mod_int<uint32_t> c(MOD, MOD);
  EXPECT_EQ(c.get(), 0);

  ntlib::mod_int<int32_t> d(-MOD, MOD);
  EXPECT_EQ(d.get(), 0);

  ntlib::mod_int<int32_t> e(1 - MOD, MOD);
  EXPECT_EQ(e.get(), 1 % MOD);
}

TEST(Streaming, Insertion) {
  ntlib::mod_int<uint32_t> a(5, MOD), b(MOD - 1, MOD), c(MOD, MOD);
  std::string result =
      std::to_string(5 % MOD) + " " + std::to_string(MOD - 1) + " 0";
  std::ostringstream out;
  out << a << " " << b << " " << c;
  EXPECT_EQ(out.str(), result);
}

TEST(Increment, Prefix) {
  ntlib::mod_int<uint32_t> a(0, MOD);
  EXPECT_EQ((++a).get(), 1 % MOD);
  EXPECT_EQ(a.get(), 1 % MOD);

  ntlib::mod_int<uint32_t> b(MOD - 1, MOD);
  EXPECT_EQ((++b).get(), 0);
  EXPECT_EQ(b.get(), 0);
}

TEST(Increment, Postfix) {
  ntlib::mod_int<uint32_t> a(0, MOD);
  EXPECT_EQ((a++).get(), 0);
  EXPECT_EQ(a.get(), 1);

  ntlib::mod_int<uint32_t> b(MOD - 1, MOD);
  EXPECT_EQ((b++).get(), MOD - 1);
  EXPECT_EQ(b.get(), 0);
}

TEST(Decrement, Prefix) {
  ntlib::mod_int<uint32_t> a(0, MOD);
  EXPECT_EQ((--a).get(), MOD - 1);
  EXPECT_EQ(a.get(), MOD - 1);

  ntlib::mod_int<uint32_t> b(1, MOD);
  EXPECT_EQ((--b).get(), 0);
  EXPECT_EQ(b.get(), 0);
}

TEST(Decrement, Postfix) {
  ntlib::mod_int<uint32_t> a(0, MOD);
  EXPECT_EQ((a--).get(), 0);
  EXPECT_EQ(a.get(), MOD - 1);

  ntlib::mod_int<uint32_t> b(1, MOD);
  EXPECT_EQ((b--).get(), 1 % MOD);
  EXPECT_EQ(b.get(), 0);
}

TEST(Addition, Compound) {
  ntlib::mod_int<uint32_t> a(0, MOD), b(1, MOD);
  EXPECT_EQ((a += b).get(), 1 % MOD);
  EXPECT_EQ(a.get(), 1 % MOD);
}

TEST(Addition, Symmetric) {
  ntlib::mod_int<uint32_t> a(1, MOD), b(2, MOD);
  EXPECT_EQ((a + b).get(), (b + a).get());
}

TEST(Addition, Chained) {
  ntlib::mod_int<uint32_t> a(1, MOD), b(2, MOD), c(3, MOD);
  EXPECT_EQ((a + b + c).get(), 6 % MOD);
}

TEST(Subtraction, Compound) {
  ntlib::mod_int<uint32_t> a(0, MOD), b(1, MOD);
  EXPECT_EQ((a -= b).get(), MOD - 1);
  EXPECT_EQ(a.get(), MOD - 1);

  ntlib::mod_int<uint32_t> c(MOD - 1, MOD), d(1, MOD);
  EXPECT_EQ((c -= d).get(), (2 * MOD - 2) % MOD);
  EXPECT_EQ(c.get(), (2 * MOD - 2) % MOD);
}

TEST(Subtraction, Basic) {
  ntlib::mod_int<uint32_t> a(1, MOD), b(2, MOD);
  EXPECT_EQ((a - b).get(), MOD - 1);
  EXPECT_EQ((b - a).get(), 1 % MOD);
}

TEST(Subtraction, Chained) {
  static_assert(MOD > 6);
  ntlib::mod_int<uint32_t> a(1, MOD), b(2, MOD), c(3, MOD);
  EXPECT_EQ((a - b - c).get(), 6);
}

TEST(Multiplication, Compound) {
  ntlib::mod_int<uint32_t> a(2, MOD), b(3, MOD);
  EXPECT_EQ((a *= b).get(), 6 % MOD);
  EXPECT_EQ(a.get(), 6 % MOD);
}

TEST(Multiplication, Symmetric) {
  ntlib::mod_int<uint32_t> a(2, MOD), b(3, MOD);
  EXPECT_EQ((a * b).get(), (b * a).get());
}

TEST(Multiplication, Chained) {
  ntlib::mod_int<uint32_t> a(2, MOD), b(3, MOD), c(4, MOD);
  EXPECT_EQ((a * b * c).get(), 24 % MOD);
}

TEST(Comparison, Equality) {
  static_assert(MOD > 2);
  ntlib::mod_int<uint32_t> a(1, MOD), b(2, MOD);
  EXPECT_TRUE(a == a);
  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a + a == b);
}

TEST(Comparison, Inequality) {
  static_assert(MOD > 2);
  ntlib::mod_int<uint32_t> a(1, MOD), b(2, MOD);
  EXPECT_FALSE(a != a);
  EXPECT_TRUE(a != b);
  EXPECT_FALSE(a + a != b);
}

TEST(Inversion, All) {
  for (uint32_t i = 0; i < MOD; ++i) {
    ntlib::mod_int<uint32_t> a(i, MOD);
    if (ntlib::gcd(i, MOD) != 1) { continue; }
    EXPECT_EQ((a * a.invert()).get(), 1 % MOD);
  }
}

TEST(MultiplicativeNeutral, Runtime) {
  ntlib::mod_int<uint32_t> a(5, 10);
  EXPECT_EQ(ntlib::get_multiplicative_neutral(a).get(), 1);

  ntlib::mod_int<uint32_t> b(5, 1);
  EXPECT_EQ(ntlib::get_multiplicative_neutral(b).get(), 0);
}
