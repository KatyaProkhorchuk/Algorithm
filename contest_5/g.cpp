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
  std::vector<int64_t> dp;
  dp.resize(n, INT64_MAX);
  dp[0] = -INT64_MAX;
  size_t ans = 0;
  for (size_t i = 0; i < n; i++) {
    size_t j = static_cast<size_t>(
        std::lower_bound(dp.begin(), dp.end(), arr[i]) - dp.begin());
    if (dp[j - 1] < arr[i] && arr[i] < dp[j]) {
      dp[j] = arr[i];
    }
    ans = std::max(ans, j);
  }
  std::cout << ans << std::endl;
}
int main() {
  size_t n = 0;
  std::cin >> n;
  auto arr = ReadElem(n);
  Run(n, arr);
  return 0;
}
