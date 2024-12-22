#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>

import base;
import mod_int;
import modulo;

using Types = ::testing::Types<int8_t, uint8_t, int16_t, uint16_t, int32_t,
    uint32_t, int64_t, uint64_t>;

template<typename T>
class CompileTimeModInt : public testing::Test {};

TYPED_TEST_SUITE(CompileTimeModInt, Types);

TYPED_TEST(CompileTimeModInt, ClassLayout) {
  static_assert(sizeof(ntlib::ct_mod_int<TypeParam, 10>) == sizeof(TypeParam));
}

TYPED_TEST(CompileTimeModInt, DefaultConstruction) {
  static_assert(
      std::is_default_constructible_v<ntlib::ct_mod_int<TypeParam, 10>>);
  EXPECT_NO_THROW((ntlib::ct_mod_int<TypeParam, 10>{}));
}

TYPED_TEST(CompileTimeModInt, ConstructionByValueAndModulus) {
  const ntlib::ct_mod_int<TypeParam, 10> a {5};
  EXPECT_EQ(static_cast<TypeParam>(a), 5);
  EXPECT_EQ(a.get_modulus(), 10);

  const ntlib::ct_mod_int<TypeParam, 10> b {10};
  EXPECT_EQ(static_cast<TypeParam>(b), 0);
  EXPECT_EQ(b.get_modulus(), 10);

  const ntlib::ct_mod_int<TypeParam, 10> c {15};
  EXPECT_EQ(static_cast<TypeParam>(c), 5);
  EXPECT_EQ(c.get_modulus(), 10);

  if constexpr (std::is_signed_v<TypeParam>) {
    const ntlib::ct_mod_int<TypeParam, 10> d {-5};
    EXPECT_EQ(static_cast<TypeParam>(d), 5);
    EXPECT_EQ(d.get_modulus(), 10);
  }
}

TYPED_TEST(CompileTimeModInt, CopyConstruction) {
  const ntlib::ct_mod_int<TypeParam, 10> a {5};
  const auto b {a};
  EXPECT_EQ(static_cast<TypeParam>(b), 5);
  EXPECT_EQ(b.get_modulus(), 10);
}

TYPED_TEST(CompileTimeModInt, CopyAssignment) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>() =
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>()),
      ntlib::ct_mod_int<TypeParam, 10>&>);

  const ntlib::ct_mod_int<TypeParam, 10> a {5};
  const auto b = a;
  EXPECT_EQ(static_cast<TypeParam>(b), 5);
  EXPECT_EQ(b.get_modulus(), 10);
}

TYPED_TEST(CompileTimeModInt, MoveConstruction) {
  ntlib::ct_mod_int<TypeParam, 10> a {5};
  const auto b {std::move(a)};
  EXPECT_EQ(static_cast<TypeParam>(b), 5);
  EXPECT_EQ(b.get_modulus(), 10);
}

TYPED_TEST(CompileTimeModInt, MoveAssignment) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>() =
          std::move(std::declval<ntlib::ct_mod_int<TypeParam, 10>>())),
      ntlib::ct_mod_int<TypeParam, 10>&>);

  ntlib::ct_mod_int<TypeParam, 10> a {5};
  const auto b = std::move(a);
  EXPECT_EQ(static_cast<TypeParam>(b), 5);
  EXPECT_EQ(b.get_modulus(), 10);
}

TYPED_TEST(CompileTimeModInt, EqualityComparision) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>() ==
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>()),
      bool>);

  const ntlib::ct_mod_int<TypeParam, 10> a {5};
  const ntlib::ct_mod_int<TypeParam, 10> b {5};
  const ntlib::ct_mod_int<TypeParam, 10> c {6};

  EXPECT_EQ(a, a);
  EXPECT_EQ(a, b);
  EXPECT_NE(a, c);
}

TYPED_TEST(CompileTimeModInt, PrefixIncrement) {
  static_assert(std::is_same_v<
      decltype(++std::declval<ntlib::ct_mod_int<TypeParam, 10>>()),
      ntlib::ct_mod_int<TypeParam, 10>&>);

  ntlib::ct_mod_int<TypeParam, 10> a {8};

  EXPECT_EQ(static_cast<TypeParam>(++a), 9);
  EXPECT_EQ(a.get_modulus(), 10);

  EXPECT_EQ(static_cast<TypeParam>(++a), 0);
  EXPECT_EQ(a.get_modulus(), 10);
}

