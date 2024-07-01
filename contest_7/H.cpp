#include <iostream>
#include <vector>
template <typename T>
class LCA {
 public:
  LCA(size_t n)
      : graph_(n),
        colors_(n),
        used_(n),
        tin_(n),
        tout_(n),
        depth_(n + 1) {
    lg_ = std::__lg(n) + 1;
    height_.resize(n + 1, std::vector<T>(lg_ + 1, 1));
  }
  void AddEdge(T v, T u) {
    graph_[v].push_back(u);
    graph_[u].push_back(v);
  }
  void DepthFirstSearch(T vertex, int64_t p = -1) {
    used_[vertex] = true;
    tin_[vertex] = timer_;
    tout_[vertex] = timer_;
    ++timer_;
    for (auto& neighbour : graph_[vertex]) {
      if (!(p == neighbour)) {
        if (used_[neighbour]) {
          tout_[vertex] = std::min(tin_[neighbour], tout_[vertex]);
        } else {
          DepthFirstSearch(neighbour, vertex);
          tout_[vertex] = std::min(tout_[neighbour], tout_[vertex]);
        }
      }
    }
  }
  void Paint(T vertex, T color) {
    colors_[vertex] = color;
    for (auto& neighbours : graph_[vertex]) {
      if (colors_[neighbours] == 0) {
        if (tout_[neighbours] <= tin_[vertex]) {
          Paint(neighbours, color);
        } else {
          ++max_color_;
          depth_[max_color_] = depth_[color] + 1;
          height_[max_color_][0] = color;
          for (int64_t i = 1; i <= lg_; ++i) {
            height_[max_color_][i] = height_[height_[max_color_][i - 1]][i - 1];
          }
          Paint(neighbours, max_color_);
        }
      }
    }
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
      return height_[from][0];
    } else {
      return to;
    }
  }
  T Answer(T u, T v) {
    return depth_[Lca(colors_[u], colors_[v])];
  }
 private:
  std::vector<std::vector<T>> graph_;
  std::vector<std::vector<T>> height_;
  std::vector<int8_t> used_;
  std::vector<T> colors_;
  std::vector<T> tin_;
  std::vector<T> tout_;
  std::vector<T> depth_;
  int64_t timer_ = 0;
  int64_t lg_ = 1;
  int64_t max_color_ = 1;
};
int main() {
  size_t n = 0, m = 0;
  std::cin >> n >> m;
  int64_t end;
  std::cin >> end;
  LCA<int64_t> lca(n);
  int64_t from = 0, to = 0;
  for (size_t i = 0; i < m; ++i) {
    std::cin >> from >> to;
    lca.AddEdge(from - 1, to - 1);
  }
  lca.DepthFirstSearch(end - 1);
  lca.Paint(end - 1, 1);
  size_t count_ = 0;
  std::cin >> count_;
  for (size_t i = 0; i < count_; ++i) {
    std::cin >> from >> to;
    std::cout << lca.Answer(from - 1, to - 1) << '\n';
  }
  return 0;
}