#include "linked_list.hpp"

#include <stdexcept>

LinkedList::LinkedList() : head(nullptr), tail(nullptr), count(0) {}

LinkedList::LinkedList(const LinkedList &other)
    : head(nullptr), tail(nullptr), count(0) {
  for (Node *cur = other.head; cur != nullptr; cur = cur->next) {
    push_back(cur->data);
  }
}

LinkedList &LinkedList::operator=(const LinkedList &other) {
  if (this == &other) {
    return *this;
  }
  clear();

  for (Node *cur = other.head; cur != nullptr; cur = cur->next) {
    push_back(cur->data);
  }
  return *this;
}

LinkedList::LinkedList(LinkedList &&other) noexcept
    : head(other.head), tail(other.tail), count(other.count) {
  other.head = nullptr;
  other.tail = nullptr;
  other.count = 0;
}

LinkedList &LinkedList::operator=(LinkedList &&other) noexcept {
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

LinkedList::~LinkedList() { clear(); }

// ------------------------------------------------------------------------

void LinkedList::push_front(int value) {
  Node *node = new Node(value, head);
  head = node;
  if (tail == nullptr) {
    tail = node;
  }
  ++count;
}

void LinkedList::push_back(int value) {
  Node *node = new Node(value);
  if (tail == nullptr) {
    head = tail = node;
  } else {
    tail->next = node;
    tail = node;
  }
  ++count;
}

bool LinkedList::pop_front() {
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

bool LinkedList::pop_back() {
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

bool LinkedList::insert_at(std::size_t index, int value) {
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

bool LinkedList::remove_at(std::size_t index) {
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

bool LinkedList::remove_value(int value) {
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

void LinkedList::clear() {
  Node *cur = head;
  while (cur != nullptr) {
    Node *next = cur->next;
    delete cur;
    cur = next;
  }
  head = tail = nullptr;
  count = 0;
}

void LinkedList::reverse() {
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

bool LinkedList::find(int value) const {
  for (Node *cur = head; cur != nullptr; cur = cur->next) {
    if (cur->data == value)
      return true;
  }
  return false;
}

int LinkedList::at(std::size_t index) const {
  if (index >= count) {
    throw std::out_of_range("LinkedList::at - index out of range");
  }
  Node *cur = head;
  for (std::size_t i = 0; i < index; ++i) {
    cur = cur->next;
  }
  return cur->data;
}

bool LinkedList::empty() const { return count == 0; }

std::size_t LinkedList::size() const { return count; }

void LinkedList::print(std::ostream &os) const {
  os << "[ ";
  for (Node *cur = head; cur != nullptr; cur = cur->next) {
    os << cur->data << " ";
  }
  os << "]";
}
