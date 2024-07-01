#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <vector>
struct Edge {
  Edge() = default;
  Edge(int32_t vertex, int32_t weight) : vertex(vertex), weight(weight) {}
  int32_t vertex = 0;
  int32_t weight = 0;
  bool operator>(const Edge& other) const { return weight > other.weight; }
};
class Graph {
 public:
  Graph(int32_t vertex_count) : graph_(vertex_count) {}
  void ResetEdge(int32_t from, int32_t to, int32_t weight) {
    graph_[from].emplace_back(to, weight);
  }
  int32_t GetSize() const { return graph_.size(); }
  std::vector<Edge> GetAdj(int32_t v) const { return graph_.at(v); }

 private:
  std::vector<std::vector<Edge>> graph_;
};
int32_t Dijkstra(Graph& gr, int32_t end, int32_t input, int32_t output,
                 int32_t limit) {
  const int32_t kInf = std::numeric_limits<int32_t>::max();
  std::vector<int32_t> dist(gr.GetSize(), kInf);
  std::priority_queue<Edge, std::vector<Edge>, std::greater<>> queue;
  std::vector<int8_t> visited(gr.GetSize());
  dist[0] = 0;
  queue.push({0, 0});
  while (!queue.empty()) {
    auto curr = queue.top().vertex;
    auto q = gr.GetAdj(curr);
    queue.pop();
    if (visited[curr] != 0) {
      continue;
    }
    visited[curr] = 1;
    for (auto& neighbours : gr.GetAdj(curr)) {
      if (dist[curr] + neighbours.weight < dist[neighbours.vertex]) {
        dist[neighbours.vertex] = dist[curr] + neighbours.weight;
        queue.push({neighbours.vertex, dist[neighbours.vertex]});
      }
    }
    if (curr > 0 && curr < limit) {
      if (output + dist[curr] < dist[curr - 1]) {
        dist[curr - 1] = output + dist[curr];
        queue.push({curr - 1, dist[curr - 1]});
      }
    }
    if (curr < limit - 1) {
      if (input + dist[curr] < dist[curr + 1]) {
        dist[curr + 1] = input + dist[curr];
        queue.push({curr + 1, dist[curr + 1]});
      }
    }
  }
  return dist[end];
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int32_t vertex_count = 0, cost_up = 0, cost_down = 0, cost_input = 0,
          cost_output = 0, count_teleport = 0;
  const int32_t kMaxSize = 1e6;
  std::cin >> vertex_count >> cost_up >> cost_down >> cost_input >>
      cost_output >> count_teleport;
  Graph gr(kMaxSize + count_teleport);
  for (int32_t i = 0; i < count_teleport; ++i) {
    int32_t size_teleport;
    std::cin >> size_teleport;
    for (int32_t j = 0; j < size_teleport; ++j) {
      int32_t vertex;
      std::cin >> vertex;
      gr.ResetEdge(vertex - 1, kMaxSize + i, cost_input);
      gr.ResetEdge(kMaxSize + i, vertex - 1, cost_output);
    }
  }
  std::cout << Dijkstra(gr, vertex_count - 1, cost_up, cost_down, kMaxSize);
  return 0;
}