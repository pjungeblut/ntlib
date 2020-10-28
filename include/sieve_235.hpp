#pragma once

/**
 * A sieve similar to a std::bitset<N> with the additional property that all
 * multiples of 2, 3 and 5 are always zero.
 *
 * This allows to optimize for space as from every 30 consecutive numbers only
 * the eight ones with residues 1, 7, 11, 13, 17, 19, 23, 29 modulo 30 need to
 * be stored explicitly. Those can be stored in a single byte.
 */

#include <climits>
#include <cstddef>
#include <cstring>
#include <memory>

namespace ntlib {

template <class Allocator = std::allocator<std::byte>>
class sieve_235 {
  /**
   * The number of elements stored in a single byte.
   * Requires a byte to be 8 bits.
   */
  static constexpr std::size_t PER_BYTE = 2 * 3 * 5;

  /**
   * Allocator for the sieve.
   */
  Allocator sieve_allocator;

  /**
   * The actual capacity of the sieve.
   * Equal to the smallest multiple of 30 that is at least `min_capacity`.
   */
  const std::size_t capacity;

  /**
   * Memory for the sieve.
   */
  std::byte *sieve;

public:
  /**
   * Constructs the sieve with a capacity at least as big as `min_capacity`.
   *
   * @param min_capacity The minimum required capacity.
   * @param init_value The value to initalize each element with (except for
   *                   multiples of 2, 3 and 5 which are always false).
   */
  sieve_235(std::size_t min_capacity, bool init_value) :
      capacity((min_capacity + PER_BYTE - 1) / PER_BYTE) {
    static_assert(CHAR_BIT == 8,
        "235 sieve optimization only works for 8 bits per byte.");

    sieve = sieve_allocator.allocate(capacity);
    memset(sieve, init_value ? 0xFF : 0x00, capacity);
  }

  /**
   * Destructs the sieve and frees the memory.
   */
  ~sieve_235() {
    sieve_allocator.deallocate(sieve, capacity);
  }

  /**
   * A proxy class to provide an lvalue that can be returned from operator[].
   */
  class reference {
    friend class sieve_235;

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
     * Constant pointer to the element in the sieve to work on.
     */
    std::byte * const element;

    /**
     * The Index for the referenced bit.
     */
    const std::size_t idx;

    /**
     * Constructs the reference.
     *
     * @param byte The byte to work on.
     * @param idx The idx of the bit in `byte` to work on.
     */
    reference(std::byte *element, std::size_t idx) :
        element(element), idx(idx) {}

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
      *element = val ? *element |= MASK[idx] : *element &= ~MASK[idx];
      return *this;
    }

    /**
     * Copy assigns a value to the referenced sieve entry.
     *
     * @param val The value to set the current element to. If the element is a
     *            multiple of 2, 3 or 5 and the value of the other element is
     *            `true`, this operation is ignored.
     * @return Reference to this reference object.
     */
    reference& operator=(const reference &other) {
      if ((*other.element & MASK[other.idx]) != std::byte{0}) {
        *element |= MASK[idx];
      } else {
        *element &= ~MASK[idx];
      }
      return *this;
    }

    /**
     * Returns the value of the current element.
     *
     * @return Whether the current element is `true` or `false`.
     */
    operator bool() const {
      return (*element & MASK[idx]) != std::byte{0};
    }
  };

  /**
   * Subscript operator for array like access.
   */
  reference operator[](std::size_t idx) {
    return reference(sieve + (idx / PER_BYTE), idx % PER_BYTE);
  }
};

}
