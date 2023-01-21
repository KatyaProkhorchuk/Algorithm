#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>

class Treap {
 public:
  Treap() : treap_(nullptr) {}

  void Insert(int64_t x) {
    int64_t y = rand();
    std::pair<Node*, Node*> tmp = Split(treap_, x);
    Node* new_treap = new Node(x, y);
    ++new_treap->count;
    treap_ = Merge(tmp.first, Merge(new_treap, tmp.second));
  }
  int64_t Prev(int64_t x) {
    if (treap_ == nullptr) {
      return 0;
    }
    size_t res = 0;
    std::pair<Node*, Node*> tmp = Split(treap_, x - 1);
    if (tmp.first != nullptr) {
      res = tmp.first->size;
    }
    treap_ = Merge(tmp.first, tmp.second);
    return res;
  }

  ~Treap() { Clear(treap_); }

 private:
  struct Node {
    Node(int64_t x, int64_t y) : x(x), y(y), left(nullptr), right(nullptr) {}
    int64_t x{0};
    int64_t y{0};
    Node* left{nullptr};
    Node* right{nullptr};
    int64_t size{1};
    size_t count{0};
  };

  void Recalc(Node* node) {
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
  Node* Merge(Node* left, Node* right) {
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
  std::pair<Node*, Node*> Split(Node* node, int64_t key) {
    if (node == nullptr) {
      return {nullptr, nullptr};
    }
    if (key < node->x) {
      std::pair<Node*, Node*> result = Split(node->left, key);
      node->left = result.second;
      Recalc(node);
      Recalc(result.first);
      return {result.first, node};
    }
    std::pair<Node*, Node*> result = Split(node->right, key);
    node->right = result.first;
    Recalc(node);
    Recalc(result.second);
    return {node, result.second};
  }
  void Clear(Node* node) {
    if (node == nullptr) {
      return;
    }
    Clear(node->right);
    Clear(node->left);
    delete node;
  }
  Node* treap_;
};

struct Segment {
  int64_t from;
  int64_t to;
};

bool operator<=(const Segment& first, const Segment& second) {
  if (first.from == second.from) {
    return first.to > second.to;
  }
  return (first.from <= second.from);
}
bool operator!=(const Segment& first, const Segment& second) {
  return !((first.from == second.from) && (first.to == second.to));
}
bool operator==(const Segment& first, const Segment& second) {
  return ((first.from == second.from) && (first.to == second.to));
}
bool operator<(const Segment& first, const Segment& second) {
  if (first.from != second.from) {
    return first.from < second.from;
  }
  return first.to > second.to;
}

std::vector<Segment> ReadPoints() {
  std::vector<Segment> points;
  size_t segment_count;
  std::cin >> segment_count;
  for (size_t i = 0; i < segment_count; ++i) {
    int64_t x;
    int64_t y;
    std::cin >> x >> y;
    points.push_back({x, y});
  }
  return points;
}

void Run(std::vector<Segment> points) {
  Treap tree;
  std::vector<int> check_povtor(points.size());
  Segment point = points[0];
  size_t count_point = 0, ans = 0;
  for (size_t i = 0; i < points.size(); ++i) {
    ans += i;
    if (point != points[i]) {
      count_point = i;
      point = points[i];
    } else {
      ans -= i - count_point;
    }
  }
  for (size_t i = 0; i < points.size(); ++i) {
    ans -= tree.Prev(points[i].to);
    tree.Insert(points[i].to);
  }
  std::cout << ans << std::endl;
}
int main() {
  auto points = ReadPoints();
  std::sort(points.begin(), points.end());
  Run(points);
}
//5
//1 2
//1 3
//1 2
//1 4
//1 4
//
//7
//5 6
//4 7
//3 8
//3 9
//2 9
//2 10
//1 11
//7
//1 4
//3 8
//2 7
//12 14
//11 21
//15 49
//4 7