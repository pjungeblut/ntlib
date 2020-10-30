#pragma once

/**
 * A sieve with a fixed capacity. Each entry is either `true` or `false` and
 * initially all values are `true`.
 */

#include <cstddef>
#include <cstring>
#include <memory>
#include <utility>

namespace ntlib {

template <class Allocator = std::allocator<bool>>
class sieve {
  /**
   * Allocator for the sieve.
   */
  Allocator sieve_allocator;

  /**
   * Capacity of the sieve.
   */
  std::size_t capacity;

  /**
   * Memory for the sieve.
   */
  bool *memory;

public:
  /**
   * Constructs an empty sieve.
   */
  sieve() : capacity(0), memory(nullptr) {}

  /**
   * Constructs the sieve with a given capacity.
   * Initially all elements are set to `true`.
   *
   * @param capacity The capacity of the sieve.
   */
  sieve(std::size_t capacity) : capacity(capacity) {
    if (capacity) memory = sieve_allocator.allocate(capacity);
    else memory = nullptr;
  }

  /**
   * Copy constructs a new sieve.
   *
   * @param other The other sieve to copy from.
   */
  sieve(const sieve &other) : capacity(other.capacity) {
    if (capacity) {
      memory = sieve_allocator.allocate(capacity);
      memcpy(memory, other.memory, capacity);
    } else {
      memory = nullptr;
    }
  }

  /**
   * Copy assigns from another sieve.
   *
   * @param other The other sieve to copy assign from.
   * @return Reference to this sieve.
   */
  sieve& operator=(const sieve &other) {
    if (this != &other) {
      if (capacity != other.capacity) {
        if (memory) sieve_allocator.deallocate(memory, capacity);
        capacity = other.capacity;
        if (capacity) memory = sieve_allocator.allocate(capacity);
        else memory = nullptr;
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
  sieve(sieve &&other) noexcept {
    capacity = std::exchange(other.capacity, 0);
    memory = std::exchange(other.memory, nullptr);
  }

  /**
   * Move assigns from another sieve.
   *
   * @param other The other sieve to move assign from.
   * @return Reference to this sieve.
   */
  sieve& operator=(sieve &&other) noexcept {
    if (memory) sieve_allocator.deallocate(memory, capacity);
    capacity = std::exchange(other.capacity, 0);
    memory = std::exchange(other.memory, nullptr);
    return *this;
  }

  /**
   * Destructs the sieve and frees the memory.
   */
  ~sieve() {
    if (memory) sieve_allocator.deallocate(memory, capacity);
  }

  /**
   * Subscript operator for constant array like access.
   *
   * @param idx The index of the element to return.
   * @return The value at the given index.
   */
  bool operator[](std::size_t idx) const {
    return memory[idx];
  }

  /**
   * Subscript operator for array like access.
   *
   * @param idx The index of the element to return.
   * @return A reference to the element at the given index.
   */
  bool& operator[](std::size_t idx) {
    return memory[idx];
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
   * Returns a constant pointer to the underlying data array.
   *
   * @return Constant pointer to the underlying data array.
   */
  const bool* data() const noexcept {
    return memory;
  }

  /**
   * Returns a pointer to the underlying data array.
   *
   * @return Pointer to the underlying data array.
   */
  bool *data() noexcept {
    return memory;
  }
};

}
