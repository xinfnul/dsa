#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <stdexcept>

template <typename T> class Stack {
private:
  struct Node {
    T data;
    Node *next;

    explicit Node(T value, Node *next_node = nullptr)
        : data(value), next(next_node) {}
  };

  Node *head;
  std::size_t count;

public:
  Stack();
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

template <typename T> Stack<T>::Stack() : head(nullptr), count(0) {}

template <typename T>
Stack<T>::Stack(const Stack &other) : head(nullptr), count(0) {
  if (other.head == nullptr) {
    return;
  }

  // Copy preserving order
  Node *src_head = nullptr;
  Node *src_tail = nullptr;

  for (Node *cur = other.head; cur != nullptr; cur = cur->next) {
    Node *n = new Node(cur->data);
    if (src_head == nullptr) {
      src_head = src_tail = n;
    } else {
      src_tail->next = n;
      src_tail = n;
    }
  }

  head = src_head;
  count = other.count;
}

template <typename T> Stack<T> &Stack<T>::operator=(const Stack &other) {
  if (this == &other) {
    return *this;
  }

  Stack temp(other);

  Node *tmp_head = head;
  std::size_t tmp_count = count;

  head = temp.head;
  count = temp.count;

  temp.head = tmp_head;
  temp.count = tmp_count;

  return *this;
}

template <typename T>
Stack<T>::Stack(Stack &&other) noexcept : head(other.head), count(other.count) {
  other.head = nullptr;
  other.count = 0;
}

template <typename T> Stack<T> &Stack<T>::operator=(Stack &&other) noexcept {
  if (this == &other) {
    return *this;
  }

  while (pop()) {
  }

  head = other.head;
  count = other.count;

  other.head = nullptr;
  other.count = 0;

  return *this;
}

template <typename T> Stack<T>::~Stack() {
  while (pop()) {
  }
}

// -------------------------------------------------------------------------

template <typename T> void Stack<T>::push(const T &value) {
  head = new Node(value, head);
  ++count;
}

template <typename T> bool Stack<T>::pop() {
  if (head == nullptr) {
    return false;
  }

  Node *old = head;
  head = head->next;

  delete old;

  --count;

  return true;
}

template <typename T> const T &Stack<T>::top() const {
  if (head == nullptr) {
    throw std::out_of_range("Stack::top - stack is empty");
  }

  return head->data;
}

template <typename T> bool Stack<T>::empty() const { return head == nullptr; }

template <typename T> std::size_t Stack<T>::size() const { return count; }

#endif
