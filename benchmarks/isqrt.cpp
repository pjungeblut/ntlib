#include <benchmark/benchmark.h>

#include <cmath>
#include <cstdint>

#include "base.hpp"

static const int N = 1'000'000;

static void BM_sqrt_floor(benchmark::State &state) {
  for (auto _ : state) {
    for (int n = 1; n < N; ++n) {
      benchmark::DoNotOptimize(std::floor(std::sqrt(n)));
    }
  }
}
BENCHMARK(BM_sqrt_floor)->Unit(benchmark::kMillisecond);

static void BM_isqrt_int(benchmark::State &state) {
  for (auto _ : state) {
    for (int n = 1; n < N; ++n) {
      benchmark::DoNotOptimize(ntlib::isqrt(n));
    }
  }
}
BENCHMARK(BM_isqrt_int)->Unit(benchmark::kMillisecond);

static void BM_isqrt_int128(benchmark::State &state) {
  for (auto _ : state) {
    for (__int128_t n = 1; n < N; ++n) {
      benchmark::DoNotOptimize(ntlib::isqrt(n));
    }
  }
}
BENCHMARK(BM_isqrt_int128)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
