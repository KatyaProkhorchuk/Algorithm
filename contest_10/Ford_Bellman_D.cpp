#include <iostream>
#include <vector>
template <typename T>
class Graph {
 public:
  struct Edge {
    T from;
    T to;
    int cost;
  };
  std::vector<Edge> graph;
  Graph(size_t edge_count) {
    graph.resize(edge_count + 1);
    T first, second;
    int cost;
    for (size_t i = 0; i < edge_count; ++i) {
      std::cin >> first >> second >> cost;
      graph[i + 1] = {first, second, cost};
    }
  }
  ~Graph() = default;
};

void FordBellman(Graph<int32_t>& graph, size_t edge_count,
                 size_t vertex_count) {
  const int kInf = 30000;
  std::vector<int32_t> dist(vertex_count + 1, kInf);
  dist[1] = 0;
  bool checker = true;
  while (checker) {
    checker = false;
    for (size_t i = 1; i <= edge_count; ++i) {
      if (dist[graph.graph[i].from] < kInf &&
          dist[graph.graph[i].to] >
              dist[graph.graph[i].from] + graph.graph[i].cost) {
        dist[graph.graph[i].to] =
            dist[graph.graph[i].from] + graph.graph[i].cost;
        checker = true;
      }
    }
  }
  for (size_t i = 1; i <= vertex_count; ++i) {
    std::cout << dist[i] << ' ';
  }
}
int main() {
  size_t vertex_count = 0, edge_count = 0;
  std::cin >> vertex_count >> edge_count;
  Graph<int32_t> graph(edge_count);
  FordBellman(graph, edge_count, vertex_count);
  return 0;
}
