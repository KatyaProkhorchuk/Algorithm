#include <functional>
#include <iostream>
#include <set>
#include <vector>
#include <queue>
namespace graph {
template <typename T>
struct DefaultEdge {
  DefaultEdge(const T& to_vert, const T& weight)
      : to_vert(to_vert), weight(weight) {}
  DefaultEdge(const T& from, const T& to_vert, const T& weight)
      : from(from), to_vert(to_vert), weight(weight) {}
  T from;
  T to_vert;
  T weight;
  bool operator>(const DefaultEdge& other) const {
    return weight > other.weight;
  }
  bool operator<(const DefaultEdge& other) const {
    return weight > other.weight;
  }
  const T& Begin() const { return from; }
  const T& End() const { return to_vert; }
  const T& Weight() const { return weight; }
};
template <class Vertex = size_t, class Edge = DefaultEdge<Vertex>>
class Iterator {
 public:
  using FilterFunction = std::function<bool(const Edge&)>;
  // NOLINTNEXTLINE
  using iterator = Iterator;
  Iterator() = default;
  Iterator(Vertex vertex, typename std::vector<Edge>::iterator begin,
           typename std::vector<Edge>::iterator end,
           const FilterFunction& filter)
      : current_edge_({vertex, vertex}),
        vertex_(vertex),
        begin_(begin),
        end_(end),
        filter_(filter) {
    if (begin_ != end_) {
      current_edge_ = {vertex_, begin_->to_vert};
      while (!filter(current_edge_) && begin_ != end_) {
        ++begin_;
        current_edge_ = {vertex_, begin_->to_vert};
      }
    }
  }
  Iterator& operator++() {
    if (begin_ == end_) {
      return *this;
    }
    ++begin_;
    if (begin_ == end_) {
      return *this;
    }
    current_edge_ = {vertex_, begin_->to_vert};
    while (!filter_(current_edge_) && begin_ != end_) {
      ++begin_;
      if (begin_ == end_) {
        return *this;
      }
      current_edge_ = {vertex_, begin_->to_vert};
    }
    return *this;
  }
  bool operator==(const Iterator& other) const {
    return begin_ == other.begin_;
  }
  bool operator!=(const Iterator& other) const {
    return !(begin_ == other.begin_);
  }
  // NOLINTNEXTLINE
  Iterator begin() { return *this; }
  // NOLINTNEXTLINE
  Iterator end() { return Iterator(vertex_, end_, end_, filter_); }
  const Edge& operator*() { return current_edge_; }

 private:
  Edge current_edge_;
  Vertex vertex_;
  typename std::vector<Edge>::iterator begin_;
  typename std::vector<Edge>::iterator end_;
  const FilterFunction& filter_;
};
template <typename Vertex = size_t, typename Weight = size_t,
    typename Edge = DefaultEdge<Vertex>>
class AbstractGraph {
 public:
  using VertexType = Vertex;
  using EdgeType = Edge;
  using WeightType = Weight;
  using FilterFunction = std::function<bool(const Edge&)>;
  explicit AbstractGraph(size_t vertex_count, size_t edges_count)
      : vertex_count_(vertex_count), edges_count_(edges_count) {}
  size_t GetVertexCount() const { return vertex_count_; }
  size_t GetEdgesCount() const { return edges_count_; }
  virtual std::vector<Edge> GetNeighbours(const Vertex& vertex) = 0;
  virtual typename std::vector<Edge>::iterator Neighbours(Vertex vertex) = 0;
  virtual typename std::vector<Edge>::iterator NeighboursEnd(Vertex vertex) = 0;
  virtual Iterator<Vertex, Edge> NeighboursIt(Vertex vertex,
                                              const FilterFunction& filter) = 0;

 private:
  size_t vertex_count_ = 0;
  size_t edges_count_ = 0;
};

template <typename Vertex = size_t, typename Weight = size_t,
    typename Edge = DefaultEdge<Vertex>>
class ListGraph : public AbstractGraph<Vertex, Edge> {
 private:
  std::unordered_map<Vertex, std::vector<Edge>> list_;
  std::vector<Edge> edge_;
 public:
  using FilterFunction = std::function<bool(const Edge&)>;
  ListGraph(size_t vertex_count, const std::vector<Edge>& edges)
      : AbstractGraph<Vertex, Edge>(vertex_count, edges.size()) {
    for (const auto& edge : edges) {
      edge_.emplace_back(edge);
      list_[edge.Begin()].push_back({edge.Begin(), edge.End(), edge.Weight()});
      list_[edge.End()].push_back({edge.End(), edge.Begin(), edge.Weight()});
    }
  }
  typename std::vector<Edge> GetEdge() const {
    return edge_;
  }

