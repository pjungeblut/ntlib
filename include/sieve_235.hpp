#pragma once

/**
 * A sieve similar to a std::bitset<N> with the additional property that all
 * multiples of 2, 3 and 5 are always zero.
 *
 * This allows to optimize for space as from every 30 consecutive numbers only
 * the eight ones with residues 1, 7, 11, 13, 17, 19, 23, 29 modulo 30 need to
 * be stored explicitly. Those can be stored in a single byte resulting in a
 * memory improvement by a factor of 30 compared to a std::vector<std::byte>
 * or by a factor of 15/4 compared to a std::vector<bool>.
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
  std::byte *memory;

public:
  /**
   * Constructs an empty sieve.
   */
  sieve_235() : capacity(0), memory(nullptr) {}

  /**
   * Constructs the sieve with a capacity at least as big as `min_capacity`.
   *
   * @param min_capacity The minimum required capacity.
   */
  sieve_235(std::size_t min_capacity) :
      capacity((min_capacity + PER_BYTE - 1) / PER_BYTE) {
    static_assert(CHAR_BIT == 8,
        "235 sieve optimization only works for 8 bits per byte.");
    if (capacity) memory = sieve_allocator.allocate(capacity);
  }

  /**
   * Copy constructs a new sieve.
   *
   * @param other The other sieve to copy from.
   */
  sieve_235(const sieve_235 &other) : capacity(other.capacity) {
    if (capacity) {
      memory = sieve_allocator.allocate(capacity);
      memcpy(memory, other.memory, capacity);
    }
  }

  /**
   * Copy assigns form another sieve.
   *
   * @param other The other sieve to copy assign from.
   * @return Reference to this sieve.
   */
  sieve_235& operator=(const sieve_235 &other) {
    if (this != &other) {
      if (capacity != other.capacity) {
        if (memory) sieve_allocator.deallocate(memory, capacity);
        capacity = other.capacity;
        if (capacity) sieve_allocator.allocate(memory, capacity);
      }
      if (capacity) memcpy(memory, other.memory, capacity);
    }
    return *this;
  }

  /**
   * Move constructs a new sieve.
   *
   * @param other The other sieve to move from.
   */
  sieve_235(sieve_235 &&other) noexcept {
    capacity = std::exchange(other.capacity, 0);
    memory = std::exchange(other.memory, nullptr);
  }

  /**
   * Move assigns from another sieve.
   *
   * @param other The other sieve to move assign from.
   * @return Reference to this sieve.
   */
  sieve_235& operator=(sieve_235 &&other) noexcept {
    if (memory) sieve_allocator.deallocate(memory, capacity);
    capacity = std::exchange(other.capacity, 0);
    memory = std::exchange(other.memory, nullptr);
    return *this;
  }

  /**
   * Destructs the sieve and frees the memory.
   */
  ~sieve_235() {
    if (memory) sieve_allocator.deallocate(memory, capacity);
  }

  /**
   * A proxy class to provide an lvalue that can be returned from operator[].
   */
  class reference {
    friend class sieve_235;

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
   * Subscript operator for constant array like access.
   *
   * @param idx The index of the element to return.
   * @return The value at the given index.
   */
  bool operator[](std::size_t idx) const {
    return (memory[idx / PER_BYTE] & MASK[idx / PER_BYTE]) != std::byte{0};
  }

  /**
   * Subscript operator for array like access.
   *
   * @param idx The index to return.
   * @return A proxy to the element at the given index.
   */
  reference operator[](std::size_t idx) {
    return reference(memory + (idx / PER_BYTE), idx % PER_BYTE);
  }

  /**
   * Tests whehter the sieve is empty.
   *
   * @return Whether the sieve is empty.
   */
  bool empty() const noexcept {
    return capacity == 0;
  }

  /**
   * Returns the current size of the sieve.
   *
   * @return The size of the sieve.
   */
  std::size_t size() const noexcept {
    return capacity;
  }

  /**
   * Returns a pointer to the underlying data array.
   *
   * @return Pointer to the underlying data array.
   */
  std::byte* data() const {
    return memory;
  }
};

}
