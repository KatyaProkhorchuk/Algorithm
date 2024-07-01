//
// Created by katerina on 08.04.23.
//
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
class Graph {
 public:
  Graph(int n)
      : edges_(n), parent_(n), vertex_count_(n) {};
  void AddEdge(int first, int second, int cap) {
    edges_[first].emplace_back(second, cap);
  }
  int Bfs(int start, int finish) {
    std::queue<std::pair<int, int>> queue;
    parent_[start] = start;
    queue.emplace(start, kInf);
    int flow = 0;
    while (!queue.empty()) {
      auto vert = queue.front();
      queue.pop();
      for (auto& neighbour : edges_[vert.first]) {
        int capacity = neighbour.second;
        if (capacity > 0 && parent_[neighbour.first] == -1) {
          parent_[neighbour.first] = vert.first;
          flow = std::min(vert.second, capacity);
          if (neighbour.first == finish) {
            return flow;
          }
          queue.emplace(neighbour.first, flow);
        }
      }
    }
    return 0;
  }
  void MakeResidualNetwork(int start, int finish, int cap) {
    int vertex = finish;
    while (vertex != start) {
      int prev = parent_[vertex];
      for (auto& vert : edges_[prev]) {
        if (vert.first == vertex) {
          vert.second -= cap;
          break;
        }
      }
      for (auto& vert : edges_[vertex]) {
        if (vert.first == prev) {
          vert.second += cap;
          break;
        }
      }
      vertex = prev;
    }
  }
  int EdmondsKarp(int start, int finish) {
    int max_flow = 0;
    parent_.assign(vertex_count_, -1);
    int flow = Bfs(start, finish);
    while (flow > 0) {
      max_flow += flow;
      MakeResidualNetwork(start, finish, flow);
      parent_.assign(vertex_count_, -1);
      flow = Bfs(start, finish);
    }
    return max_flow;
  }
 private:
  std::vector<std::vector<std::pair<int, int>>> edges_;
  std::vector<int> parent_;
  int vertex_count_;
  const int kInf = 1000000000;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n = 0;
  int m = 0;
  int a = 0;
  int b = 0;
  std::cin >> n >> m >> a >> b;
  char symbol;
  int vertex_count = 0;
  std::vector<std::vector<int>> matrix_symbol(n, std::vector<int>(m));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      std::cin >> symbol;
      if (symbol == '*') {
        matrix_symbol[i][j] = vertex_count;
        ++vertex_count;
      } else {
        matrix_symbol[i][j] = -1;
      }
    }
  }
  Graph graph(vertex_count + 2);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      if (matrix_symbol[i][j] == -1) {
        continue;
      }
      if ((i + j) % 2 == 0) {
        if (i != 0 && matrix_symbol[i - 1][j] != -1) {
          graph.AddEdge(matrix_symbol[i][j], matrix_symbol[i - 1][j], 1);
          graph.AddEdge(matrix_symbol[i - 1][j], matrix_symbol[i][j], 0);
        }
        if (i != n - 1 && matrix_symbol[i + 1][j] != -1) {
          graph.AddEdge(matrix_symbol[i][j], matrix_symbol[i + 1][j], 1);
          graph.AddEdge(matrix_symbol[i + 1][j], matrix_symbol[i][j], 0);
        }
        if (j != 0 && matrix_symbol[i][j - 1] != -1) {
          graph.AddEdge(matrix_symbol[i][j], matrix_symbol[i][j - 1], 1);
          graph.AddEdge(matrix_symbol[i][j - 1], matrix_symbol[i][j], 0);
        }
        if (j != m - 1 && matrix_symbol[i][j + 1] != -1) {
          graph.AddEdge(matrix_symbol[i][j], matrix_symbol[i][j + 1], 1);
          graph.AddEdge(matrix_symbol[i][j + 1], matrix_symbol[i][j], 0);
        }
        graph.AddEdge(matrix_symbol[i][j], vertex_count, 0);
        graph.AddEdge(vertex_count, matrix_symbol[i][j], 1);
      } else {
        graph.AddEdge(matrix_symbol[i][j], vertex_count + 1, 1);
        graph.AddEdge(vertex_count + 1, matrix_symbol[i][j], 0);
      }
    }
  }
  int flow = graph.EdmondsKarp(vertex_count, vertex_count + 1);
  if (a < b * 2) {
    std::cout << a * flow + b * (vertex_count - 2 * flow);
  } else {
    std::cout << b * (vertex_count);
  }
  return 0;
}