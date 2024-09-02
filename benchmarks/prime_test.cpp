#include <benchmark/benchmark.h>

#include <cstddef>

#include "experiments/prime_test.hpp"
#include "int128.hpp"
#include "prime_test.hpp"

#define UNIT_MS Unit(benchmark::kMillisecond)

static const std::size_t NUM_TESTS = 100'000;

static void BM_is_prime_naive(benchmark::State &state) {
  for (auto _ : state) {
    for (uint64_t i = state.range(0); i <= state.range(0) + NUM_TESTS; ++i) {
      benchmark::DoNotOptimize(ntlib::is_prime_naive(i));
    }
  }
}
BENCHMARK(BM_is_prime_naive)
    ->Arg(0)
    ->Arg(1'000)
    ->Arg(1'000'000)
    ->UNIT_MS;

static void BM_is_prime_list(benchmark::State &state) {
  for (auto _ : state) {
    for (uint64_t i = state.range(0); i <= state.range(0) + NUM_TESTS; ++i) {
      benchmark::DoNotOptimize(ntlib::experiments::is_prime_list(i));
    }
  }
}
BENCHMARK(BM_is_prime_list)
    ->Arg(0)
    ->Arg(1'000)
    ->Arg(1'000'000)
    ->UNIT_MS;

static void BM_is_prime_miller_rabin(benchmark::State &state) {
  for (auto _ : state) {
    for (u128 i = state.range(0); i <= state.range(0) + NUM_TESTS; ++i) {
      benchmark::DoNotOptimize(ntlib::experiments::is_prime_miller_rabin(i));
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

static void BM_is_prime_bpsw(benchmark::State &state) {
  for (auto _ : state) {
    for (u128 i = state.range(0); i <= state.range(0) + NUM_TESTS; ++i) {
      benchmark::DoNotOptimize(ntlib::is_prime<u128, i128>(i));
    }
  }
}
BENCHMARK(BM_is_prime_bpsw)
    ->Arg(0)
    ->Arg(1'000)
    ->Arg(1'000'000)
    ->Arg(1'000'000'000)
    ->Arg(1'000'000'000'000LL)
    ->Arg(1'000'000'000'000'000LL)
    ->Arg(1'000'000'000'000'000'000LL)
    ->UNIT_MS;

BENCHMARK_MAIN();
