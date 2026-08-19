#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <cstddef>
#include <iostream>
#include <ostream>
#include <stdexcept>

template <typename T> class LinkedList {
private:
  struct Node {
    T data;
    Node *next;
    Node *prev;

    explicit Node(T value, Node *next_node = nullptr, Node *prev_node = nullptr)
        : data(value), next(next_node), prev(prev_node) {}
  };

  Node *head;
  Node *tail;
  std::size_t count;

public:
  LinkedList();
  LinkedList(const LinkedList &other);
  LinkedList &operator=(const LinkedList &other);

  LinkedList(LinkedList &&other) noexcept;
  LinkedList &operator=(LinkedList &&other) noexcept;

  ~LinkedList();

  void push_back(const T &value);
  void push_front(const T &value);

  bool pop_back();
  bool pop_front();

  bool insert_at(std::size_t index, const T &value);
  bool remove_at(std::size_t index);
  bool remove_value(const T &value);

  void clear();
  void reverse();

  bool find(const T &value) const;
  const T &at(std::size_t index) const;

  bool empty() const;
  std::size_t size() const;

  void print(std::ostream &os = std::cout) const;
  void rprint(std::ostream &os = std::cout) const;
};

template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), count(0) {}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList &other)
    : head(nullptr), tail(nullptr), count(0) {
  for (Node *cur = other.head; cur != nullptr; cur = cur->next) {
    push_back(cur->data);
  }
}

template <typename T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList &other) {
  if (this == &other) {
    return *this;
  }

  clear();

  for (Node *cur = other.head; cur != nullptr; cur = cur->next) {
    push_back(cur->data);
  }

  return *this;
}

template <typename T>
LinkedList<T>::LinkedList(LinkedList &&other) noexcept
    : head(other.head), tail(other.tail), count(other.count) {
  other.head = nullptr;
  other.tail = nullptr;
  other.count = 0;
}

template <typename T>
LinkedList<T> &LinkedList<T>::operator=(LinkedList &&other) noexcept {
  if (this == &other) {
    return *this;
  }

  clear();

  head = other.head;
  tail = other.tail;
  count = other.count;

  other.head = nullptr;
  other.tail = nullptr;
  other.count = 0;

  return *this;
}

template <typename T> LinkedList<T>::~LinkedList() { clear(); }

// ----------------------------------------------------------------

template <typename T> void LinkedList<T>::push_back(const T &value) {
  Node *node = new Node(value, nullptr, tail);

  if (head == nullptr) {
    tail = head = node;
  } else {
    tail->next = node;
    tail = node;
  }

  ++count;
}

template <typename T> void LinkedList<T>::push_front(const T &value) {
  Node *node = new Node(value, head, nullptr);

  if (head != nullptr) {
    head->prev = node;
  } else {
    tail = node;
  }

  head = node;

  ++count;
}

template <typename T> bool LinkedList<T>::pop_back() {
  if (tail == nullptr) {
    return false;
  }

  if (head == tail) {
    delete tail;
    head = tail = nullptr;
  } else {
    Node *old_tail = tail;

    tail = tail->prev;
    tail->next = nullptr;

    delete old_tail;
  }

  --count;

  return true;
}

template <typename T> bool LinkedList<T>::pop_front() {
  if (head == nullptr) {
    return false;
  }

  if (head == tail) {
    delete head;
    head = tail = nullptr;
  } else {
    Node *old_head = head;

    head = head->next;
    head->prev = nullptr;

    delete old_head;
  }

  --count;

  return true;
}

template <typename T>
bool LinkedList<T>::insert_at(std::size_t index, const T &value) {
  if (index > count) {
    return false;
  }

  if (index == 0) {
    push_front(value);
    return true;
  }

  if (index == count) {
    push_back(value);
    return true;
  }

  Node *cur = head;

  for (std::size_t i = 0; i < index - 1; i++) {
    cur = cur->next;
  }

  Node *node = new Node(value, cur->next, cur);

  cur->next->prev = node;
  cur->next = node;

  ++count;

  return true;
}

template <typename T> bool LinkedList<T>::remove_at(std::size_t index) {
  if (index >= count) {
    return false;
  }

  if (index == 0) {
    return pop_front();
  }

  if (index == count - 1) {
    return pop_back();
  }

  Node *cur = head;

  for (std::size_t i = 0; i < index - 1; i++) {
    cur = cur->next;
  }

  Node *target = cur->next;

  cur->next = target->next;
  target->next->prev = cur;

  delete target;

  --count;

  return true;
}

template <typename T> bool LinkedList<T>::remove_value(const T &value) {
  if (head == nullptr) {
    return false;
  }

  if (head->data == value) {
    return pop_front();
  }

  Node *prev = head;
  Node *cur = head->next;

  while (cur != nullptr) {
    if (cur->data == value) {

      if (cur == tail) {
        tail = prev;
        prev->next = nullptr;
      } else {
        prev->next = cur->next;
        cur->next->prev = prev;
      }

      delete cur;

      --count;

      return true;
    }

    prev = cur;
    cur = cur->next;
  }

  return false;
}

template <typename T> void LinkedList<T>::clear() {
  Node *cur = head;

  while (cur != nullptr) {
    Node *next = cur->next;
    delete cur;
    cur = next;
  }

  head = nullptr;
  tail = nullptr;
  count = 0;
}

template <typename T> void LinkedList<T>::reverse() {
  Node *cur = head;

  while (cur != nullptr) {
    Node *next = cur->next;

    cur->next = cur->prev;
    cur->prev = next;

    cur = next;
  }

  Node *temp = head;
  head = tail;
  tail = temp;
}

template <typename T> bool LinkedList<T>::find(const T &value) const {
  for (Node *cur = head; cur != nullptr; cur = cur->next) {
    if (cur->data == value) {
      return true;
    }
  }

  return false;
}

template <typename T> const T &LinkedList<T>::at(std::size_t index) const {
  if (index >= count) {
    throw std::out_of_range("LinkedList::at - index out of range");
  }

  Node *cur = head;

  for (std::size_t i = 0; i < index; i++) {
    cur = cur->next;
  }

  return cur->data;
}

template <typename T> bool LinkedList<T>::empty() const { return count == 0; }

template <typename T> std::size_t LinkedList<T>::size() const { return count; }

template <typename T> void LinkedList<T>::print(std::ostream &os) const {
  os << "[ \033[1;34mNULL\033[0m \033[1;32m<->\033[0m ";

  for (Node *cur = head; cur != nullptr; cur = cur->next) {
    os << cur->data << "\033[1;32m <-> \033[0m";
  }

  os << "\033[1;34mNULL\033[0m ";
  os << "]";
}

template <typename T> void LinkedList<T>::rprint(std::ostream &os) const {
  os << "[ \033[1;34mNULL\033[0m \033[1;32m<->\033[0m ";

  for (Node *cur = tail; cur != nullptr; cur = cur->prev) {
    os << cur->data << "\033[1;32m <-> \033[0m";
  }

  os << "\033[1;34mNULL\033[0m ";
  os << "]";
}

#endif
