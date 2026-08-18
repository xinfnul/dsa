#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <cstddef>
#include <iostream>
#include <ostream>

class LinkedList {
private:
  struct Node {
    int data;
    Node *next;
    explicit Node(int value, Node *next_node = nullptr)
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

  void push_front(int value);
  void push_back(int value);
  bool pop_front();
  bool pop_back();
  bool insert_at(std::size_t index, int value);
  bool remove_at(std::size_t index);
  bool remove_value(int value);
  void clear();
  void reverse();

  bool find(int value) const;
  int at(std::size_t index) const;
  bool empty() const;
  std::size_t size() const;

  void print(std::ostream &os = std::cout) const;
};

#endif
