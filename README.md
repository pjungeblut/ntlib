# Number Theory Library (NTLib)

NTLib is a C++ template library containing several common number theory implementations.
The library contains modular implementations that can easily be combined to solve mathematical programming problems.

The main motivation for NTLib was to have a generic library for [Project Euler](https://projecteuler.net) problems.

## Some Features of NTLib
* Basic number theoretic functions such as `gcd`, `lcm`, `isqrt`, ...
* Primality test, prime decomposition and prime generation.
* Generate all primitive Pythagorean triples.
* Solve several kinds of Diophantine equations, i.e., Pell's equation.
* Compute binomial coefficients, polygonal numbers, Euler's totient function, divisor function and Turan numbers.
* ...

## Documentation
See here for the [Doxygen documentation](https://pjungeblut.github.io/ntlib/).

# Getting Started

## Requirements
NTLib uses several modern C++ features, including:
* C++20 modules
* several C++23 standard library features

As a consequence, you need up to date tooling to sucessfully compile the library.
A working set of tools is:
* Clang-19.1, including Clang Tools 19
* Ninja 1.11
* CMake and CTest 3.28

## Download NTLib
Clone NTLib a new `ntlib` directory:
```
git clone https://github.com/pjungeblut/ntlib ntlib
```
There are no git submodules are any other dependencies.

## Debug Builds (for Development)
Move into the `ntlib` directory and execute the CMake `debug` preset.
This will build all unit tests.
```
cd ntlib
cmake --preset debug
cmake --build --preset tests
```

## Release Builds and Installation (for Usage in other Projects)
TODO
```
cd ntlib
cmake --preset release
...
```

## Run Tests
NTLib comes with many unit tests to guarantee correctness.
Inside the `build` directory, run `ctest` to execute all tests at once.

You can also build and run specific sets of unit tests.
For example, consider we want to test the basic functionality from `include/base.hpp`.
* Run `ninja ut_base`. In general, all unit test targets are prefixed with `ut_`.
* Run `./test/base` to start the test.

# License & Disclaimer
This software is licensed under the MIT license.
See `LICENSE` for details.
NTLib is still in development.
Therefore, things will break and the API will change.

Feedback is always welcome.

