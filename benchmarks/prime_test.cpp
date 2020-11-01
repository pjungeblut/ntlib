#include <benchmark/benchmark.h>

#include <cstddef>

#include "prime_test.hpp"

#define UNIT_MS Unit(benchmark::kMillisecond)

static const std::size_t NUM_TESTS = 1'000;

static void BM_is_prime_naive(benchmark::State &state) {
  for (auto _ : state) {
    for (__uint128_t i = state.range(0); i <= state.range(0) + NUM_TESTS; ++i) {
      benchmark::DoNotOptimize(ntlib::is_prime_naive(i));
    }
  }
}
BENCHMARK(BM_is_prime_naive)
    ->Arg(0)
    ->Arg(1'000)
    ->Arg(1'000'000)
    ->Arg(1'000'000'000)
    ->UNIT_MS;

static void BM_is_prime_miller_rabin(benchmark::State &state) {
  for (auto _ : state) {
    for (__uint128_t i = state.range(0); i <= state.range(0) + NUM_TESTS; ++i) {
      benchmark::DoNotOptimize(ntlib::is_prime_miller_rabin(i));
    }
  }
}
BENCHMARK(BM_is_prime_miller_rabin)
    ->Arg(0)
    ->Arg(1'000)
    ->Arg(1'000'000)
    ->Arg(1'000'000'000)
    ->Arg(1'000'000'000'000LL)
    ->Arg(1'000'000'000'000'000LL)
    ->Arg(1'000'000'000'000'000'000LL)
    ->UNIT_MS;

static void BM_is_prime_combined(benchmark::State &state) {
  for (auto _ : state) {
    for (__uint128_t i = state.range(0); i <= state.range(0) + NUM_TESTS; ++i) {
      benchmark::DoNotOptimize(ntlib::is_prime(i));
    }
  }
}
BENCHMARK(BM_is_prime_combined)
    ->Arg(0)
    ->Arg(1'000)
    ->Arg(1'000'000)
    ->Arg(1'000'000'000)
    ->Arg(1'000'000'000'000LL)
    ->Arg(1'000'000'000'000'000LL)
    ->Arg(1'000'000'000'000'000'000LL)
    ->UNIT_MS;

BENCHMARK_MAIN();
