#include <algorithm>
#include <iostream>
#include <vector>

std::vector<int64_t> ReadElem(size_t n) {
  std::vector<int64_t> input;
  for (size_t i = 0; i < n; ++i) {
    int val = 0;
    std::cin >> val;
    input.push_back(val);
  }
  return input;
}
void Run(size_t n, std::vector<int64_t> arr) {
  std::vector<int64_t> dp1(n);
  std::vector<int64_t> dp2(n);
  std::vector<int64_t> ans;
  for (size_t i = 0; i < n; ++i) {
    dp1[i] = 1;
    dp2[i] = 1;
  }
  size_t res = 0;
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < i; ++j) {
      if (arr[j] < arr[i] && dp1[i] < dp2[j] + 1) {
        dp1[i] = dp2[j] + 1;
      }
      if (arr[j] > arr[i] && dp2[i] < dp1[j] + 1) {
        dp2[i] = dp1[j] + 1;
      }
    }
    if (res < std::max(dp1[i], dp2[i])) {
      res = std::max(dp1[i], dp2[i]);
    }
  }
  std::cout << res << std::endl;
  size_t index_save = 0;
  if (n == 1) {
    ans.push_back(arr[0]);
  } else {
    for (size_t i = 0; i < n - 1; ++i) {
      if (dp1[i] < dp1[i + 1] || dp2[i] < dp2[i + 1]) {
        ans.push_back(arr[i]);
        index_save = i;
      }
    }
    if (ans.size() < n) {
      ans.push_back(arr[index_save + 1]);
    }
  }
  for (size_t i = 0; i < ans.size(); ++i) {
    std::cout << ans[i] << " ";
  }
}
int main() {
  size_t n = 0;
  std::cin >> n;
  auto arr = ReadElem(n);
  Run(n, arr);
  return 0;
}
