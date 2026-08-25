#include "min_heap.hpp"

#include <iostream>
#include <string>

int main() {
  MinHeap<int> heap;
  for (int v : {5, 3, 6, 1, 9, 2, 7}) {
    heap.push(v);
  }

  std::cout << "Min-heap (ints), popped in order: ";
  while (!heap.empty()) {
    std::cout << heap.top() << " ";
    heap.pop();
  }
  std::cout << "\n";

  MinHeap<std::string> words;
  words.push("c");
  words.push("rust");
  words.push("assembly");
  std::cout << "Min-heap (strings): ";
  while (!words.empty()) {
    std::cout << words.top() << " ";
    words.pop();
  }
  std::cout << "\n";

  return 0;
}
