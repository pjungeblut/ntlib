#include <benchmark/benchmark.h>

#include <bitset>
#include <cstddef>
#include <vector>

#include "sieve_235.hpp"

static constexpr std::size_t N = 1'000'000;

static void BM_set_all_sieve_235(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    ntlib::sieve_235 sieve(N, true);
    state.ResumeTiming();
    for (std::size_t i = 0; i < N; ++i) {
      sieve[i] = false;
    }
  }
}
BENCHMARK(BM_set_all_sieve_235)->Unit(benchmark::kMillisecond);

static void BM_set_all_vector_bool(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    std::vector<bool> sieve(N, true);
    state.ResumeTiming();
    for (std::size_t i = 0; i < N; ++i) {
      sieve[i] = false;
    }
  }
}
BENCHMARK(BM_set_all_vector_bool)->Unit(benchmark::kMillisecond);

static void BM_set_all_vector_char(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    std::vector<char> sieve(N, true);
    state.ResumeTiming();
    for (std::size_t i = 0; i < N; ++i) {
      sieve[i] = false;
    }
  }
}
BENCHMARK(BM_set_all_vector_char)->Unit(benchmark::kMillisecond);

static void BM_set_all_vector_byte(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    std::vector<std::byte> sieve(N, std::byte{1});
    state.ResumeTiming();
    for (std::size_t i = 0; i < N; ++i) {
      sieve[i] = std::byte{0};
    }
  }
}
BENCHMARK(BM_set_all_vector_byte)->Unit(benchmark::kMillisecond);

static void BM_set_all_bitset(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    std::bitset<N> sieve;
    sieve.set();
    state.ResumeTiming();
    for (std::size_t i = 0; i < N; ++i) {
      sieve[i] = false;
    }
  }
}
BENCHMARK(BM_set_all_bitset)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
