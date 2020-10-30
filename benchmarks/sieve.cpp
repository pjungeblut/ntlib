#include <benchmark/benchmark.h>

#include <bitset>
#include <cstddef>
#include <vector>

#include "sieve.hpp"
#include "sieve_235.hpp"

static constexpr std::size_t N = 1'000'000;

static void BM_set_all_sieve(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    ntlib::sieve sieve(N);
    state.ResumeTiming();
    for (std::size_t i = 0; i < N; ++i) {
      sieve[i] = true;
    }
  }
}
BENCHMARK(BM_set_all_sieve)->Unit(benchmark::kMicrosecond);

static void BM_set_all_sieve_235(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    ntlib::sieve_235 sieve(N);
    state.ResumeTiming();
    for (std::size_t i = 0; i < N; ++i) {
      sieve[i] = true;
    }
  }
}
BENCHMARK(BM_set_all_sieve_235)->Unit(benchmark::kMicrosecond);

static void BM_set_all_vector_bool(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    std::vector<bool> sieve(N);
    state.ResumeTiming();
    for (std::size_t i = 0; i < N; ++i) {
      sieve[i] = true;
    }
  }
}
BENCHMARK(BM_set_all_vector_bool)->Unit(benchmark::kMicrosecond);

static void BM_init235_sieve(benchmark::State &state) {
  for (auto _ : state) {
    std::size_t M = (N + 29) / 30 * 30;
    ntlib::sieve sieve(M);
    for (std::size_t i = 0; i < N; i += 30) {
      sieve[i + 1] = true;
      sieve[i + 7] = true;
      sieve[i + 11] = true;
      sieve[i + 13] = true;
      sieve[i + 17] = true;
      sieve[i + 19] = true;
      sieve[i + 23] = true;
      sieve[i + 29] = true;
    }
  }
}
BENCHMARK(BM_init235_sieve)->Unit(benchmark::kMicrosecond);

static void BM_init235_sieve235(benchmark::State &state) {
  for (auto _ : state) {
    ntlib::sieve_235 sieve(N);
    memset(sieve.data(), 0xFF, (N + 29) / 30);
  }
}
BENCHMARK(BM_init235_sieve235)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
