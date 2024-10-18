#include <benchmark/benchmark.h>

#include <cmath>
#include <cstdint>

#include "experiments/isqrt.hpp"

import base;
import int128;

static const int N = 1'000'000;

static void BM_isqrt_int_floor_sqrt(benchmark::State &state) {
  for (auto _ : state) {
    for (int n = 1; n < N; ++n) {
      benchmark::DoNotOptimize(ntlib::experiments::isqrt_int_floor_sqrt(n));
    }
  }
}
BENCHMARK(BM_isqrt_int_floor_sqrt)->Unit(benchmark::kMillisecond);

static void BM_isqrt_int_abacus(benchmark::State &state) {
  for (auto _ : state) {
    for (int n = 1; n < N; ++n) {
      benchmark::DoNotOptimize(ntlib::experiments::isqrt_int_abacus(n));
    }
  }
}
BENCHMARK(BM_isqrt_int_abacus)->Unit(benchmark::kMillisecond);

static void BM_isqrt_int_newton(benchmark::State &state) {
  for (auto _ : state) {
    for (int n = 1; n < N; ++n) {
      benchmark::DoNotOptimize(ntlib::experiments::isqrt_int_newton(n));
    }
  }
}
BENCHMARK(BM_isqrt_int_newton)->Unit(benchmark::kMillisecond);

static void BM_isqrt_int_binsearch(benchmark::State &state) {
  for (auto _ : state) {
    for (int n = 1; n < N; ++n) {
      benchmark::DoNotOptimize(ntlib::experiments::isqrt_int_binsearch(n));
    }
  }
}
BENCHMARK(BM_isqrt_int_binsearch)->Unit(benchmark::kMillisecond);

static void BM_isqrt_int128(benchmark::State &state) {
  for (auto _ : state) {
    for (ntlib::i128 n = 1; n < N; ++n) {
      benchmark::DoNotOptimize(ntlib::isqrt(n));
    }
  }
}
BENCHMARK(BM_isqrt_int128)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
