#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <vector>

#include "include/prime_decomposition.hpp"
#include "include/sieve_eratosthenes.hpp"

template<typename NumberType>
void test_prime_decomposition_naive(NumberType n) {
  std::vector<NumberType> factors;

  auto t0 = std::chrono::high_resolution_clock::now();
  ntlib::prime_decomposition_naive(n, factors);
  auto t1 = std::chrono::high_resolution_clock::now();
  double t = std::chrono::duration_cast<std::chrono::milliseconds>
      (t1 - t0).count() / 1'000.0;

  printf("Naive decomposition:\n");
  printf("Factors of %ld:", n);
  for (NumberType f : factors) printf(" %ld", f);
  printf("\n");
  printf("Time: %.3lf seconds\n", t);
  printf("\n");
}

template<typename NumberType>
void test_prime_decomposition_list(NumberType n, std::vector<NumberType> &list) {
  std::vector<NumberType> factors;

  auto t0 = std::chrono::high_resolution_clock::now();
  ntlib::prime_decomposition_list(n, factors, list);
  auto t1 = std::chrono::high_resolution_clock::now();
  double t = std::chrono::duration_cast<std::chrono::milliseconds>
      (t1 - t0).count() / 1'000.0;

  printf("List decomposition:\n");
  printf("Factors of %ld:", n);
  for (NumberType f : factors) printf(" %ld", f);
  printf("\n");
  printf("Time: %.3lf seconds\n", t);
  printf("\n");
}

template<typename NumberType>
void test_prime_decomposition_rho(NumberType n) {
  std::vector<NumberType> factors;

  auto t0 = std::chrono::high_resolution_clock::now();
  ntlib::prime_decomposition_rho(n, factors);
  auto t1 = std::chrono::high_resolution_clock::now();
  double t = std::chrono::duration_cast<std::chrono::milliseconds>
      (t1 - t0).count() / 1'000.0;

  printf("Rho decomposition:\n");
  printf("Factors of %ld:", n);
  for (NumberType f : factors) printf(" %ld", f);
  printf("\n");
  printf("Time: %.3lf seconds\n", t);
  printf("\n");
}

int main() {
  using NumberType = uint64_t;
  const NumberType L = 1'000'000'007LL * 1'000'000'007LL; // Composite.
  const NumberType N = 1'000'003LL * 1'000'003LL * 1'000'003LL; // Composite.
  const NumberType M = 952'016'363'681'739'749LL; // Prime.

  test_prime_decomposition_naive(L);
  test_prime_decomposition_naive(N);
  test_prime_decomposition_naive(M);

  std::vector<NumberType> primes;
  ntlib::sieve_eratosthenes_generate_segmented(1'000'000'007, primes);
  test_prime_decomposition_list(L, primes);
  test_prime_decomposition_list(N, primes);
  test_prime_decomposition_list(M, primes);

  test_prime_decomposition_rho(L);
  test_prime_decomposition_rho(N);
  test_prime_decomposition_rho(M);

  return 0;
}
