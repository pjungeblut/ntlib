#include <gtest/gtest.h>

#include "sieve.hpp"

const static std::size_t capacity = 100;

TEST(Construction, DefaultConstruction) {
  ntlib::sieve sieve;
  EXPECT_EQ(sieve.size(), 0);
  EXPECT_TRUE(sieve.empty());
}

TEST(Construction, BySize) {
  ntlib::sieve sieve(capacity);
  EXPECT_EQ(sieve.size(), capacity);
}

TEST(Construction, CopyConstruction) {
  ntlib::sieve s1(capacity);
  for (std::size_t i = 0; i < capacity; ++i) {
    s1[i] = i & 1;
  }

  ntlib::sieve s2(s1);
  EXPECT_EQ(s1.size(), capacity);
  EXPECT_EQ(s2.size(), capacity);
  for (std::size_t i = 0; i < capacity; ++i) {
    EXPECT_EQ(s1[i], s2[i]);
  }
}

TEST(Construction, MoveConstruction) {
  ntlib::sieve s1(capacity);
  for (std::size_t i = 0; i < capacity; ++i) {
    s1[i] = i & 1;
  }

  ntlib::sieve s2(std::move(s1));
  EXPECT_EQ(s1.size(), 0);
  EXPECT_EQ(s2.size(), capacity);
  for (std::size_t i = 0; i < capacity; ++i) {
    EXPECT_EQ(i & 1, s2[i]);
  }
}

TEST(Assignment, CopyAssignment) {
  ntlib::sieve s1(capacity);
  for (std::size_t i = 0; i < capacity; ++i) {
    s1[i] = i & 1;
  }

  ntlib::sieve s2 = s1;
  EXPECT_EQ(s1.size(), capacity);
  EXPECT_EQ(s2.size(), capacity);
  for (std::size_t i = 0; i < capacity; ++i) {
    EXPECT_EQ(s1[i], s2[i]);
  }
}

TEST(Assignment, MoveAssignment) {
  ntlib::sieve s1(capacity);
  for (std::size_t i = 0; i < capacity; ++i) {
    s1[i] = i & 1;
  }

  ntlib::sieve s2 = std::move(s1);
  EXPECT_EQ(s1.size(), 0);
  EXPECT_EQ(s2.size(), capacity);
  for (std::size_t i = 0; i < capacity; ++i) {
    EXPECT_EQ(i & 1, s2[i]);
  }
}

TEST(Modify, FlipAll) {
  ntlib::sieve sieve(capacity);
  for (std::size_t i = 0; i < capacity; ++i) {
    sieve[i] = true;
    EXPECT_TRUE(sieve[i]);
    sieve[i] = !sieve[i];
    EXPECT_FALSE(sieve[i]);
  }
}
