# Number Theory Library (NTLib)

A small C++ header only template library containing common number theory implementations.
The library contains modular implementations that can easily be combined to solve mathematical problems.
Most code was used and developed to solve [Project Euler](https://projecteuler.net) problems.

Some features on NTLib:
* Basic number theoretic functions such as `gcd`, `lcm`, `isqrt`, ...
* Primality test, prime decomposition and prime generation with a heavily optimized variant of the sieve of Eratosthenes.
* Generate all primitive Pythagorean triples.
* Solving Pell's equation.
* Compute binomial coefficients, polygonal numbers, Euler's totient function, divisor function and Turan numbers.

# Requirements and Installation
* Uses C++20, namely the three-way comparison operator. G++-10, Clang-10 and MSVC-19.20 should work.
* Cmake and CTest version 3.16 or newer.

## Install whole Library as a Submodule
In the main directory of your project execute:
```
git submodule add https://github.com/pjungeblut/ntlib ./external/ntlib
```
NTLib is then located `./external/ntlib`.
To build all tests and benchmarks:
```
cd external/ntlib
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Install only Header Files
To get the library clone the git repositiory:
```
git clone https://github.com/pjungeblut/ntlib
```
All header files can than be installed to your project by:
```
./install path_to_your_project
```
The install script copies all header files into subdirectory `path_to_your_project/include/ntlib` from where you can `#include` them.

# Unit Tests
The library comes with extensive unit tests to ensure its correctness.
To run all tests yourself run:
```
cd build
make tests
```
Alternatively you can build and run the tests for only a specific part of the library.
All unit test targets are prefixed with `ut_`.
So to test the base funcitonality (all tests in `./test/base.cpp`) run:
```
cd build
make ut_base
./test/base
```

# Performance Benchmarks
Some components of the libary come with performance benchmarks to test their implementations agains others.
All benchmark targets are prefixed with `bm_`.
Normally the NTLib-implementation is compared to alternative implementations that can be found in `./benchmarks/experiments/`.
To benchmark the different variants to generate a prime sieve (all benchmarks in `./benchmarks/prime_generation.cpp`) run:
```
cd build
make bm_prime_generation
./benchmark/prime_generation
```
**Note 1:** To get realistic benchmarks you should build the library in release mode (see install section).

**Note 2:** To get more constisten benchmarks CPU scaling should be disabled.
This can for example be achieved by (source [here](https://github.com/google/benchmark#disabling-cpu-frequency-scaling)):
```
sudo cpupower frequency-set --governor performance
./benchmark/prime_generation
sudo cpupower frequency-set --governor powersave
```

# License & Disclaimer
This software is licensed under the MIT license. See `LICENSE` for details.
NTLib is still in development. Therefore, things could break and the API might change.

Feedback is always welcome.

