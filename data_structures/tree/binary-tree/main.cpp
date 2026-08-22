#include "tree.hpp"

#include <iostream>

template <typename T>
void printVec(const std::string &label, const std::vector<T> &v) {
  std::cout << label << ": ";
  for (const auto &x : v)
    std::cout << x << " ";
  std::cout << "\n";
}

int main() {
  BinaryTree<int> tree;
  for (int v : {50, 30, 70, 20, 40, 60, 80}) {
    tree.insert(v);
  }

  std::cout << "Size: " << tree.size() << "\n";
  std::cout << "Height: " << tree.height() << "\n";
  std::cout << "Contains 40? " << (tree.contains(40) ? "yes" : "no") << "\n";
  std::cout << "Contains 98? " << (tree.contains(98) ? "yes" : "no") << "\n";

  printVec("Inorder", tree.inorder());
  printVec("Preorder", tree.preorder());
  printVec("Postorder", tree.postorder());
  printVec("Levelorder", tree.levelorder());

  BinaryTree<std::string> strTree;
  strTree.insert("c");
  strTree.insert("rust");
  strTree.insert("assembly");
  printVec("String inorder", strTree.inorder());

  return 0;
}
