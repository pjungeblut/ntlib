#include <benchmark/benchmark.h>

#include <cstddef>

#include "experiments/prime_test.hpp"

import int128;
import prime_test;

#define UNIT_MS Unit(benchmark::kMillisecond)

static const std::size_t NUM_TESTS = 100'000;

static void BM_is_prime_naive(benchmark::State &state) {
  for (auto _ : state) {
    for (uint64_t i = state.range(0); i <= state.range(0) + NUM_TESTS; ++i) {
      benchmark::DoNotOptimize(ntlib::experiments::is_prime_naive(i));
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

static void BM_is_prime_miller_selfridge_rabin(benchmark::State &state) {
  for (auto _ : state) {
    for (uint64_t i = state.range(0); i <= state.range(0) + NUM_TESTS; ++i) {
      benchmark::DoNotOptimize(
          ntlib::experiments::is_prime_miller_selfridge_rabin(i));
    }
  }
}
BENCHMARK(BM_is_prime_miller_selfridge_rabin)
    ->Arg(0)
    ->Arg(1'000)
    ->Arg(1'000'000)
    ->Arg(1'000'000'000)
    ->Arg(1'000'000'000'000LL)
    ->Arg(1'000'000'000'000'000LL)
    ->Arg(1'000'000'000'000'000'000LL)
    ->UNIT_MS;

static void BM_is_prime_baillie_psw(benchmark::State &state) {
  for (auto _ : state) {
    for (ntlib::u128 i = state.range(0); i <= state.range(0) + NUM_TESTS; ++i) {
      benchmark::DoNotOptimize(
          ntlib::is_prime_baillie_psw<ntlib::u128, ntlib::i128>(i));
    }
  }
}
BENCHMARK(BM_is_prime_baillie_psw)
    ->Arg(0)
    ->Arg(1'000)
    ->Arg(1'000'000)
    ->Arg(1'000'000'000)
    ->Arg(1'000'000'000'000LL)
    ->Arg(1'000'000'000'000'000LL)
    ->Arg(1'000'000'000'000'000'000LL)
    ->UNIT_MS;

static void BM32_is_prime_miller_selfridge_rabin(benchmark::State &state) {
  for (auto _ : state) {
    for (uint32_t i = 1; i < NUM_TESTS; i += 2) {
      uint32_t n = state.range(0) + i;
      benchmark::DoNotOptimize(
          ntlib::experiments::is_prime_miller_selfridge_rabin(n));
    }
  }
}
BENCHMARK(BM32_is_prime_miller_selfridge_rabin)
    ->Arg(0)
    ->Arg(1'000)
    ->Arg(1'000'000)
    ->Arg(1'000'000'000)
    ->UNIT_MS;

static void BM32_is_prime_forisek_jancina(benchmark::State &state) {
  for (auto _ : state) {
    for (uint32_t i = 1; i < NUM_TESTS; i += 2) {
      uint32_t n = state.range(0) + i;
      benchmark::DoNotOptimize(
          ntlib::experiments::is_prime_forisek_jancina(n));
    }
  }
}
BENCHMARK(BM32_is_prime_forisek_jancina)
    ->Arg(0)
    ->Arg(1'000)
    ->Arg(1'000'000)
    ->Arg(1'000'000'000)
    ->UNIT_MS;

static void BM32_is_prime_trial_division(benchmark::State &state) {
  for (auto _ : state) {
    for (uint32_t i = 1; i < NUM_TESTS; i += 2) {
      uint32_t n = state.range(0) + i;
      benchmark::DoNotOptimize(
          ntlib::is_prime_trial_division(n, ntlib::SMALL_PRIMES<uint32_t>));
    }
  }
}
BENCHMARK(BM32_is_prime_trial_division)
    ->Arg(0)
    ->Arg(1'000)
    ->Arg(900'000)
    ->UNIT_MS;

static void BM32_is_prime(benchmark::State &state) {
  for (auto _ : state) {
    for (uint32_t i = 1; i < NUM_TESTS; i += 2) {
      uint32_t n = state.range(0) + i;
      benchmark::DoNotOptimize(ntlib::is_prime(n));
    }
  }
}
BENCHMARK(BM32_is_prime)
    ->Arg(0)
    ->Arg(1'000)
    ->Arg(1'000'000)
    ->Arg(1'000'000'000)
    ->UNIT_MS;

static void BM64_is_prime(benchmark::State &state) {
  for (auto _ : state) {
    for (uint64_t i = 1; i < NUM_TESTS; i += 2) {
      uint64_t n = state.range(0) + i;
      benchmark::DoNotOptimize(ntlib::is_prime(n));
    }
  }
}
BENCHMARK(BM64_is_prime)
    ->Arg(0)
    ->Arg(1'000)
    ->Arg(1'000'000)
    ->Arg(1'000'000'000)
    ->Arg(1'000'000'000'000)
    ->Arg(1'000'000'000'000'000)
    ->Arg(1'000'000'000'000'000'000)
    ->UNIT_MS;

BENCHMARK_MAIN();
