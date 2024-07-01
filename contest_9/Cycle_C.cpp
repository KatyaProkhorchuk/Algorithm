#include <iostream>
#include <vector>
int32_t start = -1, end = -1;
bool Dfs(int32_t vertex, std::vector<int>& color,
         std::vector<std::vector<int32_t>>& graph, std::vector<int32_t>& path) {
  color[vertex] = 1;
  for (auto to : graph[vertex]) {
    if (color[to] == 0) {
      path[to] = vertex;
      if (Dfs(to, color, graph, path)) {
        return true;
      }
    } else if (color[to] == 1) {
      start = to;
      end = vertex;
      return true;
    }
  }
  color[vertex] = 2;
  return false;
}
int main() {
  size_t vertex_count = 0;
  size_t edge_count = 0;
  std::cin >> vertex_count >> edge_count;
  std::vector<std::vector<int32_t>> graph(vertex_count + 1);
  std::vector<int> color(vertex_count + 1);
  std::vector<int32_t> path(vertex_count + 1);
  size_t from = 0;
  size_t to = 0;
  for (size_t i = 0; i < edge_count; ++i) {
    std::cin >> from >> to;
    graph[from].push_back(to);
  }
  for (size_t i = 1; i <= vertex_count; ++i) {
    if (Dfs(i, color, graph, path)) {
      break;
    }
  }
  if (start == -1) {
    std::cout << "NO";
  } else {
    std::cout << "YES\n";
    std::vector<int32_t> result;
    for (int32_t i = end; i != start; i = path[i]) {
      result.push_back(i);
    }
    result.push_back(start);
    for (int32_t i = result.size() - 1; i >= 0; --i) {
      std::cout << result[i] << " ";
    }
  }
  return 0;
}