#include "base.hpp"

#include <random>

namespace ntlib {

template<Integral I>
I abs(I n) {
  if (n >= 0) return n;
  return -n;
}

template<Integral I>
I gcd(I a, I b) {
  return b == 0 ? abs(a) : gcd(b, a % b);
}

template<Integral I>
I lcm(I a, I b) {
  return abs(a) * (abs(b) / gcd(a, b));
}

template<Integral I>
triple<I,I,I> extended_euclid(I a, I b) {
  if (a == 0) return triple<I,I,I> {b, 0, 1};
  triple<I,I,I> gxy = extended_euclid(b % a, a);
  return triple<I,I,I> {gxy.a, gxy.c - (b / a) * gxy.b, gxy.b};
}

template<Integral I>
I pow(I a, I b) {
  if (b == 0) return 1;
  if (b == 1) return a;
  if (b & 1) return pow(a, b - 1) * a;
  return pow(a * a, b / 2);
}

template<Integral I>
I isqrt(I n) {
  I l = 0;
  I u = n;
  while (u - l > 16) {
    I m = (u + l) / 2;
    if (m * m <= n) l = m;
    else u = m;
  }
  while (l * l <= n) ++l;
  return l - 1;
}

template<Integral I>
bool is_square(I n) {
  if (n < 0) return false;
  if (n == 0) return true;

  I last_digit = n % 10;
  I second_last_digit = n / 10 % 10;
  I third_last_digit = n / 100 % 10;

  // If n is a multiple of four, we can look at n/4 instead.
  while (n && (n & 3) == 0) n >>= 2;
  if (n == 0) return true;

  // If n is not divisible by four (it is not by above test), its binary
  // representation must end with 001.
  if (!((n & 7) == 1)) return false;

  // All squares end in the numbers 0, 1, 4, 5, 6, or 9.
  if (last_digit == 2 || last_digit == 3 || last_digit == 7 ||
      last_digit == 8) {
    return false;
  }

  // The last two digits cannot both be odd.
  if ((last_digit & 1) && (second_last_digit & 1)) return false;

  // If the last digit is 1 or 9, the two digits befor must be a multiple of 4.
  if (last_digit == 1 || last_digit == 9) {
    if ((third_last_digit * 10 + second_last_digit) % 4 != 0) return false;
  }

  // If the last digit is 4, the digit before it must be even.
  if (last_digit == 4 && (second_last_digit & 1)) return false;

  // If the last digit is 6, the digit before it must be odd.
  if (last_digit == 6 && !(second_last_digit & 1)) return false;

  // If the last digit is 5, the digit before it must be 2.
  if (last_digit == 5 && second_last_digit != 2) return false;

  // Take the integer root and square it to check, if the real root is an
  // integer.
  I iroot = isqrt(n);
  return iroot * iroot == n;
}

template<Integral I>
I mod_pow(I a, I b, I n) {
  if (b == 0) return 1;
  if (b == 1) return a % n;
  if (b & 1) return (mod_pow(a, b - 1, n) * a) % n;
  return mod_pow((a * a) % n, b / 2, n);
}

template<Integral I>
I mod_mult_inv(I n, I m) {
  auto gxy = extended_euclid(n, m);
  return ((gxy.b % m) + m) % m;
}

template<Integral I>
bool mod_is_square(I n, I p) {
  if (p == 2) return n & 1;
  return mod_pow(n, (p - 1) / 2, p) == 1;
}

template<Integral I>
I mod_sqrt(I n, I p) {
  // Find q, s with p-1 = q*2^s.
  I q = p - 1;
  I s = 0;
  while (!(q & 1)) {
    q /= 2;
    ++s;
  }

  // If and only if s == 1, we have p = 3 (mod 4).
  // In this case we can compute root x directly.
  if (s == 1) return mod_pow(n, (p + 1) / 4, p);

  // Find a quadratic non-residue z.
  // Half the numbers in 1, ..., p-1 are, so we randomly guess.
  // On average, two tries are necessary.
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, p - 1);
  I z = dis(gen);
  while (mod_is_square(z, p)) z = dis(gen);

  I c = mod_pow(z, q, p);
  I x = mod_pow(n, (q + 1) / 2, p);
  I t = mod_pow(n, q, p);
  I m = s;

  while (t % p != 1) {
    // Find lowest 0 < i < m such that t^2^i = 1 (mod p).
    I i = 0;
    I test = t;
    while (test != 1) {
      test = test * test % p;
      ++i;
    }

    // U cexp = mod_pow(static_cast<U>(2), m - i - 1, p - 1);
    I cexp = static_cast<I>(1) << (m - i - 1);
    I b = mod_pow(c, cexp, p);
    x = x * b % p;
    t = t * b % p * b % p;
    c = b * b % p;
    m = i;
  }

  return std::min(x, p - x);
}

}
