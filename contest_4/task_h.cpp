#include <cstdlib>
#include <iostream>
#include <utility>


template <typename T>
struct Node {
  Node(int64_t priority, const T& value)
      : size(1),
        priority(priority),
        value(value),
        left(nullptr),
        right(nullptr),
        min(0) {}
  int64_t size{0};
  int64_t priority{0};
  T value;
  bool reverse{false};
  Node<T>* left{nullptr};
  Node<T>* right{nullptr};
  T min;
};
template <typename T>
class TreapArray {
 public:
  TreapArray() : root_(nullptr) {}

  ~TreapArray();

  int64_t Size();

  bool Empty();

  void Insert(int64_t pos, const T& value);

  int64_t FoundMin(int64_t x, int64_t y);

  void Reverse(int64_t left, int64_t right);

 private:
  void Push(Node<T>* node);

  std::pair<Node<T>*, Node<T>*> Find(Node<T>* parent, Node<T>* node,
                                     int64_t pos);

  void Clear(Node<T>* node);

  Node<T>* Merge(Node<T>* first, Node<T>* second);
  void UpdateMin(Node<T>* node);
  int64_t GetSizeRight(Node<T>* node);
  int64_t GetSizeLeft(Node<T>* node);
  int64_t Min(Node<T>* node, int64_t check_l, int64_t check_r);

  std::pair<Node<T>*, Node<T>*> Split(Node<T>* node, int64_t pos);

  void Update(Node<T>* node);

  int64_t Size(Node<T>* node);
  int64_t min_ = 0;
  int64_t left_ = 0;
  int64_t right_ = 0;
  int64_t size_ = 0;
  Node<T>* root_;
};

template <typename T>
TreapArray<T>::~TreapArray() {
  Clear(root_);
}
template <typename T>
int64_t TreapArray<T>::Size() {
  return Size(root_);
}
template <typename T>
bool TreapArray<T>::Empty() {
  return Size(root_) == 0;
}
template <typename T>
void TreapArray<T>::Insert(int64_t pos, const T& value) {
  int64_t priority = rand();
  Node<T>* node = new Node<T>(priority, value);
  node->min = value;
  std::pair<Node<T>*, Node<T>*> tmp = Split(root_, pos);
  root_ = Merge(Merge(tmp.first, node), tmp.second);
  ++size_;
}
template <typename T>
std::pair<Node<T>*, Node<T>*> TreapArray<T>::Find(Node<T>* parent,
                                                  Node<T>* node, int64_t pos) {
  if (node == nullptr) {
    return {parent, node};
  }
  int64_t left_size = Size(node->left);
  if (pos == left_size) {
    return {parent, node};
  }
  if (pos < left_size) {
    return Find(node, node->left, pos);
  }
  return Find(node, node->right, pos - left_size - 1);
}
template <typename T>
int64_t TreapArray<T>::FoundMin(int64_t x, int64_t y) {
  min_ = INT64_MAX;
  left_ = x;
  right_ = y;
  return Min(root_, 1, size_);
}
template <typename T>
void TreapArray<T>::Reverse(int64_t left, int64_t right) {
  std::pair<Node<T>*, Node<T>*> tmp = Split(root_, left - 1);
  std::pair<Node<T>*, Node<T>*> tmp1 = Split(tmp.second, right - left + 1);
  if (tmp1.first != nullptr) {
    tmp1.first->reverse = !tmp1.first->reverse;
  }
  root_ = Merge(tmp.first, Merge(tmp1.first, tmp1.second));
}
template <typename T>
void TreapArray<T>::Push(Node<T>* node) {
  if (node == nullptr) {
    return;
  }
  if (!static_cast<bool>(node->reverse)) {
    return;
  }
  Node<T>* tmp = node->left;
  node->left = node->right;
  node->right = tmp;
  node->reverse = false;
  if (node->left != nullptr) {
    node->left->reverse ^= true;
  }
  if (node->right != nullptr) {
    node->right->reverse ^= true;
  }
}

