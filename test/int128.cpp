#include <gtest/gtest.h>

import int128;

TEST(MultiplicativeNeutral, Signed) {
  auto neutral = ntlib::get_multiplicative_neutral(ntlib::i128{-5});
  EXPECT_TRUE((std::is_same_v<decltype(neutral), ntlib::i128>));
  EXPECT_EQ(neutral, 1);
}

TEST(MultiplicativeNeutral, Unsigned) {
  auto neutral = ntlib::get_multiplicative_neutral(ntlib::u128{5});
  EXPECT_TRUE((std::is_same_v<decltype(neutral), ntlib::u128>));
  EXPECT_EQ(neutral, 1);
}