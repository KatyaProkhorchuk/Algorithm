#include <iostream>
#include <vector>
void Merge(std::vector<size_t>& arr, int left, int middle,
           int right, size_t& count) {
  int i_index = left;
  int j_index = middle + 1;
  std::vector<size_t> res;
  while (i_index <= middle && j_index <= right) {
    if (arr[i_index] < arr[j_index]) {
      res.emplace_back(arr[i_index++]);
    } else {
      count += middle - i_index + 1;
      res.emplace_back(arr[j_index++]);
    }
  }
  while (i_index <= middle) {
    res.emplace_back(arr[i_index++]);
  }
  while (j_index <= right) {
    res.emplace_back(arr[j_index++]);
  }

  for (int i = left; i <= right; ++i) {
    arr[i] = res[i - left];
  }
}

void MergeSort(std::vector<size_t>& arr, int left, int right, size_t& count) {
  if (left < right) {
    int middle = (left + right) / 2;
    MergeSort(arr, left, middle, count);
    MergeSort(arr, middle + 1, right, count);
    Merge(arr, left, middle, right, count);
  }
}

int main() {
  size_t n;
  std::cin >> n;
  std::vector<size_t> arr(n);
  std::vector<size_t> brr(n);
  for (size_t i = 0; i < n; ++i) {
    size_t tmp;
    std::cin >> tmp;
    arr[i] = tmp;
    brr[i] = tmp;
  }
  size_t count = 0;
  MergeSort(arr, 0, arr.size() - 1, count);
  std::cout << count << std::endl;
}
