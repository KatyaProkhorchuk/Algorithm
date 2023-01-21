#include <iostream>
#include <list>
#include <string>
#include <vector>

template <typename T>
class HashMap {
 public:
  HashMap();
  void Insert(const T& key);
  void Erase(const T& key);
  void Reserve(size_t new_capacity);
  bool Find(const T& key);
 private:
  void Insert(std::vector<std::list<T>>& data, size_t capacity, const T& key);
  std::vector<std::list<T>> data_;
  size_t size_;
  size_t capacity_;
  const size_t kKCapacity{3};
};

template <typename T>
HashMap<T>::HashMap() {
  data_.resize(kKCapacity, std::list<T>());
  capacity_ = kKCapacity;
  size_ = 0;
}

template <typename T>
void HashMap<T>::Insert(const T& key) {
  if (Find(key)) {
    return;
  }
  if (size_ >= 0.75 * capacity_) {
    Reserve(2 * capacity_);
  }
  ++size_;
  Insert(data_, capacity_, key);
}
template <typename T>
void HashMap<T>::Erase(const T& key) {
  if (!Find(key)) {
    return;
  }
  size_t id = std::hash<T>{}(key) % capacity_;
  data_[id].remove(key);
  --size_;
}
template <typename T>
void HashMap<T>::Reserve(size_t new_capacity) {
  if (new_capacity > capacity_) {
    std::vector<std::list<T>> tmp(new_capacity, std::list<T>());
    for (auto& arr : data_) {
      for (auto& item : arr) {
        Insert(tmp, new_capacity, item);
      }
    }
    data_ = tmp;
    capacity_ = new_capacity;
  }
}
template <typename T>
bool HashMap<T>::Find(const T& key) {
  size_t id = std::hash<T>{}(key) % capacity_;
  for (auto& item : data_[id]) {
    if (item == key) {
      return true;
    }
  }
  return false;
}
template <typename T>
void HashMap<T>::Insert(std::vector<std::list<T>>& data, size_t capacity,
                        const T& key) {
  size_t id = std::hash<T>{}(key) % capacity;
  data[id].emplace_back(key);
}
void AnswerForQuestion(size_t n) {
  HashMap<int> hash;
  char op = '\0';
  size_t val = 0;
  for (size_t i = 0; i < n; ++i) {
    std::cin >> op >> val;
    if (op == '+') {
      hash.Insert(val);
    } else if (op == '-') {
      hash.Erase(val);
    } else if (op == '?') {
      if (hash.Find(val)) {
        std::cout << "YES" << std::endl;
      } else {
        std::cout << "NO" << std::endl;
      }
    }
  }
}

int main() {
  size_t n = 0;
  std::cin >> n;
  AnswerForQuestion(n);
}