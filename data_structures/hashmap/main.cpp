#include <iostream>
#include <string>

#include "hashmap.hpp"

int main() {
  HashMap<std::string, int> map;

  std::cout << "Empty: " << std::boolalpha << map.empty() << '\n';
  std::cout << "Size: " << map.size() << "\n\n";

  map.set("z", 10);
  map.set("zz", 100);
  map.set("zzz", 1000);
  map.set("zzzz", 10000);

  std::cout << "After inserting 4 elements:\n";
  std::cout << "Size: " << map.size() << "\n\n";

  const int *z = map.get("z");
  const int *zz = map.get("zz");
  const int *x = map.get("x");

  if (z != nullptr) {
    std::cout << "z = " << *z << '\n';
  }

  if (zz != nullptr) {
    std::cout << "zz = " << *zz << '\n';
  }

  if (x == nullptr) {
    std::cout << "z was not found\n";
  }

  std::cout << '\n';

  std::cout << "Contains z: " << map.contains("z") << '\n';

  std::cout << "Contains x: " << map.contains("x") << "\n\n";

  map.set("z", 100);

  std::cout << "After updating z:\n";
  std::cout << "z = " << *map.get("z") << "\n\n";

  std::cout << "Deleting zz: " << map.del("zz") << '\n';

  std::cout << "Contains zz: " << map.contains("zz") << '\n';

  std::cout << "Size: " << map.size() << "\n\n";

  std::cout << "Deleting x: " << map.del("x") << "\n\n";

  std::cout << "Adding more elements...\n";

  for (int i = 0; i < 20; ++i) {
    map.set("key" + std::to_string(i), i);
  }

  std::cout << "Size after adding 20 elements: " << map.size() << '\n';

  std::cout << "key0 = " << *map.get("key0") << '\n';
  std::cout << "key10 = " << *map.get("key10") << '\n';
  std::cout << "key19 = " << *map.get("key19") << "\n\n";

  map.clear();

  std::cout << "After clear():\n";
  std::cout << "Size: " << map.size() << '\n';
  std::cout << "Empty: " << map.empty() << '\n';

  return 0;
}
