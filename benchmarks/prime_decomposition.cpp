#include <benchmark/benchmark.h>

#include <cstddef>

#include "experiments/prime_decomposition.hpp"
#include "prime_decomposition.hpp"
#include "prime_generation.hpp"

#define UNIT_MS Unit(benchmark::kMillisecond)

const static std::size_t N = 1'000'000;

static void BM_prime_decomposition(benchmark::State &state) {
  const uint64_t max_i = state.range(0) + N;
  std::vector<uint64_t> primes;
  ntlib::prime_sieve(max_i, primes);

  for (auto _ : state) {
    for (uint64_t i = state.range(0); i < state.range(0) + N; ++i) {
      std::map<uint64_t,uint64_t> factors;
      ntlib::prime_decomposition(i, primes, factors);
    }
  }
}
BENCHMARK(BM_prime_decomposition)
    ->Arg(0)
    ->Arg(1'000)
    ->Arg(1'000'000)
    ->Arg(1'000'000'000)
    ->UNIT_MS;

static void BM_pollard_rho(benchmark::State &state) {
  for (auto _ : state) {
    for (uint64_t i = state.range(0); i < state.range(0) + N; ++i) {
      std::map<uint64_t,uint64_t> factors;
      ntlib::experiments::pollard_rho(i, factors);
    }
  }
}
BENCHMARK(BM_pollard_rho)
    ->Arg(0)
    ->Arg(1'000)
    ->Arg(1'000'000)
    ->Arg(1'000'000'000)
    ->UNIT_MS;

BENCHMARK_MAIN();
