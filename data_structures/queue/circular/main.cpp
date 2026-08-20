#include "queue.hpp"

#include <iostream>
#include <string>

int main() {
  Queue<int> cq(5);

  std::cout << "Enqueuing 10, 100, 1000, 10000, 100000\n";
  cq.enqueue(10);
  cq.enqueue(100);
  cq.enqueue(1000);
  cq.enqueue(10000);
  cq.enqueue(100000);

  std::cout << "Front: " << cq.front() << "\n";
  std::cout << "Rear: " << cq.rear() << "\n";
  std::cout << "Size: " << cq.size() << " / Capacity: " << cq.capacity()
            << "\n";

  try {
    cq.enqueue(60);
  } catch (const std::overflow_error &e) {
    std::cout << "Caught expected error: " << e.what() << "\n";
  }

  std::cout << "\nDequeuing two elements: " << cq.dequeue() << " "
            << cq.dequeue() << "\n";

  // This is the key difference from a linear queue: after dequeuing,
  // there's room at the (wrapped-around) front, so we can enqueue
  // again even though rearIndex previously reached the last slot.
  std::cout << "Enqueuing 60, 70 (wraps around to the front of the array)\n";
  cq.enqueue(60);
  cq.enqueue(70);

  std::cout << "Size: " << cq.size() << " / Capacity: " << cq.capacity()
            << "\n";
  std::cout << "Contents front-to-rear: ";
  while (!cq.is_empty()) {
    std::cout << cq.dequeue() << " ";
  }
  std::cout << "\n";

  try {
    cq.dequeue();
  } catch (const std::underflow_error &e) {
    std::cout << "Caught expected error: " << e.what() << "\n";
  }

  std::cout << "\n";
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
