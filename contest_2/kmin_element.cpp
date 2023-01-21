#include <algorithm>
#include <iostream>
#include <vector>

class HeapMax {
 public:
  size_t GetSize() { return arr_.size(); }

  size_t GetMax() { return arr_[0]; }

  void Insert(size_t value) {
    arr_.push_back(value);
    SiftUp(GetSize() - 1);
  }

  void ExtractMax() {
    std::swap(arr_[0], arr_[GetSize() - 1]);
    arr_.pop_back();
    SiftDown(0);
  }

  void PrintSort() {
    std::sort(arr_.begin(), arr_.end());
    for (size_t i = 0; i < GetSize(); ++i) {
      std::cout << arr_[i] << ' ';
    }
    std::cout << std::endl;
  }

 private:
  void SiftDown(size_t index) {
    size_t i;
    while (2 * index + 1 < GetSize()) {
      i = 2 * index + 1;
      if (i + 1 < GetSize() && arr_[i + 1] > arr_[i]) {
        ++i;
      }
      if (arr_[i] > arr_[index]) {
        std::swap(arr_[index], arr_[i]);
        index = i;
      } else {
        break;
      }
    }
  }

  void SiftUp(size_t index) {
    while (index > 0) {
      if (arr_[index] > arr_[(index - 1) / 2]) {
        std::swap(arr_[index], arr_[(index - 1) / 2]);
        index = (index - 1) / 2;
      } else {
        break;
      }
    }
  }

  std::vector<size_t> arr_;
};

int main() {
  size_t n, k;
  size_t a, x, y;
  std::cin >> n >> k;
  std::cin >> a >> x >> y;
  HeapMax heap;
  for (size_t i = 0; i < n; ++i) {
    a = (a * x + y) % (1ll << 30);
    if (i < k) {
      heap.Insert(a);
    } else {
      if (heap.GetMax() > a) {
        heap.ExtractMax();
        heap.Insert(a);
      }
    }
  }
  heap.PrintSort();
  return 0;
}
