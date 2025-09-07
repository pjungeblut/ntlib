#include <gtest/gtest.h>

import base;
import int128;

TEST(AdditiveNeutal, Existence) {
  const auto n1 = ntlib::zero<ntlib::i128>();
  EXPECT_EQ(n1, ntlib::i128{0});
}

TEST(MulitplicativeNeutral, Existence) {
  const auto n1 = ntlib::one<ntlib::i128>();
  EXPECT_EQ(n1, ntlib::i128{1});
}
