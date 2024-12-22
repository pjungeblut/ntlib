/**
 * @file
 * @brief Primary module interface unit for module `base`.
 */
module;

#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <climits>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <functional>
#include <numeric>
#include <type_traits>
#include <vector>

/**
 * @module base
 * @brief Basic functionality used frequently throughout the whole library
 * 
 * Functionality contained in this module has no further dependencies and only
 * depends on the C++ standard library.
 */
export module base;

namespace ntlib {

/**
 * @brief A list with all prime numbers up to `ntlib::SMALL_PRIMES_BIGGEST`.
 * 
 * Can be used for trial division in primality tests and prime factorizations.
 * 
 * @tparam T An integer-like type.
 */
export template<typename T>
constexpr auto SMALL_PRIMES = std::to_array<T>({
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
    157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
    239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317,
    331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419,
    421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503,
    509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607,
    613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701,
    709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811,
    821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911,
    919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997, 1'009});
static_assert(std::ranges::is_sorted(SMALL_PRIMES<uint32_t>),
    "Some NTLIB-internal functions assume that the list is sorted.");

/**
 * @brief Helper constant to get the biggest among the small primes in
 * `ntlib::SMALL_PRIMES`.
 * 
 * @tparam T An integer-like type.
 */
export template<typename T>
constexpr T SMALL_PRIMES_BIGGEST =
    *std::ranges::max_element(SMALL_PRIMES<T>);

/**
 * @brief Checks whether a given number is odd.
 * 
 * @tparam T An integer-like type.
 * @param n The given number.
 * @return Whether `n` is odd.
 */
export template<typename T>
[[nodiscard]] constexpr
bool is_odd(T n) noexcept {
  if constexpr (requires(T n) { n & T{1}; }) {
    return n & T{1};
  } else if constexpr (requires(T n) { n % T{2}; }) {
    return n % T{2};
  } else {
    assert(round(n) == n);
    return std::abs(std::fmod(n, 2.0)) == 1.0;
  }
}

/**
 * @brief Checks whether a given number is even.
 * 
 * @tparam T An integer-like type.
 * @param n The given number.
 * @return Whether `n` is even.
 */
export template<typename T>
[[nodiscard]] constexpr
bool is_even(T n) noexcept {
  return !is_odd(n);
}

/**
 * @brief Computes the absolute value of a given number.
 *
 * @tparam T An integer-like type.
 * @param n The given number.
 * @return The absolute value of `n`.
 */
export template<typename T>
[[nodiscard]] constexpr
T abs(T n) noexcept {
  return n >= T{0} ? n : -n;
}

/**
 * @brief Computes the difference between two numbers.
 * 
 * For integral types this is safe against overflows.
 * 
 * @tparam T An integer-like type.
 * @tparam U The unsigned-integer-like type corresponding to `T`. 
 * @param a The first number.
 * @param b The second number.
 * @return The difference, i.e., `abs(a-b)`.
 */
export template<typename T, typename U = std::make_unsigned_t<T>>
[[nodiscard]] constexpr
U difference(T a, T b) noexcept {
  if (a > b) { return static_cast<U>(a) - b; }
  else { return static_cast<U>(b) - a; }
}

/**
 * @brief Computes the sign of a given number.
 * 
 * @tparam T An integer-like type.
 * @param n The given number.
 * @return The sign. One of `-1`, `0` or `+1`.
 */
export template <typename T>
[[nodiscard]] constexpr
int sgn(T n) noexcept {
    return (T{0} < n) - (n < T{0});
}

/**
 * @brief Given a number `n`, computes a pair `(e, o)`, such that `n=2^e*o`.
 * 
 * @tparam T An integer-like type.
 * @param n The number `n` to decompose.
 * @return A pair with `e` and `o` as its first and second element,
 *     respectively. 
 */
export template<typename T>
[[nodiscard]] constexpr
std::pair<T,T> odd_part(T n) noexcept {
  constexpr auto odd_part_non_negative = [](T n) {
    if constexpr (std::is_integral_v<T>) {
      using U = std::make_unsigned_t<T>;
      const T e = std::countr_zero(static_cast<U>(n)) * (n != 0);
      const T o = n >> e;
      return std::make_pair(e, o);
    } else {
      T e{0};
      while (n >= T{1} && is_even(n)) {
        n /= T{2};
        ++e;
      }
      return std::make_pair(e, n);
    }
  };

  auto res = odd_part_non_negative(abs(n));
  res.second *= sgn(n);
  return res;
}

/**
 * @brief Computes the greatest common divisor of two numbers using the
 * Euclidean algorithm.
 *
 * The greatest common divisor of two numbers `a` and `b` (not both zero) is the
 * smallest positive number that divides both `a` and `b`.
 * 
 * Runtime: `O(log a + log b)`.
 *
 * @tparam T An integer-like type.
 * @param a The first number.
 * @param b The second number.
 * @return The greatest common divisor of `a` and `b`.
 */
export template<typename T>
[[nodiscard]] constexpr
T gcd(T a, T b) noexcept {
  assert(!(a == T{0} && b == T{0}));
  return b == T{0} ? ntlib::abs(a) : ntlib::gcd(b, static_cast<T>(a % b));
}

/**
 * @brief Computes the least common multiple of two numbers.
 *
 * The least common multiple of two non-zero integers `a` and `b` is the
 * smallest positive integer that can be divided by both `a` and `b`.
 * 
 * Runtime: `O(log a + log b)`
 *
 * @tparam T An integer-like type.
 * @param a The first number.
 * @param b The second number.
 * @return The least common multiple of `a` and `b`.
 */
export template<typename T>
[[nodiscard]] constexpr
T lcm(T a, T b) noexcept {
  assert(a != T{0});
  assert(b != T{0});
  return abs(a) * (abs(b) / gcd(a, b));
}

/**
 * @brief Extended Euclidean algorithm.
 * 
 * Given two integers `a` and `b`, finds whole number solutions for
 * `a*x + b*y = ntlib::gcd(a,b)`.
 * 
 * @tparam T An integer-like type.
 * @tparam S The signed-integer-like type corresponding to `T`.
 * @param a The first number.
 * @param b The second number.
 * @return Tuple `(gcd(a,b), x, y)`.
 */
export template<typename T, typename S = std::make_signed_t<T>>
[[nodiscard]] constexpr
std::tuple<T, S, S> extended_euclid(T a, T b) noexcept {
  assert(!(a == T{0} && b == T{0}));

  // Extended Euclidean algorithm for non-negative values.
  const std::function<std::tuple<T, S, S>(T, T)>
      extended_euclid_non_negative = [&extended_euclid_non_negative](T a, T b) {
    if (a == S{0}) { return std::make_tuple(b, S{0}, S{1}); }
    auto [gcd, xx, yy] = extended_euclid_non_negative(b % a, a);

    S x{static_cast<S>(yy - static_cast<S>(b / a) * xx)};
    S y{xx};
    return std::make_tuple(gcd, x, y);
  };

  auto [gcd, x, y] = extended_euclid_non_negative(abs(a), abs(b));
  if (a < T{0}) x = -x;
  if (b < T{0}) y = -y;
  return std::make_tuple(gcd, x, y);
}

/**
 * @brief Returns the multiplicative neutral element `1` for integral types.
 * 
 * @tparam T An integral type.
 * @return The multiplicative neutral element of the type.
 */
export template<std::integral T>
[[nodiscard]] constexpr
T get_multiplicative_neutral(T) noexcept
    requires (sizeof(T) <= 8) {
  return T{1};
}

/**
 * @concept HasMultiplicativeNeutral
 * @brief Restricts to types with a multiplicative neutral element.
 * 
 * @tparam T An integer-like type.
 * @param n Element of type `T`.
 */
export template<typename T>
concept HasMultiplicativeNeutral = requires(T n) {
    { get_multiplicative_neutral(n) } -> std::convertible_to<T>;
};

/**
 * @brief Binary exponentation.
 * 
 * Runtime: O(log b)
 *
 * @tparam A A type that is closed under multiplication and contains a
 *     multiplicative neutral element.
 * @tparam B An integer-like type.
 * @param a The base.
 * @param b The exponent, non-negative.
 * @return The power `a^b`.
 */
export template<HasMultiplicativeNeutral A, typename B>
[[nodiscard]] constexpr
A pow(A a, B b) noexcept {
  assert(b >= B{0});

  if (b == B{0}) { return get_multiplicative_neutral(a); }
  else if (b == B{1}) { return a; }
  else if (is_odd(b)) { return pow(a, b - B{1}) * a; }
  else { return pow(a * a, b / B{2}); }
}

/**
 * @brief Computes the integer part of the binary logarithm of a given number.
 *
 * @tparam T An integer-like type.
 * @param n The given number.
 * @return The integer part of the binary logarithm of `n`, i.e.,
 *     `floor(log2(n))`.
 */
export template<typename T>
[[nodiscard]] constexpr
T ilog2(T n) noexcept {
  assert(n > T{0});

  if constexpr (std::is_integral_v<T>) {
    using U = std::make_unsigned_t<T>;
    constexpr std::size_t length = sizeof(T) * CHAR_BIT;
    return static_cast<T>(length - std::countl_zero(static_cast<U>(n)) - 1);
  } else {
    T result{0};
    while ((n /= T{2}) >= T{1}) { ++result; }
    return result;
  }
}

/**
 * @brief Computes the integer square root of a given number.
 *
 * @tparam T An integer-like type.
 * @param n The given number. Must be non-negative.
 * @return The integer square root of `n`, i.e., `floor(sqrt(n))`.
 */
export template<typename T>
[[nodiscard]] constexpr
T isqrt(T n) noexcept {
  assert(n >= T{0});

  if constexpr ((std::is_integral_v<T> && sizeof(T) <= 4) ||
      std::is_same_v<T, double>) {
    return floor(sqrt(n));
  } else if constexpr (std::is_same_v<T, float>) {
    return floorf(sqrtf(n));
  } else if constexpr (std::is_same_v<T, long double>) {
    return floorl(sqrtl(n));
  } else {
    // Checks without overflows that `a*a <= b`.
    const auto square_atmost = [](auto a, auto b) {
      return a <= b / a;
    };

    T result{0}, summand{1};
    while (square_atmost(result + summand, n)) {
      while (square_atmost(result + T{2} * summand, n)) { summand *= T{2}; }
      result += summand;
      summand = T{1};
    }
    return result;
  }
}

/**
 * @brief Tests, if a given number is a perfect square.
 *
 * Uses ideas from this site:
 * https://math.stackexchange.com/questions/131330/detecting-perfect-squares-faster-than-by-extracting-square-root/712818#712818
 * 
 * Runtime: O(log n)
 *
 * @tparam T An integer-like type.
 * @param n The given number.
 * @return Whether `n` is a perfect square.
 */
export template<typename T>
[[nodiscard]] constexpr
bool is_square(T n) noexcept {
  if (n < T{0}) { return false; }
  if (n == T{0}) { return true; }

  T last_digit = n % T{10};
  T second_last_digit = n / T{10} % T{10};
  T third_last_digit = n / T{100} % T{10};

  // If n is a multiple of four, we can look at n/4 instead.
  while (n && (n & T{3}) == T{0}) n >>= 2;
  if (n == T{0}) { return true; }

  // If n is not divisible by four (it is not by above test), its binary
  // representation must end with 001.
  if (!((n & T{7}) == T{1})) { return false; }

  // All squares end in the numbers 0, 1, 4, 5, 6, or 9.
  if (last_digit == T{2} || last_digit == T{3} || last_digit == T{7} ||
      last_digit == T{8}) {
    return false;
  }

  // The last two digits cannot both be odd.
  if (is_odd(last_digit) && is_odd(second_last_digit)) { return false; }

  // If the last digit is 1 or 9, the two digits befor must be a multiple of 4.
  if ((last_digit == T{1} || last_digit == T{9}) &&
      ((third_last_digit * T{10} + second_last_digit) % T{4} != T{0})) {
    return false;
  }

  // If the last digit is 4, the digit before it must be even.
  if (last_digit == T{4} && is_odd(second_last_digit)) { return false; }

  // If the last digit is 6, the digit before it must be odd.
  if (last_digit == T{6} && is_even(second_last_digit)) { return false; }

  // If the last digit is 5, the digit before it must be 2.
  if (last_digit == T{5} && second_last_digit != T{2}) { return false; }

  // Take the integer root and square it to check, if the real root is an
  // integer.
  T iroot = isqrt(n);
  return iroot * iroot == n;
}

/**
 * @brief Computes the factorial of a given number.
 *
 * @tparam T An integer-like type.
 * @param n The given number.
 * @return The factorial of `n`, i.e, `n! = 1 * 2 * ... * n`.
 */
export template<typename T>
[[nodiscard]] constexpr
T factorial(T n) noexcept {
  assert(n >= T{0});

  T result{1};
  for (T i{2}; i <= n; ++i) { result *= i; }
  return result;
}

}