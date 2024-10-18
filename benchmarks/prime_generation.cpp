#include <benchmark/benchmark.h>

#include "experiments/prime_generation.hpp"
#include "sieve.hpp"
#include "sieve_235.hpp"

import prime_generation;

#define UNIT_MS Unit(benchmark::kMillisecond)

static const uint64_t N = 100'000'000;

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
static void BM_prime_sieve(benchmark::State &state) {
  for (auto _ : state) {
    ntlib::prime_sieve<uint64_t, std::allocator<uint64_t>, SieveType>(N);
  }
}
BENCHMARK_TEMPLATE(BM_prime_sieve, ntlib::sieve<>)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_prime_sieve, ntlib::sieve_235<>)->UNIT_MS;

template<std::size_t SEG_SIZE>
static void BM_prime_sieve_segsize(benchmark::State &state) {
  for (auto _ : state) {
    ntlib::prime_sieve<
        uint64_t, std::allocator<uint64_t>, ntlib::sieve_235<>, SEG_SIZE>(N);
  }
}
BENCHMARK_TEMPLATE(BM_prime_sieve_segsize, 1 << 17)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_prime_sieve_segsize, 1 << 18)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_prime_sieve_segsize, 1 << 19)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_prime_sieve_segsize, 1 << 20)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_prime_sieve_segsize, 1 << 21)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_prime_sieve_segsize, 1 << 22)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_prime_sieve_segsize, 1 << 23)->UNIT_MS;

template<typename SieveType>
static void BM_prime_sieve_list(benchmark::State &state) {
  for (auto _ : state) {
    std::vector<std::size_t> primes;
    ntlib::prime_sieve<
        uint64_t, std::allocator<uint64_t>, SieveType>(N, primes);
  }
}
BENCHMARK_TEMPLATE(BM_prime_sieve_list, ntlib::sieve<>)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_prime_sieve_list, ntlib::sieve_235<>)->UNIT_MS;

template<typename SieveType>
static void BM_prime_sieve_lookup(benchmark::State &state) {
  const auto sieve =
      ntlib::prime_sieve<uint64_t, std::allocator<uint64_t>, SieveType>(N);
  for (auto _ : state) {
    for (std::size_t i = 0; i <= N; ++i) {
      benchmark::DoNotOptimize(sieve[i]);
    }
  }
}
BENCHMARK_TEMPLATE(BM_prime_sieve_lookup, ntlib::sieve<>)->UNIT_MS;
BENCHMARK_TEMPLATE(BM_prime_sieve_lookup, ntlib::sieve_235<>)->UNIT_MS;

BENCHMARK_MAIN();
