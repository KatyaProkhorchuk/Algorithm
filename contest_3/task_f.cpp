#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
template <typename T>
struct Node {
  T value;
  Node<T>* left{nullptr};
  Node<T>* right{nullptr};
  Node<T>* parent{nullptr};
  size_t height{0};
  size_t sum{0};
};
template <typename T>
class AVLTree {
 public:
  AVLTree() : root_(nullptr) {}
  static bool FindAdd(Node<T>* node, T value) {
    if (node == nullptr) {
      return false;
    }
    if (node->value == value) {
      return true;
    }
    if (value < node->value) {
      return FindAdd(node->left, value);
    }
    return FindAdd(node->right, value);
  }
  void Add(T val) {
    if (!FindAdd(root_, val)) {
      root_ = Insert(root_, nullptr, val);
    }
  }
  static size_t GetHeight(Node<T>* root) {
    if (root == nullptr) {
      return 0;
    }
    return root->height;
  }
  static void CalcHeight(Node<T>* node) {
    if (node == nullptr) {
      return;
    }
    node->height = std::max(GetHeight(node->left), GetHeight(node->right)) + 1;
  }
  Node<T>* Rotateright(Node<T>*& root) {
    Node<T>* node = root->left;
    node->parent = root->parent;
    root->left = node->right;
    node->right = root;
    node->right->parent = node;
    if (root != nullptr) {
      root->height =
          1 + std::max(GetHeight(root->left), GetHeight(root->right));
    }

    root = node;
    if (node != nullptr) {
      node->height =
          1 + std::max(GetHeight(node->left), GetHeight(node->right));
    }
    SumCheckRight(node);
    return node;
  }
  Node<T>* Rotateleft(Node<T>*& root) {
    Node<T>* node = root->right;
    node->parent = root->parent;
    root->right = node->left;
    node->left = root;
    node->left->parent = node;
    if (root != nullptr) {
      root->height =
          1 + std::max(GetHeight(root->left), GetHeight(root->right));
    }

    root = node;
    if (node != nullptr) {
      node->height =
          1 + std::max(GetHeight(node->left), GetHeight(node->right));
    }
    SumCheckLeft(node);
    return node;
  }
  int Bfactor(Node<T>* root) {
    return GetHeight(root->right) - GetHeight(root->left);
  }
  void SumCheckRight(Node<T>* root) {
    if (root == nullptr) {
      return;
    }
    if (root->right != nullptr) {
      root = root->right;
      root->sum = root->value;
      if (root->left != nullptr) {
        root->sum += root->left->sum;
      }
      if (root->right != nullptr) {
        root->sum += root->right->sum;
      }
      root = root->parent;
    }
    while (root != nullptr) {
      root->sum = root->value;
      if (root->left != nullptr) {
        root->sum += root->left->sum;
      }
      if (root->right != nullptr) {
        root->sum += root->right->sum;
      }
      root = root->parent;
    }
  }
  void SumCheckLeft(Node<T>* root) {
    if (root == nullptr) {
      return;
    }
    if (root->left != nullptr) {
      root = root->left;
      root->sum = root->value;
      if (root->left != nullptr) {
        root->sum += root->left->sum;
      }
      if (root->right != nullptr) {
        root->sum += root->right->sum;
      }
      root = root->parent;
    }
    while (root != nullptr) {
      root->sum = root->value;
      if (root->left != nullptr) {
        root->sum += root->left->sum;
      }
      if (root->right != nullptr) {
        root->sum += root->right->sum;
      }
      root = root->parent;
    }
  }
  Node<T>* Balance(Node<T>*& root) {
    root->height = 1 + std::max(GetHeight(root->left), GetHeight(root->right));
    if (Bfactor(root) == 2) {
      if (Bfactor(root->right) == -1) {
        root->right = Rotateright(root->right);
      }
      return Rotateleft(root);
    }
    if (Bfactor(root) == -2) {
      if (Bfactor(root->left) == 1) {
        root->left = Rotateleft(root->left);
      }
      return Rotateright(root);
    }
    return root;
  }
  Node<T>* Insert(Node<T>*& root, Node<T>* parent, T val) {
    if (root == nullptr) {
      Node<T>* new_node = new Node<T>;
      new_node->value = val;
      new_node->sum = val;
      new_node->parent = parent;
      return new_node;
    }
    root->sum += val;
    if (root->value > val) {
      root->left = Insert(root->left, root, val);
    } else {
      root->right = Insert(root->right, root, val);
    }
    return Balance(root);
  }
  T SumLeft(Node<T>* node, T value) {
    if (node == nullptr) {
      return 0;
    }
    T less_value = 0;
    T sum = node->sum;
    while (node != nullptr && node->value != value) {
      if (node->value > value) {
        node = node->left;
      } else if (node->value < value) {
        less_value += node->value;
        if (node->left != nullptr) {
          less_value += node->left->sum;
        }
        if (node->right == nullptr) {
          break;
        }
        node = node->right;
      }
    }
    if (node != nullptr && node->value == value) {
      if (node->left != nullptr) {
        return sum - node->left->sum - less_value;
      }
      if (node->left == nullptr) {
        return sum - less_value;
      }
    }
    return sum - less_value;
  }
  T SumRight(Node<T>* node, T value) {
    if (node == nullptr) {
      return 0;
    }
    T less_value = 0;
    T sum = node->sum;
    while (node != nullptr && node->value != value) {
      if (node->value > value) {
        node = node->left;
      }
      else if (node->value < value) {
        less_value += node->value;
        if (node->left != nullptr) {
          less_value += node->left->sum;
        }
        if (node->right == nullptr) {
          break;
        }
        node = node->right;
      }
    }
    if (node != nullptr && node->value == value) {
      if (node->left != nullptr) {
        return sum - node->left->sum - less_value - node->value;
      }
      if (node->left == nullptr) {
        return sum - less_value - node->value;
      }
    }
    return sum - less_value;
  }
  T AnswerFind(T left, T right) {
    T sum = 0;
    T sum_left = SumLeft(root_, left);
    T sum_right = SumRight(root_, right);
    sum = sum_left - sum_right;
    return sum;
  }
  void Clear(Node<T>* node) {
    if (node == nullptr) {
      return;
    }
    Clear(node->right);
    Clear(node->left);
    delete node;
  }

  ~AVLTree() {
    Clear(root_);
    root_ = nullptr;
  }

 private:
  Node<T>* root_;
};

void Run(size_t n) {
  AVLTree<int64_t> tree;
  bool check = false;
  int64_t y = 0;
  for (size_t i = 0; i < n; ++i) {
    char operation = 0;
    int64_t element = 0;
    int64_t l = 0, r = 0;
    std::cin >> operation;
    if (operation == '+' && !check) {
      std::cin >> element;
      tree.Add(element);
    } else if (operation == '+' && check) {
      std::cin >> element;
      int64_t val = (element + y) % 1000000000;
      tree.Add(val);
      check = false;
    } else if (operation == '?') {
      std::cin >> l >> r;
      check = true;
      y = tree.AnswerFind(l, r);
      std::cout << y << std::endl;
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t n = 0;
  std::cin >> n;
  Run(n);
}
