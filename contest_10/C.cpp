#include <iostream>
#include <list>
#include <set>
#include <vector>
struct Edge {
  Edge() = default;
  Edge(size_t vertex_from, size_t vertex, size_t price, size_t time)
      : vertex_from(vertex_from), vertex(vertex), price(price), time(time) {}
  Edge(size_t vertex, size_t price, size_t time)
      : vertex(vertex), price(price), time(time) {}
  size_t vertex_from = 0;
  size_t vertex = 0;
  size_t price = 0;
  size_t time = 0;
};
bool operator<(const Edge& curr, const Edge& other) {
  if (curr.price < other.price) {
    return true;
  }
  if (curr.price == other.price && curr.time < other.time) {
    return true;
  }
  return (curr.vertex < other.vertex && curr.time == other.time &&
          curr.price == other.price);
}
template <typename T>
using Pair = std::pair<std::pair<T, T>, Edge>;
class Graph {
 public:
  Graph(size_t vertex_count) : graph_(vertex_count), path_(vertex_count) {
    dist.resize(vertex_count, std::list<Pair<size_t>>());
  }
  void AddEdge(size_t from, size_t to, size_t price, size_t weight) {
    graph_[from].emplace_back(to, price, weight);
    graph_[to].emplace_back(from, price, weight);
  }
  void AddPath(size_t from, size_t to) { path_[from] = to; }
  size_t GetSize() const { return graph_.size(); }
  std::vector<Edge> GetAdj(size_t v) const { return graph_.at(v); }
  size_t GetPath(size_t v) const { return path_.at(v); }
  std::vector<std::list<Pair<size_t>>> dist;

 private:
  std::vector<std::vector<Edge>> graph_;
  std::vector<size_t> path_;
};

void DijkstraTime(Graph& gr, size_t start, size_t time) {
  std::set<Edge> edge;
  edge.insert({start, 0, 0});
  gr.dist[start].push_back({{0, 0}, {start, start, 0, 0}});
  while (!edge.empty()) {
    auto curr = edge.begin();
    Edge current = {curr->vertex, curr->price, curr->time};
    edge.erase(edge.begin());
    for (const auto& neighbours : gr.GetAdj(current.vertex)) {
      Edge tmp = {neighbours.vertex, current.price + neighbours.price,
                  current.time + neighbours.time};
      if (tmp.time > time) {
        continue;
      }
      int8_t check_add = 1;
      auto dist_to = gr.dist[tmp.vertex].begin();
      for (; dist_to != gr.dist[tmp.vertex].end();) {
        size_t price_old = dist_to->first.first;
        size_t old_time = dist_to->first.second;
        auto new_it = std::next(dist_to);
        // вдруг такое есть
        if (price_old == tmp.price && tmp.time == old_time) {
          check_add = 0;
          break;
        }
        if (price_old >= tmp.price && old_time >= tmp.time) {
          gr.dist[tmp.vertex].erase(dist_to);
          edge.erase({tmp.vertex, price_old, old_time});
        }
        if ((old_time <= tmp.time && price_old <= tmp.price) ||
            (tmp.time > time)) {
          check_add = 0;
          break;
        }
        dist_to = new_it;
      }
      if (check_add == 1) {
        edge.insert({tmp.vertex, tmp.price, tmp.time});
        gr.dist[tmp.vertex].push_back(
            {{tmp.price, tmp.time},
             {tmp.vertex, current.vertex, neighbours.price, neighbours.time}});
      }
    }
  }
}
void ResetPath(Graph& gr, size_t time) {
  Pair<size_t> begin;
  std::vector<size_t> path;
  size_t min_path = SIZE_MAX;
  if (gr.dist[gr.GetSize() - 1].empty()) {
    std::cout << -1 << std::endl;
    return;
  }
  for (auto& vertex : gr.dist[gr.GetSize() - 1]) {
    // найдем begin
    if (vertex.first.first < min_path && vertex.first.second <= time) {
      begin = vertex;
      min_path = vertex.first.first;
    }
  }
  path.emplace_back(begin.second.vertex_from);
  while (begin.second.vertex != 0) {
    for (auto& vertex : gr.dist[begin.second.vertex]) {
      if (vertex.first.first + begin.second.price == begin.first.first &&
          vertex.first.second + begin.second.time == begin.first.second) {
        path.emplace_back(vertex.second.vertex_from);
        begin = vertex;
      }
    }
  }
  path.emplace_back(0);
  std::cout << min_path << std::endl;
  std::cout << path.size() << std::endl;
  for (int32_t i = static_cast<int32_t>(path.size()) - 1; i >= 0; --i) {
    std::cout << path[i] + 1 << ' ';
  }
}
int main() {
  size_t vertex_count = 0, edge_count = 0, time = 0;
  std::cin >> vertex_count >> edge_count >> time;
  Graph gr(vertex_count);
  size_t from = 0, to = 0, price = 0, t = 0;
  for (size_t i = 0; i < edge_count; ++i) {
    std::cin >> from >> to >> price >> t;
    if (from != to) {
      gr.AddEdge(from - 1, to - 1, price, t);
    }
  }
  DijkstraTime(gr, 0, time);
  ResetPath(gr, time);
  return 0;
}