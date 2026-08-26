#ifndef MAX_HEAP_HPP
#define MAX_HEAP_HPP

#include <stdexcept>
#include <vector>

template <typename T> class MaxHeap {
private:
  std::vector<T> data_;

  void sift_up(std::size_t index);
  void sift_down(std::size_t index);

  static std::size_t parent(std::size_t i) { return (i - 1) / 2; }
  static std::size_t left(std::size_t i) { return 2 * i + 1; }
  static std::size_t right(std::size_t i) { return 2 * i + 2; }

public:
  MaxHeap() = default;

  ~MaxHeap() = default;

  void push(const T &value);
  void push(T &&value);

  void pop();

  const T &top() const;

  bool empty() const noexcept;
  std::size_t size() const noexcept;
};

template <typename T> void MaxHeap<T>::push(const T &value) {
  data_.push_back(value);
  sift_up(data_.size() - 1);
}

template <typename T> void MaxHeap<T>::push(T &&value) {
  data_.push_back(std::move(value));
  sift_up(data_.size() - 1);
}

template <typename T> void MaxHeap<T>::pop() {
  if (empty()) {
    throw std::out_of_range("MaxHeap::pop(): heap is empty");
  }

  std::swap(data_.front(), data_.back());
  data_.pop_back();

  if (!data_.empty()) {
    sift_down(0);
  }
}

template <typename T> const T &MaxHeap<T>::top() const {
  if (empty()) {
    throw std::out_of_range("MaxHeap::top(): heap is empty");
  }

  return data_.front();
}

template <typename T> bool MaxHeap<T>::empty() const noexcept {
  return data_.empty();
}

template <typename T> std::size_t MaxHeap<T>::size() const noexcept {
  return data_.size();
}

template <typename T> void MaxHeap<T>::sift_up(std::size_t index) {
  while (index > 0) {
    std::size_t p = parent(index);

    if (data_[index] > data_[p]) {
      std::swap(data_[index], data_[p]);
      index = p;
    } else {
      break;
    }
  }
}

template <typename T> void MaxHeap<T>::sift_down(std::size_t index) {
  const std::size_t n = data_.size();

  while (true) {
    std::size_t l = left(index);
    std::size_t r = right(index);
    std::size_t largest = index;

    if (l < n && data_[l] > data_[largest]) {
      largest = l;
    }

    if (r < n && data_[r] > data_[largest]) {
      largest = r;
    }

    if (largest == index) {
      break;
    }

    std::swap(data_[index], data_[largest]);
    index = largest;
  }
}

#endif
