#include <iostream>
struct Node {
  Node(int x, int y, int id)
      : x(x), y(y), parents(nullptr), left(nullptr), right(nullptr), id(id) {}
  int x = 0, y = 0;
  Node* parents = nullptr;
  Node* left = nullptr;
  Node* right = nullptr;
  int id = 0;
};

class Treap {
 public:
  Treap() : root_(nullptr) {}

  Node* FindLowersNode(Node* last, int y) {
    if (last->y > y) {
      return last;
    }
    if (last->parents != nullptr) {
      return FindLowersNode(last->parents, y);
    }
    return nullptr;
  }

  void Insert(int x, int y, int id) {
    Node* new_node = new Node(x, y, id);
    if (root_ == nullptr) {
      last_ = new_node;
      root_ = new_node;
      return;
    }
    Node* lowers = FindLowersNode(last_, y);
    if (lowers == nullptr) {
      new_node->left = root_;
      root_->parents = new_node;
      root_ = new_node;
    } else {
      Node* tmp = lowers->right;
      new_node->left = lowers->right;
      if (tmp != nullptr) {
        tmp->parents = new_node;
      }
      lowers->right = new_node;
      new_node->parents = lowers;
    }
    last_ = new_node;
  }

  void PrintBeautiful(Node* node) {
    if (node->parents != nullptr) {
      std::cout << node->parents->id << " ";
    } else {
      std::cout << 0 << " ";
    }
    if (node->left != nullptr) {
      std::cout << node->left->id << " ";
    } else {
      std::cout << 0 << " ";
    }
    if (node->right != nullptr) {
      std::cout << node->right->id << " ";
    } else {
      std::cout << 0 << " ";
    }
    std::cout << std::endl;
  }

  void InorderTraversal(Node* node) {
    if (node == nullptr) {
      return;
    }
    InorderTraversal(node->left);
    PrintBeautiful(node);
    InorderTraversal(node->right);
  }

  void Clear(Node* node) {
    if (node == nullptr) {
      return;
    }
    Clear(node->right);
    Clear(node->left);
    delete node;
  }

  ~Treap() { Clear(root_); }

  void Print() { InorderTraversal(root_); }

 private:
  Node* root_ = nullptr;
  Node* last_ = nullptr;
};

void Run(size_t n) {
  int x = 0, y = 0;
  Treap tree;
  for (size_t i = 0; i < n; ++i) {
    std::cin >> x >> y;
    tree.Insert(x, (-1) * y, i + 1);
  }
  std::cout << "YES" << std::endl;
  tree.Print();
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t n = 0;
  std::cin >> n;
  Run(n);
}