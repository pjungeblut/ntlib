#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <vector>

#include "include/prime_decomposition.hpp"

template<typename NumberType>
void test_is_prime_naive(NumberType n) {
  auto t0 = std::chrono::high_resolution_clock::now();
  bool is_prime = ntlib::is_prime_naive(n);
  auto t1 = std::chrono::high_resolution_clock::now();
  double t = std::chrono::duration_cast<std::chrono::milliseconds>
      (t1 - t0).count() / 1'000.0;

  printf("Naive prime test of %ld: %d\n", n, is_prime);
  printf("Time: %.3lf seconds\n", t);
  printf("\n");
}

template<typename NumberType>
void test_is_prime_miller_rabin(NumberType n) {
  auto t0 = std::chrono::high_resolution_clock::now();
  bool is_prime = ntlib::is_prime_miller_rabin(n);
  auto t1 = std::chrono::high_resolution_clock::now();
  double t = std::chrono::duration_cast<std::chrono::milliseconds>
      (t1 - t0).count() / 1'000.0;

  printf("Miller Rabin prime test of %ld: %d\n", n, is_prime);
  printf("Time: %.3lf seconds\n", t);
  printf("\n");
}

int main() {
  using NumberType = uint64_t;
  const NumberType L = 1'000'000'007LL * 1'000'000'007LL; // Composite.
  const NumberType N = 4'120'038'565'055'551LL; // Composite.
  const NumberType M = 952'016'363'681'739'749LL; // Prime.

  test_is_prime_naive(L);
  test_is_prime_naive(N);
  test_is_prime_naive(M);

  test_is_prime_miller_rabin(L);
  test_is_prime_miller_rabin(N);
  test_is_prime_miller_rabin(M);

  return 0;
}
