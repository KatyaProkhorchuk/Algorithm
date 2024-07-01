#include <iostream>
#include <string>
#include <vector>
//ababbababa
//    aba
std::vector<size_t> PrefixFunction(std::string& str) {
  std::vector<size_t> result(str.length());
  size_t k;
  for (size_t i = 1; i < str.length(); ++i) {
    k = result[i - 1];
    while (k > 0 && str[i] != str[k]) {
      k = result[k - 1];
    }
    if (str[k] == str[i]) {
      result[i] = k + 1;
    }
  }
  return result;
}
void KnutMorrisPratt(std::string& str, std::string& pattern) {
  std::string kmp = pattern + "#" + str;
  std::vector<size_t> pref_func = PrefixFunction(kmp);
  size_t pattern_size = pattern.length();
  size_t str_size = str.length();
  for (size_t i = 0; i < str.length(); ++i) {
    if (pref_func[i + pattern_size + 1] == pattern_size) {
      std::cout << i + 1 - pattern_size << std::endl;
    }
  }
}
int main() {
  std::string str;
  std::string pattern;
  std::cin >> str >> pattern;
  KnutMorrisPratt(str, pattern);
}