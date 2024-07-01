#include <iostream>
#include <vector>
class LCA {
 public:
  LCA(size_t n) : size_(n), graph_(n), t_in_(n), t_out_(n), height_(n) {
    for(;(1<<lg) <= n; ++lg) {}
    for (size_t i = 0; i < n; ++i) {
      height_[i].resize(lg+1);
    }
    dist_.assign(n, kInf_);
  }
  void AddEdge(size_t from, size_t to) {
    graph_[from].emplace_back(to);
    graph_[to].emplace_back(from);
  }
  void Dfs(size_t vertex, size_t p = 0, size_t height = 0) {
    ++time;
    t_in_[vertex] = time;
    height_[vertex][0] = p;
    dist_[vertex] = height;
    for (size_t i = 1; i <= lg; ++i) {
      height_[vertex][i] = height_[height_[vertex][i - 1]][i - 1];
    }
    for (size_t i = 0; i <= lg; ++i) {
      if (i <= size_ && dist_[i] == kInf_) {
        dist_[i] = dist_[vertex] + 1;
      }
    }
    for (size_t i = 0; i < graph_[vertex].size(); ++i) {
      if (graph_[vertex][i] == p) {
        continue;
      }
      Dfs(graph_[vertex][i], vertex, height + 1);
    }
    ++time;
    t_out_[vertex] = time;
  }
  size_t GetDist(size_t index) { return dist_[index]; }
  size_t Lca(size_t from, size_t to) {
    if (t_in_[from] <= t_in_[to] && t_out_[from] >= t_out_[to]) {
      return from;
    }
    if (t_in_[from] >= t_in_[to] && t_out_[from] <= t_out_[to]) {
      return to;
    }
    for (auto i = static_cast<int32_t>(lg); i >= 0; --i) {
      if (t_in_[height_[from][i]] > t_in_[to] || t_out_[height_[from][i]] < t_out_[to]) {
        from = height_[from][i];
      }
    }
    return height_[from][0];
  }

 private:
  size_t size_;
  const size_t kInf_ = 1e9;
  std::vector<std::vector<size_t>> graph_;
  std::vector<size_t> t_in_;
  std::vector<size_t> t_out_;
  std::vector<std::vector<size_t>> height_;
  std::vector<size_t>dist_;
  size_t time = 0;
  size_t lg = 1;
};
int main() {
  size_t n = 0;
  std::cin >> n;
  LCA lca(n);
  size_t from = 0, to = 0;
  for(size_t i = 1; i < n; ++i) {
    std::cin >> from >> to;
    lca.AddEdge(from - 1, to - 1);
  }
  lca.Dfs(0);
  size_t q = 0;
  std::cin >> q;
  for (size_t i = 0; i < q; ++i) {
    std::cin >> from >> to;
    size_t result = lca.GetDist(from - 1) + lca.GetDist(to - 1) - 2 * lca.GetDist(lca.Lca(from - 1, to - 1));
    std::cout << result << '\n';
  }
  return 0;
}