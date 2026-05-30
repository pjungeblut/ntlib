# Number Theory Library (NTLib)

NTLib is a C++ template library containing common number theory implementations.
All library types and templates are compatible to one another and can easily be combined to solve mathematical programming problems.

The main motivation for NTLib was to have a generic library for [Project Euler](https://projecteuler.net) problems.
As such, most of the first 100 problems can easily solved with NTLib in just a few lines of C++ code.

## Some Features of NTLib
* Basic number theoretic functions such as `gcd`, `lcm`, `isqrt`, ...
* Primality test, prime decomposition and prime generation.
* Generate primitive Pythagorean triples.
* Solve several kinds of Diophantine equations, i.e., Pell's equation.
* Compute binomial coefficients, polygonal numbers, Euler's totient function, divisor function and Turan numbers.
* ...

## Documentation
See here for the complete [Doxygen documentation](https://pjungeblut.github.io/ntlib/).

# Getting Started

## Requirements
NTLib uses up-to-date C++ features.
In particular:
* C++20 modules (including C++23 `import std;`)
* C++23 standard library features

As a consequence (especially because of the modules), you need recent tooling to sucessfully compile the library.
A working set of tools under Ubuntu 26.04 is:
* g++ 16.0.1
* Ninja 1.13.2
* CMake and CTest 4.2.3

## Download NTLib
Clone NTLib into a new `ntlib` directory:

```bash
git clone https://github.com/pjungeblut/ntlib ntlib
```

There are no git submodules and no other dependencies are required to use NTLib.

## Usage
TODO (pjungeblut): Document how to include NTLib using CMake.

## Development
If you don't just want to use but extend/adapt NTLib, this is how to build all unit tests:
Move into the `ntlib` directory and configure CMake using the `debug` preset (there is also a `release` preset to build everything in release mode).
Then build the library and unit tests using the `tests` preset.

```bash
cd ntlib
cmake --preset debug            # configure the build
cmake --build --preset tests    # build the library and all unit tests
ctest --preset tests            # run all unit tests
```

To run the tests, navigate to `build/debug/` (or `build/release/`) and run `ctest`.

### Build and Run Tests for Individual Modules
Each module comes with its own unit test which can be build individually.

For example, consider the `base` module.
The following can be used to build and run only the unit tests for the `base` module:

```bash
cd build/debug   # alternatively, go to `build/release`
ninja ut_base    # all unit-test targets are name with `ut_<module-name>`
./test/base      # the test executable is named like the module under test
```

### Generate Code Coverage Reports

**Note:** This requires `gcovr` to be installed.

**Note:** This currently only works with clang.
For other compilers, adapt the `--gcov-executable` flag in the root `CMakeLists.txt` file.

The following can be used to run all tests and execute coverage reports for the library.
The report is written to `build/tests/coverage/index.html`.

```bash
cmake --build --preset tests --target coverage
```

# License & Disclaimer
This software is licensed under the MIT license.
See `LICENSE` for details.
NTLib is still in development.
Therefore, things will break and the API/ABI will change.

Feedback is always welcome.

