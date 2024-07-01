#include <algorithm>
#include<iostream>
#include<vector>
struct Edge {
  int32_t from;
  int32_t to;
  int32_t cost;
  Edge() = default;
  Edge(int32_t from, int32_t to, int32_t cost)
      : from(from), to(to), cost(cost) {}
  bool operator<(Edge& other) { return cost < other.cost; }
};
class DSU {
 public:
  DSU(size_t n) {
    ancestors_ = std::vector<int32_t>(n + 1, 0);
    for (size_t i = 1; i <= n; ++i) {
      ancestors_[i] = i;
    }
    ranks_ = std::vector<int32_t>(n, 0);
  }
  bool AreSame(int32_t u, int32_t v) {
    return FindSet(u) == FindSet(v);
  }
  void Unite(int32_t u, int32_t v) {
    u = FindSet(u);
    v = FindSet(v);
    if (u != v) {
      if (ranks_[u] < ranks_[v]) {
        std::swap(u, v);
      }
      ancestors_[v] = u;
      if (ranks_[v] == ranks_[u]) {
        ++ranks_[u];
      }
    }
  }
  int32_t FindSet(int32_t elem) {
    if (elem == ancestors_[elem]) {
      return elem;
    }
    return ancestors_[elem] = FindSet(ancestors_[elem]);
  }
 private:
  std::vector<int32_t> ancestors_;
  std::vector<int32_t> ranks_;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int32_t n = 0;
  std::cin >> n;
  int32_t cost = 0;
  std::vector<Edge> graph(n + 2);
  for (int32_t i = 1; i <= n; ++i) {
    for (int32_t j = 1; j <= n; ++j) {
      std::cin >> cost;
      graph.emplace_back(i, j, cost);
    }
  }
  for (int32_t i = 1; i <= n; ++i) {
    std::cin >> cost;
    graph.emplace_back(n + 1, i, cost);
  }
  sort(graph.begin(), graph.end());
  DSU dsu(n + 2);
  int32_t answer = 0;
  for (int32_t i = 0; i < graph.size(); ++i) {
    if (dsu.FindSet(graph[i].from) != dsu.FindSet(graph[i].to)) {
      answer += graph[i].cost;
      dsu.Unite(graph[i].from, graph[i].to);
    }
  }
  std::cout << answer << std::endl;
  return 0;
}