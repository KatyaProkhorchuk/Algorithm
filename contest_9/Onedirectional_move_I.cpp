#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
#include <map>
template <typename T>
class Graph {
 private:
  std::vector<std::vector<T>> graph_;
  std::vector<T> tin_;
  std::vector<T> tout_;
  std::map<std::pair<T, T>, T> edges_;
  std::set<T> bridge_;
  std::vector<uint8_t> used_;
  int timer_ = 0;
 public:
  Graph(size_t vertex_count, size_t edge_count) {
    graph_.resize(vertex_count + 1);
    tin_.resize(vertex_count + 1);
    tout_.resize(vertex_count + 1);
    used_.resize(vertex_count + 1);
    T first, second;
    for (size_t i = 0; i < edge_count; ++i) {
      std::cin >> first >> second;
      if (first <= second) {
        edges_[{first, second}] = i + 1;
      } else {
        edges_[{second, first}] = i + 1;
      }
      graph_[first].push_back(second);
      graph_[second].push_back(first);
    }
  }
  ~Graph() = default;
  bool Used(size_t i) { return used_[i]; }
  void Dfs(T vertex, T p = -1) {
    used_[vertex] = true;
    tin_[vertex] = tout_[vertex] = timer_++;
    T children = 0;
    for (auto to : graph_[vertex]) {
      if (p == to) {
        continue;
      }
      if (used_[to]) {
        tout_[vertex] = std::min(tout_[vertex], tin_[to]);
      } else {
        Dfs(to, vertex);
        tout_[vertex] = std::min(tout_[to], tout_[vertex]);
        if (tin_[vertex] < tout_[to]) {
          if (to >= vertex) {
            bridge_.insert(edges_[{vertex, to}]);
          } else {
            bridge_.insert(edges_[{to, vertex}]);
          }
        }
      }
    }
  }
  void PrintArticulationPoint() {
    std::vector<T> sorted;
    for (auto point : bridge_) {
      sorted.push_back(point);
    }
    std::sort(sorted.begin(), sorted.end());
    std::cout << sorted.size() << std::endl;
    for (size_t i = 0; i < sorted.size(); ++i) {
      std::cout << sorted[i] << std::endl;
    }
  }
};
int main() {
  size_t vertex_count = 0, edge_count = 0;
  std::cin >> vertex_count >> edge_count;
  Graph<int64_t> graph(vertex_count, edge_count);
  for (size_t i = 1; i <= vertex_count; ++i) {
    if (graph.Used(i)) {
      continue;
    }
    graph.Dfs(i);
  }
  graph.PrintArticulationPoint();
}