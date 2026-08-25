#ifndef MIN_HEAP_HPP
#define MIN_HEAP_HPP

#include <stdexcept>
#include <vector>

template <typename T> class MinHeap {
private:
  std::vector<T> data_;

  void sift_up(std::size_t index);
  void sift_down(std::size_t index);

  static std::size_t parent(std::size_t i) { return (i - 1) / 2; }
  static std::size_t left(std::size_t i) { return 2 * i + 1; }
  static std::size_t right(std::size_t i) { return 2 * i + 2; }

public:
  MinHeap() = default;

  ~MinHeap() = default;

  void push(const T &value);
  void push(T &&value);

  void pop();

  const T &top() const;

  bool empty() const noexcept;
  std::size_t size() const noexcept;
};

template <typename T> void MinHeap<T>::push(const T &value) {
  data_.push_back(value);
  sift_up(data_.size() - 1);
}

template <typename T> void MinHeap<T>::push(T &&value) {
  data_.push_back(value);
  sift_up(data_.size() - 1);
}

template <typename T> void MinHeap<T>::pop() {
  if (empty()) {
    throw std::out_of_range("MinHeap::pop(): heap is empty");
  }

  std::swap(data_.front(), data_.back());

  data_.pop_back();

  if (!data_.empty()) {
    sift_down(0);
  }
}

template <typename T> const T &MinHeap<T>::top() const {
  if (empty()) {
    throw std::out_of_range("MinHeap::top(): heap is empty");
  }

  return data_.front();
}

template <typename T> bool MinHeap<T>::empty() const noexcept {
  return data_.empty();
}

template <typename T> std::size_t MinHeap<T>::size() const noexcept {
  return data_.size();
}

template <typename T> void MinHeap<T>::sift_up(std::size_t index) {
  while (index > 0) {
    std::size_t p = parent(index);

    if (data_[index] < data_[p]) {
      std::swap(data_[index], data_[p]);
      index = p;
    } else {
      break;
    }
  }
}

template <typename T> void MinHeap<T>::sift_down(std::size_t index) {
  const std::size_t n = data_.size();

  while (true) {
    std::size_t l = left(index);
    std::size_t r = right(index);
    std::size_t smallest = index;

    if (l < n && data_[l] < data_[smallest]) {
      smallest = l;
    }
    if (r < n && data_[r] < data_[smallest]) {
      smallest = r;
    }
    if (smallest == index) {
      break;
    }

    std::swap(data_[index], data_[smallest]);
    index = smallest;
  }
}

#endif
