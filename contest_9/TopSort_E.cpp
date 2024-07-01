#include <iostream>
#include <vector>
std::vector<int32_t> result;
void Dfs(int32_t vertex, std::vector<int>& color,
         std::vector<std::vector<int32_t>>& graph) {
  color[vertex] = 1;
  for (auto to : graph[vertex]) {
    if (color[to] == 0) {
      Dfs(to, color, graph);
    } else if (color[to] == 1) {
      std::cout << "-1";
      exit(0);
    }
  }
  color[vertex] = 2;
  result.push_back(vertex);
}
int main() {
  size_t vertex_count = 0;
  size_t edge_count = 0;
  std::cin >> vertex_count >> edge_count;
  std::vector<std::vector<int32_t>> graph(vertex_count + 1);
  std::vector<int> color(vertex_count + 1);
  size_t from = 0;
  size_t to = 0;
  for (size_t i = 0; i < edge_count; ++i) {
    std::cin >> from >> to;
    graph[from].push_back(to);
  }
  for (size_t i = 1; i <= vertex_count; ++i) {
    if (color[i] == 0) {
      Dfs(i, color, graph);
    }
  }
  for (int32_t i = result.size() - 1; i >= 0; --i) {
    std::cout << result[i] << ' ';
  }
  return 0;
}
