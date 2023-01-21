#include <cstdlib>
#include <iostream>
#include <utility>

class Treap {
 public:
  Treap() : treap_(nullptr) {}

  void Insert(int64_t x);
  void Delete(int64_t x);
  void Next(int64_t x);
  void Prev(int64_t x);
  bool CheckExist(int64_t x);
  void PrintKth(int64_t number);
  ~Treap();

 private:
  struct Node {
    Node(int64_t x, int64_t y) : x(x), y(y), left(nullptr), right(nullptr) {}
    int64_t x{0}, y{0};
    Node* left{nullptr};
    Node* right{nullptr};
    int64_t size{1};
  };
  static void Recalc(Node* node);
  Node* Merge(Node* left, Node* right);

  std::pair<Node*, Node*> Split(Node* node, int64_t key);
  void Clear(Node* node);
  void Erase(int64_t x);
  static Node* SearchKStatistics(Node* root, int64_t k);
  int64_t NextElement(Node* node, int64_t value);
  int64_t PrevElement(Node* node, int64_t value);
  bool Find(Node* node, int64_t x);
  Node* treap_;
};

void Treap::Insert(int64_t x) {
  if (Find(treap_, x)) {
    return;
  }
  int64_t y = rand();
  std::pair<Treap::Node*, Treap::Node*> tmp = Split(treap_, x);
  Treap::Node* new_treap = new Treap::Node(x, y);
  treap_ = Merge(tmp.first, Merge(new_treap, tmp.second));
}
void Treap::Delete(int64_t x) {
  if (!Find(treap_, x)) {
    return;
  }
  Erase(x);
}
void Treap::Next(int64_t x) {
  int64_t result = NextElement(treap_, x);
  if (result != INT64_MAX) {
    std::cout << result << std::endl;
    return;
  }
  std::cout << "none" << std::endl;
}
void Treap::Prev(int64_t x) {
  int64_t result = PrevElement(treap_, x);
  if (result != INT64_MAX) {
    std::cout << result << std::endl;
    return;
  }
  std::cout << "none" << std::endl;
}
bool Treap::CheckExist(int64_t x) { return (Find(treap_, x)); }
void Treap::PrintKth(int64_t number) {
  if ((treap_ != nullptr && number >= treap_->size) || treap_ == nullptr) {
    std::cout << "none" << std::endl;
  } else {
    Treap::Node* tmp = SearchKStatistics(treap_, number);
    if (tmp != nullptr) {
      std::cout << tmp->x << std::endl;
    } else {
      std::cout << "none" << std::endl;
    }
  }
}
Treap::~Treap() { Clear(treap_); }

void Treap::Recalc(Treap::Node* node) {
  if (node != nullptr) {
    node->size = 1;
    if (node->left != nullptr) {
      node->size += node->left->size;
    }
    if (node->right != nullptr) {
      node->size += node->right->size;
    }
  }
}

Treap::Node* Treap::Merge(Treap::Node* left, Treap::Node* right) {
  if (right == nullptr) {
    return left;
  }
  if (left == nullptr) {
    return right;
  }
  if (left->y > right->y) {
    left->right = Merge(left->right, right);
    Recalc(left->right);
    Recalc(left);
    return left;
  }
  right->left = Merge(left, right->left);
  Recalc(right->left);
  Recalc(right);
  return right;
}

std::pair<Treap::Node*, Treap::Node*> Treap::Split(Treap::Node* node,
                                                   int64_t key) {
  if (node == nullptr) {
    return {nullptr, nullptr};
  }
  if (key < node->x) {
    std::pair<Treap::Node*, Treap::Node*> result = Split(node->left, key);
    node->left = result.second;
    Recalc(node);
    Recalc(result.first);
    return {result.first, node};
  }
  std::pair<Treap::Node*, Treap::Node*> result = Split(node->right, key);
  node->right = result.first;
  Recalc(node);
  Recalc(result.second);
  return {node, result.second};
}
void Treap::Clear(Treap::Node* node) {
  if (node == nullptr) {
    return;
  }
  Clear(node->right);
  Clear(node->left);

  delete node;
}
void Treap::Erase(int64_t x) {
  std::pair<Treap::Node*, Treap::Node*> tmp = Split(treap_, x);
  std::pair<Treap::Node*, Treap::Node*> tmp_after = Split(tmp.first, x - 1);
  delete tmp_after.second;
  treap_ = Merge(tmp_after.first, tmp.second);
}
Treap::Node* Treap::SearchKStatistics(Treap::Node* root, int64_t k) {
  while (root != nullptr) {
    int64_t size_left = 0;
    if (root->left != nullptr) {
      size_left = root->left->size;
    }
    if (size_left == k) {
      return root;
    }
    root = (size_left) > k ? root->left : root->right;
    if (size_left < k) {
      k -= size_left + 1;
    }
  }
  return nullptr;
}
int64_t Treap::NextElement(Treap::Node* node, int64_t value) {
  if (node == nullptr) {
    return INT64_MAX;
  }
  int64_t result = 0;
  int64_t potential_result = 0;
  if (node->x <= value) {
    result = NextElement(node->right, value);
  } else {
    result = node->x;
    potential_result = NextElement(node->left, value);
    if (potential_result != INT64_MAX) {
      result = potential_result;
    }
  }
  return result;
}
int64_t Treap::PrevElement(Treap::Node* node, int64_t value) {
  if (node == nullptr) {
    return INT64_MAX;
  }
  int64_t result = 0;
  int64_t potential_result = 0;
  if (node->x >= value) {
    result = PrevElement(node->left, value);
  } else {
    result = node->x;
    potential_result = PrevElement(node->right, value);
    if (potential_result != INT64_MAX) {
      result = potential_result;
    }
  }
  return result;
}
bool Treap::Find(Treap::Node* node, int64_t x) {
  if (node == nullptr) {
    return false;
  }
  if (x == node->x) {
    return true;
  }
  if (x <= node->x) {
    return Find(node->left, x);
  }
  return Find(node->right, x);
}
void AnswerForQuestion() {
  std::string request;
  Treap tree;
  while (std::cin >> request) {
    int64_t element = 0;
    std::cin >> element;
    if (request == "insert") {
      tree.Insert(element);
    } else if (request == "delete") {
      tree.Delete(element);
    } else if (request == "exists") {
      if (tree.CheckExist(element)) {
        std::cout << "true" << std::endl;
      } else {
        std::cout << "false" << std::endl;
      }
    } else if (request == "next") {
      tree.Next(element);
    } else if (request == "prev") {
      tree.Prev(element);
    } else if (request == "kth") {
      tree.PrintKth(element);
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  AnswerForQuestion();
}

