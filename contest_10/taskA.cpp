#include <functional>
#include <iostream>
#include <set>
#include <vector>
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
      current_edge_ = *begin_;
      while (!filter(current_edge_) && begin_ != end_) {
        ++begin_;
        current_edge_ = *begin_;
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

 public:
  using FilterFunction = std::function<bool(const Edge&)>;
  ListGraph(size_t vertex_count, const std::vector<Edge>& edges)
      : AbstractGraph<Vertex, Edge>(vertex_count, edges.size()) {
    for (const auto& edge : edges) {
      list_[edge.Begin()].push_back({edge.End(), edge.Weight()});
      list_[edge.End()].push_back({edge.Begin(), edge.Weight()});
    }
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
};
template <typename Vertex = size_t, typename Weight = size_t,
          typename Edge = DefaultEdge<Vertex>>
class MatrixGraph : public AbstractGraph<Vertex, Edge> {
 private:
  std::vector<std::vector<Vertex>> matrix_;

 public:
  using FilterFunction = std::function<bool(const Edge&)>;
  MatrixGraph(size_t vertex_count, const std::vector<Edge>& edges)
      : AbstractGraph<Vertex, Edge>(vertex_count, edges.size()),
        matrix_(vertex_count + 1) {
    for (const auto& edge : edges) {
      matrix_[edge.Begin()][edge.End()] = edge.weight;
      matrix_[edge.End()][edge.Begin()] = edge.weight;
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
      if (matrix_[vertex][i] > 0) {
        res.push_back(i);
      }
    }
    return res;
  }
};
}  // namespace graph

namespace dijkstra::visitors {
template <class Vertex, class Edge>
class DijkstraVisitor {
 public:
  virtual void TreeEdge(const Edge&) = 0;
  virtual ~DijkstraVisitor() = default;
};

template <class Vertex, class Edge>
class AncestorBfsVisitor : DijkstraVisitor<Vertex, Edge> {
 public:
  virtual void TreeEdge(const Edge& edge) { ans_[edge.End()] = edge.Begin(); }
  std::unordered_map<Vertex, Vertex> GetAns() const { return ans_; };
  void Resize(size_t size, size_t value) { dist_.resize(size, value); }
  size_t GetDist(size_t index) { return dist_.at(index); }
  std::vector<size_t> Distance() { return dist_; }
  void SetDist(size_t index, Vertex value) { dist_[index] = value; }

 private:
  std::unordered_map<Vertex, Vertex> ans_;
  std::vector<size_t> dist_;
};
}  // namespace dijkstra::visitors

namespace dijkstra {
template <class Vertex, class Graph, class Visitor>
std::vector<size_t> FindWay(Vertex vertex_start, Graph& graph,
                            Visitor visitor) {
  std::set<std::pair<size_t, size_t>> queue;
  const size_t kInf = 2009000999;
  visitor.Resize(graph.GetVertexCount(), kInf);
  visitor.SetDist(vertex_start, 0);
  queue.emplace(vertex_start, visitor.GetDist(vertex_start));

  for (; !queue.empty();) {
    auto itqueue = *(queue.begin());
    size_t vertex = itqueue.first;
    size_t distcurr = itqueue.second;
    queue.erase(itqueue);
    size_t distance = visitor.GetDist(vertex);
    if (distcurr > distance) {
      continue;
    }
    for (graph::DefaultEdge neighbours : graph.GetNeighbours(vertex)) {
      if (vertex == neighbours.End()) {
        visitor.TreeEdge({vertex, neighbours.End(), neighbours.Weight()});
      }
      if (visitor.GetDist(neighbours.End()) > distance + neighbours.weight) {
        queue.erase({neighbours.End(), visitor.GetDist(neighbours.End())});
        visitor.SetDist(neighbours.to_vert, distance + neighbours.weight);
        queue.emplace(neighbours.End(), visitor.GetDist(neighbours.End()));
        visitor.TreeEdge({vertex, neighbours.End(), neighbours.weight});
      }
    }
  }
  return visitor.Distance();
}
}  // namespace dijkstra
void PrintResult(size_t vertex_count,
                 const std::vector<graph::DefaultEdge<size_t>>& edges) {
  graph::ListGraph<size_t, graph::DefaultEdge<size_t>> graph(vertex_count,
                                                             edges);
  dijkstra::visitors::AncestorBfsVisitor<size_t, graph::DefaultEdge<size_t>>
      visitor;
  size_t room;
  std::cin >> room;
  auto res = dijkstra::FindWay(room, graph, visitor);
  for (size_t j = 0; j < res.size(); ++j) {
    std::cout << res[j] << ' ';
  }
  std::cout << '\n';
}
void ReadData() {
  size_t vertex_count = 0;
  size_t edge_count = 0;
  size_t count_room = 0;
  std::cin >> count_room;
  for (size_t i = 0; i < count_room; ++i) {
    std::cin >> vertex_count >> edge_count;
    std::vector<graph::DefaultEdge<size_t>> edges;
    for (size_t j = 0; j < edge_count; ++j) {
      size_t from;
      size_t to_vert;
      size_t weight;
      std::cin >> from >> to_vert >> weight;
      edges.emplace_back(from, to_vert, weight);

    }
    PrintResult(vertex_count, edges);
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  ReadData();
  return 0;
}
