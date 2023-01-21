#include <iostream>
#include <vector>

void Partition(std::vector<int>& arr, int& left, int& right) {
  int pivot = arr[(left + right) / 2];
  while (left <= right) {
    while (arr[right] > pivot) {
      --right;
    }
    while (arr[left] < pivot) {
      ++left;
    }
    if (left <= right) {
      if (arr[left] > arr[right]) {
        std::swap(arr[left], arr[right]);
      }
      --right;
      ++left;
    }
  }
}

void QuickSort(std::vector<int>& arr, int left, int right) {
  int i = left;
  int j = right;
  Partition(arr, left, right);
  if (j > left) {
    QuickSort(arr, left, j);
  }
  if (i < right) {
    QuickSort(arr, i, right);
  }
}

int main() {
  size_t n;
  std::cin >> n;
  std::vector<int> arr;
  for (size_t i = 0; i < n; ++i) {
    size_t val;
    std::cin >> val;
    arr.push_back(val);
  }
  QuickSort(arr, 0, n - 1);
  for (size_t i = 0; i < n; ++i) {
    std::cout << arr[i] << " ";
  }
}
