# Number Theory Library (NTLib)

NTLib is a C++ template library containing several common number theory implementations.
The library contains modular implementations that can easily be combined to solve mathematical programming problems.

My main motivation for NTLib was to have a generic library for [Project Euler](https://projecteuler.net) problems.
At the same time, this project allows me to try out the newest C++ features.

## Some Features of NTLib
* Basic number theoretic functions such as `gcd`, `lcm`, `isqrt`, ...
* Primality test, prime decomposition and prime generation.
* Generate all primitive Pythagorean triples.
* Solve Pell's equation.
* Compute binomial coefficients, polygonal numbers, Euler's totient function, divisor function and Turan numbers.
* ...

## Documentation
See here for the [Doxygen documentation](https://pjungeblut.github.io/ntlib/).

# Requirements
NTLib uses several modern C++ features, in particular C++20 modules.
This requires up to date tools to compile.

Tested with:
* C++23
* Clang-18.1
* Ninja 1.11
* CMake and CTest 3.28

# Downloading NTLib
To download NTLib clone this repository:
```
git clone --recurse-submodules https://github.com/pjungeblut/ntlib
```

## Compiling NTLib
Inside the `ntlib` directory, run the following to bulid all tests and benchmarks:

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=<build-type> -DCMAKE_CXX_COMPILER=clang++-18 .. -G Ninja
ninja
```

Regarding `<build-type>`, choose `Debug` to run tests and `Release` to run benchmarks.
Alternatively, create separate `debug` and `release` directories.

## Run Tests
NTLib comes with many unit tests to guarantee correctness.
Inside the `build` directory, run `ctest` to execute all tests at once.

You can also build and run specific sets of unit tests.
For example, consider we want to test the basic functionality from `include/base.hpp`.
* Run `ninja ut_base`. In general, all unit test targets are prefixed with `ut_`.
* Run `./test/base` to start the test.

## Run Performance Benchmarks
Some components of the libary come with performance benchmarks to test their implementations agains others.
All benchmark targets are prefixed with `bm_`.

For example, to benchmark different implementations for prime generation, run (inside the `build` directory):
```
ninja bm_prime_generation
./benchmark/prime_generation
```
**Note 1:** To get realistic benchmarks you should build the library in release mode (see above).

**Note 2:** To get more consistent benchmarks CPU scaling should be disabled.
This can for example be achieved by (source [here](https://github.com/google/benchmark#disabling-cpu-frequency-scaling)):
```
sudo cpupower frequency-set --governor performance
./benchmark/prime_generation
sudo cpupower frequency-set --governor powersave
```

# License & Disclaimer
This software is licensed under the MIT license.
See `LICENSE` for details.
NTLib is still in development.
Therefore, things could break and the API might change.

Feedback is always welcome.

