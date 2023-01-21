
#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
template <typename T>
struct Node {
  std::pair<T, T> value;
  Node* left{nullptr};
  Node* right{nullptr};
  size_t height{0};
};

template <typename T>
class AVLTree {
 public:
  AVLTree() : root_(nullptr) {}
  void Add(T first, T second) {
    root_ = Insert(root_, std::make_pair(first, second));
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
    root->left = node->right;
    node->right = root;
    if (root != nullptr) {
      root->height =
          1 + std::max(GetHeight(root->left), GetHeight(root->right));
    }
    if (node != nullptr) {
      node->height =
          1 + std::max(GetHeight(node->left), GetHeight(node->right));
    }
    return node;
  }
  Node<T>* Rotateleft(Node<T>*& root) {
    Node<T>* node = root->right;
    root->right = node->left;
    node->left = root;
    if (root != nullptr) {
      root->height =
          1 + std::max(GetHeight(root->left), GetHeight(root->right));
    }
    if (node != nullptr) {
      node->height =
          1 + std::max(GetHeight(node->left), GetHeight(node->right));
    }
    return node;
  }
  int Bfactor(Node<T>* root) {
    return GetHeight(root->right) - GetHeight(root->left);
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
  Node<T>* Insert(Node<T>*& root, std::pair<T, T> val) {
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
  Node<T>* FindFirst(T q, Node<T>* root) {
    if (root == nullptr) {
      return nullptr;
    }
    if (root->value.first == q) {
      return root;
    }
    if (root->value.first < q) {
      return FindFirst(q, root->right);
    }
    return FindFirst(q, root->left);
  }
  Node<T>* FindSecond(T q, Node<T>* root) {
    if (root == nullptr) {
      return nullptr;
    }
    if (root->value.second == q) {
      return root;
    }
    if (q < root->value.second) {
      return FindSecond(q, root->right);
    }
    return FindSecond(q, root->left);
  }
  void AnswerQuestion(T question) {
    Node<T>* first = FindFirst(question, root_);
    Node<T>* second;
    if (first == nullptr) {
      second = FindSecond(question, root_);
    }
    if (first == nullptr && second == nullptr) {
      return;
    }
    if (first == nullptr) {
      std::cout << second->value.first << std::endl;
      return;
    }
    std::cout << first->value.second << std::endl;
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
  AVLTree<std::string> tree;
  for (size_t i = 0; i < n; ++i) {
    std::string name, car;
    std::cin >> name >> car;
    tree.Add(name, car);
  }
  size_t q = 0;
  std::cin >> q;
  for (size_t i = 0; i < q; ++i) {
    std::string question;
    std::cin >> question;
    tree.AnswerQuestion(question);
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