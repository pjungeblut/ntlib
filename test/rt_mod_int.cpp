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
class RunTimeModInt : public testing::Test {};

TYPED_TEST_SUITE(RunTimeModInt, Types);

TYPED_TEST(RunTimeModInt, ClassLayout) {
  static_assert(sizeof(ntlib::rt_mod_int<TypeParam>) == 2 * sizeof(TypeParam));
}

TYPED_TEST(RunTimeModInt, DefaultConstruction) {
  static_assert(std::is_default_constructible_v<ntlib::rt_mod_int<TypeParam>>);
  EXPECT_NO_THROW(ntlib::rt_mod_int<TypeParam>{});
}

TYPED_TEST(RunTimeModInt, ConstructionByValueAndModulus) {
  const ntlib::rt_mod_int<TypeParam> a {5, 10};
  EXPECT_EQ(static_cast<TypeParam>(a), 5);
  EXPECT_EQ(a.get_modulus(), 10);

  const ntlib::rt_mod_int<TypeParam> b {10, 10};
  EXPECT_EQ(static_cast<TypeParam>(b), 0);
  EXPECT_EQ(b.get_modulus(), 10);

  const ntlib::rt_mod_int<TypeParam> c {15, 10};
  EXPECT_EQ(static_cast<TypeParam>(c), 5);
  EXPECT_EQ(c.get_modulus(), 10);

  if constexpr (std::is_signed_v<TypeParam>) {
    const ntlib::rt_mod_int<TypeParam> d {-5, 10};
    EXPECT_EQ(static_cast<TypeParam>(d), 5);
    EXPECT_EQ(d.get_modulus(), 10);
  }
}

TYPED_TEST(RunTimeModInt, CopyConstruction) {
  const ntlib::rt_mod_int<TypeParam> a {5, 10};
  const ntlib::rt_mod_int<TypeParam> b {a};
  EXPECT_EQ(static_cast<TypeParam>(b), 5);
  EXPECT_EQ(b.get_modulus(), 10);
}

TYPED_TEST(RunTimeModInt, CopyAssignment) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::rt_mod_int<TypeParam>>() =
          std::declval<ntlib::rt_mod_int<TypeParam>>()),
      ntlib::rt_mod_int<TypeParam>&>);

  const ntlib::rt_mod_int<TypeParam> a {5, 10};
  const ntlib::rt_mod_int<TypeParam> b = a;
  EXPECT_EQ(static_cast<TypeParam>(b), 5);
  EXPECT_EQ(b.get_modulus(), 10);
}

TYPED_TEST(RunTimeModInt, MoveConstruction) {
  ntlib::rt_mod_int<TypeParam> a {5, 10};
  const ntlib::rt_mod_int<TypeParam> b {std::move(a)};
  EXPECT_EQ(static_cast<TypeParam>(b), 5);
  EXPECT_EQ(b.get_modulus(), 10);
}

TYPED_TEST(RunTimeModInt, MoveAssignment) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::rt_mod_int<TypeParam>>() =
          std::move(std::declval<ntlib::rt_mod_int<TypeParam>>())),
      ntlib::rt_mod_int<TypeParam>&>);

  ntlib::rt_mod_int<TypeParam> a {5, 10};
  const ntlib::rt_mod_int<TypeParam> b = std::move(a);
  EXPECT_EQ(static_cast<TypeParam>(b), 5);
  EXPECT_EQ(b.get_modulus(), 10);
}

TYPED_TEST(RunTimeModInt, EqualityComparision) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::rt_mod_int<TypeParam>>() ==
          std::declval<ntlib::rt_mod_int<TypeParam>>()),
      bool>);

  const ntlib::rt_mod_int<TypeParam> a {5, 10};
  const ntlib::rt_mod_int<TypeParam> b {5, 10};
  const ntlib::rt_mod_int<TypeParam> c {6, 10};
  const ntlib::rt_mod_int<TypeParam> d {5, 11};

  EXPECT_EQ(a, a);
  EXPECT_EQ(a, b);
  EXPECT_NE(a, c);
  EXPECT_NE(a, d);
}

TYPED_TEST(RunTimeModInt, PrefixIncrement) {
  static_assert(std::is_same_v<
      decltype(++std::declval<ntlib::rt_mod_int<TypeParam>>()),
      ntlib::rt_mod_int<TypeParam>&>);

  ntlib::rt_mod_int<TypeParam> a {8, 10};

  EXPECT_EQ(static_cast<TypeParam>(++a), 9);
  EXPECT_EQ(a.get_modulus(), 10);

  EXPECT_EQ(static_cast<TypeParam>(++a), 0);
  EXPECT_EQ(a.get_modulus(), 10);
}

