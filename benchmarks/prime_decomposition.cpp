#include <benchmark/benchmark.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <cstdint>

#include "experiments/prime_decomposition.hpp"
#include "prime_decomposition.hpp"
#include "prime_generation.hpp"

import int128;

// How many consecutive numbers to decompose.
constexpr static uint64_t N = 10'000;

// Large primes (whose squares yield large composites without small prime
// factors).
constexpr static auto large_primes = std::to_array<uint64_t>({
    4000000007, 4000000009, 4000000019, 4000000063, 4000000133, 4000000157,
    4000000163, 4000000187, 4000000229, 4000000231, 4000000241, 4000000273,
    4000000351, 4000000373, 4000000387, 4000000427, 4000000451, 4000000453,
    4000000483, 4000000531, 4000000553, 4000000559, 4000000561, 4000000573,
    4000000579, 4000000591, 4000000597, 4000000607, 4000000619, 4000000661,
    4000000663, 4000000687, 4000000723, 4000000727, 4000000733, 4000000787,
    4000000801, 4000000813, 4000000831, 4000000861, 4000000913, 4000000951,
    4000000981, 4000001003, 4000001039, 4000001051, 4000001099, 4000001101,
    4000001113, 4000001119, 4000001191, 4000001197, 4000001209, 4000001221,
    4000001227, 4000001249, 4000001261, 4000001263, 4000001297, 4000001311,
    4000001333, 4000001339, 4000001351, 4000001363, 4000001381, 4000001419,
    4000001431, 4000001443, 4000001477, 4000001501, 4000001521, 4000001567,
    4000001609, 4000001683, 4000001699, 4000001711, 4000001729, 4000001839,
    4000001851, 4000001891, 4000001897, 4000001899, 4000001959, 4000001963,
    4000001969, 4000001977, 4000002001, 4000002091, 4000002121, 4000002191,
    4000002221, 4000002241, 4000002263, 4000002313, 4000002329, 4000002361,
    4000002367, 4000002389, 4000002421, 4000002431, 4000002439, 4000002469,
    4000002491, 4000002499, 4000002529, 4000002551, 4000002553, 4000002619,
    4000002647, 4000002659, 4000002667, 4000002673, 4000002679, 4000002689,
    4000002751, 4000002781, 4000002791, 4000002817, 4000002829, 4000002833});

static void BM_prime_decomposition_32(benchmark::State &state) {
  for (auto _ : state) {
    for (uint32_t i = state.range(0); i < state.range(0) + N; ++i) {
      const auto pd = ntlib::prime_decomposition_32(i);
    }
  }
}
BENCHMARK(BM_prime_decomposition_32)
    ->Arg(1)
    ->Arg(1'000)
    ->Arg(1'000'000)
    ->Arg(1'000'000'000)
    ->Unit(benchmark::kMillisecond);

static void BM_prime_decomposition_ntlib(benchmark::State &state) {
  for (auto _ : state) {
    for (uint64_t i = state.range(0); i < state.range(0) + N; ++i) {
      const auto pd = ntlib::prime_decomposition(i);
    }
  }
}
BENCHMARK(BM_prime_decomposition_ntlib)
    ->Arg(1)
    ->Arg(1'000)
    ->Arg(1'000'000)
    ->Arg(1'000'000'000)
    ->Arg(1'000'000'000'000ull)
    ->Arg(1'000'000'000'000'000ull)
    ->Arg(1'000'000'000'000'000'000ull)
    ->Unit(benchmark::kMillisecond);

static void BM_factor_pollard_rho(benchmark::State &state) {
  for (auto _ : state) {
    for (const uint64_t p : large_primes) {
      const uint64_t n = p * p;
      const auto f = [n](uint64_t x) { return (ntlib::u128{x} * x + 1) % n; };
      auto res = ntlib::experiments::find_factor_pollard_rho(n, f, 2ul);
      benchmark::DoNotOptimize(res);
    }
  }
}
BENCHMARK(BM_factor_pollard_rho)
    ->Unit(benchmark::kMillisecond);

static void BM_factor_pollard_rho_mult(benchmark::State &state) {
  for (auto _ : state) {
    for (const uint64_t p : large_primes) {
      const uint64_t n = p * p;
      const auto f = [n](uint64_t x) { return (ntlib::u128{x} * x + 1) % n; };
      auto res = ntlib::find_factor_pollard_rho_mult(
          n, f, 2ul, state.range(0));
      benchmark::DoNotOptimize(res);
    }
  }
}
BENCHMARK(BM_factor_pollard_rho_mult)
    ->Arg(32)
    ->Arg(64)
    ->Arg(128)
    ->Arg(256)
    ->Unit(benchmark::kMillisecond);

static void BM_factor_pollard_rho_brent(benchmark::State &state) {
  for (auto _ : state) {
    for (const uint64_t p : large_primes) {
      const uint64_t n = p * p;
      const auto f = [n](uint64_t x) { return (ntlib::u128{x} * x + 1) % n; };
      auto res = ntlib::experiments::find_factor_pollard_rho_brent(n, f, 2ul);
      benchmark::DoNotOptimize(res);
    }
  }
}
BENCHMARK(BM_factor_pollard_rho_brent)
    ->Unit(benchmark::kMillisecond);

static void BM_factor_pollard_rho_cpalgos(benchmark::State &state) {
  for (auto _ : state) {
    for (const uint64_t p : large_primes) {
      const uint64_t n = p * p;
      const auto f = [n](uint64_t x) { return (ntlib::u128{x} * x + 1) % n; };
      auto res = ntlib::experiments::find_factor_pollard_rho_cpalgos(n, f, 2ul);
      benchmark::DoNotOptimize(res);
    }
  }
}
BENCHMARK(BM_factor_pollard_rho_cpalgos)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
