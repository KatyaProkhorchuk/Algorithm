//
// Created by katerina on 18.11.22.
//
#include <iostream>
#include <cstdlib>
#include <utility>
#include <random>


template <typename T>
class TreapArray {
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
    Node* left{nullptr};
    Node* right{nullptr};
    T min;
  };

 public:
  TreapArray() : root_(nullptr) {}

  ~TreapArray() {
    Clear(root_);
  }

  int64_t Size() {
    return Size(root_);
  }

  bool Empty() {
    return Size(root_) == 0;
  }

  void Insert(int64_t pos, const T& value) {
    int64_t priority = rand() % 150000;
    Node* node = new Node(priority, value);
    node->min = value;
    std::pair<Node*, Node*> tmp = Split(root_, pos);
    root_ = Merge(Merge(tmp.first, node), tmp.second);
    ++size_;
  }

  T& operator[](int64_t pos) {
    std::pair<Node*, Node*> tmp = Find(nullptr, root_, pos);
    return tmp.second->value;
  }
  void Inorder(){
    In(root_);
  }
  int64_t FoundMin(int64_t x, int64_t y) {
    return Min(root_, INT64_MAX, x, y, 1, size_);
  }
 private:
  void In(Node* root){
    if(root == nullptr){
      return;
    }
    In(root->left);
    std::cout<<root->value<< " ";
    In(root->right);

  }
  std::pair<Node*, Node*> Find(Node* parent, Node* node, int64_t pos) {
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

  void Clear(Node* node) {
    if (node == nullptr) {
      return;
    }
    Clear(node->left);
    Clear(node->right);
    delete node;
  }

  Node* Merge(Node* first, Node* second) {
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
  void UpdateMin(Node* node){
    if (node == nullptr) {
      return;
    }
    int64_t min = node->value;
    if (node->right != nullptr) {
      min = std::min(min, node->right->min);
    }
    if(node->left != nullptr) {
      min = std::min(min, node->left->min);
    }
    node->min = min;
  }

  int64_t Min(Node* node, int64_t min, int64_t left, int64_t right,int64_t check_l, int64_t check_r ) {
    if(node == nullptr) {
      return min;
    }
    if (check_r < left || right < check_l) {
      return min;
    }
    if (left < check_l && check_r < right) {
      return std::min(node->min, min);
    }
    int64_t sz_right = 0, sz_left = 0;
    if ( node->right != nullptr){
      sz_right += node->right->size;
    }
    if (node->left != nullptr) {
      sz_left += node->left->size;
    }
    if (check_r - sz_right <= right && check_r - sz_right >= left){
      min = std::min(min, node->value);
    }
    int64_t left_min = Min(node->left, min, left, right, check_l, check_r - sz_right - 1);
    int64_t right_min = Min(node->right, min, left, right, check_l + 1 + sz_left, check_r );
    return std::min(left_min, right_min);
  }

  std::pair<Node*, Node*> Split(Node* node, int64_t pos) {
    if (node == nullptr) {
      return {nullptr, nullptr};
    }
    int64_t left_size = Size(node->left);
    if (pos <= left_size) {
      std::pair<Node*, Node*> tmp = Split(node->left, pos);
      UpdateMin(tmp.second);
      node->left = tmp.second;
      UpdateMin(node);
      Update(node);
      return {tmp.first, node};
    }
    std::pair<Node*, Node*> tmp = Split(node->right, pos - left_size - 1);
    UpdateMin(tmp.first);
    node->right = tmp.first;
    UpdateMin(node);
    Update(node);
    return {node, tmp.second};
  }

  void Update(Node* node) {
    if (node == nullptr) {
      return;
    }
    node->size = 1 + Size(node->left) + Size(node->right);
  }

  int64_t Size(Node* node) {
    if (node == nullptr) {
      return 0;
    }
    return node->size;
  }

  int64_t size_ = 0;
  Node* root_;
};

void Run(int64_t n) {
  char operation = '\0';
  int64_t x = 0, y = 0;
  TreapArray<int64_t> tree;
  for (int64_t i = 0; i < n; ++i) {
    std::cin>>operation>>x>>y;
    if(operation == '+') {
      tree.Insert(x,y);
    } else if (operation == '?') {
      std::cout<<tree.FoundMin(x,y)<<std::endl;
    }
  }
}

int main(){
  int64_t n;
  std::cin>>n;
  Run(n);
}