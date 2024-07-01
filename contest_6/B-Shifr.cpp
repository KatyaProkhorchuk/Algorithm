#include <iostream>
#include <vector>
#include <string>

int main() {
  int32_t n;
  std::cin >> n;

  std::vector<std::string> words(n);
  for (int32_t i = 0; i < n; ++i) {
    std::cin >> words[i];
  }
  std::string result = words[0];
  for (int32_t i = 1; i < n; ++i) {
    for (int32_t j = std::min(result.size(), words[i].size()); j >= 0; --j) {
      if (result.substr(result.size() - j) == words[i].substr(0, j)) {
        result += words[i].substr(j);
        break;
      }
    }
  }
  std::cout << result << std::endl;
  return 0;
}
