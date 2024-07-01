#include <functional>
#include <iostream>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>

namespace graph {
template <typename T>
struct DefaultEdge : std::pair<T, T> {
  DefaultEdge(const T& first, const T& second)
      : std::pair<T, T>(first, second) {}
  using BaseClass = std::pair<T, T>;
  const T& Begin() const { return BaseClass::first; }
  const T& End() const { return BaseClass::second; }
};

template <class Vertex = int32_t, class Edge = DefaultEdge<Vertex>>
class Iterator {
 public:
  using FilterFunction = std::function<bool(const Edge&)>;
  // NOLINTNEXTLINE
  using iterator = Iterator;
  Iterator() = default;
  Iterator(Vertex vertex, typename std::vector<Vertex>::iterator begin,
           typename std::vector<Vertex>::iterator end,
           const FilterFunction& filter)
      : current_edge_({vertex, vertex}),
        vertex_(vertex),
        begin_(begin),
        end_(end),
        filter_(filter) {
    if (begin_ != end_) {
      current_edge_ = {vertex_, *begin_};
      while (!filter(current_edge_) && begin_ != end_) {
        ++begin_;
        current_edge_ = {vertex_, *begin_};
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
    current_edge_ = {vertex_, *begin_};
    while (!filter_(current_edge_) && begin_ != end_) {
      ++begin_;
      if (begin_ == end_) {
        return *this;
      }
      current_edge_ = {vertex_, *begin_};
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
  Iterator Begin() { return *this; }
  // NOLINTNEXTLINE
  Iterator end() { return Iterator(vertex_, end_, end_, filter_); }
  const Edge& operator*() { return current_edge_; }

 private:
  Edge current_edge_;
  Vertex vertex_;
  typename std::vector<Vertex>::iterator begin_;
  typename std::vector<Vertex>::iterator end_;
  const FilterFunction& filter_;
};
template <typename Vertex = int32_t, typename Edge = DefaultEdge<Vertex>>
class AbstractGraph {
 public:
  using VertexType = Vertex;
  using EdgeType = Edge;
  using FilterFunction = std::function<bool(const Edge&)>;
  explicit AbstractGraph(size_t vertex_count, size_t edges_count)
      : vertex_count_(vertex_count), edges_count_(edges_count) {}
  size_t GetVertexCount() const { return vertex_count_; }
  size_t GetEdgesCount() const { return edges_count_; }
  virtual std::vector<Vertex> GetNeighbours(const Vertex& vertex) = 0;
  virtual typename std::vector<Vertex>::iterator Neighbours(Vertex vertex) = 0;
  virtual typename std::vector<Vertex>::iterator NeighboursEnd(
      Vertex vertex) = 0;
  virtual Iterator<Vertex, Edge> NeighboursIt(Vertex vertex,
                                              const FilterFunction& filter) = 0;

 private:
  size_t vertex_count_ = 0;
  size_t edges_count_ = 0;
};

template <typename Vertex = int32_t, typename Edge = DefaultEdge<Vertex>>
class ListGraph : public AbstractGraph<Vertex, Edge> {
 private:
  std::unordered_map<Vertex, std::vector<Vertex>> list_;

 public:
  using FilterFunction = std::function<bool(const Edge&)>;
  ListGraph(size_t vertex_count, const std::vector<Edge>& edges)
      : AbstractGraph<Vertex, Edge>(vertex_count, edges.size()) {
    for (const auto& edge : edges) {
      list_[edge.Begin()].push_back(edge.End());
      list_[edge.End()].push_back(edge.Begin());
    }
  }
  typename std::vector<Vertex>::iterator Neighbours(Vertex vertex) override {
    return list_[vertex].begin();
  }
  typename std::vector<Vertex>::iterator NeighboursEnd(Vertex vertex) override {
    return list_[vertex].end();
  }
  Iterator<Vertex, Edge> NeighboursIt(Vertex vertex,
                                      const FilterFunction& filter) override {
    return Iterator(vertex, Neighbours(vertex), NeighboursEnd(vertex), filter);
  }
  std::vector<Vertex> GetNeighbours(const Vertex& vertex) final {
    return list_[vertex];
  }
};
template <typename Vertex = int32_t, typename Edge = DefaultEdge<Vertex>>
class MatrixGraph : public AbstractGraph<Vertex, Edge> {
 private:
  std::vector<std::vector<Vertex>> matrix_;

 public:
  using FilterFunction = std::function<bool(const Edge&)>;
  MatrixGraph(size_t vertex_count, const std::vector<Edge>& edges)
      : AbstractGraph<Vertex, Edge>(vertex_count, edges.size()),
        matrix_(vertex_count + 1) {
    for (const auto& edge : edges) {
      matrix_[edge.Begin()][edge.End()] = 1;
      matrix_[edge.End()][edge.Begin()] = 1;
    }
  }
  typename std::vector<Vertex>::iterator Neighbours(Vertex vertex) override {
    return matrix_[vertex].Begin();
  }
  typename std::vector<Vertex>::iterator NeighboursEnd(Vertex vertex) override {
    return matrix_[vertex].end();
  }
  Iterator<Vertex, Edge> NeighboursIt(Vertex vertex,
                                      const FilterFunction& filter) override {
    return Iterator(vertex, Neighbours(vertex), NeighboursEnd(vertex), filter);
  }
  std::vector<Vertex> GetNeighbours(const Vertex& vertex) final {
    std::vector<Vertex> res;
    for (size_t i = 0; i < matrix_.size(); ++i) {
      if (matrix_[vertex][i] == 1) {
        res.push_back(i);
      }
    }
    return res;
  }
};
}  // namespace graph

namespace dfs::visitors {
template <class Vertex, class Edge>
class DfsVisitor {
 public:
  virtual void TreeEdge(const Edge&) = 0;
  virtual ~DfsVisitor() = default;
};

template <class Vertex, class Edge>
class AncestorDfsVisitor : DfsVisitor<Vertex, Edge> {
 public:
  virtual void TreeEdge(const Edge& edge) { ans_[edge.End()] = edge.Begin(); }
  std::unordered_map<Vertex, Vertex> GetAns() const { return ans_; }

 private:
  std::unordered_map<Vertex, Vertex> ans_;
};
}  // namespace dfs::visitors

namespace dfs {
class DFS {
 public:
  DFS(size_t count_vertex)
      : used_(count_vertex + 1),
        tup_(count_vertex + 1),
        tin_(count_vertex + 1) {}

  template <class Vertex = int32_t, class Graph, class Visitor>
  void DepthFirstSearch(Vertex vertex, Graph& graph, Visitor& visitor,
                        int32_t prev = -1) {
    used_[vertex] = true;
    tin_[vertex] = timer_;
    tup_[vertex] = timer_;
    timer_++;
    auto neighbours = graph.GetNeighbours(vertex);
    for (const Vertex& neighbour : neighbours) {
      if (!(prev == neighbour)) {
        if (used_[neighbour]) {
          tup_[vertex] = std::min(tin_[neighbour], tup_[vertex]);
        } else {
          DepthFirstSearch(neighbour, graph, visitor, vertex);
          tup_[vertex] = std::min(tup_[neighbour], tup_[vertex]);
          if (tin_[vertex] < tup_[neighbour]) {
            visitor.TreeEdge({vertex, neighbour});
          }
        }
      }
    }
  }
  template <class Visitor>
  void Print(Visitor visitor,
             std::map<std::pair<int32_t, int32_t>, size_t>& edges,
             std::unordered_map<size_t, uint8_t>& cnt) {
    auto map = visitor.GetAns();
    std::vector<size_t> res;
    const size_t kHashCnst = 100000;
    for (auto& [key, value] : map) {
      size_t key_mp = std::min(key, value) * kHashCnst;
      key_mp += std::max(key, value);
      if (cnt[key_mp] > 1) {
        continue;
      }
      if (edges[{value, key}] && !edges[{key, value}]) {
        res.push_back(edges[{value, key}]);
      } else if (!edges[{value, key}] && edges[{key, value}]) {
        res.push_back(edges[{key, value}]);
      }
    }
    std::cout << res.size() << std::endl;
    std::sort(res.begin(), res.end());
    for (const auto& value : res) {
      std::cout << value << std::endl;
    }
  }
  int32_t Used(size_t ind) { return used_[ind]; }

 private:
  std::vector<int32_t> used_;
  std::vector<int32_t> cnt_;
  std::vector<int32_t> tup_;
  std::vector<int32_t> tin_;
  int32_t timer_ = 0;
};
}  // namespace dfs

void ReadData(std::vector<graph::DefaultEdge<int32_t>>& edges,
              std::map<std::pair<int32_t, int32_t>, size_t>& edges_id,
              std::unordered_map<size_t, uint8_t>& cnt, size_t count_edge) {
  int32_t first = 0;
  int32_t second = 0;
  const size_t kHashCnst = 100000;
  const int kTen = 10;
  for (size_t i = 0; i < count_edge; ++i) {
    std::cin >> first >> second;
    if (first == second) {
      continue;
    }
    edges.emplace_back(first, second);
    edges_id[{first, second}] = i + 1;
    size_t key = std::min(first, second) * kHashCnst;
    key += std::max(first, second);
    cnt[key] = std::min(cnt[key] + 1, kTen);
  }
}
void RunDfs(size_t count_vertex,
            std::vector<graph::DefaultEdge<int32_t>>& edges,
            std::map<std::pair<int32_t, int32_t>, size_t>& edges_id,
            std::unordered_map<size_t, uint8_t>& cnt, size_t count_edge) {
  graph::ListGraph<int32_t, graph::DefaultEdge<int32_t>> graph(count_edge,
                                                               edges);
  dfs::visitors::AncestorDfsVisitor<int32_t, graph::DefaultEdge<int32_t>>
      visitor;
  dfs::DFS depth_first_search(count_vertex);
  for (size_t i = 0; i < count_vertex; ++i) {
    if (depth_first_search.Used(i + 1) != 0) {
      continue;
    }
    depth_first_search.DepthFirstSearch(static_cast<int32_t>(i + 1), graph,
                                        visitor);
  }
  depth_first_search.Print(visitor, edges_id, cnt);
}
int main() {
  size_t count_vertex = 0;
  size_t count_edge = 0;
  std::cin >> count_vertex >> count_edge;
  std::vector<graph::DefaultEdge<int32_t>> edges;
  std::map<std::pair<int32_t, int32_t>, size_t> edges_id;
  std::unordered_map<size_t, uint8_t> cnt;
  ReadData(edges, edges_id, cnt, count_edge);
  RunDfs(count_vertex, edges, edges_id, cnt, count_edge);
}