template <typename T>
void TreapArray<T>::Clear(Node<T>* node) {
  if (node == nullptr) {
    return;
  }
  Clear(node->left);
  Clear(node->right);
  delete node;
}
template <typename T>
Node<T>* TreapArray<T>::Merge(Node<T>* first, Node<T>* second) {
  Push(first);
  UpdateMin(first);
  Update(first);
  Push(second);
  UpdateMin(second);
  Update(second);
  if (first == nullptr) {
    return second;
  }
  if (second == nullptr) {
    return first;
  }
  if (first->priority > second->priority) {
    first->right = Merge(first->right, second);
    UpdateMin(first);
    Update(first);
    return first;
  }
  second->left = Merge(first, second->left);
  UpdateMin(second);
  Update(second);
  return second;
}
template <typename T>
void TreapArray<T>::UpdateMin(Node<T>* node) {
  if (node == nullptr) {
    return;
  }
  int64_t min = node->value;
  if (node->right != nullptr) {
    min = std::min(min, node->right->min);
  }
  if (node->left != nullptr) {
    min = std::min(min, node->left->min);
  }
  node->min = min;
}
template <typename T>
int64_t TreapArray<T>::GetSizeRight(Node<T>* node) {
  if (node->right != nullptr) {
    return node->right->size;
  }
  return 0;
}
template <typename T>
int64_t TreapArray<T>::GetSizeLeft(Node<T>* node) {
  if (node->left != nullptr) {
    return node->left->size;
  }
  return 0;
}
template <typename T>
int64_t TreapArray<T>::Min(Node<T>* node, int64_t check_l, int64_t check_r) {
  Push(node);
  if (node == nullptr || check_r < left_ || right_ < check_l) {
    return min_;
  }
  if (left_ < check_l && check_r < right_) {
    return std::min(node->min, min_);
  }
  int64_t sz_right = 0;
  int64_t sz_left = 0;
  sz_right = GetSizeRight(node);
  sz_left = GetSizeLeft(node);
  if (check_r - sz_right <= right_ && check_r - sz_right >= left_) {
    min_ = std::min(min_, node->value);
  }
  int64_t left_min = Min(node->left, check_l, check_r - sz_right - 1);
  int64_t right_min = Min(node->right, check_l + 1 + sz_left, check_r);
  return std::min(left_min, right_min);
}
template <typename T>
std::pair<Node<T>*, Node<T>*> TreapArray<T>::Split(Node<T>* node, int64_t pos) {
  Push(node);
  UpdateMin(node);
  Update(node);
  if (node == nullptr) {
    return {nullptr, nullptr};
  }
  int64_t left_size = Size(node->left);
  if (pos <= left_size) {
    std::pair<Node<T>*, Node<T>*> tmp = Split(node->left, pos);
    UpdateMin(tmp.second);
    node->left = tmp.second;
    UpdateMin(node);
    Update(node);
    return {tmp.first, node};
  }
  std::pair<Node<T>*, Node<T>*> tmp = Split(node->right, pos - left_size - 1);
  UpdateMin(tmp.first);
  node->right = tmp.first;
  UpdateMin(node);
  Update(node);
  return {node, tmp.second};
}
template <typename T>
void TreapArray<T>::Update(Node<T>* node) {
  if (node == nullptr) {
    return;
  }
  node->size = 1 + Size(node->left) + Size(node->right);
}
template <typename T>
int64_t TreapArray<T>::Size(Node<T>* node) {
  if (node == nullptr) {
    return 0;
  }
  return node->size;
}

void RmqAndReverse(size_t n, size_t m) {
  int operation = 0;
  int64_t x = 0;
  int64_t y = 0;
  TreapArray<int64_t> tree;
  for (size_t i = 0; i < n; ++i) {
    int64_t elem = 0;
    std::cin >> elem;
    tree.Insert(i, elem);
  }
  for (size_t i = 0; i < m; ++i) {
    std::cin >> operation >> x >> y;
    if (operation == 1) {
      tree.Reverse(x, y);
    } else if (operation == 2) {
      std::cout << tree.FoundMin(x, y) << std::endl;
    }
  }
}

int main() {
  size_t n = 0;
  size_t m = 0;
  std::cin >> n >> m;
  RmqAndReverse(n, m);
}

