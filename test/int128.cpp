#include <gtest/gtest.h>

#include "int128.hpp"

TEST(MultiplicativeNeutral, Signed) {
  auto neutral = ntlib::get_multiplicative_neutral(i128{-5});
  EXPECT_TRUE((std::is_same_v<decltype(neutral), i128>));
  EXPECT_EQ(neutral, 1);
}

TEST(MultiplicativeNeutral, Unsigned) {
  auto neutral = ntlib::get_multiplicative_neutral(u128{5});
  EXPECT_TRUE((std::is_same_v<decltype(neutral), u128>));
  EXPECT_EQ(neutral, 1);
}