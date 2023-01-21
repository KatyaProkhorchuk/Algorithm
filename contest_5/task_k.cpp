#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

void FindPalindrom(int64_t n, std::vector<int64_t> arr) {
  const int64_t kTen = 1e9;
  std::vector<std::vector<int64_t>> dp;
  dp.resize(n, std::vector<int64_t>(n));
  for (int64_t i = 0; i < n; ++i) {
    dp[i][i] = 1;
    if (i > 0) {
      dp[i - 1][i] = 2;
      if (arr[i] == arr[i - 1]) {
        ++dp[i - 1][i];
      }
    }
  }
  for (int64_t i = n - 3; i >= 0; --i) {
    for (int64_t j = i + 2; j < n; ++j) {
      dp[i][j] = (dp[i + 1][j] + dp[i][j - 1] - dp[i + 1][j - 1] + kTen) % kTen;
      if (arr[i] == arr[j]) {
        dp[i][j] = (dp[i][j] + (dp[i + 1][j - 1] + 1) % kTen) % kTen;
      } else {
        dp[i][j] %= kTen;
      }
    }
  }
  std::cout << dp[0][n - 1];
}
int main() {
  int64_t n = 0;
  std::cin >> n;
  std::vector<int64_t> arr(n);
  for (int64_t i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }
  FindPalindrom(n, arr);
}