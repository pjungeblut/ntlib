#include <cstdio>
#include <iostream>
#include <string>
#include <utility>

#include "include/big_integer.hpp"

int main() {
  ntlib::big_integer a(5);
  ntlib::big_integer b;
  ntlib::big_integer c("12345678998765432112345678910111213141516171819");
  ntlib::big_integer d = 10;
  ntlib::big_integer e = c;
  ntlib::big_integer _f("-1234567891011121314151617181920212223242526272829");
  ntlib::big_integer f = std::move(_f);

  printf("Printf output.\n");
  printf("a: %s\n", a.to_string().c_str());
  printf("b: %s\n", b.to_string().c_str());
  printf("c: %s\n", c.to_string().c_str());
  printf("d: %s\n", d.to_string().c_str());
  printf("e: %s\n", e.to_string().c_str());
  printf("f: %s\n", f.to_string().c_str());
  printf("\n");

  std::cout << "Stream output:" << std::endl;
  std::cout << "a: " << a << std::endl;
  std::cout << "b: " << b << std::endl;
  std::cout << "c: " << c << std::endl;
  std::cout << "d: " << d << std::endl;
  std::cout << "e: " << e << std::endl;
  std::cout << "f: " << f << std::endl;
  printf("\n");

  printf("Addition:\n");
  ntlib::big_integer a_plus_c = a + c;
  printf("a + c: %s\n", a_plus_c.to_string().c_str());
  ntlib::big_integer g1("999999999999999999999999999999999999999999999999999998");
  ntlib::big_integer g2 = 1;
  ntlib::big_integer g = g1 + g2;
  printf("g1: %s\n", g1.to_string().c_str());
  printf("g2: %s\n", g2.to_string().c_str());
  printf("g1 + g2: %s\n", g.to_string().c_str());
  g2 += g;
  printf("g1 + g2 + g2: %s\n", g2.to_string().c_str());
  printf("\n");

  printf("Subtraction:\n");
  ntlib::big_integer c_minus_a = c - a;
  printf("c - a: %s\n", c_minus_a.to_string().c_str());
  ntlib::big_integer h1("1000000000000000000000000000000000000000000000000000001");
  ntlib::big_integer h2 = 1;
  ntlib::big_integer h = h1 - h2;
  printf("h1: %s\n", h1.to_string().c_str());
  printf("h2: %s\n", h2.to_string().c_str());
  printf("h1 - h2: %s\n", h.to_string().c_str());
  h -= h2;
  printf("h1 - h2 - h2: %s\n", h.to_string().c_str());
  printf("\n");

  printf("Multiplication:\n");
  ntlib::big_integer c_times_f = c * f;
  printf("c * f: %s\n", c_times_f.to_string().c_str());
  ntlib::big_integer i = c_times_f;
  i *= f;
  printf("c * f * f: %s\n", i.to_string().c_str());
  ntlib::big_integer j("999999999999999999999999999999999999");
  printf("j: %s\n", j.to_string().c_str());
  j *= j;
  printf("j *= j: %s\n", j.to_string().c_str());
  printf("\n");

  printf("Division:\n");
  c_times_f = i / f;
  printf("c * f: %s\n", c_times_f.to_string().c_str());
  ntlib::big_integer k = c_times_f;
  k /= f;
  printf("c: %s\n", k.to_string().c_str());
  printf("\n");

  return 0;
}
