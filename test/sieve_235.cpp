#include <gtest/gtest.h>

#include "sieve_235.hpp"

const static std::size_t min_capacity = 1'000;
const static std::size_t capacity = (min_capacity + 29) / 30 * 30;

static ntlib::sieve_235<> get_dummy_sieve(std::size_t min_capacity) {
  ntlib::sieve_235 sieve(min_capacity);
  for (std::size_t i = 0; i < sieve.size(); ++i) sieve[i] = i % 7;
  return sieve;
}

static const ntlib::sieve_235 dummy = get_dummy_sieve(min_capacity);

TEST(Construction, DefaultConstruction) {
  ntlib::sieve_235 sieve;
  EXPECT_TRUE(sieve.empty());
  EXPECT_EQ(sieve.data(), nullptr);
}

TEST(Construction, BySize) {
  ntlib::sieve_235 sieve(min_capacity);
  EXPECT_EQ(sieve.size(), capacity);
}

TEST(Construction, BySizeEmpty) {
  ntlib::sieve_235 sieve(0);
  EXPECT_TRUE(sieve.empty());
  EXPECT_EQ(sieve.data(), nullptr);
}

TEST(Construction, CopyConstruction) {
  ntlib::sieve_235 s1 = get_dummy_sieve(min_capacity);
  ntlib::sieve_235 s2(s1);
  EXPECT_EQ(s2.size(), capacity);
  for (std::size_t i = 0; i < capacity; ++i) EXPECT_EQ(s1[i], s2[i]);
}

TEST(Construction, CopyConstructionEmpty) {
  ntlib::sieve_235 s1;
  ntlib::sieve_235 s2(s1);
  EXPECT_TRUE(s2.empty());
  EXPECT_EQ(s2.data(), nullptr);
}

TEST(Construction, MoveConstruction) {
  ntlib::sieve_235 s1 = get_dummy_sieve(min_capacity);
  ntlib::sieve_235 s2(std::move(s1));
  EXPECT_TRUE(s1.empty());
  EXPECT_EQ(s1.data(), nullptr);
  EXPECT_EQ(s2.size(), capacity);
  for (std::size_t i = 0; i < capacity; ++i) EXPECT_EQ(dummy[i], s2[i]);
}

TEST(Assignment, CopyAssignment) {
  ntlib::sieve_235 s1 = get_dummy_sieve(min_capacity);
  ntlib::sieve_235 s2;
  s2 = s1;
  EXPECT_EQ(s2.size(), capacity);
  if (capacity) EXPECT_NE(s1.data(), s2.data());
  for (std::size_t i = 0; i < capacity; ++i) EXPECT_EQ(s1[i], s2[i]);
}

TEST(Assignment, SelfAssignment) {
  ntlib::sieve_235 s = get_dummy_sieve(min_capacity);

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wself-assign-overloaded"
  s = s;
  #pragma GCC diagnostic pop

  EXPECT_EQ(s.size(), capacity);
  for (std::size_t i = 0; i < capacity; ++i) EXPECT_EQ(dummy[i], s[i]);
}

TEST(Assignment, AssignEmptyFromEmpty) {
  ntlib::sieve_235 s1;
  ntlib::sieve_235 s2;
  s2 = s1;
  EXPECT_TRUE(s2.empty());
  EXPECT_EQ(s2.data(), nullptr);
}

TEST(Assignment, AssignFromEmpty) {
  ntlib::sieve_235 s1;
  ntlib::sieve_235 s2 = get_dummy_sieve(min_capacity);
  s2 = s1;
  EXPECT_TRUE(s2.empty());
  EXPECT_EQ(s2.data(), nullptr);
}

TEST(Assignment, AssignEmpty) {
  ntlib::sieve_235 s1;
  ntlib::sieve_235 s2 = get_dummy_sieve(min_capacity);
  s1 = s2;
  EXPECT_EQ(s1.size(), capacity);
  for (std::size_t i = 0; i < capacity; ++i) EXPECT_EQ(s1[i], s2[i]);
}

TEST(Assingment, AssignFromDifferentSize) {
  ntlib::sieve_235 s1 = get_dummy_sieve(2 * min_capacity);
  ntlib::sieve_235 s2 = get_dummy_sieve(capacity);
  s1 = s2;
  EXPECT_EQ(s1.size(), capacity);
  for (std::size_t i = 0; i < capacity; ++i) EXPECT_EQ(s1[i], s2[i]);
}

TEST(Assignment, MoveAssignment) {
  ntlib::sieve_235 s1 = get_dummy_sieve(min_capacity);
  ntlib::sieve_235 s2;
  s2 = std::move(s1);
  EXPECT_TRUE(s1.empty());
  EXPECT_EQ(s1.data(), nullptr);
  EXPECT_EQ(s2.size(), capacity);
  for (std::size_t i = 0; i < capacity; ++i) EXPECT_EQ(dummy[i], s2[i]);
}

TEST(Assignment, MoveEmptyIntoEmpty) {
  ntlib::sieve_235 s1;
  ntlib::sieve_235 s2;
  s2 = std::move(s1);
  EXPECT_TRUE(s1.empty());
  EXPECT_EQ(s1.data(), nullptr);
  EXPECT_TRUE(s2.empty());
  EXPECT_EQ(s2.data(), nullptr);
}

TEST(Assingment, MoveFromEmpty) {
  ntlib::sieve_235 s1 = get_dummy_sieve(min_capacity);
  ntlib::sieve_235 s2;
  s1 = std::move(s2);
  EXPECT_TRUE(s1.empty());
  EXPECT_EQ(s1.data(), nullptr);
  EXPECT_TRUE(s2.empty());
  EXPECT_EQ(s2.data(), nullptr);
}

TEST(Access, ConstAndMutable) {
  const ntlib::sieve_235 s1 = get_dummy_sieve(min_capacity);
  ntlib::sieve_235 s2(min_capacity);
  for (std::size_t i = 0; i < capacity; ++i) {
    s2[i] = s1[i];
    EXPECT_EQ(s1[i], s2[i]);
  }
}
