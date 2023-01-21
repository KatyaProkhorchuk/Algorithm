#include <iostream>
#include <vector>

std::vector<int> ReadElem(size_t n) {
  std::vector<int> input;
  for (size_t i = 0; i < n; ++i) {
    int val = 0;
    std::cin >> val;
    input.push_back(val);
  }
  return input;
}
void Run(size_t n, size_t m, std::vector<int> arr1, std::vector<int> arr2) {
  std::vector<std::vector<int>> dp;
  dp.resize(n + 1, std::vector<int>(m + 1, 0));
  std::vector<int> result;
  for (size_t i = 1; i < n + 1; ++i) {
    for (size_t j = 1; j < m + 1; ++j) {
      if (arr1[i - 1] == arr2[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
      } else {
        dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
      }
    }
  }
  int id = dp[n][m];
  int sz_id = id;
  std::vector<int> lcs(id);
  size_t i = n;
  size_t j = m;
  while (i > 0 && j > 0) {
    if (arr1[i - 1] == arr2[j - 1]) {
      lcs[id - 1] = arr1[i - 1];
      --i;
      --j;
      --id;
    } else if (dp[i - 1][j] > dp[i][j - 1]) {
      --i;
    } else {
      --j;
    }
  }
  for (int i = 0; i < sz_id; ++i) {
    std::cout << lcs[i] << " ";
  }
}
int main() {
  size_t n = 0;
  size_t m = 0;
  std::cin >> n;
  auto arr1 = ReadElem(n);
  std::cin >> m;
  auto arr2 = ReadElem(m);
  Run(n, m, arr1, arr2);
  return 0;
}