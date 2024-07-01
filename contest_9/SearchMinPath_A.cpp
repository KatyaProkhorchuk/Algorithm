// 82071077
#include <functional>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
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

template <class Vertex = size_t, class Edge = DefaultEdge<Vertex>>
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
template <typename Vertex = size_t, typename Edge = DefaultEdge<Vertex>>
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

template <typename Vertex = size_t, typename Edge = DefaultEdge<Vertex>>
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
template <typename Vertex = size_t, typename Edge = DefaultEdge<Vertex>>
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

namespace bfs::visitors {
template <class Vertex, class Edge>
class BfsVisitor {
 public:
  virtual void TreeEdge(const Edge&) = 0;
  virtual ~BfsVisitor() = default;
};

template <class Vertex, class Edge>
class AncestorBfsVisitor : BfsVisitor<Vertex, Edge> {
 public:
  virtual void TreeEdge(const Edge& edge) { ans_[edge.End()] = edge.Begin(); }
  std::unordered_map<Vertex, Vertex> GetAns() const { return ans_; };

 private:
  std::unordered_map<Vertex, Vertex> ans_;
};
}  // namespace bfs::visitors

namespace bfs {
template <class Vertex, class Graph, class Visitor>
std::unordered_map<Vertex, Vertex> BFS(Vertex vertex, Graph& graph,
                                       Visitor visitor) {
  std::queue<Vertex> bfs_queue;
  std::unordered_set<Vertex> visited_vertex;
  bfs_queue.push(vertex);
  visited_vertex.insert(vertex);
  while (!bfs_queue.empty()) {
    Vertex current = bfs_queue.front();
    bfs_queue.pop();
    for (const Vertex& neighbour : graph.GetNeighbours(current)) {
      if (current == neighbour) {
        visitor.TreeEdge({current, neighbour});
      }
      if (visited_vertex.find(neighbour) == visited_vertex.end()) {
        visitor.TreeEdge({current, neighbour});
        bfs_queue.push(neighbour);
        visited_vertex.insert(neighbour);
      }
    }
  }
  return visitor.GetAns();
}
}  // namespace bfs
template <typename Vertex>
void FindWay(std::unordered_map<Vertex, Vertex> map, size_t start, size_t end) {
  std::vector<size_t> res;
  res.push_back(end);
  size_t current = end;
  while (map[current]) {
    current = map[current];
    res.push_back(current);
  }
  if (res.size() > 1 && res[res.size() - 1] == start) {
    std::cout << res.size() - 1 << std::endl;
    for (int i = res.size() - 1; i >= 0; --i) {
      std::cout << res[i] << ' ';
    }
  } else {
    std::cout << -1 << std::endl;
  }
}
int ReadData(std::vector<graph::DefaultEdge<size_t>>& edges, size_t start,
             size_t end, size_t size) {
  size_t first = 0;
  size_t second = 0;
  int checker = (start == end) ? 1 : 0;
  for (size_t i = 0; i < size; ++i) {
    std::cin >> first >> second;
    if (start == end && first == second && start == first) {
      checker = 1;
    }
    if (start != end && first != second) {
      edges.push_back({first, second});
    }
  }
  return checker;
}
void PrintResult(const std::vector<graph::DefaultEdge<size_t>>& edges,
                 size_t size, size_t start, size_t end) {
  graph::ListGraph<size_t, graph::DefaultEdge<size_t>> graph(size, edges);
  bfs::visitors::AncestorBfsVisitor<size_t, graph::DefaultEdge<size_t>> visitor;
  auto map = bfs::BFS(start, graph, visitor);
  FindWay(map, start, end);
}
int main() {
  size_t count = 0;
  size_t size = 0;
  std::cin >> count >> size;
  size_t start = 0;
  size_t end = 0;
  std::cin >> start >> end;
  std::vector<graph::DefaultEdge<size_t>> edges;
  int checker = ReadData(edges, start, end, size);
  if (checker == 1) {
    std::cout << 0 << std::endl;
    std::cout << end;
  } else {
    PrintResult(edges, size, start, end);
  }
}
