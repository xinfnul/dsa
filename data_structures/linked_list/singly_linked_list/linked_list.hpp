#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <cstddef>
#include <iostream>
#include <ostream>

template <typename T> 
class LinkedList {
private:
  struct Node {
    T data;
    Node *next;

    explicit Node(const T value, Node *next_node = nullptr)
        : data(value), next(next_node) {}
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

  void push_front(const T &value);
  void push_back(const T &value);

  bool pop_front();
  bool pop_back();

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

template <typename T> 
LinkedList<T>::~LinkedList() { clear(); }

// ------------------------------------------------------------------------

template <typename T> 
void LinkedList<T>::push_front(const T &value) {
  Node *node = new Node(value, head);

  head = node;

  if (tail == nullptr) {
    tail = node;
  }

  ++count;
}

template <typename T> 
void LinkedList<T>::push_back(const T &value) {
  Node *node = new Node(value);

  if (tail == nullptr) {
    head = tail = node;
  } else {
    tail->next = node;
    tail = node;
  }

  ++count;
}

template <typename T> 
bool LinkedList<T>::pop_front() {
  if (head == nullptr) {
    return false;
  }

  Node *old = head;
  head = head->next;

  if (head == nullptr) {
    tail = nullptr;
  }

  delete old;
  --count;

  return true;
}

template <typename T> 
bool LinkedList<T>::pop_back() {
  if (head == nullptr) {
    return false;
  }

  if (head == tail) {
    delete head;
    head = tail = nullptr;
    --count;
    return true;
  }

  Node *cur = head;

  while (cur->next != tail) {
    cur = cur->next;
  }

  delete tail;

  tail = cur;
  tail->next = nullptr;

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

  Node *prev = head;
  
  for (std::size_t i = 0; i < index - 1; i++) {
    prev = prev->next;
  }
  
  prev->next = new Node(value, prev->next);
  ++count;
  
  return true;
}

template<typename T>
bool LinkedList<T>::remove_at(std::size_t index) {
  if (index >= count) {
    return false;
  }
  
  if (index == 0) {
    return pop_front();
  }
  
  if (index == count - 1) {
    return pop_back();
  }

  Node *prev = head;
  
  for (std::size_t i = 0; i < index - 1; i++) {
    prev = prev->next;
  }

  Node *target = prev->next;
  prev->next = target->next;
  
  delete target;
  --count;
  
  return true;
}

template<typename T>
bool LinkedList<T>::remove_value(const T&value) {
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
      prev->next = cur->next;
      
      if (cur == tail) {
        tail = prev;
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

template<typename T>
void LinkedList<T>::clear() {
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

template<typename T>
void LinkedList<T>::reverse() {
  Node *prev = nullptr;
  Node *cur = head;
  
  tail = head;
  
  while (cur != nullptr) {
    Node *next = cur->next;
    
    cur->next = prev;
    prev = cur;
    cur = next;
  }
  
  head = prev;
}

template<typename T>
bool LinkedList<T>::find(const T&value) const {
  for (Node *cur = head; cur != nullptr; cur = cur->next) {
    if (cur->data == value)
      return true;
  }
  
  return false;
}

template<typename T>
const T &LinkedList<T>::at(std::size_t index) const {
  if (index >= count) {
    throw std::out_of_range("LinkedList::at - index out of range");
  }
  Node *cur = head;
  for (std::size_t i = 0; i < index; ++i) {
    cur = cur->next;
  }
  return cur->data;
}

template<typename T>
bool LinkedList<T>::empty() const { return count == 0; }

template<typename T>
std::size_t LinkedList<T>::size() const { return count; }

template<typename T>
void LinkedList<T>::print(std::ostream &os) const {
  os << "[ ";
  
  for (Node *cur = head; cur != nullptr; cur = cur->next) {
    os << cur->data << " ";
  }
  
  os << "]";
}


#endif
