#include <benchmark/benchmark.h>

#include <cstdint>

#include "experiments/is_square.hpp"

import base;

#define UNIT_MS Unit(benchmark::kMillisecond)

static const uint64_t N = 1'000'000;

static void BM_divisibility_checks(benchmark::State &state) {
  for (auto _ : state) {
    for (uint64_t i = 0; i < N; ++i) {
      benchmark::DoNotOptimize(ntlib::is_square(i));
    }

  }
}
BENCHMARK(BM_divisibility_checks)->UNIT_MS;

static void BM_isqrt(benchmark::State &state) {
  for (auto _ : state) {
    for (uint64_t i = 0; i < N; ++i) {
      benchmark::DoNotOptimize(ntlib::experiments::is_square_isqrt(i));
    }
  }
}
BENCHMARK(BM_isqrt)->UNIT_MS;

static void BM_newton(benchmark::State &state) {
  for (auto _ : state) {
    for (uint64_t i = 0; i < N; ++i) {
      benchmark::DoNotOptimize(ntlib::experiments::is_square_newton(i));
    }
  }
}
BENCHMARK(BM_newton)->UNIT_MS;

BENCHMARK_MAIN();
