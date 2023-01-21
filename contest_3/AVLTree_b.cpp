#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

template <typename T>
struct Node {
  T value;
  Node* left{nullptr};
  Node* right{nullptr};
  size_t height{0};
};

template <typename T>
class AVLTree {
 public:
  AVLTree() : root_(nullptr) {}
  bool FindAdd(Node<T>* node, T value);
  void Add(T val);
  static size_t GetHeight(Node<T>* root);
  static void CalcHeight(Node<T>* node);
  Node<T>* Rotateright(Node<T>*& root);
  Node<T>* Rotateleft(Node<T>*& root);
  int Bfactor(Node<T>* root);
  Node<T>* Balance(Node<T>*& root);
  Node<T>* Insert(Node<T>*& root, T val);
  T Find(Node<T>* node, T value);

  T AnswerFind(T val);
  void Clear(Node<T>* node);

  ~AVLTree();

 private:
  Node<T>* root_;
};

template <typename T>
bool AVLTree<T>::FindAdd(Node<T>* node, T value) {
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
template <typename T>
void AVLTree<T>::Add(T val) {
  if (!FindAdd(root_, val)) {
    root_ = Insert(root_, val);
  }
}
template <typename T>
size_t AVLTree<T>::GetHeight(Node<T>* root) {
  if (root == nullptr) {
    return 0;
  }
  return root->height;
}
template <typename T>
void AVLTree<T>::CalcHeight(Node<T>* node) {
  if (node == nullptr) {
    return;
  }
  node->height = std::max(GetHeight(node->left), GetHeight(node->right)) + 1;
}
template <typename T>
Node<T>* AVLTree<T>::Rotateright(Node<T>*& root) {
  Node<T>* node = root->left;
  root->left = node->right;
  node->right = root;
  if (root != nullptr) {
    root->height = 1 + std::max(GetHeight(root->left), GetHeight(root->right));
  }
  if (node != nullptr) {
    node->height = 1 + std::max(GetHeight(node->left), GetHeight(node->right));
  }
  return node;
}
template <typename T>
Node<T>* AVLTree<T>::Rotateleft(Node<T>*& root) {
  Node<T>* node = root->right;
  root->right = node->left;
  node->left = root;
  if (root != nullptr) {
    root->height = 1 + std::max(GetHeight(root->left), GetHeight(root->right));
  }
  if (node != nullptr) {
    node->height = 1 + std::max(GetHeight(node->left), GetHeight(node->right));
  }
  return node;
}
template <typename T>
int AVLTree<T>::Bfactor(Node<T>* root) {
  return GetHeight(root->right) - GetHeight(root->left);
}
template <typename T>
Node<T>* AVLTree<T>::Balance(Node<T>*& root) {
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
template <typename T>
Node<T>* AVLTree<T>::Insert(Node<T>*& root, T val) {
  if (root == nullptr) {
    Node<T>* new_node = new Node<T>;
    new_node->value = val;
    return new_node;
  }
  if (root->value > val) {
    root->left = Insert(root->left, val);
  } else {
    root->right = Insert(root->right, val);
  }
  return Balance(root);
}
template <typename T>
T AVLTree<T>::Find(Node<T>* node, T value) {
  Node<T>* tmp = nullptr;
  if (node != nullptr && node->value == value) {
    return node->value;
  }
  if (node != root_ || (node != nullptr && node->value > value)) {
    for (; node->left != nullptr && node->left->value >= value;) {
      node = node->left;
    }
    if (node != nullptr) {
      if (node->value == value) {
        return node->value;
      }
      if (node->left == nullptr && node->value >= value) {
        return node->value;
      }
      tmp = node;
      if (node->left != nullptr) {
        node = node->left;
      }
    }
  }
  for (; node != nullptr && node->right != nullptr &&
         node->right->value <= value;) {
    node = node->right;
  }
  if (node != nullptr && node->value == value) {
    return node->value;
  }
  if (node != nullptr && node->right == nullptr) {
    if (tmp != nullptr && tmp->value >= value) {
      return tmp->value;
    }
    if (tmp == nullptr) {
      return -1;
    }
  }
  if (node != nullptr) {
    node = node->right;
  }
  return Find(node, value);
}
template <typename T>
T AVLTree<T>::AnswerFind(T val) {
  return Find(root_, val);
}
template <typename T>
void AVLTree<T>::Clear(Node<T>* node) {
  if (node == nullptr) {
    return;
  }
  Clear(node->right);
  Clear(node->left);
  delete node;
}
template <typename T>
AVLTree<T>::~AVLTree() {
  Clear(root_);
  root_ = nullptr;
}

void AnswerForQuestion(size_t n) {
  AVLTree<int64_t> tree;
  bool check = false;
  int64_t y = 0;
  for (size_t i = 0; i < n; ++i) {
    char operation = 0;
    int64_t element = 0;
    std::cin >> operation >> element;
    if (operation == '+' && !check) {
      tree.Add(element);
    } else if (operation == '+' && check) {
      int64_t val = (element + y) % 1000000000;
      tree.Add(val);
      check = false;
    } else if (operation == '?') {
      check = true;
      y = tree.AnswerFind(element);
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
  AnswerForQuestion(n);
}