TYPED_TEST(CompileTimeModInt, PostfixIncrement) {
  static_assert(std::is_same_v<
      decltype(std::declval<ntlib::ct_mod_int<TypeParam, 10>>()++),
      ntlib::ct_mod_int<TypeParam, 10>>);

  ntlib::ct_mod_int<TypeParam, 10> a {8};
  
  EXPECT_EQ(static_cast<TypeParam>(a++), 8);
  EXPECT_EQ(static_cast<TypeParam>(a), 9);
  EXPECT_EQ(a.get_modulus(), 10);

  EXPECT_EQ(static_cast<TypeParam>(a++), 9);
  EXPECT_EQ(static_cast<TypeParam>(a), 0);
  EXPECT_EQ(a.get_modulus(), 10);
}

TYPED_TEST(CompileTimeModInt, PrefixDecrement) {
  static_assert(std::is_same_v<
      decltype(--std::declval<ntlib::ct_mod_int<TypeParam, 10>>()),
      ntlib::ct_mod_int<TypeParam, 10>&>);

  ntlib::ct_mod_int<TypeParam, 10> a {1};

  EXPECT_EQ(static_cast<TypeParam>(--a), 0);
  EXPECT_EQ(a.get_modulus(), 10);

  EXPECT_EQ(static_cast<TypeParam>(--a), 9);
  EXPECT_EQ(a.get_modulus(), 10);
}

TYPED_TEST(CompileTimeModInt, PostfixDecrement) {
  static_assert(std::is_same_v<
      decltype(std::declval<ntlib::ct_mod_int<TypeParam, 10>>()--),
      ntlib::ct_mod_int<TypeParam, 10>>);

  ntlib::ct_mod_int<TypeParam, 10> a {1};
  
  EXPECT_EQ(static_cast<TypeParam>(a--), 1);
  EXPECT_EQ(static_cast<TypeParam>(a), 0);
  EXPECT_EQ(a.get_modulus(), 10);

  EXPECT_EQ(static_cast<TypeParam>(a--), 0);
  EXPECT_EQ(static_cast<TypeParam>(a), 9);
  EXPECT_EQ(a.get_modulus(), 10);
}

TYPED_TEST(CompileTimeModInt, CompoundAddition) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>() +=
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>()),
      ntlib::ct_mod_int<TypeParam, 10>&>);

  ntlib::ct_mod_int<TypeParam, 10> a {5};
  const ntlib::ct_mod_int<TypeParam, 10> b {3};

  a += b;
  EXPECT_EQ(static_cast<TypeParam>(a), 8);
  EXPECT_EQ(a.get_modulus(), 10);

  a += b;
  EXPECT_EQ(static_cast<TypeParam>(a), 1);
  EXPECT_EQ(a.get_modulus(), 10);
}

TYPED_TEST(CompileTimeModInt, Addition) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>() +
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>()),
      ntlib::ct_mod_int<TypeParam, 10>>);

  const ntlib::ct_mod_int<TypeParam, 10> a {5};
  const ntlib::ct_mod_int<TypeParam, 10> b {3};

  const auto c = a + b;
  EXPECT_EQ(static_cast<TypeParam>(c), 8);
  EXPECT_EQ(c.get_modulus(), 10);

  const auto d = c + b;
  EXPECT_EQ(static_cast<TypeParam>(d), 1);
  EXPECT_EQ(d.get_modulus(), 10);
}

TYPED_TEST(CompileTimeModInt, CompoundSubtraction) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>() -=
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>()),
      ntlib::ct_mod_int<TypeParam, 10>&>);

  ntlib::ct_mod_int<TypeParam, 10> a {5};
  const ntlib::ct_mod_int<TypeParam, 10> b {3};

  a -= b;
  EXPECT_EQ(static_cast<TypeParam>(a), 2);
  EXPECT_EQ(a.get_modulus(), 10);

  a -= b;
  EXPECT_EQ(static_cast<TypeParam>(a), 9);
  EXPECT_EQ(a.get_modulus(), 10);
}

