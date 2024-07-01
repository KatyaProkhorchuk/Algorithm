#include <iostream>
#include <queue>
#include <vector>
class Graph {
 public:
  Graph(size_t size) : graph_(size, std::vector<int>(size, 0)) {}
  void AddEdge(int from, int to) { graph_[from][to] = 1; }
  void Bfs(size_t start, size_t finish) {
    min_capacity_.assign(graph_.size(), kInf);
    parent_.assign(graph_.size(), -1);
    std::queue<int> queue;
    parent_[start] = start;
    min_capacity_[start] = kInf;
    queue.push(start);
    while (!queue.empty() && parent_[finish] == -1) {
      int vertex_2 = queue.front();
      queue.pop();
      for (size_t vertex_1 = 0; vertex_1 < graph_.size(); ++vertex_1) {
        if (graph_[vertex_2][vertex_1] > 0 && parent_[vertex_1] == -1) {
          parent_[vertex_1] = vertex_2;
          min_capacity_[vertex_1] =
              std::min(min_capacity_[vertex_2], graph_[vertex_2][vertex_1]);
          queue.push(vertex_1);
        }
      }
    }
  }
  int EdmondsKarp(size_t start, size_t finish) {
    int max_flow = 0;
    while (true) {
      Bfs(start, finish);
      if (parent_[finish] == -1) {
        break;
      }
      max_flow += min_capacity_[finish];
      size_t vertex_1 = finish;
      while (vertex_1 != start) {
        size_t vertex_2 = parent_[vertex_1];
        graph_[vertex_1][vertex_2] += min_capacity_[finish];
        graph_[vertex_2][vertex_1] -= min_capacity_[finish];
        vertex_1 = vertex_2;
      }
    }
    return max_flow;
  }
  int Get(size_t start, size_t end) { return graph_[start][end]; }
  
 private:
  std::vector<std::vector<int>> graph_;
  const int kInf = 1000000000;
  std::vector<int> parent_;
  std::vector<int> min_capacity_;
};
void Run(size_t count_l, size_t count_r) {
  size_t size = count_l + count_r + 2;
  size_t start = 0;
  size_t finish = size - 1;
  Graph gr(size);
  for (size_t i = 1; i <= count_l; ++i) {
    int vertex = 0;
    std::cin >> vertex;
    while (vertex != 0) {
      gr.AddEdge(start, i);
      gr.AddEdge(i, count_l + vertex);
      std::cin >> vertex;
    }
  }
  for (size_t j = count_l + 1; j <= count_l + count_r; ++j) {
    gr.AddEdge(j, finish);
  }
  int max_flow = gr.EdmondsKarp(start, finish);
  std::cout << max_flow << '\n';
  for (size_t i = 1; i <= count_l; ++i) {
    for (size_t j = count_l + 1; j <= count_l + count_r; ++j) {
      if (gr.Get(i, j) == 0 && gr.Get(j, i) == 1) {
        std::cout << i << " " << j - count_l << '\n';
      }
    }
  }
}
int main() {
  size_t count_l;
  size_t count_r;
  std::cin >> count_l >> count_r;
  Run(count_l, count_r);
  return 0;
}