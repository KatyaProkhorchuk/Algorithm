#include <iostream>
#include <vector>
int64_t DFS(int src, std::vector<std::vector<int64_t>> dfs, int mask,
            std::vector<std::vector<int64_t>>& dp) {
  if (dp[mask][src] != INT64_MAX) {
    return dp[mask][src];
  }
  int64_t ans = dp[mask][src];
  for (size_t i = 0; i < dfs.size(); ++i) {
    if ((mask & (1 << i)) == 0) {
      int64_t tmp = dfs[src][i] + DFS(i, dfs, mask ^ (1 << i), dp);
      ans = std::min(ans, tmp);
    }
  }
  return dp[mask][src] = ans;
}

std::vector<int> FindWay(std::vector<std::vector<int64_t>> dp,
                         std::vector<std::vector<int64_t>> dfs, int ind) {
  std::vector<int> way;
  int mask = 1 << ind;
  int index = ind;
  way.push_back(index);
  while (mask != static_cast<int>(dp.size() - 1)) {
    for (size_t i = 0; i < dfs.size(); ++i) {
      int new_mask = mask ^ (1 << i);
      if (dp[mask][index] == dfs[index][i] + dp[new_mask][i]) {
        way.push_back(i);
        index = i;
        mask = new_mask;
        break;
      }
    }
  }
  return way;
}
std::vector<std::vector<int64_t>> ReadData(size_t n) {
  std::vector<std::vector<int64_t>> dfs(n, std::vector<int64_t>(n, 0));
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      std::cin >> dfs[i][j];
    }
  }
  return dfs;
}

void Clear(std::vector<std::vector<int64_t>>& dp, size_t n) {
  dp.resize((1 << n), std::vector<int64_t>(n, -1));
  for (size_t j = 0; j < n; ++j) {
    dp[(1 << n) - 1][j] = 0;
  }
}

int main() {
  size_t n = 0;
  std::cin >> n;
  auto dfs = ReadData(n);
  std::vector<std::vector<int64_t>> dp((1 << n),
                                       std::vector<int64_t>(n, INT64_MAX));
  for (size_t j = 0; j < n; ++j) {
    dp[(1 << n) - 1][j] = 0;
  }
  int64_t min = INT64_MAX;
  int index = 0;
  std::vector<int> way;
  for (size_t i = 0; i < n; ++i) {
    int mask = 1 << i;
    int64_t ans = DFS(i, dfs, mask, dp);
    if (min > ans) {
      index = i;
    }
    min = std::min(min, ans);
  }
  way = FindWay(dp, dfs, index);
  std::cout << min << "\n";
  for (size_t i = 0; i < way.size(); ++i) {
    std::cout << way[i] + 1 << " ";
  }
  return 0;
}