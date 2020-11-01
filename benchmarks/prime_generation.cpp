#include <benchmark/benchmark.h>

#include "experiments/prime_generation.hpp"
#include "prime_generation.hpp"
#include "sieve.hpp"
#include "sieve_235.hpp"

#define UNIT_MS Unit(benchmark::kMillisecond)

static const std::size_t N = 1'000'000'000;

template<typename SieveType>
static void BM_eratosthenes_textbook(benchmark::State &state) {
  for (auto _ : state) {
    ntlib::experiments::eratosthenes_textbook<SieveType>(N);
  }
}
BENCHMARK_TEMPLATE(BM_eratosthenes_textbook, ntlib::sieve<>)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_eratosthenes_textbook, ntlib::sieve_235<>)->UNIT_MS;

template<typename SieveType>
static void BM_eratosthenes_jumps(benchmark::State &state) {
  for (auto _ : state) {
    ntlib::experiments::eratosthenes_jumps<SieveType>(N);
  }
}
BENCHMARK_TEMPLATE(BM_eratosthenes_jumps, ntlib::sieve<>)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_eratosthenes_jumps, ntlib::sieve_235<>)->UNIT_MS;

template<typename SieveType>
static void BM_eratosthenes_segmented(benchmark::State &state) {
  for (auto _ : state) {
    ntlib::experiments::eratosthenes_segmented<SieveType>(N);
  }
}
BENCHMARK_TEMPLATE(BM_eratosthenes_segmented, ntlib::sieve<>)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_eratosthenes_segmented, ntlib::sieve_235<>)->UNIT_MS;

template<std::size_t SEG_SIZE>
static void BM_eratosthenes_segmented_segsize(benchmark::State &state) {
  for (auto _ : state) {
    ntlib::experiments::eratosthenes_segmented<ntlib::sieve_235<>, SEG_SIZE>(N);
  }
}
BENCHMARK_TEMPLATE(BM_eratosthenes_segmented_segsize, 1 << 17)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_eratosthenes_segmented_segsize, 1 << 18)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_eratosthenes_segmented_segsize, 1 << 19)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_eratosthenes_segmented_segsize, 1 << 20)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_eratosthenes_segmented_segsize, 1 << 21)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_eratosthenes_segmented_segsize, 1 << 22)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_eratosthenes_segmented_segsize, 1 << 23)->UNIT_MS;

BENCHMARK_MAIN();
