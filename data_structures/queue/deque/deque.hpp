#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <cstddef>
#include <stdexcept>

template <typename T> class Deque {
private:
  T *data;
  std::size_t cap;
  int front_index;
  int rear_index;
  std::size_t count;

public:
  explicit Deque(std::size_t capacity = 10);
  ~Deque();

  Deque(const Deque &) = delete;
  Deque &operator=(const Deque &) = delete;

  void insert_front(const T &value);
  void insert_rear(const T &value);

  T delete_front();
  T delete_rear();

  T front() const;
  T rear() const;

  bool is_empty() const;
  bool is_full() const;
  std::size_t size() const;
  std::size_t capacity() const;

  void clear();
};

template <typename T>
Deque<T>::Deque(std::size_t capacity)
    : data(new T[capacity]), cap(capacity), front_index(0), rear_index(-1),
      count(0) {}

template <typename T> Deque<T>::~Deque() { delete[] data; }

// -----------------------------------------------------------------

template <typename T> void Deque<T>::insert_front(const T &value) {
  if (is_full()) {
    throw std::overflow_error("Deque is full: cannot insert at front");
  }

  front_index =
      (front_index - 1 + static_cast<int>(cap)) % static_cast<int>(cap);
  data[front_index] = value;
  count++;
}

template <typename T> void Deque<T>::insert_rear(const T &value) {
  if (is_full()) {
    throw std::overflow_error("Deque is full: cannot insert at rear");
  }

  rear_index = (rear_index + 1) % static_cast<int>(cap);
  data[rear_index] = value;
  count++;
}

template <typename T> T Deque<T>::delete_front() {
  if (is_empty()) {
    throw std::underflow_error("Deque is empty: cannot delete from front");
  }

  T value = data[front_index];
  front_index = (front_index + 1) % static_cast<int>(cap);
  count--;

  return value;
}

template <typename T> T Deque<T>::delete_rear() {
  if (is_empty()) {
    throw std::underflow_error("Deque is empty: cannot delete from rear");
  }

  T value = data[rear_index];
  rear_index = (rear_index - 1 + static_cast<int>(cap)) % static_cast<int>(cap);
  count--;

  return value;
}

template <typename T> T Deque<T>::front() const {
  if (is_empty()) {
    throw std::underflow_error("Deque is empty: no front element");
  }
  return data[front_index];
}

template <typename T> T Deque<T>::rear() const {
  if (is_empty()) {
    throw std::underflow_error("Deque is empty: no rear element");
  }
  return data[rear_index];
}

template <typename T> bool Deque<T>::is_empty() const { return count == 0; }

template <typename T> bool Deque<T>::is_full() const { return count == cap; }

template <typename T> std::size_t Deque<T>::size() const { return count; }

template <typename T> std::size_t Deque<T>::capacity() const { return cap; }

template <typename T> void Deque<T>::clear() {
  front_index = 0;
  rear_index = -1;
  count = 0;
}

#endif
