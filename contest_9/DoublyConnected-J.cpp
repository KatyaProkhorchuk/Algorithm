#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>
template <typename T>
class Graph {
  std::vector<std::unordered_set<T>> graph_;
  std::vector<T> leafs_;
  std::vector<T> tin_;
  std::vector<T> tout_;
  std::vector<int8_t> used_;
  int timer_ = 0;

 public:
  Graph(size_t vertex_count, size_t edge_count)
      : graph_(vertex_count),
        leafs_(vertex_count),
        tin_(vertex_count),
        tout_(vertex_count),
        used_(vertex_count) {
    T first, second;
    for (size_t i = 0; i < edge_count; ++i) {
      std::cin >> first >> second;
      --first;
      --second;
      graph_[first].insert(second);
      graph_[second].insert(first);
    }
  }
  ~Graph() = default;
  void Dfs(T vertex, T p = -1) {
    used_[vertex] = true;
    tin_[vertex] = timer_;
    tout_[vertex] = timer_;
    ++timer_;
    for (auto to : graph_[vertex]) {
      if (p != to) {
        if (used_[to]) {
          tout_[vertex] = std::min(tout_[vertex], tin_[to]);
        } else {
          Dfs(to, vertex);
          tout_[vertex] = std::min(tout_[to], tout_[vertex]);
          leafs_[vertex] +=
              ((tin_[to] == tout_[to]) ? (1 > leafs_[to] ? 1 : leafs_[to])
                                       : leafs_[to]);
        }
      }
    }
  }
  T BCC() {
    Dfs(0);
    if (graph_[0].size() != 1) {
      return ++leafs_[0] / 2;
    }
    return (leafs_[0] + 2) / 2;
  }
};
int main() {
  size_t vertex_count = 0, edge_count = 0;
  std::cin >> vertex_count >> edge_count;
  Graph<int64_t> graph(vertex_count, edge_count);
  std::cout << graph.BCC();
}
