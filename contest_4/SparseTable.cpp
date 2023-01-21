//
// Created by katerina on 15.11.22.
//
#include <algorithm>
#include <iostream>
#include <vector>
template <typename T>
class SparseTable {
 public:
  SparseTable(std::vector<std::pair<T, T>> data);

  T Query(size_t l, size_t r);

 private:
  void BuildSparse(const std::vector<std::pair<T, T>>& data);
  std::pair<T, T> Min(const std::pair<T, T>& a, const std::pair<T, T>& b);
  void Logs(size_t n);

  size_t n_;
  size_t log_n_;
  std::vector<size_t> logs_;
  std::vector<std::vector<std::pair<T, T>>> sparse_;
};

template <typename T>
SparseTable<T>::SparseTable(std::vector<std::pair<T, T>> data) {
  size_t size_lg = data.size() + 2;
  logs_.resize(size_lg, 0);
  n_ = data.size();
  Logs(size_lg);
  log_n_ = logs_[n_];
  sparse_.resize(log_n_ + 1, std::vector<std::pair<T, T>>(n_ + 1, {0, 0}));
  BuildSparse(data);
}

template <typename T>
T SparseTable<T>::Query(size_t l, size_t r) {
  std::vector<T> res;
  while (l <= r) {
    size_t lg = logs_[r - l];
    res.push_back(sparse_[lg][l].first);
    res.push_back(sparse_[lg][l].second);
    l += (1 << lg);
  }
  std::sort(res.begin(), res.end());
  return res[1];
}
template <typename T>
void SparseTable<T>::BuildSparse(const std::vector<std::pair<T, T>>& data) {
  sparse_[0] = data;
  for (size_t k = 1; k <= log_n_; ++k) {
    for (size_t i = 0; i <= n_ - (1 << k); ++i) {
      sparse_[k][i] =
          Min(sparse_[k - 1][i], sparse_[k - 1][i + (1 << (k - 1))]);
    }
  }
}
template <typename T>
std::pair<T, T> SparseTable<T>::Min(const std::pair<T, T>& a,
                                    const std::pair<T, T>& b) {
  std::vector<T> arr(4);
  arr[0] = a.first;
  arr[1] = a.second;
  arr[2] = b.first;
  arr[3] = b.second;
  std::sort(arr.begin(), arr.end());
  return {arr[0], arr[1]};
}
template <typename T>
void SparseTable<T>::Logs(size_t n) {
  for (size_t i = 2; i < n; ++i) {
    logs_[i] = logs_[int(i / 2)] + 1;
  }
}

void SparseSum(std::vector<std::pair<int64_t, int64_t>> data, size_t m) {
  SparseTable<int64_t> sparse(data);
  int left = 0, right = 0;
  for (size_t i = 0; i < m; ++i) {
    std::cin >> left >> right;
    int64_t tmp = sparse.Query(left - 1, right - 1);
    std::cout << tmp << std::endl;
  }
}
std::vector<std::pair<int64_t, int64_t>> ReadData(size_t n) {
  std::vector<std::pair<int64_t, int64_t>> data;
  for (size_t i = 0; i < n; ++i) {
    int64_t value = 0;
    std::cin >> value;
    data.push_back({value, INT64_MAX});
  }
  return data;
}
int main() {
  size_t n = 0, m = 0;
  std::cin >> n >> m;
  auto data = ReadData(n);
  SparseSum(data, m);
}