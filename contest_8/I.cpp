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
        used_(count_vertex),
        size_(count_vertex) {}
  void AddEdge(int from, int to_vert, int64_t cap, int64_t cap_back = 0) {
    graph_[from].emplace_back(edges_.size());
    graph_[to_vert].emplace_back(edges_.size() + 1);
    edges_.emplace_back(from, to_vert, cap);
    edges_.emplace_back(to_vert, from, cap_back);
  }
  void Construct(std::vector<int>& subject, int start, int end) {
    size_t size = 0;
    int vertex;
    for (size_t i = 0; i < size_ - 2; ++i) {
      if (subject[i] <= 0) {
        AddEdge(i + 1, end, subject[i] * (-1));
      } else {
        AddEdge(start, i + 1, subject[i]);
      }
    }
    for (size_t i = 0; i < size_ - 2; ++i) {
      std::cin >> size;
      for (size_t j = 0; j < size; ++j) {
        std::cin >> vertex;
        AddEdge(i + 1, vertex, kInf);
      }
    }
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
  void PrintRes(std::string first, std::string second) {
    for (size_t i = 0; i < first.size(); ++i) {
      if (first[i] == '?') {
        std::cout << res_[2 * i + 1];
      } else {
        std::cout << first[i];
      }
    }
    std::cout << '\n';
    for (size_t i = 0; i < second.size(); ++i) {
      if (second[i] == '?') {
        std::cout << res_[2 * i];
      } else {
        std::cout << second[i];
      }
    }
  }
 private:
  std::vector<Edge> edges_;
  std::vector<std::vector<int>> graph_;
  std::vector<size_t> colors_;
  std::vector<int> dist_;
  std::vector<bool> res_;
  std::vector<bool> used_;
  size_t size_;
};
int main() {
  int count_subject = 0;
  std::cin >> count_subject;
  std::vector<int> subject(count_subject);
  int64_t sum = 0;
  for (int i = 0; i < count_subject; ++i) {
    std::cin >> subject[i];
    if (subject[i] > 0) {
      sum += subject[i];
    }
  }
  Graph graph(count_subject + 2);
  graph.Construct(subject, 0, count_subject + 1);
  int64_t max_flow = graph.Dinic(0, count_subject + 1);
  std::cout << sum - max_flow << '\n';
  return 0;
}