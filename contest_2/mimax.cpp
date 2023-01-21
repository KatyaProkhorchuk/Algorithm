#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

class MinMax {
 public:
  void Insert(int64_t val) {
    heap_.push_back(val);
    SiftUp(heap_.size() - 1);
  }
  size_t Size() { return heap_.size(); }

  void Clear() { heap_.clear(); }
  void ExtractMin() {
    if (heap_.size() != 1) {
      size_t min_index = 0;
      std::swap(heap_[min_index], heap_[Size() - 1]);
      heap_.pop_back();
      SiftDown(min_index);
    } else {
      heap_.clear();
    }
  }
  void ExtractMax() {
    if (heap_.size() != 1) {
      size_t max_index = GetMaxId();
      std::swap(heap_[max_index], heap_[Size() - 1]);
      heap_.pop_back();
      SiftDown(max_index);
    } else {
      heap_.clear();
    }
  }
  size_t GetMaxId() {
    size_t max_id = 0;
    if (!heap_.empty()) {
      max_id = 0;
      if (heap_.size() >= 2) {
        max_id = 1;
      }
      if (heap_.size() >= 3) {
        max_id = (heap_[max_id] > heap_[2]) ? max_id : 2;
      }
    }
    return max_id;
  }
  int64_t GetMax() {
    if (heap_.empty()) {
      throw std::invalid_argument("error");
    }
    return heap_[GetMaxId()];
  }
  int64_t GetMin() {
    if (heap_.empty()) {
      throw std::invalid_argument("error");
    }
    return heap_[0];
  }
  void ExtractMinPrint() {
    if (heap_.empty()) {
      std::cout << "error" << std::endl;
    } else {
      std::cout << GetMin() << std::endl;
      ExtractMin();
    }
  }
  void ExtractMaxPrint() {
    if (heap_.empty()) {
      std::cout << "error" << std::endl;
    } else {
      std::cout << GetMax() << std::endl;
      ExtractMax();
    }
  }

