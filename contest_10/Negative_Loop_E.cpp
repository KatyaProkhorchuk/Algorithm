#include <iostream>
#include <vector>
class Graph {
 public:
  struct Edge {
    int32_t from;
    int32_t to;
    int cost;
  };
  std::vector<Edge> graph;
  Graph(int32_t vertex_count) {
    int cost;
    for (int32_t i = 0; i < vertex_count; ++i) {
      for (int32_t j = 0; j < vertex_count; ++j) {
        std::cin >> cost;
        if (cost != 0 && cost != 100000) {
          graph.push_back({j, i, cost});
        }
      }
    }
  }
  ~Graph() = default;
};
class NegativeLoop {
 public:
  NegativeLoop(Graph graph, int32_t vertex_count)
      : graph_(graph), vertex_count_(vertex_count) {}
  void Find() {
    dist_.resize(vertex_count_);
    parent_.resize(vertex_count_, -1);
    int32_t vertex = -1;
    for (int32_t i = 0; i < vertex_count_; ++i) {
      vertex = -1;
      for (size_t j = 0; j < graph_.graph.size(); ++j) {
        if (dist_[graph_.graph[j].from] < 100000 &&
            dist_[graph_.graph[j].to] >
                dist_[graph_.graph[j].from] + graph_.graph[j].cost) {
          dist_[graph_.graph[j].to] = std::max(
              -100000000, dist_[graph_.graph[j].from] + graph_.graph[j].cost);
          parent_[graph_.graph[j].to] = graph_.graph[j].from;
          vertex = graph_.graph[j].to;
        }
      }
    }
    if (vertex == -1) {
      std::cout << "NO" << std::endl;
    } else {
      int32_t prev = vertex;
      std::vector<int32_t> path;
      for (int32_t i = 0; i < vertex_count_; ++i) {
        prev = parent_[prev];
      }
      int32_t current = prev;
      while (true) {
        path.push_back(current + 1);
        if (current == prev && path.size() > 1) {
          break;
        }
        current = parent_[current];
      }
      std::cout << "YES" << std::endl;
      std::cout << path.size() << std::endl;
      for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i] << ' ';
      }
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
  NegativeLoop negative_loop(graph, vertex_count);
  negative_loop.Find();
  return 0;
}
