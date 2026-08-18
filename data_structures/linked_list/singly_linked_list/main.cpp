#include "linked_list.hpp"

#include <iostream>
#include <string>

int main() {
  LinkedList<int> list;

  list.push_back(10);
  list.push_back(100);
  list.push_back(1000);
  list.push_front(0);

  std::cout << "After pushes: ";
  list.print();
  std::cout << " ( size=" << list.size() << " )\n";

  list.insert_at(4, 10000);

  std::cout << "After insert_at(4,10000): ";
  list.print();
  std::cout << '\n';

  list.remove_value(10000);

  std::cout << "After remove_value(10000): ";
  list.print();
  std::cout << '\n';

  list.reverse();

  std::cout << "After reverse: ";
  list.print();
  std::cout << '\n';

  std::cout << "find(19): " << (list.find(19) ? "true" : "false") << '\n';

  std::cout << "at(1): " << list.at(1) << '\n';

  list.pop_front();
  list.pop_back();

  std::cout << "After pop_front + pop_back: ";
  list.print();
  std::cout << '\n';

  LinkedList<int> copy = list;
  copy.push_back(101);

  std::cout << "Original: ";
  list.print();

  std::cout << "\nCopy: ";
  copy.print();

  std::cout << "\n\n";

  LinkedList<std::string> names;

  names.push_back("z");
  names.push_back("zz");
  names.push_front("zzz");

  std::cout << "String list: ";
  names.print();
  std::cout << '\n';

  std::cout << "find(z): " << (names.find("z") ? "true" : "false") << '\n';

  return 0;
}
