#include <gtest/gtest.h>

#include "sieve.hpp"

const static std::size_t capacity = 1'000;

static ntlib::sieve<> get_dummy_sieve(std::size_t N) {
  ntlib::sieve sieve(N);
  for (std::size_t i = 0; i < N; ++i) sieve[i] = i & 1;
  return sieve;
}

static const ntlib::sieve dummy = get_dummy_sieve(capacity);

TEST(Construction, DefaultConstruction) {
  ntlib::sieve sieve;
  EXPECT_EQ(sieve.size(), 0);
  EXPECT_TRUE(sieve.empty());
  EXPECT_EQ(sieve.data(), nullptr);
}

TEST(Construction, BySize) {
  ntlib::sieve sieve(capacity);
  EXPECT_EQ(sieve.size(), capacity);
}

TEST(Construction, BySizeEmpty) {
  ntlib::sieve sieve(0);
  EXPECT_TRUE(sieve.empty());
  EXPECT_EQ(sieve.data(), nullptr);
}

TEST(Construction, CopyConstruction) {
  ntlib::sieve s1 = get_dummy_sieve(capacity);
  ntlib::sieve s2(s1);
  EXPECT_EQ(s1.size(), capacity);
  EXPECT_EQ(s2.size(), capacity);
  for (std::size_t i = 0; i < capacity; ++i) EXPECT_EQ(s1[i], s2[i]);
}

TEST(Construction, CopyConstructionEmpty) {
  ntlib::sieve s1;
  ntlib::sieve s2(s1);
  EXPECT_TRUE(s2.empty());
  EXPECT_EQ(s2.data(), nullptr);
}

TEST(Construction, MoveConstruction) {
  ntlib::sieve s1 = get_dummy_sieve(capacity);
  ntlib::sieve s2(std::move(s1));
  EXPECT_TRUE(s1.empty());
  EXPECT_EQ(s1.data(), nullptr);
  EXPECT_EQ(s2.size(), capacity);
  for (std::size_t i = 0; i < capacity; ++i) EXPECT_EQ(dummy[i], s2[i]);
}

TEST(Assignment, CopyAssignment) {
  ntlib::sieve s1 = get_dummy_sieve(capacity);
  ntlib::sieve s2;
  s2 = s1;
  EXPECT_EQ(s2.size(), capacity);
  if (capacity) {
    EXPECT_NE(s1.data(), s2.data());
  }
  for (std::size_t i = 0; i < capacity; ++i) EXPECT_EQ(s1[i], s2[i]);
}

TEST(Assignment, SelfAssignment) {
  ntlib::sieve s = get_dummy_sieve(capacity);

  #if defined(__clang__)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wself-assign-overloaded"
  #endif

  s = s;

  #if defined(__clang__)
  #pragma GCC diagnostic pop
  #endif

  EXPECT_EQ(s.size(), capacity);
  for (std::size_t i = 0; i < capacity; ++i) EXPECT_EQ(dummy[i], s[i]);
}

TEST(Assignment, AssignEmptyFromEmpty) {
  ntlib::sieve s1;
  ntlib::sieve s2;
  s2 = s1;
  EXPECT_TRUE(s2.empty());
  EXPECT_EQ(s2.data(), nullptr);
}

TEST(Assignment, AssignFromEmpty) {
  ntlib::sieve s1;
  ntlib::sieve s2 = get_dummy_sieve(capacity);
  s2 = s1;
  EXPECT_TRUE(s2.empty());
  EXPECT_EQ(s2.data(), nullptr);
}

TEST(Assignment, AssignEmpty) {
  ntlib::sieve s1;
  ntlib::sieve s2 = get_dummy_sieve(capacity);
  s1 = s2;
  EXPECT_EQ(s1.size(), capacity);
  for (std::size_t i = 0; i < capacity; ++i) EXPECT_EQ(s1[i], s2[i]);
}

TEST(Assignment, AssignFromDifferentSize) {
  ntlib::sieve s1 = get_dummy_sieve(2 * capacity);
  ntlib::sieve s2 = get_dummy_sieve(capacity);
  s1 = s2;
  EXPECT_EQ(s1.size(), capacity);
  for (std::size_t i = 0; i < capacity; ++i) EXPECT_EQ(s1[i], s2[i]);
}

TEST(Assignment, MoveAssignment) {
  ntlib::sieve s1 = get_dummy_sieve(capacity);
  ntlib::sieve s2;
  s2 = std::move(s1);
  EXPECT_TRUE(s1.empty());
  EXPECT_EQ(s1.data(), nullptr);
  EXPECT_EQ(s2.size(), capacity);
  for (std::size_t i = 0; i < capacity; ++i) EXPECT_EQ(dummy[i], s2[i]);
}

TEST(Assignment, MoveEmptyIntoEmpty) {
  ntlib::sieve s1;
  ntlib::sieve s2;
  s2 = std::move(s1);
  EXPECT_TRUE(s1.empty());
  EXPECT_EQ(s1.data(), nullptr);
  EXPECT_TRUE(s2.empty());
  EXPECT_EQ(s2.data(), nullptr);
}

TEST(Assignment, MoveFromEmpty) {
  ntlib::sieve s1 = get_dummy_sieve(capacity);
  ntlib::sieve s2;
  s1 = std::move(s2);
  EXPECT_TRUE(s1.empty());
  EXPECT_EQ(s1.data(), nullptr);
  EXPECT_TRUE(s2.empty());
  EXPECT_EQ(s2.data(), nullptr);
}

TEST(Access, ConstAndMutable) {
  const ntlib::sieve s1 = get_dummy_sieve(capacity);
  ntlib::sieve s2(capacity);
  for (std::size_t i = 0; i < capacity; ++i) {
    s2[i] = s1[i];
    EXPECT_EQ(s1[i], s2[i]);
  }
}
