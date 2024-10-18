#include <benchmark/benchmark.h>

#include <cstdint>

#include "experiments/odd_part.hpp"

import base;

static const uint32_t N = 1'000'000;

static void BM_odd_part_naive(benchmark::State &state) {
  for (auto _ : state) {
    for (uint32_t n = 1; n < N; ++n) {
      auto [e, o] = ntlib::experiments::odd_part_naive(n);
      benchmark::DoNotOptimize(e);
      benchmark::DoNotOptimize(o);
    }
  }
}
BENCHMARK(BM_odd_part_naive)->Unit(benchmark::kMillisecond);

static void BM_odd_part_ntlib(benchmark::State &state) {
  for (auto _ : state) {
    for (uint32_t n = 1; n < N; ++n) {
      auto [e, o] = ntlib::odd_part(n);
      benchmark::DoNotOptimize(e);
      benchmark::DoNotOptimize(o);
    }
  }
}
BENCHMARK(BM_odd_part_ntlib)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();