TYPED_TEST(RunTimeModInt, PostfixIncrement) {
  static_assert(std::is_same_v<
      decltype(std::declval<ntlib::rt_mod_int<TypeParam>>()++),
      ntlib::rt_mod_int<TypeParam>>);

  ntlib::rt_mod_int<TypeParam> a {8, 10};
  
  EXPECT_EQ(static_cast<TypeParam>(a++), 8);
  EXPECT_EQ(static_cast<TypeParam>(a), 9);
  EXPECT_EQ(a.get_modulus(), 10);

  EXPECT_EQ(static_cast<TypeParam>(a++), 9);
  EXPECT_EQ(static_cast<TypeParam>(a), 0);
  EXPECT_EQ(a.get_modulus(), 10);
}

TYPED_TEST(RunTimeModInt, PrefixDecrement) {
  static_assert(std::is_same_v<
      decltype(--std::declval<ntlib::rt_mod_int<TypeParam>>()),
      ntlib::rt_mod_int<TypeParam>&>);
      
  ntlib::rt_mod_int<TypeParam> a {1, 10};

  EXPECT_EQ(static_cast<TypeParam>(--a), 0);
  EXPECT_EQ(a.get_modulus(), 10);

  EXPECT_EQ(static_cast<TypeParam>(--a), 9);
  EXPECT_EQ(a.get_modulus(), 10);
}

TYPED_TEST(RunTimeModInt, PostfixDecrement) {
  static_assert(std::is_same_v<
      decltype(std::declval<ntlib::rt_mod_int<TypeParam>>()--),
      ntlib::rt_mod_int<TypeParam>>);

  ntlib::rt_mod_int<TypeParam> a {1, 10};
  
  EXPECT_EQ(static_cast<TypeParam>(a--), 1);
  EXPECT_EQ(static_cast<TypeParam>(a), 0);
  EXPECT_EQ(a.get_modulus(), 10);

  EXPECT_EQ(static_cast<TypeParam>(a--), 0);
  EXPECT_EQ(static_cast<TypeParam>(a), 9);
  EXPECT_EQ(a.get_modulus(), 10);
}

TYPED_TEST(RunTimeModInt, CompoundAddition) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::rt_mod_int<TypeParam>>() +=
          std::declval<ntlib::rt_mod_int<TypeParam>>()),
      ntlib::rt_mod_int<TypeParam>&>);

  ntlib::rt_mod_int<TypeParam> a {5, 10};
  const ntlib::rt_mod_int<TypeParam> b {3, 10};

  a += b;
  EXPECT_EQ(static_cast<TypeParam>(a), 8);
  EXPECT_EQ(a.get_modulus(), 10);

  a += b;
  EXPECT_EQ(static_cast<TypeParam>(a), 1);
  EXPECT_EQ(a.get_modulus(), 10);
}

TYPED_TEST(RunTimeModInt, Addition) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::rt_mod_int<TypeParam>>() +
          std::declval<ntlib::rt_mod_int<TypeParam>>()),
      ntlib::rt_mod_int<TypeParam>>);

  const ntlib::rt_mod_int<TypeParam> a {5, 10};
  const ntlib::rt_mod_int<TypeParam> b {3, 10};

  const auto c = a + b;
  EXPECT_EQ(static_cast<TypeParam>(c), 8);
  EXPECT_EQ(c.get_modulus(), 10);

  const auto d = c + b;
  EXPECT_EQ(static_cast<TypeParam>(d), 1);
  EXPECT_EQ(d.get_modulus(), 10);
}

TYPED_TEST(RunTimeModInt, CompoundSubtraction) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::rt_mod_int<TypeParam>>() -=
          std::declval<ntlib::rt_mod_int<TypeParam>>()),
      ntlib::rt_mod_int<TypeParam>&>);

  ntlib::rt_mod_int<TypeParam> a {5, 10};
  const ntlib::rt_mod_int<TypeParam> b {3, 10};

  a -= b;
  EXPECT_EQ(static_cast<TypeParam>(a), 2);
  EXPECT_EQ(a.get_modulus(), 10);

  a -= b;
  EXPECT_EQ(static_cast<TypeParam>(a), 9);
  EXPECT_EQ(a.get_modulus(), 10);
}

