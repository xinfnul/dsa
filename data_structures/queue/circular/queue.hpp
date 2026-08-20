#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>
#include <stdexcept>

template <typename T> class Queue {
private:
  T *data;
  std::size_t cap;
  int front_index;
  int rear_index;
  std::size_t count;

public:
  explicit Queue(std::size_t capacity);
  ~Queue();

  Queue(const Queue &) = delete;
  Queue &operator=(const Queue &) = delete;

  void enqueue(const T &value);
  T dequeue();

  T front() const;
  T rear() const;

  bool is_empty() const;
  bool is_full() const;
  std::size_t size() const;
  std::size_t capacity() const;

  void clear();
};

template <typename T>
Queue<T>::Queue(std::size_t capacity)
    : data(new T[capacity]), cap(capacity), front_index(0), rear_index(-1),
      count(0) {}

template <typename T> Queue<T>::~Queue() { delete[] data; }

// -------------------------------------------------------------------------------

template <typename T> void Queue<T>::enqueue(const T &value) {
  if (is_full()) {
    throw std::overflow_error("Queue is full: cannot enqueue");
  }

  // Wrap around to the beginning once we pass the last slot.
  rear_index = (rear_index + 1) % static_cast<int>(cap);
  data[rear_index] = value;
  count++;
}

template <typename T> T Queue<T>::dequeue() {
  if (is_empty()) {
    throw std::underflow_error("Queue is empty: cannot dequeue");
  }

  T value = data[front_index];
  front_index = (front_index + 1) % static_cast<int>(cap);
  count--;

  return value;
}

template <typename T> T Queue<T>::front() const {
  if (is_empty()) {
    throw std::underflow_error("Queue is empty: no front element");
  }

  return data[front_index];
}

template <typename T> T Queue<T>::rear() const {
  if (is_empty()) {
    throw std::underflow_error("Queue is empty: no rear element");
  }

  return data[rear_index];
}

template <typename T> bool Queue<T>::is_empty() const { return count == 0; }

template <typename T> bool Queue<T>::is_full() const { return count == cap; }

template <typename T> std::size_t Queue<T>::size() const { return count; }

template <typename T> std::size_t Queue<T>::capacity() const { return cap; }

template <typename T> void Queue<T>::clear() {
  front_index = 0;
  rear_index = -1;
  count = 0;
}

#endif
