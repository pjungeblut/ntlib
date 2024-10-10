#include <gtest/gtest.h>

#include <vector>

#include "chinese_remainder.hpp"

TEST(ChineseRemainder_Coprime, SmallValues1) {
  const std::vector<ntlib::crt_congruence<uint32_t>> congruences = {
      {2, 3}, {3, 5}, {2, 7}};
  EXPECT_EQ(ntlib::crt_coprime(congruences), 23); // mod 105
}

TEST(ChineseRemainder_Coprime, SmallValues2) {
  const std::vector<ntlib::crt_congruence<uint32_t>> congruences = {
      {0, 3}, {3, 4}, {4, 5}};
  EXPECT_EQ(ntlib::crt_coprime(congruences), 39); // mod 60
}

TEST(ChineseRemainder_Coprime, SmallValues3) {
  const std::vector<ntlib::crt_congruence<uint32_t>> congruences = {
      {6, 7}, {4, 8}};
  EXPECT_EQ(ntlib::crt_coprime(congruences), 20); // mod 56
}

TEST(ChineseRemainder_Coprime, SmallValues4) {
  const std::vector<ntlib::crt_congruence<uint32_t>> congruences = {
      {2, 5}, {3, 7}, {10, 11}};
  EXPECT_EQ(ntlib::crt_coprime(congruences), 87); // mod 385
}

TEST(ChineseRemainder, Impossible1) {
  const std::vector<ntlib::crt_congruence<uint32_t>> congruences = {
      {1, 4}, {2, 6}};
  auto res = ntlib::crt(congruences);
  EXPECT_FALSE(res.has_value());
}

TEST(ChineseRemainder, Impossible2) {
  const std::vector<ntlib::crt_congruence<uint32_t>> congruences = {
      {3, 4}, {0, 6}};
  auto res = ntlib::crt(congruences);
  EXPECT_FALSE(res.has_value());
}

TEST(ChineseRemainder, SmallValues1) {
  const std::vector<ntlib::crt_congruence<uint32_t>> congruences = {
      {3, 10}, {5, 12}};
  auto res = ntlib::crt(congruences);
  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(res.value(), 53); // mod 60
}

TEST(ChineseRemainder, SmallValues2) {
  const std::vector<ntlib::crt_congruence<uint32_t>> congruences = {
      {3, 5}, {3, 7}, {4, 12}};
  auto res = ntlib::crt(congruences);
  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(res.value(), 388); // mod 420
}