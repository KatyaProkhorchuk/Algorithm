#include <iostream>
#include <vector>
struct Pair {
  long long val;
  long long id;
};
class Heap {
 public:
  Heap(size_t count) { request_.resize(count); }
  void SiftUp(size_t index) {
    if (index > 0 && (heap_[index].val < heap_[(index - 1) / 2].val)) {
      std::swap(request_[heap_[index].id], request_[heap_[(index - 1) / 2].id]);
      std::swap(heap_[index], heap_[(index - 1) / 2]);
      SiftUp((index - 1) / 2);
    }
  }
  void SiftDown(size_t id) {
    size_t min_ind = id;
    size_t size = heap_.size();
    size_t l = 2 * id + 1;
    if (l < size && heap_[l].val < heap_[min_ind].val) {
      min_ind = l;
    }
    size_t r = 2 * id + 2;
    if (r < size && heap_[r].val < heap_[min_ind].val) {
      min_ind = r;
    }
    if (min_ind != id) {
      std::swap(request_[heap_[id].id], request_[heap_[min_ind].id]);
      std::swap(heap_[id], heap_[min_ind]);
      SiftDown(min_ind);
    }
  }
  void Insert(long long val, long long id) {
    heap_.push_back({val, id});
    request_[id] = heap_.size() - 1;
    SiftUp(heap_.size() - 1);
  }
  void ExtractMin() {
    if (heap_.empty()) {
      throw std::invalid_argument("error");
    }
    size_t size = heap_.size();
    std::swap(request_[heap_[0].id], request_[heap_[size - 1].id]);
    std::swap(heap_[0], heap_[size - 1]);
    heap_.pop_back();
    SiftDown(0);
  }
  void DecreaseKey(size_t index, long long key) {
    size_t id = request_[index];
    heap_[id].val = heap_[id].val - key;
    SiftUp(id);
  }
  long long GetMin() {
    if (heap_.empty()) {
      throw std::invalid_argument("error");
    }
    return heap_[0].val;
  }

 private:
  std::vector<Pair> heap_;
  std::vector<size_t> request_;
};

void Run(Heap& heap, std::string str, long long id) {
  if (str == "insert") {
    long long val;
    std::cin >> val;
    heap.Insert(val, id);
  } else if (str == "getMin") {
    std::ios_base::sync_with_stdio(false);
    std::cout << heap.GetMin() << std::endl;
  } else if (str == "extractMin") {
    heap.ExtractMin();
  } else if (str == "decreaseKey") {
    long long num;
    long long key;
    std::cin >> num >> key;
    heap.DecreaseKey(num - 1, key);
  }
}

int main() {
  long long count;
  std::cin >> count;
  std::string str;
  Heap heap(count);
  long long id = 0;
  while (count > 0) {
    std::cin >> str;
    Run(heap, str, id);
    --count;
    ++id;
  }
}
