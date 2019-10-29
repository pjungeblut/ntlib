#pragma once

/**
 * Implementations to generate pythagorean triples (a,b,c), such that
 * a^2 + b^2 = c^2.
 * A triple is called primitive, if gcd(a,b,c) = 1.
 *
 * For any natural u > v > 1 we get a pythagorean triple by setting
 * a := u^2 - v^2
 * b := 2uv
 * c := u^2 + v^2.
 * The smalles primitive triple is (3,4,5), created by u = 2, v = 1.
 *
 * (a,b,c) is primitive, if and only if gcd(u,v) = 1 and at least one of them
 * is even. Every primitive triple an be constructed by above formula.
 */

#include <algorithm>
#include <vector>

#include "include/base.hpp"
#include "include/integral.hpp"

namespace ntlib {

/**
 * Creates all primitive pythagorean triples (a,b,c) with a <= b <= c <= maxi.
 *
 * @param maxi The maximum value of any component.
 * @param triples The vector to push the triples into.
 */
template<UnsignedIntegral U>
void primitive_pythagorean_triples(U maxi, std::vector<triple<U>> &triples) {
  for (U v = 1; 2 * v * v <= maxi; ++v) {
    for (U u = v + 1; u * u + v * v <= maxi; ++u) {
      if (!((u & 1) && (v & 1)) && gcd(u, v) == 1) {
        U a = u * u - v * v;
        U b = 2 * u * v;
        U c = u * u + v * v;
        if (a > b) std::swap(a, b);
        triples.push_back(triple<U> {a, b, c});
      }
    }
  }
}

}
