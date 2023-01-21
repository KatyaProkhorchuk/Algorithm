#include <algorithm>
#include <iostream>
bool IsLen(const long int* array_magic, int k_count, long int middle,
           int n_count) {
  long int right = array_magic[0] + middle;
  for (int i = 0; i < n_count; ++i) {
    if (right < array_magic[i]) {
      k_count--;
      right = array_magic[i] + middle;
      if (k_count == 0) {
        return false;
      }
    }
  }
  return true;
}

long int Run(const long int* array_magic, int n_count, int k_count) {
  if (k_count >= n_count) {
    return 0;
  }
  long int min = 0;
  long int middle;
  long int max = array_magic[n_count - 1] - array_magic[0] + 1;
  while (max - min > 1) {
    middle = min + (max - min) / 2;
    if (IsLen(array_magic, k_count, middle, n_count)) {
      max = middle;
    } else {
      min = middle;
    }
  }
  if (IsLen(array_magic, k_count, min, n_count)) {
    return min;
  }
  return max;
}
int main() {
  int n_count;
  int k_count;
  std::cin >> n_count >> k_count;
  long int* array_magic = new long int[n_count];
  for (int i = 0; i < n_count; i++) {
    std::cin >> array_magic[i];
  }
  std::sort(array_magic, array_magic + n_count);
  std::cout << Run(array_magic, n_count, k_count) << std::endl;
  delete[] array_magic;
}