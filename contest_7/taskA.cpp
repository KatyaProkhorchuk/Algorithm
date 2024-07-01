// 83832309
#include <functional>
#include <iostream>
#include <set>
#include <vector>
namespace graph {
template <typename T>
struct DefaultEdge {
  DefaultEdge() = default;
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
    return weight < other.weight;
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
      edge_.emplace_back(edge.from, edge.to_vert, edge.weight);
      edge_.emplace_back(edge.to_vert, edge.from, edge.weight);
      list_[edge.Begin()].push_back({edge.Begin(), edge.End(), edge.Weight()});
      list_[edge.End()].push_back({edge.End(), edge.Begin(), edge.Weight()});
    }
  }
  ListGraph(size_t vertex_count)
      : AbstractGraph<Vertex, Edge>(vertex_count, 0) {}
  typename std::vector<Edge> GetEdge() const { return edge_; }
  void AddEdge(const Edge& edge) {
    edge_.emplace_back(edge.from, edge.to_vert, edge.weight);
    edge_.emplace_back(edge.to_vert, edge.from, edge.weight);
    list_[edge.Begin()].emplace_back(edge.Begin(), edge.End(), edge.Weight());
    list_[edge.End()].emplace_back(edge.End(), edge.Begin(), edge.Weight());
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
}  // namespace graph

namespace dsu {
template <typename T>
class DSU {
 public:
  DSU(size_t n) {
    ancestors_ = std::vector<T>(n + 1, T());
    for (size_t i = 1; i <= n; ++i) {
      ancestors_[i] = i;
    }
    ranks_ = std::vector<T>(n, T());
    sum_ = std::vector<T>(n, T());
  }
  bool AreSame(T first, T second) { return FindSet(first) == FindSet(second); }
  void Unite(T first, T second, T weight) {
    first = FindSet(first);
    second = FindSet(second);
    if (first != second) {
      if (ranks_[first] < ranks_[second]) {
        std::swap(first, second);
      }
      sum_[first] += sum_[second] + weight;
      ancestors_[second] = first;
      if (ranks_[second] == ranks_[first]) {
        ++ranks_[first];
      }
    } else {
      sum_[first] += weight;
    }
  }
  size_t FindSet(T elem) {
    if (elem == ancestors_[elem]) {
      return elem;
    }
    return ancestors_[elem] = FindSet(ancestors_[elem]);
  }
  int GetSum(int vert) { return sum_[FindSet(vert)]; }

 private:
  std::vector<T> sum_;
  std::vector<T> ancestors_;
  std::vector<T> ranks_;
};
}  // namespace dsu

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  size_t vertex_count = 0;
  size_t q_count = 0;
  size_t code = 0;
  size_t from = 0;
  size_t vert_to = 0;
  size_t weight = 0;
  std::cin >> vertex_count >> q_count;
  dsu::DSU<size_t> dsu(vertex_count + 2);
  for (size_t i = 0; i < q_count; ++i) {
    std::cin >> code;
    if (code == 1) {
      std::cin >> from >> vert_to >> weight;
      dsu.Unite(from, vert_to, weight);
    } else if (code == 2) {
      std::cin >> from;
      std::cout << dsu.GetSum(from) << '\n';
    }
  }
  return 0;
}
