#include <iostream>
#include <vector>
template <typename T>
class Graph {
 private:
  std::vector<std::vector<T>> graph_;
  std::vector<std::vector<T>> graph_transpose_;
  std::vector<T> time_graph_;
  std::vector<T> time_transpose_graph_;
  std::vector<bool> used_;
  std::vector<bool> used_transpose_;
  T timer_ = 0;

 public:
  Graph(size_t vertex_count, size_t edge_count) {
    graph_.resize(vertex_count + 1);
    graph_transpose_.resize(vertex_count + 1);
    used_.resize(vertex_count + 1, false);
    used_transpose_.resize(vertex_count + 1, false);
    time_transpose_graph_.resize(vertex_count + 1);
    T first, second;
    for (size_t i = 0; i < edge_count; ++i) {
      std::cin >> first >> second;
      graph_[first].push_back(second);
      graph_transpose_[second].push_back(first);
    }
  }
  ~Graph() = default;
  bool Used(size_t i) { return used_[i]; }
  void Dfs(T vertex) {
    used_[vertex] = true;
    for (auto v : graph_[vertex]) {
      if (used_[v]) {
        continue;
      }
      Dfs(v);
    }
    time_graph_.push_back(vertex);
  }
  void DfsTranspose(T vertex, int count) {
    used_transpose_[vertex] = true;
    time_transpose_graph_[vertex] = count;
    for (auto v : graph_transpose_[vertex]) {
      if (used_transpose_[v]) {
        continue;
      }
      DfsTranspose(v, count);
    }
  }
  void RunDfsTranspose(size_t n) {
    int count = 1;
    for (size_t i = 0; i < n; ++i) {
      T vertex = time_graph_[n - i - 1];
      if (used_transpose_[vertex]) {
        continue;
      }
      DfsTranspose(vertex, count);
      ++count;
    }
    std::cout << count - 1 << std::endl;
    for (size_t i = 1; i <= n; ++i) {
      std::cout << time_transpose_graph_[i] << ' ';
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
  graph.RunDfsTranspose(vertex_count);
}
