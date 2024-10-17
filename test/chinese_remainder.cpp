#include <gtest/gtest.h>

#include <vector>

// #include "chinese_remainder.hpp"
import chinese_remainder;

TEST(ChineseRemainder_Coprime, SmallValues1) {
  const std::vector<ntlib::crt_congruence<uint32_t>> congruences = {
      {2, 3}, {3, 5}, {2, 7}};
  const auto res = ntlib::crt_coprime(congruences);
  EXPECT_EQ(res.a, 23);
  EXPECT_EQ(res.m, 105);
}

TEST(ChineseRemainder_Coprime, SmallValues2) {
  const std::vector<ntlib::crt_congruence<uint32_t>> congruences = {
      {0, 3}, {3, 4}, {4, 5}};
  const auto res = ntlib::crt_coprime(congruences);
  EXPECT_EQ(res.a, 39);
  EXPECT_EQ(res.m, 60);
}

TEST(ChineseRemainder_Coprime, SmallValues3) {
  const std::vector<ntlib::crt_congruence<uint32_t>> congruences = {
      {6, 7}, {4, 8}};
  const auto res = ntlib::crt_coprime(congruences);
  EXPECT_EQ(res.a, 20);
  EXPECT_EQ(res.m, 56);
}

TEST(ChineseRemainder_Coprime, SmallValues4) {
  const std::vector<ntlib::crt_congruence<uint32_t>> congruences = {
      {2, 5}, {3, 7}, {10, 11}};
  const auto res = ntlib::crt_coprime(congruences);
  EXPECT_EQ(res.a, 87);
  EXPECT_EQ(res.m, 385);
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
  EXPECT_EQ(res.value().a, 53);
  EXPECT_EQ(res.value().m, 60);
}

TEST(ChineseRemainder, SmallValues2) {
  const std::vector<ntlib::crt_congruence<uint32_t>> congruences = {
      {3, 5}, {3, 7}, {4, 12}};
  auto res = ntlib::crt(congruences);
  EXPECT_TRUE(res.has_value());
  EXPECT_EQ(res.value().a, 388);
  EXPECT_EQ(res.value().m, 420);
}