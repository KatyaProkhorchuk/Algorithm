#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class Hash {
 public:
  Hash(size_t len, std::string& str) : pow_(len + 1), hash_(len + 1) {
    pow_[0] = 1;
    for (size_t i = 1; i <= len; ++i) {
//      std::cout << str <<' ' << i << hash_[i - 1] + pow_[i - 1] * str[i - 1] << '\n';
      hash_[i] = (hash_[i - 1] * k_ + str[i - 1]);
      pow_[i] = (pow_[i - 1] * k_);
    }
  }
  uint64_t GetHashSubstring(size_t left, size_t right) {
    return hash_[right] - hash_[left] * pow_[right - left];
  }
 private:
  std::vector<uint64_t> pow_;
  std::vector<uint64_t> hash_;
  const uint64_t k_ = 31;
//  const int64_t mod_ = 1e+16;
};
int8_t Find(std::vector<uint64_t>& arr, uint64_t value) {
  size_t left = 0;
  size_t right = arr.size() - 1;
  size_t mid = (right - left) / 2 + left;
  while (left < right && mid < arr.size()) {
    if (arr[left] == value) {
      mid = left;
      break;
    }
    if (arr[right] == value) {
      mid = right;
      break;
    }
    if (arr[mid] < value) {
      left = mid + 1;
    }
    if (arr[mid] > value) {
      right = mid - 1;
    }
    if (arr[mid] == value) {
      break;
    }
    mid = (right - left) / 2 + left;
  }
  if (arr[mid] == value) {
    return 1;
  } else {
    return -1;
  }
}
int64_t Answer(std::string& pattern, std::string& str) {
  Hash hash_str(str.length(), str);
  std::string double_pattern = pattern + pattern;
  Hash hash_pattern(double_pattern.length(), double_pattern);
  // найдем хэши циклических сдвигов
  size_t size_pattern = pattern.length();
  std::vector<uint64_t> hash_cyclic;
  for (size_t i = 0; i < size_pattern; ++i) {
    hash_cyclic.emplace_back(hash_pattern.GetHashSubstring(i,
                                                           i + size_pattern));
  }
  // перебираем все циклические сдвиги хэшей
  int64_t res = 0;
  std::sort(hash_cyclic.begin(), hash_cyclic.end());
  for (size_t i = 0; i <= str.length() - size_pattern; ++i) {
    uint64_t hash = hash_str.GetHashSubstring(i, i + size_pattern);
    auto get_hash = Find(hash_cyclic, hash);
    if (get_hash == 1) {
      ++res;
    }
  }
  return res;
}
int main() {
  std::string pattern;
  std::string str;
  std::cin >> pattern >> str;
  if (pattern.size() > str.size()) {
    std::cout << "0\n";
  } else {
    std::cout << Answer(pattern, str);
  }
  return 0;
}