 private:
  std::vector<int64_t> heap_;
  int64_t GetLevel(size_t index) { return (int)floor(log2(index + 1)); }
  bool IsMinLevel(size_t index) { return (GetLevel(index) % 2 == 0); }
  size_t GetIndexMaxChild(size_t index) {
    size_t first_child = (2 * index + 1);
    size_t second_child = (2 * index + 2);
    size_t max_child = first_child;
    if (second_child < Size() && heap_[second_child] > heap_[max_child]) {
      max_child = second_child;
    }
    size_t first_first_child = 2 * first_child + 1;
    size_t first_second_child = 2 * first_child + 2;
    size_t second_first_child = 2 * second_child + 1;
    size_t second_second_child = 2 * second_child + 2;
    if (first_first_child < Size() &&
        heap_[first_first_child] > heap_[max_child]) {
      max_child = first_first_child;
    }
    if (first_second_child < Size() &&
        heap_[first_second_child] > heap_[max_child]) {
      max_child = first_second_child;
    }
    if (second_first_child < Size() &&
        heap_[second_first_child] > heap_[max_child]) {
      max_child = second_first_child;
    }
    if (second_second_child < Size() &&
        heap_[second_second_child] > heap_[max_child]) {
      max_child = second_second_child;
    }
    return max_child;
  }
  size_t GetIndexMinChild(size_t index) {
    size_t first_child = (2 * index + 1);
    size_t second_child = (2 * index + 2);
    size_t min_child = first_child;
    if (second_child < Size() && heap_[second_child] < heap_[min_child]) {
      min_child = second_child;
    }
    size_t first_first_child = 2 * first_child + 1;
    size_t first_second_child = 2 * first_child + 2;
    size_t second_first_child = 2 * second_child + 1;
    size_t second_second_child = 2 * second_child + 2;
    if (first_first_child < Size() &&
        heap_[first_first_child] < heap_[min_child]) {
      min_child = first_first_child;
    }
    if (first_second_child < Size() &&
        heap_[first_second_child] < heap_[min_child]) {
      min_child = first_second_child;
    }
    if (second_first_child < Size() &&
        heap_[second_first_child] < heap_[min_child]) {
      min_child = second_first_child;
    }
    if (second_second_child < Size() &&
        heap_[second_second_child] < heap_[min_child]) {
      min_child = second_second_child;
    }
    return min_child;
  }
  void SiftDownMinimum(size_t index) {
    size_t size = heap_.size();
    size_t id = 2 * index + 1;
    if (id < size) {
      size_t index_min_child = GetIndexMinChild(index);
      if (GetLevel(index) == GetLevel(index_min_child) - 2) {
        if (heap_[index] > heap_[index_min_child]) {
          std::swap(heap_[index], heap_[index_min_child]);
          if (heap_[index_min_child] > heap_[(index_min_child - 1) / 2]) {
            std::swap(heap_[index_min_child], heap_[(index_min_child - 1) / 2]);
          }
          SiftDownMinimum(index_min_child);
        }
      } else if (heap_[index_min_child] < heap_[index]) {
        std::swap(heap_[index_min_child], heap_[index]);
      }
    }
  }
  void SiftDownMaximum(size_t index) {
    size_t size = heap_.size();
    size_t id = 2 * index + 1;
    if (id < size) {
      size_t index_max_child = GetIndexMaxChild(index);
      if (GetLevel(index) == GetLevel(index_max_child) - 2) {
        if (heap_[index] < heap_[index_max_child]) {
          std::swap(heap_[index], heap_[index_max_child]);
          if (heap_[index_max_child] < heap_[(index_max_child - 1) / 2]) {
            std::swap(heap_[index_max_child], heap_[(index_max_child - 1) / 2]);
          }
          SiftDownMaximum(index_max_child);
        }
      } else if (heap_[index_max_child] > heap_[index]) {
        std::swap(heap_[index_max_child], heap_[index]);
      }
    }
  }
  void SiftDown(size_t index) {
    if (IsMinLevel(index)) {
      SiftDownMinimum(index);
    } else {
      SiftDownMaximum(index);
    }
  }
  void SiftUpMinimum(size_t index) {
    size_t id = (index - 1) / 2;
    if (GetLevel(index) >= 2 && heap_[index] < heap_[(id - 1) / 2]) {
      std::swap(heap_[index], heap_[(id - 1) / 2]);
      SiftUpMinimum((id - 1) / 2);
    }
  }
  void SiftUpMaximum(size_t index) {
    size_t id = (index - 1) / 2;
    if (GetLevel(index) >= 2 && heap_[index] > heap_[(id - 1) / 2]) {
      std::swap(heap_[index], heap_[(id - 1) / 2]);
      SiftUpMaximum((id - 1) / 2);
    }
  }
  void SiftUp(size_t index) {
    if (index != 0) {
      if (IsMinLevel(index)) {
        if (heap_[index] > heap_[(index - 1) / 2]) {
          std::swap(heap_[index], heap_[(index - 1) / 2]);
          SiftUpMaximum((index - 1) / 2);
        } else {
          SiftUpMinimum(index);
        }
      } else {
        if (heap_[index] < heap_[(index - 1) / 2]) {
          std::swap(heap_[index], heap_[(index - 1) / 2]);
          SiftUpMinimum((index - 1) / 2);
        } else {
          SiftUpMaximum(index);
        }
      }
    }
  }
};

void Run(MinMax& heap, std::string str, int64_t val) {
  if (str == "insert") {
    std::cin >> val;
    heap.Insert(val);
    std::cout << "ok" << std::endl;
  } else if (str == "size") {
    std::ios_base::sync_with_stdio(false);
    std::cout << heap.Size() << std::endl;
  } else if (str == "clear") {
    heap.Clear();
    std::cout << "ok" << std::endl;
  }
  try {
    if (str == "extract_min") {
      heap.ExtractMinPrint();
    } else if (str == "get_max") {
      std::cout << heap.GetMax() << std::endl;
    } else if (str == "extract_max") {
      heap.ExtractMaxPrint();
    } else if (str == "get_min") {
      std::cout << heap.GetMin() << std::endl;
    }
  } catch (...) {
    std::cout << "error" << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t count;
  std::cin >> count;
  std::string str;
  MinMax heap;
  int64_t value = 0;
  while (count > 0) {
    std::cin >> str;
    Run(heap, str, value);
    --count;
  }
}