  typename std::vector<Edge>::iterator Neighbours(Vertex vertex) override {
    return list_[vertex].begin();
  }
  typename std::vector<Edge>::iterator NeighboursEnd(Vertex vertex) override {
    return list_[vertex].end();
  }
  Iterator<Vertex, Edge> NeighboursIt(Vertex vertex,
                                      const FilterFunction& filter) override {
    return Iterator(vertex, Neighbours(vertex), NeighboursEnd(vertex), filter);
  }
  std::vector<Edge> GetNeighbours(const Vertex& vertex) final {
    return list_[vertex];
  }
  std::vector<Edge> GetAdj(const Vertex& vertex) const {
    return list_[vertex];
  }
};
} //namespace graph
namespace MSTTree {
template <typename Vertex = size_t, typename Weight = size_t,
    typename Edge = graph::DefaultEdge<Vertex>>
class MSTTree : public graph::ListGraph<Vertex, Weight, Edge> {
 public:
  MSTTree(const size_t& size, const std::vector<Edge>& edge) : graph::ListGraph<
      Vertex,
      Weight,
      Edge>(size, edge) {}
  size_t GetResult(Vertex from, Vertex to, std::vector<int8_t>& used, std::vector<size_t>& dist, int32_t prev = -1) {
    used[from] = true;
    auto neighbours = graph::ListGraph<Vertex, Weight, Edge>::GetNeighbours(from);
    for (auto& neigbour : neighbours) {
      if(!used[neigbour.to_vert]) {
        dist[neigbour.to_vert] = std::min(neigbour.weight, dist[neigbour.from]);
        GetResult(neigbour.to_vert, to, used, dist, from);
      }
    }
    return dist[to];
  }
};
} // namespace MSTTree

namespace DSU {
class DSU {
 public:
  DSU(size_t n) {
    ancestors_ = std::vector<size_t>(n + 1, 0);
    for (size_t i = 1; i <= n; ++i) {
      ancestors_[i] = i;
    }
    ranks_ = std::vector<size_t>(n, 0);
  }
  bool AreSame(size_t u, size_t v) {
    return FindSet(u) == FindSet(v);
  }
  void Unite(size_t u, size_t v) {
    u = FindSet(u);
    v = FindSet(v);
    if (u != v) {
      if (ranks_[u] < ranks_[v]) {
        std::swap(u, v);
      }
      ancestors_[v] = u;
      if (ranks_[v] == ranks_[u]) {
        ++ranks_[u];
      }
    }
  }
  size_t FindSet(size_t elem) {
    if (elem == ancestors_[elem]) {
      return elem;
    }
    return ancestors_[elem] = FindSet(ancestors_[elem]);
  }
 private:
  std::vector<size_t> ancestors_;
  std::vector<size_t> ranks_;
};
} // namespace DSU

template <typename T>
MSTTree::MSTTree<size_t> MSTKruskal(const graph::ListGraph<T,
                                                           T,
                                                           graph::DefaultEdge<T>>& graph) {
  std::vector<graph::DefaultEdge<T>> result;

  DSU::DSU dsu(graph.GetVertexCount());
  std::vector<graph::DefaultEdge<T>> edges = graph.GetEdge();
  std::sort(edges.begin(), edges.end());
  for (auto& edge : edges) {
    if (dsu.FindSet(edge.from) != dsu.FindSet(edge.to_vert)) {
      result.emplace_back(edge);
      dsu.Unite(edge.from, edge.to_vert);
    }
  }

  MSTTree::MSTTree<size_t, size_t, graph::DefaultEdge<size_t>>
      mst(result.size(), result);
  return mst;
}
template <typename T>
MSTTree::MSTTree<size_t> Prima(graph::ListGraph<T,
                                                T,
                                                graph::DefaultEdge<T>>& graph,
                               size_t vertex) {
  std::vector<graph::DefaultEdge<T>> result;
  std::vector<size_t> min_e(graph.GetVertexCount() + 1, -1);
  std::vector<size_t> sel_e(graph.GetVertexCount() + 1, -1);
  std::vector<int8_t> used(graph.GetVertexCount() + 1);
  min_e[0] = 0;
  T weight = 0;
  std::set<std::pair<size_t, size_t>> q;
  q.insert(std::make_pair(0, vertex));
  for (int i = 0; i < graph.GetVertexCount(); ++i) {
    if (q.empty()) {
      break;
    }
    size_t v = q.begin()->second;
    used[v] = 1;
    q.erase(q.begin());
    for (auto edge : graph.GetNeighbours(v)) {
      size_t to = edge.to_vert,
          cost = edge.weight;
      if (cost > min_e[to] && used[to] == 0) {
        q.erase(std::make_pair(min_e[to], to));
        min_e[to] = cost;
        sel_e[to] = v;
        result.emplace_back(v, to, cost);
        q.insert(std::make_pair(min_e[to], to));
      }
    }
  }
  MSTTree::MSTTree<size_t, size_t, graph::DefaultEdge<size_t>>
      mst(result.size(), result);
  return mst;
}
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  size_t vertex_count = 0;
  size_t edge_count = 0;
  size_t count_question = 0;
  size_t from = 0;
  size_t vert_to = 0;
  size_t weight = 0;
  std::cin >> vertex_count >> edge_count >> count_question;
  std::vector<graph::DefaultEdge<size_t>> edges;
  for (size_t i = 0; i < edge_count; ++i) {
    std::cin >> from >> vert_to >> weight;
    edges.emplace_back(from, vert_to, weight);
  }
  graph::ListGraph<size_t, size_t, graph::DefaultEdge<size_t>>
      list_graph(vertex_count, edges);
  auto res = MSTKruskal(list_graph);
  for (size_t i = 0; i < count_question; ++i) {
    std::cin >> from >> vert_to;
    std::vector<int8_t> used(vertex_count + 1);
    std::vector<size_t> dist(vertex_count + 1, -1);
    std::cout << res.GetResult(from, vert_to, used,dist) << '\n';
  }
  return 0;
}