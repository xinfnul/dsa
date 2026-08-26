#include "max_heap.hpp"

#include <iostream>
#include <string>

int main() {
  MaxHeap<int> heap;
  for (int v : {5, 3, 6, 1, 9, 2, 7}) {
    heap.push(v);
  }

  std::cout << "Max-heap (ints), popped in order: ";
  while (!heap.empty()) {
    std::cout << heap.top() << " ";
    heap.pop();
  }
  std::cout << "\n";

  MaxHeap<std::string> words;
  words.push("c");
  words.push("rust");
  words.push("assembly");
  std::cout << "Max-heap (strings): ";
  while (!words.empty()) {
    std::cout << words.top() << " ";
    words.pop();
  }
  std::cout << "\n";

  return 0;
}
