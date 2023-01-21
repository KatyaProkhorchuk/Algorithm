#include <iostream>
#include <queue>
#include <vector>
struct Elem {
  int val;
  size_t id;
};
class HeapMin {
 public:
  size_t GetSize() { return arr_.size(); }

  Elem GetMin() { return arr_[0]; }

  void Insert(int value, size_t i) {
    arr_.push_back({value, i});
    SiftUp(GetSize() - 1);
  }

  void ExtractMin() {
    std::swap(arr_[0], arr_[GetSize() - 1]);
    arr_.pop_back();
    SiftDown(0);
  }

 private:
  void SiftDown(size_t index) {
    size_t i;
    while (2 * index + 1 < GetSize()) {
      i = 2 * index + 1;
      if (i + 1 < GetSize() && arr_[i + 1].val < arr_[i].val) {
        ++i;
      }
      if (arr_[i].val < arr_[index].val) {
        std::swap(arr_[index], arr_[i]);
        index = i;
      } else {
        break;
      }
    }
  }

  void SiftUp(size_t index) {
    while (index > 0) {
      if (arr_[index].val < arr_[(index - 1) / 2].val) {
        std::swap(arr_[index], arr_[(index - 1) / 2]);
        index = (index - 1) / 2;
      } else {
        break;
      }
    }
  }

  std::vector<Elem> arr_;
};
void PushArr(HeapMin& heap, std::vector<std::queue<int>>& arr, size_t k,
             size_t& size) {
  size_t n;
  int val;
  for (size_t i = 0; i < k; ++i) {
    std::queue<int> temp;
    std::cin >> n;
    size += n;
    for (size_t j = 0; j < n; ++j) {
      std::cin >> val;
      temp.push(val);
    }
    heap.Insert(temp.front(), i);
    temp.pop();
    arr.push_back(temp);
  }
}
void PrintResult(HeapMin& heap, std::vector<std::queue<int>>& arr,
                 size_t size) {
  for (size_t i = 0; i < size; ++i) {
    std::cout << heap.GetMin().val << " ";
    size_t id = heap.GetMin().id;
    heap.ExtractMin();
    if (!arr[id].empty()) {
      heap.Insert(arr[id].front(), id);
      arr[id].pop();
    }
  }
}
int main() {
  size_t k;
  std::cin >> k;
  size_t size = 0;
  HeapMin heap;
  std::vector<std::queue<int>> arr;
  PushArr(heap, arr, k, size);
  PrintResult(heap, arr, size);
  return 0;
}
