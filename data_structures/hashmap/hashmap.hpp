#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <functional>
#include <memory>
#include <utility>
#include <vector>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class HashMap {
private:
  struct Node {
    Key key;
    Value value;
    std::unique_ptr<Node> next;

    explicit Node(Key k, Value v)
        : key(std::move(k)), value(std::move(v)), next(nullptr) {}
  };

  static constexpr size_t k_default_capacity = 10;
  static constexpr double k_max_load_factor = 0.75;

  std::vector<std::unique_ptr<Node>> buckets_;
  size_t size_;

  size_t bucket_index(const Key &key, size_t capacity) const;

  void resize(size_t new_capcity);

public:
  explicit HashMap(size_t initial_capacity = k_default_capacity)
      : buckets_(initial_capacity == 0 ? k_default_capacity : initial_capacity),
        size_(0) {}

  ~HashMap() = default;

  bool set(const Key &key, const Value &value);

  const Value *get(const Key &key) const;

  bool contains(const Key &key) const;

  bool del(const Key &key);

  size_t size() const noexcept;
  bool empty() const noexcept;

  void clear();
};

// ---------------------------------------------------------------------------

template <typename Key, typename Value, typename Hash>
size_t HashMap<Key, Value, Hash>::bucket_index(const Key &key,
                                               size_t capacity) const {
  return Hash{}(key) % capacity;
}

template <typename Key, typename Value, typename Hash>
void HashMap<Key, Value, Hash>::resize(size_t new_capacity) {
  std::vector<std::unique_ptr<Node>> new_buckets(new_capacity);

  for (auto &head : buckets_) {
    while (head != nullptr) {
      std::unique_ptr<Node> node = std::move(head);
      head = std::move(node->next);

      size_t index = bucket_index(node->key, new_capacity);
      node->next = std::move(new_buckets[index]);
      new_buckets[index] = std::move(node);
    }
  }

  buckets_ = std::move(new_buckets);
}

// ---------------------------------------------------------------------------

template <typename Key, typename Value, typename Hash>
bool HashMap<Key, Value, Hash>::set(const Key &key, const Value &value) {
  size_t index = bucket_index(key, buckets_.size());

  for (Node *node = buckets_[index].get(); node != nullptr;
       node = node->next.get()) {
    if (node->key == key) {
      node->value = value;
      return true;
    }
  }

  auto entry = std::make_unique<Node>(key, value);
  entry->next = std::move(buckets_[index]);
  buckets_[index] = std::move(entry);
  ++size_;

  if (static_cast<double>(size_) / static_cast<double>(buckets_.size()) >
      k_max_load_factor) {
    resize(buckets_.size() * 2);
  }

  return true;
}

template <typename Key, typename Value, typename Hash>
const Value *HashMap<Key, Value, Hash>::get(const Key &key) const {
  size_t index = bucket_index(key, buckets_.size());

  for (Node *node = buckets_[index].get(); node != nullptr;
       node = node->next.get()) {
    if (node->key == key) {
      return &node->value;
    }
  }

  return nullptr;
}

template <typename Key, typename Value, typename Hash>
bool HashMap<Key, Value, Hash>::contains(const Key &key) const {
  return get(key) != nullptr;
}

template <typename Key, typename Value, typename Hash>
bool HashMap<Key, Value, Hash>::del(const Key &key) {
  size_t index = bucket_index(key, buckets_.size());

  std::unique_ptr<Node> *slot = &buckets_[index];

  while (*slot != nullptr) {
    if ((*slot)->key == key) {
      *slot = std::move((*slot)->next);
      --size_;
      return true;
    }
    slot = &(*slot)->next;
  }

  return false;
}

template <typename Key, typename Value, typename Hash>
size_t HashMap<Key, Value, Hash>::size() const noexcept {
  return size_;
}

template <typename Key, typename Value, typename Hash>
bool HashMap<Key, Value, Hash>::empty() const noexcept {
  return size_ == 0;
}

template <typename Key, typename Value, typename Hash>
void HashMap<Key, Value, Hash>::clear() {
  for (auto &head : buckets_) {
    head.reset();
  }

  size_ = 0;
}

#endif
