#include "linked_list.hpp"

#include <iostream>
#include <string>

int main() {
  LinkedList<std::string> list;

  std::cout << "=== push_back ===\n";

  list.push_back("https://duckduckgo.com");
  list.push_back("https://docs.rust-lang.org");
  list.push_back("https://github.com");

  list.print();
  std::cout << '\n';

  std::cout << "\n=== push_front ===\n";

  list.push_front("https://www.google.com");

  list.print();
  std::cout << '\n';

  std::cout << "\n=== rprint ===\n";

  list.rprint();
  std::cout << '\n';

  std::cout << "\n=== size / empty ===\n";

  std::cout << "Size: " << list.size() << '\n';
  std::cout << "Empty: " << std::boolalpha << list.empty() << '\n';

  std::cout << "\n=== at ===\n";

  std::cout << "Index 0: " << list.at(0) << '\n';
  std::cout << "Index 2: " << list.at(2) << '\n';

  std::cout << "\n=== find ===\n";

  std::cout << "Find github: "
            << list.find("https://github.com") << '\n';

  std::cout << "Find example: "
            << list.find("https://example.com") << '\n';

  std::cout << "\n=== insert_at ===\n";

  list.insert_at(2, "https://www.mozilla.org");

  list.print();
  std::cout << '\n';

  std::cout << "\n=== remove_at ===\n";

  list.remove_at(2);

  list.print();
  std::cout << '\n';

  std::cout << "\n=== remove_value ===\n";

  list.remove_value("https://github.com");

  list.print();
  std::cout << '\n';

  std::cout << "\n=== pop_front ===\n";

  list.pop_front();

  list.print();
  std::cout << '\n';

  std::cout << "\n=== pop_back ===\n";

  list.pop_back();

  list.print();
  std::cout << '\n';

  std::cout << "\n=== reverse ===\n";

  list.push_back("https://www.rust-lang.org");
  list.push_back("https://cplusplus.com");

  std::cout << "Before reverse: ";
  list.print();
  std::cout << '\n';

  list.reverse();

  std::cout << "After reverse:  ";
  list.print();
  std::cout << '\n';

  std::cout << "\n=== reverse print ===\n";

  list.rprint();
  std::cout << '\n';

  std::cout << "\n=== copy constructor ===\n";

  LinkedList<std::string> copied_list(list);

  std::cout << "Original: ";
  list.print();
  std::cout << '\n';

  std::cout << "Copied:   ";
  copied_list.print();
  std::cout << '\n';

  std::cout << "\n=== copy assignment ===\n";

  LinkedList<std::string> assigned_list;
  assigned_list = list;

  std::cout << "Assigned: ";
  assigned_list.print();
  std::cout << '\n';

  std::cout << "\n=== move constructor ===\n";

  LinkedList<std::string> moved_list(std::move(list));

  std::cout << "Moved list: ";
  moved_list.print();
  std::cout << '\n';

  std::cout << "Original after move: ";
  list.print();
  std::cout << '\n';

  std::cout << "\n=== move assignment ===\n";

  LinkedList<std::string> move_assigned_list;
  move_assigned_list = std::move(moved_list);

  std::cout << "Move-assigned: ";
  move_assigned_list.print();
  std::cout << '\n';

  std::cout << "Source after move: ";
  moved_list.print();
  std::cout << '\n';

  std::cout << "\n=== clear ===\n";

  move_assigned_list.clear();

  std::cout << "After clear: ";
  move_assigned_list.print();
  std::cout << '\n';

  std::cout << "Size: " << move_assigned_list.size() << '\n';
  std::cout << "Empty: " << move_assigned_list.empty() << '\n';

  return 0;
}