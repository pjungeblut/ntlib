#pragma once

/**
 * A sieve with a fixed capacity. Each entry is either `true` or `false` and
 * initially all values are `true`.
 */

#include <cstddef>
#include <cstring>
#include <memory>

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
  bool *data;

public:
  /**
   * Constructs an empty sieve.
   */
  sieve() : capacity(0) {}

  /**
   * Constructs the sieve with a given capacity.
   * Initially all elements are set to `true`.
   *
   * @param capacity The capacity of the sieve.
   */
  sieve(std::size_t capacity) : capacity(capacity) {
    data = sieve_allocator.allocate(capacity);
    memset(data, true, capacity);
  }

  /**
   * Copy constructs a new sieve.
   *
   * @param other The other sieve to copy from.
   */
  sieve(const sieve &other) : capacity(other.capacity) {
    data = sieve_allocator.allocate(capacity);
    memcpy(data, other.data, capacity);
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
        sieve_allocator.deallocate(data, capacity);
        capacity = other.capacity;
        sieve_allocator.allocate(data, capacity);
      }
      memcpy(data, other.data, capacity);
    }
    return *this;
  }

  /**
   * Move constructs a new sieve.
   *
   * @param other The other sieve to move from.
   */
  sieve(sieve &&other) noexcept : capacity(other.capacity) {
    data = other.data;
    other.capacity = 0;
  }

  /**
   * Move assigns from another sieve.
   *
   * @param other The other sieve to move assign from.
   * @return Reference to this sieve.
   */
  sieve& operator=(sieve &&other) noexcept {
    capacity = other.capacity;
    data = other.data;
    other.capacity = 0;
    return *this;
  }

  /**
   * Destructs the sieve and frees the memory.
   */
  ~sieve() {
    if (capacity > 0) {
      sieve_allocator.deallocate(data, capacity);
    }
  }

  /**
   * Support constant array like access.
   *
   * @param idx The index to return.
   * @return The value at the given index.
   */
  bool operator[](std::size_t idx) const {
    return data[idx];
  }

  /**
   * Support array like access.
   *
   * @param idx The index to return the element at.
   * @return A reference to the element at the given index.
   */
  bool& operator[](std::size_t idx) {
    return data[idx];
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
};

}
