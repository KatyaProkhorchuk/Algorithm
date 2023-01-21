#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

void MaxMiddle(int64_t n, std::vector<int64_t> arr) {
  std::vector<std::vector<double>> dp;
  dp.resize(n, std::vector<double>(n));
  for (int64_t i = 0; i < n; ++i) {
    dp[0][i] = arr[i];
  }
  double max = 0;
  for (int64_t i = 1; i < n; ++i) {
    for (int64_t j = 0; j < n - i; ++j) {
      max = 0;
      for (int k = 0; k < i; ++k) {
        max = std::max(max, (dp[k][j] + dp[i - (k + 1)][j + k + 1]) / 2);
      }
      dp[i][j] = max;
    }
  }
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      std::cout << dp[i][j] <<" ";
    }
    std::cout<<"\n";
  }
  std::cout << std::setprecision(6) << dp[n - 1][0];
}
int main() {
  int64_t n = 0;
  std::cin >> n;
  std::vector<int64_t> arr(n);
  for (int64_t i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }
  MaxMiddle(n, arr);
}