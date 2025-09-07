#include <gtest/gtest.h>

#include <cstdint>
#include <limits>
#include <memory>
#include <utility>

import base;
import mod_int;
import modulo;

using Types = ::testing::Types<
    int8_t, uint8_t,
    int16_t, uint16_t,
    int32_t, uint32_t,
    int64_t, uint64_t>;

template<typename>
class ModInt : public testing::Test {};

TYPED_TEST_SUITE(ModInt, Types);

TYPED_TEST(ModInt, ClassLayout) {
  static_assert(sizeof(ntlib::mod_int<TypeParam, 10>) == sizeof(TypeParam));
}

TYPED_TEST(ModInt, DefaultConstruction) {
  static_assert(
      std::is_default_constructible_v<ntlib::mod_int<TypeParam, 10>>);
  EXPECT_NO_THROW((ntlib::mod_int<TypeParam, 10>{}));
  EXPECT_EQ(static_cast<TypeParam>(ntlib::mod_int<TypeParam, 10>{}), 0);
}

TYPED_TEST(ModInt, ConstructionByValue) {
  const ntlib::mod_int<TypeParam, 10> a {5};
  EXPECT_EQ(static_cast<TypeParam>(a), 5);

  const ntlib::mod_int<TypeParam, 10> b {10};
  EXPECT_EQ(static_cast<TypeParam>(b), 0);

  const ntlib::mod_int<TypeParam, 10> c {15};
  EXPECT_EQ(static_cast<TypeParam>(c), 5);

  if constexpr (std::numeric_limits<TypeParam>::is_signed) {
    const ntlib::mod_int<TypeParam, 10> d {-13};
    EXPECT_EQ(static_cast<TypeParam>(d), 7);
  }
}

TYPED_TEST(ModInt, EqualityComparision) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::mod_int<TypeParam, 10>>() ==
          std::declval<ntlib::mod_int<TypeParam, 10>>()),
      bool>);

  const ntlib::mod_int<TypeParam, 10> a {5};
  const ntlib::mod_int<TypeParam, 10> b {15};
  const ntlib::mod_int<TypeParam, 10> c {6};

  EXPECT_EQ(a, a);
  EXPECT_EQ(a, b);
  EXPECT_NE(a, c);
}

TYPED_TEST(ModInt, PrefixIncrement) {
  static_assert(std::is_same_v<
      decltype(++std::declval<ntlib::mod_int<TypeParam, 10>>()),
      ntlib::mod_int<TypeParam, 10>&>);

  ntlib::mod_int<TypeParam, 10> a {8};

  EXPECT_EQ(static_cast<TypeParam>(++a), 9);

  EXPECT_EQ(static_cast<TypeParam>(++a), 0);
}

TYPED_TEST(ModInt, PostfixIncrement) {
  static_assert(std::is_same_v<
      decltype(std::declval<ntlib::mod_int<TypeParam, 10>>()++),
      ntlib::mod_int<TypeParam, 10>>);

  ntlib::mod_int<TypeParam, 10> a {8};
  
  EXPECT_EQ(static_cast<TypeParam>(a++), 8);
  EXPECT_EQ(static_cast<TypeParam>(a), 9);

  EXPECT_EQ(static_cast<TypeParam>(a++), 9);
  EXPECT_EQ(static_cast<TypeParam>(a), 0);
}

TYPED_TEST(ModInt, PrefixDecrement) {
  static_assert(std::is_same_v<
      decltype(--std::declval<ntlib::mod_int<TypeParam, 10>>()),
      ntlib::mod_int<TypeParam, 10>&>);

  ntlib::mod_int<TypeParam, 10> a {1};

  EXPECT_EQ(static_cast<TypeParam>(--a), 0);

  EXPECT_EQ(static_cast<TypeParam>(--a), 9);
}

TYPED_TEST(ModInt, PostfixDecrement) {
  static_assert(std::is_same_v<
      decltype(std::declval<ntlib::mod_int<TypeParam, 10>>()--),
      ntlib::mod_int<TypeParam, 10>>);

  ntlib::mod_int<TypeParam, 10> a {1};
  
  EXPECT_EQ(static_cast<TypeParam>(a--), 1);
  EXPECT_EQ(static_cast<TypeParam>(a), 0);

  EXPECT_EQ(static_cast<TypeParam>(a--), 0);
  EXPECT_EQ(static_cast<TypeParam>(a), 9);
}

TYPED_TEST(ModInt, CompoundAddition) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::mod_int<TypeParam, 10>>() +=
          std::declval<ntlib::mod_int<TypeParam, 10>>()),
      ntlib::mod_int<TypeParam, 10>&>);

  ntlib::mod_int<TypeParam, 10> a {5};
  const ntlib::mod_int<TypeParam, 10> b {3};

  a += b;
  EXPECT_EQ(static_cast<TypeParam>(a), 8);

  a += b;
  EXPECT_EQ(static_cast<TypeParam>(a), 1);
}

