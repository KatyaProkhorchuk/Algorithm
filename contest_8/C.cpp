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
      : graph_(count_vertex), used_(count_vertex), size_(count_vertex) {}
  void AddEdge(int from, int to_vert, int64_t cap) {
    graph_[from].emplace_back(edges_.size());
    graph_[to_vert].emplace_back(edges_.size() + 1);
    edges_.emplace_back(from, to_vert, cap);
    edges_.emplace_back(to_vert, from, cap);
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
  void ResDfs(int vertex, std::vector<int>& achieved) {
    achieved[vertex] = 1;
    for (auto edge : graph_[vertex]) {
      if (achieved[edges_[edge].to_vert] == 0 &&
          edges_[edge].cap - edges_[edge].flow > 0) {
        ResDfs(edges_[edge].to_vert, achieved);
      }
    }
  }
  void Result(std::vector<int>& res, int64_t min_flow, size_t start,
              size_t count_edge) {
    std::vector<int> achieved;
    achieved.resize(graph_.size());
    ResDfs(start, achieved);
    for (size_t i = 0; i < count_edge * 2; i += 2) {
      if (((achieved[edges_[i].from] != 0) &&
           (achieved[edges_[i].to_vert] == 0)) ||
          ((achieved[edges_[i].from] == 0) &&
           (achieved[edges_[i].to_vert] != 0))) {
        res.emplace_back(i / 2 + 1);
      }
    }
    std::cout << res.size() << ' ' << min_flow << '\n';
    for (auto val : res) {
      std::cout << val << ' ';
    }
  }
 private:
  std::vector<Edge> edges_;
  std::vector<std::vector<int>> graph_;
  std::vector<size_t> colors_;
  std::vector<int> dist_;
  std::vector<bool> used_;
  size_t size_;
};
void ReadData(Graph& graph, size_t m_count) {
  for (size_t i = 0; i < m_count; ++i) {
    int lang_a;
    int lang_b;
    size_t cap;
    std::cin >> lang_a >> lang_b >> cap;
    graph.AddEdge(--lang_a, --lang_b, cap);
  }
}
int main() {
  size_t n_count;
  size_t m_count;
  std::cin >> n_count >> m_count;
  size_t start = 0;
  size_t end = n_count - 1;
  Graph graph(n_count);
  ReadData(graph, m_count);
  int64_t min_flow = graph.Dinic(start, end);
  std::vector<int> res;
  graph.Result(res, min_flow, start, m_count);
  return 0;
}