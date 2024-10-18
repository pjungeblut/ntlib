#include <benchmark/benchmark.h>

#include <bitset>
#include <cstddef>
#include <vector>

#include "sieve.hpp"

import sieve_235;

#define UNIT_MS Unit(benchmark::kMicrosecond)

static constexpr std::size_t N = 1'000'000;

template<typename SieveType>
static void BM_set_all(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    SieveType sieve(N);
    state.ResumeTiming();
    for (std::size_t i = 0; i < N; ++i) {
      sieve[i] = true;
    }
  }
}
BENCHMARK_TEMPLATE(BM_set_all, ntlib::sieve<>)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_set_all, ntlib::sieve_235<>)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_set_all, std::vector<bool>)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_set_all, std::vector<unsigned char>)->UNIT_MS;

template<typename SieveType>
static void BM_init235(benchmark::State &state) {
  for (auto _ : state) {
    std::size_t M = (N + 29) / 30 * 30;
    SieveType sieve(M);
    sieve.init235();
  }
}
BENCHMARK_TEMPLATE(BM_init235, ntlib::sieve<>)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_init235, ntlib::sieve_235<>)->UNIT_MS;

BENCHMARK_MAIN();
