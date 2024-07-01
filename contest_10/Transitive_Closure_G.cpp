#include <iostream>
#include <vector>
class Graph {
 public:
  struct Edge {
    int32_t from;
    int32_t to;
    int cost;
  };
  std::vector<std::vector<int32_t>> graph;
  Graph(int32_t vertex_count) {
    graph.resize(vertex_count, std::vector<int32_t>(vertex_count));
    int vertex;
    for (int32_t i = 0; i < vertex_count; ++i) {
      for (int32_t j = 0; j < vertex_count; ++j) {
        std::cin >> vertex;
        graph[i][j] = vertex;
      }
    }
  }
  ~Graph() = default;
};

class TransitiveClosure {
 public:
  TransitiveClosure(Graph graph, int32_t vertex_count)
      : graph_(graph), vertex_count_(vertex_count) {}
  void Find() {
    for (int32_t k = 0; k < vertex_count_; ++k) {
      for (int32_t i = 0; i < vertex_count_; ++i) {
        for (int32_t j = 0; j < vertex_count_; ++j) {
          graph_.graph[i][j] =
              graph_.graph[i][j] | (graph_.graph[i][k] & graph_.graph[k][j]);
        }
      }
    }
    for (int32_t i = 0; i < vertex_count_; ++i) {
      for (int32_t j = 0; j < vertex_count_; ++j) {
        std::cout << graph_.graph[i][j] << ' ';
      }
      std::cout << std::endl;
    }
  }

 private:
  Graph graph_;
  int32_t vertex_count_ = 0;
  std::vector<int32_t> dist_;
  std::vector<int32_t> parent_;
};

int main() {
  int32_t vertex_count = 0;
  std::cin >> vertex_count;
  Graph graph(vertex_count);
  TransitiveClosure transitive_closure(graph, vertex_count);
  transitive_closure.Find();
  return 0;
}
