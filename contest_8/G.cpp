#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <vector>

const int64_t kInf = std::numeric_limits<int64_t>::max();
struct Edge {
  int from;
  int to_vert;
  int64_t cap;
  int64_t flow = 0;
  int64_t vert;
  size_t id_edge;
  Edge(int from, int to_vert, int64_t cap, int64_t vert, size_t id_edge)
      : from(from), to_vert(to_vert), cap(cap), vert(vert), id_edge(id_edge) {}
  bool operator<(const Edge& other) const { return id_edge < other.id_edge; }
};
class Graph {
 public:
  Graph(int count_vertex)
      : graph_(count_vertex),
        res_(count_vertex, true),
        used_(count_vertex, false),
        size_(count_vertex) {}
  void AddEdge(int from, int to_vert, int64_t cap, size_t id_edge) {
    graph_[from].emplace_back(from, to_vert, cap, graph_[to_vert].size(),
                              id_edge);
    graph_[to_vert].emplace_back(to_vert, from, 0, graph_[from].size() - 1,
                                 kInf);
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
        if (dist_[neighbours.to_vert] == -1 &&
            neighbours.cap - neighbours.flow > 0) {
          queue.push(neighbours.to_vert);
          dist_[neighbours.to_vert] = dist_[vertex] + 1;
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
      auto& current = graph_[vertex][colors_[vertex]];
      if (dist_[current.to_vert] != dist_[vertex] + 1) {
        ++colors_[vertex];
        continue;
      }
      if (current.cap - current.flow > 0) {
        int64_t curr_flow = DFS(current.to_vert, end,
                                std::min(current.cap - current.flow, flow));
        if (curr_flow != 0) {
          current.flow += curr_flow;
          graph_[current.to_vert][current.vert].flow -= curr_flow;
          return curr_flow;
        }
      }
      ++colors_[vertex];
    }
    return 0;
  }
  int64_t Dinic(int start, int finish) {
    int64_t flow = 0;
    while (BFS(start, finish)) {
      colors_.assign(graph_.size(), 0);
      int64_t delta = 0;
      do {
        delta = DFS(start, finish, kInf);
        flow += delta;
      } while (delta > 0);
    }
    return flow;
  }
  void PrintMaxFlow() {
    std::set<Edge> res;
    for (auto item : graph_) {
      for (auto val : item) {
        res.insert(val);
      }
    }
    for (auto edge : res) {
      if (edge.id_edge != kInf && edge.flow >= 0) {
        std::cout << edge.flow << '\n';
      }
    }
  }

 private:
  std::vector<Edge> edges_;
  std::vector<std::vector<Edge>> graph_;
  std::vector<size_t> colors_;
  std::vector<int> dist_;
  std::vector<bool> res_;
  std::vector<bool> used_;
  size_t size_;
};
void InputData(Graph& graph, int m_count) {
  for (int i = 0; i < m_count; ++i) {
    int from;
    int to;
    int64_t cost;
    std::cin >> from >> to >> cost;
    graph.AddEdge(from - 1, to - 1, cost, i);
  }
}
int main() {
  int n_count;
  int m_count;
  std::cin >> n_count >> m_count;
  Graph graph(n_count);
  InputData(graph, m_count);
  int64_t max_flow = graph.Dinic(0, n_count - 1);
  std::cout << max_flow << std::endl;
  graph.PrintMaxFlow();
  return 0;
}