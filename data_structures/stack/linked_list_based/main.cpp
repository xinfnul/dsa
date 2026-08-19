#include "stack.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

int main(void) {
  Stack<int> s;

  s.push(10);
  s.push(100);
  s.push(1000);
  std::cout << "size=" << s.size() << ", top=" << s.top() << "\n";

  s.pop();
  std::cout << "After pop -> size=" << s.size() << ", top=" << s.top() << "\n";

  Stack<int> copy = s;
  copy.push(10000);
  std::cout << "Original top=" << s.top() << " (size " << s.size() << ")\n";
  std::cout << "Copy top=" << copy.top() << " (size " << copy.size() << ")\n";

  while (!s.empty()) {
    std::cout << "popping " << s.top() << "\n";
    s.pop();
  }
  std::cout << "empty=" << (s.empty() ? "true" : "false") << "\n";

  try {
    s.top();
  } catch (const std::out_of_range &e) {
    std::cout << "Caught expected exception: " << e.what() << "\n";
  }

  std::cout << '\n';

  Stack<std::string> ss;

  ss.push("z");
  ss.push("zz");
  ss.push("zzz");

  std::cout << "size=" << ss.size() << ", top=" << ss.top() << "\n";

  ss.pop();
  std::cout << "After pop -> size=" << ss.size() << ", top=" << ss.top()
            << "\n";

  Stack<std::string> moved = std::move(ss);

  std::cout << "( Original ) After move -> size=" << ss.size() << "\n";
  std::cout << "( Moved ) After move -> size=" << moved.size()
            << ", top=" << moved.top() << "\n";

  return 0;
}
