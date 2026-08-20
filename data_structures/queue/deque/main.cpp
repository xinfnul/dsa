#include "deque.hpp"

#include <iostream>
#include <string>

int main() {
  Deque<int> dq(5);

  std::cout << "insertRear(1000), insertRear(10000), insertFront(100), "
               "insertFront(10)\n";
  dq.insert_rear(1000);
  dq.insert_rear(10000);
  dq.insert_front(100);
  dq.insert_front(10);

  std::cout << "Front: " << dq.front() << "\n";
  std::cout << "Rear: " << dq.rear() << "\n";
  std::cout << "Size: " << dq.size() << " / Capacity: " << dq.capacity()
            << "\n";

  std::cout << "insertRear(31) -> now full\n";
  dq.insert_rear(31);
  std::cout << "Size: " << dq.size() << " / Capacity: " << dq.capacity()
            << "\n";

  try {
    dq.insert_front(98);
  } catch (const std::overflow_error &e) {
    std::cout << "Caught expected error: " << e.what() << "\n";
  }

  std::cout << "deleteFront(): " << dq.delete_front() << "\n";
  std::cout << "deleteRear(): " << dq.delete_rear() << "\n";

  std::cout << "\ninsertRear(39), insertFront(0) (wraps around the array)\n";
  dq.insert_rear(39);
  dq.insert_front(0);

  std::cout << "Size: " << dq.size() << " / Capacity: " << dq.capacity()
            << "\n";
  std::cout << "Draining front to rear: ";
  while (!dq.is_empty()) {
    std::cout << dq.delete_front() << " ";
  }
  std::cout << "\n";

  try {
    dq.delete_rear();
  } catch (const std::underflow_error &e) {
    std::cout << "Caught expected error: " << e.what() << "\n";
  }

  std::cout << '\n';

  Deque<std::string> names(4);
  names.insert_rear("zz");
  names.insert_rear("zzz");
  names.insert_front("z");
  names.insert_rear("zzzz");

  std::cout << "Front to rear: ";
  while (!names.is_empty()) {
    std::cout << names.delete_front() << " ";
  }
  std::cout << "\n";

  return 0;
}