TYPED_TEST(RunTimeModInt, Subtraction) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::rt_mod_int<TypeParam>>() -
          std::declval<ntlib::rt_mod_int<TypeParam>>()),
      ntlib::rt_mod_int<TypeParam>>);

  const ntlib::rt_mod_int<TypeParam> a {5, 10};
  const ntlib::rt_mod_int<TypeParam> b {3, 10};

  const auto c = a - b;
  EXPECT_EQ(static_cast<TypeParam>(c), 2);
  EXPECT_EQ(c.get_modulus(), 10);

  const auto d = c - b;
  EXPECT_EQ(static_cast<TypeParam>(d), 9);
  EXPECT_EQ(d.get_modulus(), 10);
}

TYPED_TEST(RunTimeModInt, CompoundMultiplication) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::rt_mod_int<TypeParam>>() *=
          std::declval<ntlib::rt_mod_int<TypeParam>>()),
      ntlib::rt_mod_int<TypeParam>&>);

  ntlib::rt_mod_int<TypeParam> a {4, 10};
  const ntlib::rt_mod_int<TypeParam> b {2, 10};

  a *= b;
  EXPECT_EQ(static_cast<TypeParam>(a), 8);
  EXPECT_EQ(a.get_modulus(), 10);

  a *= b;
  EXPECT_EQ(static_cast<TypeParam>(a), 6);
  EXPECT_EQ(a.get_modulus(), 10);
}

TYPED_TEST(RunTimeModInt, Multiplication) {
  static_assert(std::is_same_v<
      decltype(
          std::declval<ntlib::rt_mod_int<TypeParam>>() *
          std::declval<ntlib::rt_mod_int<TypeParam>>()),
      ntlib::rt_mod_int<TypeParam>>);

  const ntlib::rt_mod_int<TypeParam> a {4, 10};
  const ntlib::rt_mod_int<TypeParam> b {2, 10};

  const auto c = a * b;
  EXPECT_EQ(static_cast<TypeParam>(c), 8);
  EXPECT_EQ(c.get_modulus(), 10);

  const auto d = c * b;
  EXPECT_EQ(static_cast<TypeParam>(d), 6);
  EXPECT_EQ(d.get_modulus(), 10);
}

TYPED_TEST(RunTimeModInt, UnaryPlus) {
  static_assert(std::is_same_v<
      decltype(+std::declval<ntlib::rt_mod_int<TypeParam>>()),
      ntlib::rt_mod_int<TypeParam>>);
  
  const ntlib::rt_mod_int<TypeParam> a {5, 10};
  EXPECT_EQ(static_cast<TypeParam>(+a), 5);
  EXPECT_EQ((+a).get_modulus(), 10);
}

TYPED_TEST(RunTimeModInt, UnaryMinus) {
  static_assert(std::is_same_v<
      decltype(-std::declval<ntlib::rt_mod_int<TypeParam>>()),
      ntlib::rt_mod_int<TypeParam>>);
  
  const ntlib::rt_mod_int<TypeParam> a {4, 10};
  EXPECT_EQ(static_cast<TypeParam>(-a), 6);
  EXPECT_EQ((-a).get_modulus(), 10);
}

TYPED_TEST(RunTimeModInt, Inversion) {
  const TypeParam m = 10;
  for (TypeParam n = 0; n < m; ++n) {
    if (ntlib::gcd(n, m) != 1) { continue; }

    ntlib::rt_mod_int<TypeParam> a {n, m};
    a.invert();
    EXPECT_EQ(
        ntlib::mod(static_cast<TypeParam>(static_cast<TypeParam>(a) * n), m),
        1);
    EXPECT_EQ(a.get_modulus(), m);
  }
}

TYPED_TEST(RunTimeModInt, MultiplicativeNeutral) {
  static_assert(std::is_same_v<
      decltype(get_multiplicative_neutral(
          std::declval<ntlib::rt_mod_int<TypeParam>>())),
      ntlib::rt_mod_int<TypeParam>>);

  const ntlib::rt_mod_int<TypeParam> a {5, 10};
  const auto mna = get_multiplicative_neutral(a);
  EXPECT_EQ(static_cast<TypeParam>(mna), 1);
  EXPECT_EQ(mna.get_modulus(), 10);

  const ntlib::rt_mod_int<TypeParam> b {5, 1};
  const auto mnb = get_multiplicative_neutral(b);
  EXPECT_EQ(static_cast<TypeParam>(mnb), 0);
  EXPECT_EQ(mnb.get_modulus(), 1);
}
