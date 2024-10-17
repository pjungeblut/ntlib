#pragma once

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "sieve.hpp"

import base;

namespace ntlib {
namespace experiments {

template<typename SieveType>
inline void clear_multiples_unitl(SieveType &sieve, std::size_t p,
    std::size_t &m, std::size_t &o, std::size_t N) {
  static const std::size_t OFFSETS[8] = {6, 4, 2, 4, 2, 4, 6, 2};
  for (; m <= N; m += OFFSETS[++o % 8] * p) {
    sieve[m] = false;
  }
}

template<typename SieveType = ntlib::sieve<>>
SieveType eratosthenes_textbook(std::size_t N) {
  N = N / 30 * 30 + 29;
  SieveType sieve(N + 1);
  sieve.init235();

  sieve[1] = false;
  sieve[2] = true;
  sieve[3] = true;
  sieve[5] = true;

  for (std::size_t i = 7; i * i <= N; i += 2) {
    if (sieve[i]) {
      for (std::size_t j = i * i; j <= N; j += 2 * i) {
        sieve[j] = false;
      }
    }
  }
  return sieve;
}

template<typename SieveType = ntlib::sieve<>>
SieveType eratosthenes_jumps(std::size_t N) {
  N = N / 30 * 30 + 29;
  SieveType sieve(N + 1);
  sieve.init235();

  sieve[1] = false;
  sieve[2] = true;
  sieve[3] = true;
  sieve[5] = true;

  static const std::size_t REMAINDERS[8] = {1, 7, 11, 13, 17, 19, 23, 29};
  for (std::size_t i = 0; i * i <= N; i += 30) {
    for (std::size_t j = 0; j < 8; ++j) {
      std::size_t cand = i + REMAINDERS[j];
      if (sieve[cand]) {
        std::size_t sq = cand * cand;
        std::size_t o = j + 7;
        clear_multiples_unitl(sieve, cand, sq, o, N);
      }
    }
  }

  return sieve;
}

}
}
