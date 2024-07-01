#include <iostream>
#include <queue>
#include <vector>
const int kK = 30;
class Edge {
 public:
  int32_t to;
  int32_t weight;
  Edge() {}
  Edge(int32_t edge, int32_t w) : to(edge), weight(w){};
  Edge& operator=(Edge& e) {
    to = e.to;
    weight = e.weight;
    return *this;
  }
};
void FindWay(std::vector<std::queue<int32_t>>& que,
             std::vector<std::vector<Edge>> graph,
             std::vector<int32_t>& distance, std::vector<int32_t>& color) {
  int32_t vertex = 0;
  int position = 0, count = 1;
  while (count > 0) {
    while (que[position % (kK + 1)].empty()) {
      ++position;
    }
    vertex = que[position % (kK + 1)].front();
    que[position % (kK + 1)].pop();
    --count;
    if (color[vertex] == 0) {
      color[vertex] = 1;
      for (auto g : graph[vertex]) {
        if (distance[g.to] > distance[vertex] + g.weight) {
          distance[g.to] = distance[vertex] + g.weight;
          que[distance[g.to] % (kK + 1)].push(g.to);
          ++count;
        }
      }
    }
  }
}
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t vertex_count = 0;
  size_t edge_count = 0;
  int32_t start = 0;
  int32_t end = 0;
  std::cin >> vertex_count >> edge_count;
  std::cin >> start >> end;
  std::vector<std::vector<Edge>> graph(vertex_count + 1, std::vector<Edge>());
  std::vector<std::queue<int32_t>> que(kK + 1);
  std::vector<int32_t> distance(vertex_count + 1, 999999);
  std::vector<int32_t> color(vertex_count + 1);
  int32_t from = 0, to = 0, weight = 0;
  for (size_t i = 0; i < edge_count; ++i) {
    std::cin >> from >> to >> weight;
    graph[from].push_back(Edge(to, weight));
  }
  distance[start] = 0;
  que[0].push(start);
  FindWay(que, graph, distance, color);
  std::cout << ((distance[end] == 999999) ? -1 : distance[end]);
  return 0;
}