#pragma once

/**
 * A sieve with a fixed capacity. Each entry is either `true` or `false` and
 * initially all values are `true`.
 */

#include <cstddef>
#include <memory>
#include <vector>

namespace ntlib {

template <class Allocator = std::allocator<std::byte>>
class sieve {
  /**
   * Memory for the sieve.
   */
  std::vector<std::byte, Allocator> memory;

public:
  /**
   * Constructs an empty sieve.
   */
  sieve() = default;

  /**
   * Constructs the sieve with at least the given capacity.
   *
   * @param min_capacity The minimum capacity of the sieve.
   */
  sieve(std::size_t min_capacity) : memory((min_capacity + 29) / 30 * 30) {}

  /**
   * Initializes the sieve with `true` everywhere execept for multiples of 2, 3
   * and 5. This needs the invariant that `memory.size()` is always a multiple
   * of 30.
   */
  void init235() {
    for (std::size_t i = 0; i < memory.size(); i += 30) {
      memory[i + 0] = std::byte{false};
      memory[i + 1] = std::byte{true};
      memory[i + 2] = std::byte{false};
      memory[i + 3] = std::byte{false};
      memory[i + 4] = std::byte{false};
      memory[i + 5] = std::byte{false};
      memory[i + 6] = std::byte{false};
      memory[i + 7] = std::byte{true};
      memory[i + 8] = std::byte{false};
      memory[i + 9] = std::byte{false};
      memory[i + 10] = std::byte{false};
      memory[i + 11] = std::byte{true};
      memory[i + 12] = std::byte{false};
      memory[i + 13] = std::byte{true};
      memory[i + 14] = std::byte{false};
      memory[i + 15] = std::byte{false};
      memory[i + 16] = std::byte{false};
      memory[i + 17] = std::byte{true};
      memory[i + 18] = std::byte{false};
      memory[i + 19] = std::byte{true};
      memory[i + 20] = std::byte{false};
      memory[i + 21] = std::byte{false};
      memory[i + 22] = std::byte{false};
      memory[i + 23] = std::byte{true};
      memory[i + 24] = std::byte{false};
      memory[i + 25] = std::byte{false};
      memory[i + 26] = std::byte{false};
      memory[i + 27] = std::byte{false};
      memory[i + 28] = std::byte{false};
      memory[i + 29] = std::byte{true};
    }
  }

  /**
   * Subscript operator for constant array like access.
   *
   * @param idx The index of the element to return.
   * @return The value at the given index.
   */
  [[nodiscard]]
  bool operator[](std::size_t idx) const {
    return std::to_integer<bool>(memory[idx]);
  }

  /**
   * A proxy class to provide an lvalue that can be returned from operator[]
   * that can be assigned booleans.
   */
  class reference {
    friend class sieve;

    /**
     * Constant pointer to the element in the sieve to work on.
     */
    std::byte * const element;

    /**
     * Constructs the reference.
     *
     * @param element The element to work on.
     */
    reference(std::byte *element) : element(element) {};

  public:
    /**
     * Assigns a value to the referenced sieve entry.
     *
     * @param val The value to set.
     * @return Reference to this reference object.
     */
    reference& operator=(bool val) {
      *element = std::byte{val};
      return *this;
    }

    /**
     * Returns the value of the current element.
     *
     * @return Whether the current element is `true` or `false`.
     */
    [[nodiscard]]
    operator bool() const {
      return std::to_integer<bool>(*element);
    }
  };

  /**
   * Subscript operator for array like access.
   *
   * @param idx The index of the element to return.
   * @return A reference to the element at the given index.
   */
  [[nodiscard]]
  reference operator[](std::size_t idx) {
    return reference(&memory[idx]);
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
    return memory.size();
  }

  /**
   * Returns a constant pointer to the underlying data array.
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
