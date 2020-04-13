#pragma once

#include "base.hpp"
#include "integral.hpp"

namespace ntlib {

/**
 * Computes an integer solution (x,y) to the diophantine equation ax + by = c.
 * A solution exists if and only if c is a multiple of gcd(a,b).
 *
 * @param a Parameter a.
 * @param b Parameter b.
 * @param c Parameter c.
 * @param x Value for x if a solution was found.
 * @param y Value for y if a solution was found.
 * @return Whether a solution exists (and therefore was computed).
 */
template<Integral I>
bool diophantine_solution(I a, I b, I c, I &x, I &y) {
  // Special case: a = 0 and b = 0.
  if (a == 0 && b == 0) {
    if (c == 0) {
      x = 0;
      y = 0;
      return true;
    } else return false;
  }

  // Special case: a = 0.
  if (a == 0) {
    if (c % b == 0) {
      x = 0;
      y = c / b;
      return true;
    } else return false;
  }

  // Special case: b = 0.
  if (b == 0) {
    if (c % a == 0) {
      y = 0;
      x = c / a;
      return true;
    } else return false;
  }

  // General case.
  triple<I,I,I> gxy = extended_euclid(abs(a), abs(b));
  if (x % gxy.a != 0) return false;
  x = gxy.b * c / gxy.a;
  y = gxy.c * c / gxy.a;
  if (a < 0) x = -x;
  if (b < 0) y = -y;
  return true;
}

}
