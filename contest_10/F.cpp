#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
struct Edge {
  Edge() = default;
  Edge(int32_t vertex, int32_t weight) : vertex(vertex), weight(weight) {}
  int32_t vertex = 0;
  int32_t weight = 0;
};
class Graph {
 public:
  Graph(int32_t vertex_count, int32_t count_lecture) : city_(count_lecture) {
    adj_.resize(vertex_count, std::vector<int32_t>(vertex_count, INT32_MAX));
    path_.resize(vertex_count, std::vector<int32_t>(vertex_count, INT32_MAX));
    for (int32_t i = 0; i < vertex_count; ++i) {
      adj_[i][i] = 0;
    }
  }
  void AddEdge(int32_t from, int32_t to, int32_t weight, int32_t index) {
    id_races_[std::make_pair(from, to)] = index;
    adj_[from][to] = std::min(adj_[from][to], -weight);
    path_[from][to] = to;
  }
  void AddCity(int32_t index, int32_t city) { city_[index] = city; }
  int32_t GetAdj(int32_t i, int32_t j) const { return adj_[i][j]; }
  int32_t GetPath(int32_t i, int32_t j) const { return path_[i][j]; }
  int32_t GetIdRaces(std::pair<int32_t, int32_t> i) { return id_races_[i]; }
  void SetPath(int32_t i, int32_t j, const int32_t& value) {
    path_[i][j] = value;
  }
  void SetAdj(int32_t i, int32_t j, const int32_t& value) {
    adj_[i][j] = value;
  }
  int32_t GetCity(int32_t i) { return city_[i]; }

 private:
  std::map<std::pair<int32_t, int32_t>, int32_t> id_races_;
  std::vector<std::vector<int32_t>> adj_;
  std::vector<std::vector<int32_t>> path_;
  std::vector<int32_t> city_;
};
void Floyd(Graph& graph, int32_t vertex_count) {
  for (int32_t i = 0; i < vertex_count; ++i) {
    graph.SetPath(i, i, i);
  }
  for (int32_t k = 0; k < vertex_count; ++k) {
    for (int32_t i = 0; i < vertex_count; ++i) {
      for (int32_t j = 0; j < vertex_count; ++j) {
        if (graph.GetAdj(i, k) < INT32_MAX && graph.GetAdj(k, j) < INT32_MAX &&
            graph.GetAdj(i, j) > graph.GetAdj(i, k) + graph.GetAdj(k, j)) {
          graph.SetPath(i, j, graph.GetPath(i, k));
          graph.SetAdj(i, j, graph.GetAdj(i, k) + graph.GetAdj(k, j));
        }
      }
    }
  }
}
void GetResult(Graph& gr, int32_t vertex_count, int32_t count_lecture) {
  std::vector<int32_t> result;
  for (int32_t i = 0; i < count_lecture - 1; ++i) {
    for (int32_t j = 0; j < vertex_count; ++j) {
      if (gr.GetAdj(j, j) < 0) {
        if (gr.GetAdj(gr.GetCity(i), j) < INT32_MAX &&
            gr.GetAdj(j, gr.GetCity(i)) < INT32_MAX) {
          std::cout << "infinitely kind" << std::endl;
          return;
        }
      }
    }
    int32_t curr_city = gr.GetCity(i);
    while (curr_city != gr.GetCity(i + 1)) {
      result.emplace_back(curr_city);
      curr_city = gr.GetPath(curr_city, gr.GetCity(i + 1));
    }
  }
  result.emplace_back(gr.GetCity(count_lecture - 1));
  std::cout << result.size() - 1 << std::endl;
  for (size_t i = 0; i < result.size() - 1; ++i) {
    std::cout << gr.GetIdRaces({result[i], result[i + 1]}) << ' ';
  }
}
int main() {
  int32_t vertex_count = 0, edge_count = 0, count_lecture = 0;
  std::cin >> vertex_count >> edge_count >> count_lecture;
  Graph gr(vertex_count, count_lecture);
  int32_t from = 0, to = 0, weight = 0;
  for (int32_t i = 0; i < edge_count; ++i) {
    std::cin >> from >> to >> weight;
    gr.AddEdge(from - 1, to - 1, weight, i + 1);
  }
  int32_t city;
  for (int32_t i = 0; i < count_lecture; ++i) {
    std::cin >> city;
    gr.AddCity(i, city - 1);
  }
  Floyd(gr, vertex_count);
  GetResult(gr, vertex_count, count_lecture);
  return 0;
}