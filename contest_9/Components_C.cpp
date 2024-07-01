#include <iostream>
#include <vector>
void Dfs(int32_t vertex, std::vector<int>& color,
         std::vector<std::vector<int32_t>>& graph,
         std::vector<int32_t>& components) {
  color[vertex] = 1;
  components.push_back(vertex);
  for (auto to : graph[vertex]) {
    if (color[to] == 0) {
      Dfs(to, color, graph, components);
    }
  }
  color[vertex] = 2;
}
void FindComponents(std::vector<int>& color,
                    std::vector<std::vector<int32_t>>& components,
                    std::vector<std::vector<int32_t>> graph) {
  std::vector<int32_t> comp;
  for (size_t i = 1; i < color.size(); ++i) {
    if (color[i] == 0) {
      comp.clear();
      Dfs(i, color, graph, comp);
      components.push_back(comp);
    }
    if (color[i] == -1) {
      comp.clear();
      comp.push_back(i);
      components.push_back(comp);
    }
  }
  std::cout << components.size() << std::endl;
  for (size_t i = 0; i < components.size(); ++i) {
    std::cout << components[i].size() << std::endl;
    for (size_t j = 0; j < components[i].size(); ++j) {
      std::cout << components[i][j] << ' ';
    }
    std::cout << "\n";
  }
}
int main() {
  size_t vertex_count = 0;
  size_t edge_count = 0;
  std::cin >> vertex_count >> edge_count;
  std::vector<std::vector<int32_t>> graph(vertex_count + 1);
  std::vector<int> color(vertex_count + 1, -1);
  std::vector<std::vector<int32_t>> components;
  size_t from = 0;
  size_t to = 0;
  for (size_t i = 0; i < edge_count; ++i) {
    std::cin >> from >> to;
    graph[from].push_back(to);
    graph[to].push_back(from);
    color[from] = 0;
    color[to] = 0;
  }
  FindComponents(color, components, graph);
  return 0;
}