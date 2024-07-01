//
// Created by katerina on 28.04.23.
//
#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <set>
const int64_t k = 911;
//const int64_t q = 1e9 + 10;
const size_t kWordSize_ = 11;
std::vector<int64_t> x(11);
void SetX() {
  x[0] = 1;
  for (size_t i = 1; i < kWordSize_; ++i) {
    x[i] = (x[i - 1] * k);
  }
}

class Hash {
 public:
//  Hash () : x_(11), reversed_hash_(11), hash_(11) { SetX(); }
  Hash(std::string& str) : reversed_hash_(kWordSize_), hash_(kWordSize_) {
    size_t len = str.length();
    hash_[0] = str[0];
    for (size_t i = 1; i <= len; ++i) {
      char ch = str[i] - 'a' + 1;
      hash_[i] = (hash_[i - 1] + str[i] * x[i]);
      reversed_hash_[i] = (reversed_hash_[i - 1] * k + str[i - 1]);
    }
  }
  uint64_t GetReversedHash(size_t l, size_t r) {
    return (reversed_hash_[r] - reversed_hash_[l] * x[r - l]);
  }
  uint64_t GetHashSubstring(size_t l, size_t r) {
    if (l == 0) {
      return hash_[r];
    }
    return hash_[r] - hash_[l - 1];
  }
 private:
  const size_t n = 10;
  std::vector<uint64_t> hash_; // прямой полиномиальный хэш
  std::vector<uint64_t> reversed_hash_; // обратный полиномиальный хэш
// h_f = (s[0] + s[1] * p + s[2] * p^2 + ... + s[n - 1]*p^(n-1)) % mod_
};

int8_t IsPalindrom(Hash& str, size_t l, size_t r) {
  size_t sz = r - l;
  if (sz == 1 || sz == 0) {
    return true;
  }
  // смотрим текущую строку и проверяем что [l:r] палиндром -> hash = rev_hash :)
  // учитываем нечетную длину
  if (x[l + sz / 2] * str.GetReversedHash(l, l + sz / 2 + sz % 2)
      == str.GetHashSubstring(l + sz / 2, r - 1)) {
    return true;
  }
  return false;
}
void Answer(size_t count,
            std::vector<Hash>& str,
            std::vector<std::string>& data,
            std::unordered_map<int64_t, std::list<int64_t>>& map) {
  std::set<std::pair<int64_t, int64_t>> result;
  for (size_t i = 0; i < count; ++i) {
    for (size_t j = 0; j <= data[i].length(); ++j) {
      // проверяем можем ли что-то добавить вначало
      if (IsPalindrom(str[i], 0, j)) {
        int64_t hash = str[i].GetReversedHash(j, data[i].length());
        auto tmp = map.find(hash);
        if (tmp != map.end()) {
          for (auto id : tmp->second) {
            if (i != id) {
              result.insert({id + 1, i + 1});
            }
          }
        }
      }
      // аналогично в конец
      if (IsPalindrom(str[i], data[i].length() - j, data[i].length())) {
        int64_t hash = str[i].GetReversedHash(0, data[i].length() - j);
        auto tmp = map.find(hash);
        if (tmp != map.end()) {
          for (auto id : tmp->second) {
            if (i != id) {
              result.insert({i + 1, id + 1});
            }
          }
        }
      }
    }
  }
  std::cout << result.size() << std::endl;
  for (auto val : result) {
    std::cout << val.first << ' ' << val.second << '\n';
  }
}
int main() {
  size_t count = 0;
  std::cin >> count;
  std::vector<Hash> str;
  SetX();
  std::string input_str;
  std::vector<std::string> data_str;
  for (size_t i = 0; i < count; ++i) {
    std::cin >> input_str;
    data_str.emplace_back(input_str);
    str.emplace_back(input_str);
  }
  std::unordered_map<int64_t, std::list<int64_t>> map;
  for (size_t i = 0; i < count; ++i) {
    map[str[i].GetHashSubstring(0, data_str[i].length() - 1)].push_back(i);
  }
  Answer(count, str, data_str, map);
  return 0;
}