#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include <algorithm>
#include <cstddef>
#include <memory>
#include <queue>
#include <vector>

template <typename T> class BinaryTree {
private:
  struct Node {
    T data;

    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    explicit Node(const T &value)
        : data(value), left(nullptr), right(nullptr) {}
  };

  std::unique_ptr<Node> root;
  size_t node_count;

  // Helper methods
  bool contains_helper(const Node *node, const T &value) const;
  int height_helper(const Node *node) const;

  void inorder_helper(const Node *node, std::vector<T> &result) const;
  void preorder_helper(const Node *node, std::vector<T> &result) const;
  void postorder_helper(const Node *node, std::vector<T> &result) const;

public:
  BinaryTree();
  BinaryTree(const BinaryTree &other) = delete;
  BinaryTree &operator=(const BinaryTree &other) = delete;

  ~BinaryTree() = default;

  void insert(const T &value);
  bool contains(const T &value) const;
  int height() const;
  size_t size() const;
  bool empty() const;
  void clear();

  std::vector<T> inorder() const;
  std::vector<T> preorder() const;
  std::vector<T> postorder() const;
  std::vector<T> levelorder() const;
};

template <typename T>
BinaryTree<T>::BinaryTree() : root(nullptr), node_count(0) {}

// -------------------------------------------------------------------------------

template <typename T> void BinaryTree<T>::insert(const T &value) {
  if (!root) {
    root = std::make_unique<Node>(value);
    node_count++;
    return;
  }

  std::queue<Node *> q;
  q.push(root.get());

  while (!q.empty()) {
    Node *current = q.front();

    q.pop();

    if (!current->left) {
      current->left = std::make_unique<Node>(value);
      node_count++;
      return;
    }

    if (!current->right) {
      current->right = std::make_unique<Node>(value);
      node_count++;
      return;
    }

    q.push(current->left.get());
    q.push(current->right.get());
  }
}

// -------------------------------------------------------------------------------

template <typename T>
bool BinaryTree<T>::contains_helper(const Node *node, const T &value) const {
  if (!node) {
    return false;
  }

  if (node->data == value) {
    return true;
  }

  return contains_helper(node->left.get(), value) ||
         contains_helper(node->right.get(), value);
}

template <typename T> bool BinaryTree<T>::contains(const T &value) const {
  return contains_helper(root.get(), value);
}

// -------------------------------------------------------------------------------

template <typename T> int BinaryTree<T>::height_helper(const Node *node) const {
  if (!node) {
    return -1;
  }

  int left_height = height_helper(node->left.get());
  int right_height = height_helper(node->right.get());

  return 1 + std::max(left_height, right_height);
}

template <typename T> int BinaryTree<T>::height() const {
  return height_helper(root.get());
}

// -------------------------------------------------------------------------------

template <typename T> size_t BinaryTree<T>::size() const { return node_count; }

template <typename T> bool BinaryTree<T>::empty() const {
  return root == nullptr;
}

template <typename T> void BinaryTree<T>::clear() {
  root.reset();
  node_count = 0;
}

// -------------------------------------------------------------------------------

template <typename T>
void BinaryTree<T>::inorder_helper(const Node *node,
                                   std::vector<T> &result) const {
  if (!node) {
    return;
  }

  inorder_helper(node->left.get(), result);
  result.push_back(node->data);
  inorder_helper(node->right.get(), result);
}

template <typename T>
void BinaryTree<T>::preorder_helper(const Node *node,
                                    std::vector<T> &result) const {
  if (!node) {
    return;
  }

  result.push_back(node->data);
  preorder_helper(node->left.get(), result);
  preorder_helper(node->right.get(), result);
}

template <typename T>
void BinaryTree<T>::postorder_helper(const Node *node,
                                     std::vector<T> &result) const {
  if (!node) {
    return;
  }

  postorder_helper(node->left.get(), result);
  postorder_helper(node->right.get(), result);
  result.push_back(node->data);
}

template <typename T> std::vector<T> BinaryTree<T>::inorder() const {
  std::vector<T> result;

  inorder_helper(root.get(), result);

  return result;
}

template <typename T> std::vector<T> BinaryTree<T>::preorder() const {
  std::vector<T> result;
  preorder_helper(root.get(), result);
  return result;
}

template <typename T> std::vector<T> BinaryTree<T>::postorder() const {
  std::vector<T> result;
  postorder_helper(root.get(), result);
  return result;
}

template <typename T> std::vector<T> BinaryTree<T>::levelorder() const {
  std::vector<T> result;

  if (!root) {
    return result;
  }

  std::queue<const Node *> q;
  q.push(root.get());

  while (!q.empty()) {
    const Node *current = q.front();

    q.pop();

    result.push_back(current->data);

    if (current->left) {
      q.push(current->left.get());
    }
    if (current->right) {
      q.push(current->right.get());
    }
  }

  return result;
}

#endif
