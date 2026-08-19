#ifndef STACK_HPP
#define STACK_HPP

#include <algorithm>
#include <cstddef>
#include <stdexcept>

template <typename T> class Stack {
private:
  T *data;
  std::size_t capacity;
  std::size_t count;

  void resize(std::size_t new_capacity);

public:
  explicit Stack(std::size_t initial_capacity = 4);
  Stack(const Stack &other);
  Stack &operator=(const Stack &other);

  Stack(Stack &&other) noexcept;
  Stack &operator=(Stack &&other) noexcept;

  ~Stack();

  void push(const T &value);
  bool pop();

  const T &top() const;

  bool empty() const;
  std::size_t size() const;
};

template <typename T>
Stack<T>::Stack(std::size_t initial_capacity)
    : data(new T[initial_capacity == 0 ? 1 : initial_capacity]),
      capacity(initial_capacity == 0 ? 1 : initial_capacity), count(0) {}

template <typename T>
Stack<T>::Stack(const Stack &other)
    : data(new T[other.capacity]), capacity(other.capacity),
      count(other.count) {
  std::copy(other.data, other.data + other.count, data);
}

template <typename T> Stack<T> &Stack<T>::operator=(const Stack &other) {
  if (this == &other) {
    return *this;
  }

  T *new_data = new T[other.capacity];
  std::copy(other.data, other.data + other.count, new_data);

  delete[] data;

  data = new_data;
  capacity = other.capacity;
  count = other.count;

  return *this;
}

template <typename T>
Stack<T>::Stack(Stack &&other) noexcept
    : data(other.data), capacity(other.capacity), count(other.count) {
  other.data = nullptr;
  other.capacity = 0;
  other.count = 0;
}

template <typename T> Stack<T> &Stack<T>::operator=(Stack &&other) noexcept {
  if (this == &other) {
    return *this;
  }

  delete[] data;

  data = other.data;
  capacity = other.capacity;
  count = other.count;

  other.data = nullptr;
  other.capacity = 0;
  other.count = 0;

  return *this;
}

template <typename T> Stack<T>::~Stack() { delete[] data; }

// -----------------------------------------------------------------------

template <typename T> void Stack<T>::resize(std::size_t new_capacity) {
  T *new_data = new T[new_capacity == 0 ? 1 : new_capacity];

  std::copy(data, data + count, new_data);

  delete[] data;

  data = new_data;
  capacity = new_capacity;
}

// -----------------------------------------------------------------------

template <typename T> void Stack<T>::push(const T &value) {
  if (count == capacity) {
    resize(capacity * 2);
  }

  data[count++] = value;
}

template <typename T> bool Stack<T>::pop() {
  if (count == 0) {
    return false;
  }

  --count;

  // Shrink when usage drops to a quarter, MIN_CAPACITY = 4.
  if (count > 0 && count <= capacity / 4 && capacity > 4) {
    resize(capacity / 2);
  }

  return true;
}

template <typename T> const T &Stack<T>::top() const {
  if (count == 0) {
    throw std::out_of_range("Stack::top - stack is empty");
  }

  return data[count - 1];
}

template <typename T> bool Stack<T>::empty() const { return count == 0; }

template <typename T> std::size_t Stack<T>::size() const { return count; }

#endif
