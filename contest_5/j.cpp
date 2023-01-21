#include <iostream>
#include <vector>

std::vector<int> ReadElem(size_t n) {
  std::vector<int> input;
  input.push_back(0);
  for (size_t i = 0; i < n; ++i) {
    int val = 0;
    std::cin >> val;
    input.push_back(val);
  }
  return input;
}

void FindAns(size_t i, size_t j, std::vector<std::vector<int>> dp,
             std::vector<int> arr) {
  std::vector<int> ans;
  while (i > 0 && j > 0) {
    if (dp[i][j] == 0) {
      break;
    }
    if (dp[i - 1][j] == dp[i][j]) {
      --i;
    } else {
      ans.push_back(i);
      j -= arr[i];
      --i;
    }
  }
  for (int i = ans.size() - 1; i >= 0; --i) {
    std::cout << ans[i] << std::endl;
  }
}
void Run(size_t n, size_t w, std::vector<int> arr1, std::vector<int> arr2) {
  std::vector<std::vector<int>> dp;
  dp.resize(n + 1, std::vector<int>(w + 1, 0));
  std::vector<int> result(n + 1, 0);
  for (size_t i = 1; i <= n; ++i) {
    for (int j = 1; j <= w; ++j) {
      dp[i][j] = dp[i - 1][j];
      if (arr1[i] <= (int)j) {
        dp[i][j] = std::max(dp[i - 1][j], dp[i - 1][j - arr1[i]] + arr2[i]);
      }
    }
  }
  FindAns(n, w, dp, arr1);
}
int main() {
  size_t n = 0;
  size_t w = 0;
  std::cin >> n >> w;
  auto massa = ReadElem(n);
  auto prices = ReadElem(n);
  Run(n, w, massa, prices);
  return 0;
}