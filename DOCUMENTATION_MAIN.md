\mainpage
# Documentation

The *Number Theory Library*, or *NTLib* is a C++ library providing useful functions for number theoretic (and related) computations.

It is written using modern C++.
In particular, all functionality is provided as C++20 modules.
Thus, building NTLib requires an up to date compiler and build system.

# Feature Overview

## Basic Functionality

* base.cpp:
  Several important primitives that used throughout the library, e.g., `ntlib::gcd()` to compute the greatest common divisor.
  Code in this module must not depend on anything except for the C++ standard library.
* chinese_remainder.cpp:
  Solve systems of modular congruences using the Chinese remainder theorem.
* int128.cpp:
  Signed and unsigned 128 bit integer types.
* modulo.cpp:
  Compute several classical functions modulo some number $m$.
* mod_int.cpp:
  A type for integers modulo some number $m$.
  The modulus $m$ can either be provided at run time or at compile time, the latter allowing for compiler optimizations.

## Prime Numbers

* prime_decomposition.cpp:
  Decompose an integer into its prime factors and their multiplicities.
* prime_generation.cpp:
  Generate lists of primes.
* prime_test.cpp:
  Check whether an integer is prime, deterministically or probably.

## Arithmetic Functions

* divisors.cpp:
  Count and enumerate the divisors of an integer or compute its divisor function.
* euler_totient.cpp:
  Compute Euler's totient function \f$\phi\f$.

## Diophantine Equations

* diophantine_equation.cpp:
  Solve diophantine equations of different types.
* pell_equation.cpp:
  Solve Pell's equation.

## Combinatorics

* binomial_coefficient.cpp:
  Compute the binomial coefficient \f${n \choose k}\f$, possibly modulo some number \f$m\f$.
* figurate_number.cpp:
  Compute figurate numbers, e.g., polygonal numbers or polyhedral numbers.
* pythagorean_triple.cpp:
  Generate all primitive Pythagorean triples with entries up to some upper bound \f$N\f$.
* turan_number.cpp:
  Compute the Turan number \f$T(n,k)\f$.

## Types

* matrix.cpp:
  Matrix of some type `T`.
* rational.cpp:
  Rational number with numerator and denominator of some type `T`.

## Other

* continued_fraction.cpp:
  Compute the continued fraction expansion of quadratic irrationals.
  This is mainly used as a subroutine in pell_equation.cpp.
* lucas_sequence.cpp:
  Compute the $n$-th terms of the Lucas sequences \f$U_n(P,Q)\f$ and \f$V_n(P,Q)\f$.
  This is mainly used as a subroutine in a Baillie-Pomerance-Selfridge-Wagstaff primality test.

