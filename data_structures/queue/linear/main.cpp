#include "queue.hpp"

#include <iostream>
#include <string>

int main() {
  Queue<int> int_queue(5);

  std::cout << "Enqueuing 10, 100, 1000, 10000, 100000\n";
  int_queue.enqueue(10);
  int_queue.enqueue(100);
  int_queue.enqueue(1000);
  int_queue.enqueue(10000);
  int_queue.enqueue(100000);

  std::cout << "Front: " << int_queue.front() << "\n";
  std::cout << "Rear: " << int_queue.rear() << "\n";
  std::cout << "Size: " << int_queue.size()
            << " / Capacity: " << int_queue.capacity() << "\n";

  try {
    int_queue.enqueue(60);
  } catch (const std::overflow_error &e) {
    std::cout << "Caught expected error: " << e.what() << "\n";
  }

  std::cout << "\nDequeuing all elements:\n";
  while (!int_queue.is_empty()) {
    std::cout << int_queue.dequeue() << " ";
  }
  std::cout << "\n";

  try {
    int_queue.dequeue();
  } catch (const std::underflow_error &e) {
    std::cout << "Caught expected error: " << e.what() << "\n";
  }

  std::cout << '\n';
  
  Queue<std::string> name_queue(3);
  name_queue.enqueue("z");
  name_queue.enqueue("zz");
  name_queue.enqueue("zzz");

  std::cout << "Names in queue order: ";
  while (!name_queue.is_empty()) {
    std::cout << name_queue.dequeue() << " ";
  }
  std::cout << "\n";

  return 0;
}
