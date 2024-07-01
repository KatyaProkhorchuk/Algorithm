#include <iostream>
#include <vector>
template <typename T>
class LCA {
 public:
  LCA(size_t n):
        is_live_(n),
        parent_(n),
        depth_(n + 1) {
    lg_ = std::__lg(n) + 1;
    height_.resize(n + 1, std::vector<T>(lg_ + 1, 1));
  }
  void AddEdge(T v, T u) {
    parent_[u] = v;
    height_[u][0] = v;
    depth_[u] = depth_[v] + 1;
    for (int32_t i = 1; i <= lg_; ++i) {
      height_[u][i] = height_[height_[u][i - 1]][i - 1];
    }
  }
  void Delete(T vertex) {
    is_live_[vertex] = 1;
  }
  T Is_Live(T vertex) {
    if (!is_live_[parent_[vertex]]) {
      return parent_[vertex];
    }
    parent_[vertex] = Is_Live(height_[vertex][0]);
    return parent_[vertex];
  }
  T Lca(T from, T to) {
    if (depth_[from] < depth_[to]) {
      std::swap(from, to);
    }
    int tmp = depth_[from] - depth_[to];
    for (int64_t i = lg_ - 1; i >= 0; --i) {
      if ((1 << i) > tmp) {
        continue;
      }
      from = height_[from][i];
      tmp -= (1 << i);
    }
    if (from != to) {
      for (int64_t i = lg_ - 1; i >= 0; --i) {
        if (height_[from][i] == height_[to][i]) {
          continue;
        }
        from = height_[from][i];
        to = height_[to][i];
      }
      if (!is_live_[height_[from][0]]) {
        return height_[from][0];
      }
      return Is_Live(height_[from][0]);
    } else {
      return to;
    }
  }

 private:
  std::vector<std::vector<T>> graph_;
  std::vector<std::vector<T>> height_;
  std::vector<T> parent_;
  std::vector<T> is_live_;
  std::vector<T> tin_;
  std::vector<T> tout_;
  std::vector<T> depth_;
  int64_t timer_ = 0;
  int64_t lg_ = 1;
  int64_t max_color_ = 1;
};
int main() {
  size_t n = 0;
  std::cin >> n;
  LCA<int32_t> lca(n);
  int32_t vertex, to;
  char c;
  int32_t curr = 1;
  for(size_t i = 0; i < n; ++i) {
    std::cin >> c;
    if (c == '+') {
      std::cin >> vertex;
      lca.AddEdge(vertex, ++curr);
    } else if (c == '?') {
      std::cin >> vertex >> to;
      std::cout << lca.Lca(vertex, to) << '\n';
    } else if (c == '-') {
      std::cin >> vertex;
      lca.Delete(vertex);
    }
  }

  return 0;
}