TYPED_TEST(ModInt, Addition) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::mod_int<TypeParam, 10>>() +
          std::declval<ntlib::mod_int<TypeParam, 10>>()),
      ntlib::mod_int<TypeParam, 10>>);

  const ntlib::mod_int<TypeParam, 10> a {5};
  const ntlib::mod_int<TypeParam, 10> b {3};

  const auto c = a + b;
  EXPECT_EQ(static_cast<TypeParam>(c), 8);

  const auto d = c + b;
  EXPECT_EQ(static_cast<TypeParam>(d), 1);
}

TYPED_TEST(ModInt, CompoundSubtraction) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::mod_int<TypeParam, 10>>() -=
          std::declval<ntlib::mod_int<TypeParam, 10>>()),
      ntlib::mod_int<TypeParam, 10>&>);

  ntlib::mod_int<TypeParam, 10> a {5};
  const ntlib::mod_int<TypeParam, 10> b {3};

  a -= b;
  EXPECT_EQ(static_cast<TypeParam>(a), 2);

  a -= b;
  EXPECT_EQ(static_cast<TypeParam>(a), 9);
}

TYPED_TEST(ModInt, Subtraction) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::mod_int<TypeParam, 10>>() -
          std::declval<ntlib::mod_int<TypeParam, 10>>()),
      ntlib::mod_int<TypeParam, 10>>);

  const ntlib::mod_int<TypeParam, 10> a {5};
  const ntlib::mod_int<TypeParam, 10> b {3};

  const auto c = a - b;
  EXPECT_EQ(static_cast<TypeParam>(c), 2);

  const auto d = c - b;
  EXPECT_EQ(static_cast<TypeParam>(d), 9);
}

TYPED_TEST(ModInt, CompoundMultiplication) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::mod_int<TypeParam, 10>>() *=
          std::declval<ntlib::mod_int<TypeParam, 10>>()),
      ntlib::mod_int<TypeParam, 10>&>);

  ntlib::mod_int<TypeParam, 10> a {4};
  const ntlib::mod_int<TypeParam, 10> b {2};

  a *= b;
  EXPECT_EQ(static_cast<TypeParam>(a), 8);

  a *= b;
  EXPECT_EQ(static_cast<TypeParam>(a), 6);
}

TYPED_TEST(ModInt, Multiplication) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::mod_int<TypeParam, 10>>() *
          std::declval<ntlib::mod_int<TypeParam, 10>>()),
      ntlib::mod_int<TypeParam, 10>>);

  const ntlib::mod_int<TypeParam, 10> a {4};
  const ntlib::mod_int<TypeParam, 10> b {2};

  const auto c = a * b;
  EXPECT_EQ(static_cast<TypeParam>(c), 8);

  const auto d = c * b;
  EXPECT_EQ(static_cast<TypeParam>(d), 6);
}

TYPED_TEST(ModInt, UnaryPlus) {
  static_assert(std::is_same_v<
      decltype(+std::declval<ntlib::mod_int<TypeParam, 10>>()),
      ntlib::mod_int<TypeParam, 10>>);
  
  const ntlib::mod_int<TypeParam, 10> a {5};
  EXPECT_EQ(static_cast<TypeParam>(+a), 5);
}

TYPED_TEST(ModInt, UnaryMinus) {
  static_assert(std::is_same_v<
      decltype(-std::declval<ntlib::mod_int<TypeParam, 10>>()),
      ntlib::mod_int<TypeParam, 10>>);
  
  const ntlib::mod_int<TypeParam, 10> a {4};
  EXPECT_EQ(static_cast<TypeParam>(-a), 6);
}

TYPED_TEST(ModInt, Inversion) {
  if constexpr (std::numeric_limits<TypeParam>::is_signed) {
    constexpr TypeParam m = 10;
    for (TypeParam n = 0; n < m; ++n) {
      if (ntlib::gcd(n, m) != 1) { continue; }

      ntlib::mod_int<TypeParam, m> a {n};
      a.invert();
      EXPECT_EQ(
          ntlib::mod(static_cast<TypeParam>(static_cast<TypeParam>(a) * n), m),
          1);
    }
  }
}

TYPED_TEST(ModInt, AdditiveNeutral) {
  const auto n1 = ntlib::zero<ntlib::mod_int<TypeParam, 10>>();
  EXPECT_EQ(static_cast<TypeParam>(n1), 0);
}

TYPED_TEST(ModInt, MultiplicativeNeutral) {
  const auto n1 = ntlib::one<ntlib::mod_int<TypeParam, 10>>();
  EXPECT_EQ(static_cast<TypeParam>(n1), 1);

  const auto n2 = ntlib::one<ntlib::mod_int<TypeParam, 1>>();
  EXPECT_EQ(static_cast<TypeParam>(n2), 0);
}
