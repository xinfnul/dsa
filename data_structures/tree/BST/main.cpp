#include "bst.hpp"

#include <iostream>

template <typename T>
void print_vec(const std::string &label, const std::vector<T> &v) {
  std::cout << label << ": ";
  for (const auto &x : v)
    std::cout << x << " ";
  std::cout << "\n";
}

int main() {
  BST<int> tree;
  for (int v : {50, 30, 70, 20, 40, 60, 80}) {
    tree.insert(v);
  }

  std::cout << "Size: " << tree.size() << "\n";
  std::cout << "Height: " << tree.height() << "\n";
  std::cout << "Contains 40? " << (tree.contains(40) ? "yes" : "no") << "\n";
  std::cout << "Contains 98? " << (tree.contains(98) ? "yes" : "no") << "\n";

  print_vec("Inorder", tree.inorder());
  print_vec("Preorder", tree.preorder());
  print_vec("Postorder", tree.postorder());
  print_vec("Levelorder", tree.levelorder());

  BST<std::string> strTree;
  strTree.insert("c");
  strTree.insert("rust");
  strTree.insert("assembly");
  print_vec("String inorder", strTree.inorder());

  return 0;
}