TYPED_TEST(CompileTimeModInt, Subtraction) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>() -
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>()),
      ntlib::ct_mod_int<TypeParam, 10>>);

  const ntlib::ct_mod_int<TypeParam, 10> a {5};
  const ntlib::ct_mod_int<TypeParam, 10> b {3};

  const auto c = a - b;
  EXPECT_EQ(static_cast<TypeParam>(c), 2);
  EXPECT_EQ(c.get_modulus(), 10);

  const auto d = c - b;
  EXPECT_EQ(static_cast<TypeParam>(d), 9);
  EXPECT_EQ(d.get_modulus(), 10);
}

TYPED_TEST(CompileTimeModInt, CompoundMultiplication) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>() *=
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>()),
      ntlib::ct_mod_int<TypeParam, 10>&>);

  ntlib::ct_mod_int<TypeParam, 10> a {4};
  const ntlib::ct_mod_int<TypeParam, 10> b {2};

  a *= b;
  EXPECT_EQ(static_cast<TypeParam>(a), 8);
  EXPECT_EQ(a.get_modulus(), 10);

  a *= b;
  EXPECT_EQ(static_cast<TypeParam>(a), 6);
  EXPECT_EQ(a.get_modulus(), 10);
}

TYPED_TEST(CompileTimeModInt, Multiplication) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>() *
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>()),
      ntlib::ct_mod_int<TypeParam, 10>>);

  const ntlib::ct_mod_int<TypeParam, 10> a {4};
  const ntlib::ct_mod_int<TypeParam, 10> b {2};

  const auto c = a * b;
  EXPECT_EQ(static_cast<TypeParam>(c), 8);
  EXPECT_EQ(c.get_modulus(), 10);

  const auto d = c * b;
  EXPECT_EQ(static_cast<TypeParam>(d), 6);
  EXPECT_EQ(d.get_modulus(), 10);
}

TYPED_TEST(CompileTimeModInt, UnaryPlus) {
  static_assert(std::is_same_v<
      decltype(+std::declval<ntlib::ct_mod_int<TypeParam, 10>>()),
      ntlib::ct_mod_int<TypeParam, 10>>);
  
  const ntlib::ct_mod_int<TypeParam, 10> a {5};
  EXPECT_EQ(static_cast<TypeParam>(+a), 5);
  EXPECT_EQ((+a).get_modulus(), 10);
}

TYPED_TEST(CompileTimeModInt, UnaryMinus) {
  static_assert(std::is_same_v<
      decltype(-std::declval<ntlib::ct_mod_int<TypeParam, 10>>()),
      ntlib::ct_mod_int<TypeParam, 10>>);
  
  const ntlib::ct_mod_int<TypeParam, 10> a {4};
  EXPECT_EQ(static_cast<TypeParam>(-a), 6);
  EXPECT_EQ((-a).get_modulus(), 10);
}

TYPED_TEST(CompileTimeModInt, Inversion) {
  constexpr TypeParam m = 10;
  for (TypeParam n = 0; n < m; ++n) {
    if (ntlib::gcd(n, m) != 1) { continue; }

    ntlib::ct_mod_int<TypeParam, m> a {n};
    a.invert();
    EXPECT_EQ(
        ntlib::mod(static_cast<TypeParam>(static_cast<TypeParam>(a) * n), m),
        1);
    EXPECT_EQ(a.get_modulus(), m);
  }
}

TYPED_TEST(CompileTimeModInt, MultiplicativeNeutral) {
  static_assert(std::is_same_v<
      decltype(get_multiplicative_neutral(
          std::declval<ntlib::ct_mod_int<TypeParam, 10>>())),
      ntlib::ct_mod_int<TypeParam, 10>>);

  const ntlib::ct_mod_int<TypeParam, 10> a {5};
  const auto mna = get_multiplicative_neutral(a);
  EXPECT_EQ(static_cast<TypeParam>(mna), 1);
  EXPECT_EQ(mna.get_modulus(), 10);

  const ntlib::ct_mod_int<TypeParam, 1> b {5};
  const auto mnb = get_multiplicative_neutral(b);
  EXPECT_EQ(static_cast<TypeParam>(mnb), 0);
  EXPECT_EQ(mnb.get_modulus(), 1);
}
