//
// Created by katerina on 15.04.23.
//
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <vector>
const int kInf = std::numeric_limits<int>::max();
struct Edge {
  int from;
  int to_vert;
  int64_t cap;
  int64_t flow = 0;
  Edge(int from, int to_vert, int cap)
      : from(from), to_vert(to_vert), cap(cap) {}
};
class Graph {
 public:
  Graph(int count_vertex)
      : graph_(count_vertex),
        res_(count_vertex, true),
        used_(count_vertex, false),
        size_(count_vertex),
        stop_(count_vertex - 1) {}
  void AddEdge(int from, int to_vert, int64_t cap, int64_t cap_back = 0) {
    graph_[from].emplace_back(edges_.size());
    graph_[to_vert].emplace_back(edges_.size() + 1);
    edges_.emplace_back(from, to_vert, cap);
    edges_.emplace_back(to_vert, from, cap_back);
  }
  bool BFS(int start, int end) {
    std::queue<int> queue;
    queue.push(start);
    dist_.assign(size_, -1);
    dist_[start] = 0;
    while (!queue.empty()) {
      int vertex = queue.front();
      queue.pop();
      for (auto& neighbours : graph_[vertex]) {
        auto edge = edges_[neighbours];
        if (dist_[edge.to_vert] == -1 && edge.cap - edge.flow > 0) {
          queue.push(edge.to_vert);
          dist_[edge.to_vert] = dist_[vertex] + 1;
        }
      }
    }
    return dist_[end] != -1;
  }

  int64_t DFS(int vertex, int end, int64_t flow) {
    if (flow == 0) {
      return 0;
    }
    if (vertex == end) {
      return flow;
    }
    while (colors_[vertex] < graph_[vertex].size()) {
      int current = graph_[vertex][colors_[vertex]];
      if (dist_[edges_[current].to_vert] != dist_[vertex] + 1) {
        ++colors_[vertex];
        continue;
      }
      int64_t curr_flow =
          DFS(edges_[current].to_vert, end,
              std::min(edges_[current].cap - edges_[current].flow, flow));
      if (curr_flow != 0) {
        edges_[current].flow += curr_flow;
        edges_[current ^ 1].flow -= curr_flow;
        return curr_flow;
      }
      ++colors_[vertex];
    }
    return 0;
  }
  int64_t Dinic(int start, int finish) {
    int flow = 0;
    while (BFS(start, finish)) {
      colors_.assign(size_, 0);
      int delta = 0;
      do {
        delta = DFS(start, finish, kInf);
        flow += delta;
      } while (delta > 0);
    }
    return flow;
  }
  void Result(int vertex) {
    used_[vertex] = true;
    for (auto edge : graph_[vertex]) {
      if (!used_[edges_[edge].to_vert] &&
          edges_[edge].cap != edges_[edge].flow) {
        res_[edges_[edge].to_vert] = false;
        Result(edges_[edge].to_vert);
      }
    }
  }
  void Build(std::vector<int>& win, std::vector<int>& games_left,
             std::vector<int>& count_game, std::vector<Edge>& edges) {
    for (int i = 1; i < static_cast<int>(count_game.size()); ++i) {
      AddEdge(start_, i, count_game[i]);
    }
    for (auto edge : edges) {
      AddEdge(edge.from, edge.to_vert, edge.cap);
    }
    for (int i = 1; i < static_cast<int>(win.size()); ++i) {
      AddEdge(i + win.size() - 1, stop_, win[0] + games_left[0] - win[i]);
    }
  }

 private:
  std::vector<Edge> edges_;
  std::vector<std::vector<int>> graph_;
  std::vector<size_t> colors_;
  std::vector<int> dist_;
  std::vector<bool> res_;
  std::vector<bool> used_;
  int size_;
  int start_ = 0;
  int stop_ = 0;
};
int Input(Graph& graph, int count_command, int* sum_game) {
  std::vector<int> win(count_command);
  int max_win = 0;
  for (int i = 0; i < count_command; ++i) {
    std::cin >> win[i];
    if (win[i] > max_win) {
      max_win = win[i];
    }
  }
  std::vector<int> games_left(count_command);
  for (int i = 0; i < count_command; ++i) {
    std::cin >> games_left[i];
  }
  std::vector<int> count_game(count_command);
  std::vector<std::vector<int>> matrix(count_command,
                                       std::vector<int>(count_command));
  std::vector<Edge> edge;
  for (int i = 0; i < count_command; ++i) {
    for (int j = 0; j < count_command; ++j) {
      std::cin >> matrix[i][j];
      if (matrix[i][j] != 0 && j > i && i != 0) {
        count_game[i] += matrix[i][j];
        *sum_game += matrix[i][j];
      }
    }
  }
  for (int i = 1; i < count_command; ++i) {
    for (int j = i; j < count_command; ++j) {
      if (matrix[i][j] != 0) {
        edge.emplace_back(i, j - 1 + count_command, matrix[i][j]);
        edge.emplace_back(i, i - 1 + count_command, matrix[i][j]);
      }
    }
    if (win[0] + games_left[0] - win[i] < 0) {
      return 0;
    }
  }

  if (games_left[0] == 0 && max_win > win[0]) {
    return 0;
  }
  graph.Build(win, games_left, count_game, edge);
  return 1;
}
void Start() {
  int count_command = 0;
  std::cin >> count_command;
  Graph graph(2 * count_command);
  int sum_game = 0;
  if (Input(graph, count_command, &sum_game) == 0) {
    std::cout << "NO\n";
  }
  int max_flow = graph.Dinic(0, 2 * count_command - 1);
  if (max_flow == sum_game) {
    std::cout << "YES";
  } else {
    std::cout << "NO";
  }
}
int main() {
  Start();
  return 0;
}