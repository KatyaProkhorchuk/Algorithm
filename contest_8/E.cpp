#include <iostream>
#include <limits>
#include <queue>
#include <vector>
class Graph {
 public:
  struct Edge {
    int vertex = 0;
    int flow = 0;
    int cap = 1;
    Edge(int vertex, int cap) : vertex(vertex), cap(cap){};
  };
  Graph(int n_size, int m_size, int size)
      : n_size_(n_size),
        m_size_(m_size),
        size_(size),
        graph_(size),
        end_(size - 1),
        visited_(size) {}
  int GetVertex(int vert_first, int vert_second,
                std::vector<std::vector<int8_t>>& vertex) const {
    if (vert_first >= n_size_ || vert_second >= m_size_ || vert_first < 0 ||
        vert_second < 0 || vertex[vert_first][vert_second] == 1) {
      return -1;
    }
    if (vert_first * m_size_ + 1 + vert_second >= size_) {
      return -1;
    }
    return vert_first * m_size_ + 1 + vert_second;
  }
  void AddEdge(int from, int to_vert, int cap = 1) {
    graph_[from].emplace_back(edge_.size());
    edge_.emplace_back(to_vert, cap);
    graph_[to_vert].emplace_back(edge_.size());
    edge_.emplace_back(from, 0);
  }
  void BuildingSquare(int i_index, int j_index,
                      std::vector<std::vector<int8_t>>& is_empty,
                      std::vector<std::vector<int>> data) {
    if (GetVertex(i_index, j_index, is_empty) != -1) {
      AddEdge(start_, GetVertex(i_index, j_index, is_empty),
              data[i_index][j_index]);
      if (GetVertex(i_index + 1, j_index, is_empty) != -1) {
        AddEdge(GetVertex(i_index, j_index, is_empty),
                GetVertex(i_index + 1, j_index, is_empty), 1);
      }
      if (GetVertex(i_index - 1, j_index, is_empty) != -1) {
        AddEdge(GetVertex(i_index, j_index, is_empty),
                GetVertex(i_index - 1, j_index, is_empty), 1);
      }
      if (GetVertex(i_index, j_index + 1, is_empty) != -1) {
        AddEdge(GetVertex(i_index, j_index, is_empty),
                GetVertex(i_index, j_index + 1, is_empty), 1);
      }
      if (GetVertex(i_index, j_index - 1, is_empty) != -1) {
        AddEdge(GetVertex(i_index, j_index, is_empty),
                GetVertex(i_index, j_index - 1, is_empty), 1);
      }
    }
  }
  void Building(std::vector<std::vector<int>>& data,
                std::vector<std::vector<int8_t>>& is_empty) {
    for (int i = 0; i < n_size_; ++i) {
      for (int j = 0; j < m_size_; ++j) {
        if (is_empty[i][j] != 0) {
          continue;
        }
        if ((i + j) % 2 == 0) {
          if (GetVertex(i, j, is_empty) != -1) {
            AddEdge(GetVertex(i, j, is_empty), end_, data[i][j]);
            sum_flow_ += data[i][j];
          }
        } else {
          BuildingSquare(i, j, is_empty, data);
        }
      }
    }
  }
  void Bfs(size_t start, size_t finish) {
    min_capacity_.assign(graph_.size(), kInf);
    visited_.assign(graph_.size(), -1);
    std::queue<int> queue;
    visited_[start] = start;
    min_capacity_[start] = kInf;
    queue.push(start);
    while (!queue.empty() && visited_[finish] == -1) {
      int vertex_2 = queue.front();
      queue.pop();
      for (size_t vertex_1 = 0; vertex_1 < graph_.size(); ++vertex_1) {
        if (graph_[vertex_2][vertex_1] > 0 && visited_[vertex_1] == -1) {
          visited_[vertex_1] = vertex_2;
          min_capacity_[vertex_1] =
              std::min(min_capacity_[vertex_2], graph_[vertex_2][vertex_1]);
          queue.push(vertex_1);
        }
      }
    }
  }
  int DFS(int vertex, int cap) {
    if (vertex == end_) {
      return cap;
    }
    if (vertex < 0) {
      return 0;
    }
    visited_[vertex] = time_;
    for (auto neighbours : graph_[vertex]) {
      if (neighbours >= 0 && edge_[neighbours].cap > edge_[neighbours].flow &&
          visited_[edge_[neighbours].vertex] != time_) {
        int flow =
            DFS(edge_[neighbours].vertex,
                std::min(cap, edge_[neighbours].cap - edge_[neighbours].flow));
        if (flow == 0) {
          continue;
        }
        edge_[neighbours].flow += flow;
        edge_[neighbours ^ 1].flow -= flow;
        return flow;

      }
    }
    return 0;
  }
  int8_t MaxFlow(size_t start) {
    for (; DFS(start, kInf) != 0;) {
      ++time_;
    }
    for (auto vertex : graph_[start]) {
      max_flow_ += edge_[vertex].flow;
    }
    int sum = 0;
    for (auto edge : graph_[start_]) {

      sum += edge_[edge].cap;
    }
    return static_cast<int8_t>((max_flow_ == sum) && (sum == sum_flow_));
  }
  void PrintResult() {
    if (MaxFlow(start_) != 0) {
      std::cout << "Valid" << std::endl;
    } else {
      std::cout << "Invalid" << std::endl;
    }
  }

 private:
  int n_size_;
  int m_size_;
  int size_;
  std::vector<std::vector<int>> graph_;
  std::vector<Edge> edge_;
  int start_ = 0;
  int end_;
  int sum_flow_ = 0;
  int max_flow_ = 0;
  int time_ = 1;
  std::vector<int> visited_;
  std::vector<int> min_capacity_;
  const int kInf = std::numeric_limits<int>::max();
};
int GetCapacity(char symbol) {
  if (symbol == 'H') {
    return 1;
  }
  if (symbol == 'O') {
    return 2;
  }
  if (symbol == 'N') {
    return 3;
  }
  if (symbol == 'C') {
    return 4;
  }
  return 0;
}
void InputData(Graph& graph, int n_size, int m_size) {
  int checker = 0;
  std::vector<std::vector<int>> data(n_size, std::vector<int>(m_size));
  std::vector<std::vector<int8_t>> is_empty(n_size,
                                            std::vector<int8_t>(m_size));
  for (int i = 0; i < n_size; ++i) {
    for (int j = 0; j < m_size; ++j) {
      char symbol;
      std::cin >> symbol;
      data[i][j] = GetCapacity(symbol);
      if (data[i][j] == 0) {
        is_empty[i][j] = 1;
      } else {
        ++checker;
      }
    }
  }
  if (checker != 0) {
    graph.Building(data, is_empty);
    graph.PrintResult();
  } else {
    std::cout << "Invalid" << '\n';
  }
}
int main() {
  int n_count = 0;
  int m_count = 0;
  std::cin >> n_count >> m_count;
  Graph graph(n_count, m_count, n_count * m_count + 2 + n_count);
  InputData(graph, n_count, m_count);
}