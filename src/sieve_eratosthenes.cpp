#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <vector>

#include "include/sieve_eratosthenes.hpp"

void test_sieve_eratosthenes(std::size_t N) {
  std::vector<bool> sieve(N + 1, true);

  auto t0 = std::chrono::high_resolution_clock::now();
  ntlib::sieve_eratosthenes(N, sieve);
  auto t1 = std::chrono::high_resolution_clock::now();
  double t = std::chrono::duration_cast<std::chrono::milliseconds>
      (t1 - t0).count() / 1'000.0;

  std::size_t counter = 0;
  for (std::size_t i = 0; i <= N; ++i) counter += sieve[i];
  printf("Sieve of Eratosthenes (classic):\n");
  printf("Number of primes not exceeding %ld: %ld\n", N, counter);
  printf("Time: %.3lf seconds\n", t);
  printf("\n");
}

template<typename NumberType>
void test_sieve_eratosthenes_list(std::size_t N) {
  std::vector<bool> sieve(N + 1, true);
  std::vector<NumberType> primes;

  auto t0 = std::chrono::high_resolution_clock::now();
  ntlib::sieve_eratosthenes_list(N, sieve, primes);
  auto t1 = std::chrono::high_resolution_clock::now();
  double t = std::chrono::duration_cast<std::chrono::milliseconds>
      (t1 - t0).count() / 1'000.0;

  std::size_t counter = 0;
  for (std::size_t i = 0; i <= N; ++i) counter += sieve[i];
  printf("Sieve of Eratosthenes (generate):\n");
  printf("Number of primes not exceeding %ld: %ld\n", N, counter);
  printf("Size of output array: %ld\n", primes.size());
  printf("Time: %.3lf seconds\n", t);
  printf("\n");
}

template<typename NumberType>
void test_sieve_eratosthenes_list_segmented(std::size_t N) {
  std::vector<NumberType> primes;

  auto t0 = std::chrono::high_resolution_clock::now();
  ntlib::sieve_eratosthenes_list_segmented(N, primes);
  auto t1 = std::chrono::high_resolution_clock::now();
  double t = std::chrono::duration_cast<std::chrono::milliseconds>
      (t1 - t0).count() / 1'000.0;

  printf("Sieve of Eratosthenes (generate, segmented):\n");
  printf("Size of output array: %ld\n", primes.size());
  printf("Time: %.3lf seconds\n", t);
  printf("\n");
}

int main() {
  const std::size_t N = 1'000'000'000LL;
  using NumberType = uint64_t;
  test_sieve_eratosthenes(N);
  test_sieve_eratosthenes_list<NumberType>(N);
  test_sieve_eratosthenes_list_segmented<NumberType>(N);

  return 0;
}
