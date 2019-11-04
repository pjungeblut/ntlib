#include <cstdio>
#include <iostream>
#include <string>
#include <utility>

#include "include/big_unsigned.hpp"

int main() {
  ntlib::big_unsigned a(5);
  ntlib::big_unsigned b;
  ntlib::big_unsigned c("12345678998765432112345678910111213141516171819");
  ntlib::big_unsigned d = 10;
  ntlib::big_unsigned e = c;
  ntlib::big_unsigned _f("1234567891011121314151617181920212223242526272829");
  ntlib::big_unsigned f = std::move(_f);

  printf("Printf output.\n");
  printf("a: %s\n", a.to_string().c_str());
  printf("b: %s\n", b.to_string().c_str());
  printf("c: %s\n", c.to_string().c_str());
  printf("d: %s\n", d.to_string().c_str());
  printf("e: %s\n", e.to_string().c_str());
  printf("f: %s\n", f.to_string().c_str());

  std::cout << "Stream output:" << std::endl;
  std::cout << "a: " << a << std::endl;
  std::cout << "b: " << b << std::endl;
  std::cout << "c: " << c << std::endl;
  std::cout << "d: " << d << std::endl;
  std::cout << "e: " << e << std::endl;
  std::cout << "f: " << f << std::endl;

  ntlib::big_unsigned ac = a + c;
  printf("a + c: %s\n", ac.to_string().c_str());
  ntlib::big_unsigned g1("999999999999999999999999999999999999999999999999999998");
  ntlib::big_unsigned g2 = 1;
  ntlib::big_unsigned g = g1 + g2;
  printf("g1 + g2: %s\n", g.to_string().c_str());
  ntlib::big_unsigned g3 = 1;
  g3 += g;
  printf("g1 + g2 + g3: %s\n", g3.to_string().c_str());

  return 0;
}
