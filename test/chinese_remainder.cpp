#include <gtest/gtest.h>

#include <vector>

#include "chinese_remainder.hpp"

TEST(CRT, SmallValues1) {
  const std::vector<ntlib::crt_congruence<uint32_t>> congruences = {
      {2, 3}, {3, 5}, {2, 7}};
  EXPECT_EQ(ntlib::crt_coprime(congruences), 23);
}

TEST(CRT, SmallValues2) {
  const std::vector<ntlib::crt_congruence<uint32_t>> congruences = {
      {0, 3}, {3, 4}, {4, 5}};
  EXPECT_EQ(ntlib::crt_coprime(congruences), 39);
}
