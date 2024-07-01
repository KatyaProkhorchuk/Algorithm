#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <vector>
struct Edge {
  Edge(int64_t vertex, int64_t cost) : vertex(vertex), weight(cost) {}
  int64_t vertex;
  int64_t weight;
  int64_t virus = 0;
  int64_t way = 0;
  int64_t virus_used = 0;
};

class Graph {
 public:
  std::vector<std::vector<Edge>> graph;
  Graph(int64_t vertex_count, int64_t edge_count) : graph(vertex_count + 1) {
    int64_t from = 0, to = 0, cost = 0;
    for (int64_t i = 0; i < edge_count; ++i) {
      std::cin >> from >> to >> cost;
      graph[from].emplace_back(to, cost);
      graph[to].emplace_back(from, cost);
    }
  }
  ~Graph() = default;
};

class Dijkstra {
 public:
  Dijkstra(const Graph& graph, int64_t vertex_count, int64_t edge_count)
      : graph_(graph), vertex_count_(vertex_count), edge_count_(edge_count) {}
  void FindVirus(std::vector<int64_t>& virus, int64_t end) {
    const int64_t kInf = std::numeric_limits<int64_t>::max();
    std::vector<int64_t> dist(vertex_count_ + 1, kInf);
    std::priority_queue<std::pair<int64_t, int64_t>,
                        std::vector<std::pair<int64_t, int64_t>>,
                        std::greater<>>
                        pq;
    for (size_t i = 0; i < virus.size(); ++i) {
      dist[virus[i]] = 0;
      pq.emplace(dist[virus[i]], virus[i]);
    }

    for (; !pq.empty();) {
      auto& it = pq.top();
      int64_t vertex = it.second;
      int64_t distcurr = it.first;
      pq.pop();
      if (distcurr > dist[vertex]) {
        continue;
      }
      for (size_t i = 0; i < graph_.graph[vertex].size(); ++i) {
        auto& v = graph_.graph[vertex][i];
        if (dist[v.vertex] > dist[vertex] + v.weight) {
          dist[v.vertex] = dist[vertex] + v.weight;
          if ((v.virus != 0 && v.virus > dist[v.vertex]) || v.virus == 0) {
            v.virus = dist[v.vertex];
            v.virus_used = 1;
            if (v.vertex == end) {
              virus_dist_ = v.virus;
            }
          }
          pq.emplace(dist[v.vertex], v.vertex);
        }
      }
    }
  }

  void FindWay(int64_t start, int64_t end) {
    const int64_t kInf = std::numeric_limits<int64_t>::max();
    std::vector<int64_t> dist(vertex_count_ + 1, kInf);
    std::priority_queue<std::pair<int64_t, int64_t>,
                        std::vector<std::pair<int64_t, int64_t>>,
                        std::greater<>>
                        pq;
    dist[start] = 0;
    pq.emplace(dist[start], start);
    for (; !pq.empty();) {
      auto& it = pq.top();
      int64_t vertex = it.second;
      int64_t distcurr = it.first;
      pq.pop();
      if (distcurr > dist[vertex]) {
        continue;
      }
      for (size_t i = 0; i < graph_.graph[vertex].size(); ++i) {
        auto& v = graph_.graph[vertex][i];
        if ((v.virus > dist[vertex] + v.weight && v.virus != 0) ||
            (v.virus == 0 && v.virus_used == 0)) {
          if (dist[vertex] + v.weight < dist[v.vertex]) {
            dist[v.vertex] = dist[vertex] + v.weight;
            v.way = dist[v.vertex];
            if (v.vertex == end) {
              if (result_ == -1 || dist[v.vertex] < result_) {
                result_ = dist[v.vertex];
              }
            }
            pq.emplace(dist[v.vertex], v.vertex);
          }
        }
      }
    }
  }
  void Result() {
    if (result_ < virus_dist_) {
      std::cout << result_;
    } else {
      std::cout << -1;
    }
  }

 private:
  Graph graph_;
  int64_t vertex_count_;
  int64_t edge_count_;
  int64_t result_ = -1;
  int64_t virus_dist_ = std::numeric_limits<int64_t>::max();
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int64_t vertex_count = 0, edge_count = 0, k = 0;
  std::cin >> vertex_count >> edge_count >> k;
  std::vector<int64_t> virus;
  int64_t vertex;
  virus.reserve(k);
  for (int64_t i = 0; i < k; ++i) {
    std::cin >> vertex;
    virus.push_back(vertex);
  }

  Graph graph(vertex_count, edge_count);
  int64_t s = 0, t = 0;
  std::cin >> s >> t;
  Dijkstra dijkstra(graph, vertex_count, edge_count);
  int64_t checker = 0;
  for (int64_t i = 0; i < k; ++i) {
    if (virus[i] == t) {
      checker = 1;
      break;
    }
  }
  if (checker == 0) {
    dijkstra.FindVirus(virus, t);
    if (s == t) {
      std::cout << 0 << std::endl;
    } else {
      dijkstra.FindWay(s, t);
      dijkstra.Result();
    }
  } else {
    std::cout << -1;
  }
  return 0;
}

//6 6 1
//4
//4 1 1
//6 5 1
//2 4 1
//1 5 2
//5 6 2
//5 1 2
//4 6


//6 5 2
//6 1
//1 2 20
//2 3 2
//3 4 1
//2 5 1
//5 6 3
//4 1
//
//8 8 1
//8
//1 2 20
//2 3 2
//3 4 1
//2 5 1
//5 6 3
//2 7 18
//7 1 1
//8 1 22
//4 1

//8 8 1
//8
//1 2 20
//2 3 2
//3 4 1
//2 5 1
//5 6 3
//2 7 18
//7 1 1
//8 1 22
//4 1