/**
 * @file
 * @brief Module interface unit for module `prime_generation`, partition
 * `sieve_235`.
 */
module;

#include <climits>
#include <cstddef>
#include <cstring>
#include <memory>
#include <vector>

/**
 * @module prime_generation:sieve_235
 * @brief A sieve of fixed capacity that stores all multiples of 2, 3 and 5
 * only implicitly.
 * 
 * All strict multiples of 2, 3 and 5 are always zero while 2, 3 and 5 however
 * are always true.
 *
 * This allows to optimize for space as from every 30 consecutive numbers only
 * the eight ones with residues 1, 7, 11, 13, 17, 19, 23, 29 modulo 30 need to
 * be stored explicitly. Those can be stored in a single byte resulting in a
 * memory improvement by a factor of 30 compared to a `std::vector<std::byte>`
 * or by a factor of 15/4 compared to a `std::vector<bool>`.
 */
export module prime_generation:sieve_235;

namespace ntlib {

/**
 * Represents a 235-wheel optimized sieve.
 * 
 * @tparam Allocator The allocator to use for the sieve.
 */
export template <class Allocator = std::allocator<std::byte>>
class sieve_235 {
  /**
   * The number of elements stored in a single byte.
   * Assumes that a byte consists of 8 bits.
   */
  static constexpr std::size_t PER_BYTE = 2 * 3 * 5;
  static_assert(CHAR_BIT == 8);

  /**
   * Stores a bitmaks to access the sieve for each possible remainder
   * modulo 30.
   */
  static constexpr std::byte MASK[PER_BYTE] = {
      std::byte{0x00}, std::byte{0x80}, std::byte{0x00}, std::byte{0x00},
      std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x40},
      std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x20},
      std::byte{0x00}, std::byte{0x10}, std::byte{0x00}, std::byte{0x00},
      std::byte{0x00}, std::byte{0x08}, std::byte{0x00}, std::byte{0x04},
      std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x02},
      std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x00},
      std::byte{0x00}, std::byte{0x01}};

  /**
   * Memory for the sieve.
   */
  std::vector<std::byte> memory;

public:
  /**
   * Constructs an empty sieve.
   */
  sieve_235() = default;

  /**
   * Constructs the sieve with a capacity at least as big as `min_capacity`.
   *
   * @param min_capacity The minimum required capacity.
   */
  sieve_235(std::size_t min_capacity) :
      memory((min_capacity + PER_BYTE - 1) / PER_BYTE) {
    static_assert(CHAR_BIT == 8,
        "235 sieve optimization only works for 8 bits per byte.");
  }

  /**
   * A proxy class to provide an lvalue that can be returned from operator[].
   */
  class reference {
    friend class sieve_235;

    /**
     * Constant pointer to the element in the sieve to work on.
     */
    std::byte* const byte;

    /**
     * Whether the current byte is the first one.
     */
    const bool first_byte;

    /**
     * The Index for the referenced bit.
     */
    const std::size_t idx;

    /**
     * Constructs the reference.
     *
     * @param byte The byte to work on.
     * @param first_byte Whether the current byte is the first one.
     * @param idx The idx of the bit in `byte` to work on.
     */
    reference(std::byte *byte, bool first_byte, std::size_t idx) :
        byte(byte), first_byte(first_byte), idx(idx) {}

  public:
    /**
     * Assigns a value to the referenced sieve entry.
     *
     * @param val The value to set the current element to. If the element is a
     *            multiple of 2, 3 or 5 and `val` is `true`, this operation is
     *            ignored.
     * @return Reference to this reference object.
     */
    reference& operator=(bool val) {
      *byte = val ? *byte |= MASK[idx] : *byte &= ~MASK[idx];
      return *this;
    }

    /**
     * Returns the value of the current element.
     *
     * @return Whether the current element is `true` or `false`.
     */
    [[nodiscard]]
    operator bool() const {
      if (first_byte && (idx == 2 || idx == 3 || idx == 5)) return true;
      return (*byte & MASK[idx]) != std::byte{0};
    }
  };

  /**
   * Initializes the sieve with `true` everywhere execept for strict
   * multiples of 2, 3 and 5.
   */
  void init235() {
    memset(memory.data(), 0xFF, memory.size());
  }

  /**
   * Subscript operator for constant array like access.
   *
   * @param idx The index of the element to return.
   * @return The value at the given index.
   */
  [[nodiscard]]
  bool operator[](std::size_t idx) const {
    if (idx == 2 || idx == 3 || idx == 5) return true;
    return (memory[idx / PER_BYTE] & MASK[idx % PER_BYTE]) != std::byte{0};
  }

  /**
   * Subscript operator for array like access.
   *
   * @param idx The index to return.
   * @return A proxy to the element at the given index.
   */
  [[nodiscard]]
  reference operator[](std::size_t idx) {
    return reference(&memory[idx / PER_BYTE], idx < PER_BYTE, idx % PER_BYTE);
  }

  /**
   * Tests whehter the sieve is empty.
   *
   * @return Whether the sieve is empty.
   */
  [[nodiscard]]
  bool empty() const noexcept {
    return memory.empty();
  }

  /**
   * Returns the current size of the sieve.
   *
   * @return The size of the sieve.
   */
  [[nodiscard]]
  std::size_t size() const noexcept {
    return memory.size() * PER_BYTE;
  }

  /**
   * Returns a const pointer to the underlying data array.
   *
   * @return Constant pointer to the underlying data array.
   */
  [[nodiscard]]
  const std::byte* data() const noexcept {
    return memory.data();
  }

  /**
   * Returns a pointer to the underlying data array.
   *
   * @return Pointer to the underlying data array.
   */
  [[nodiscard]]
  std::byte* data() noexcept {
    return memory.data();
  }
};

}
