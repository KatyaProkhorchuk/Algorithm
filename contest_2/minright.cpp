#include <iostream>
#include <vector>
struct Inversion {
  size_t id;
  size_t val;
};
void Merge(std::vector<Inversion>& arr, int left, int middle, int right,
           std::vector<size_t>& result) {
  size_t size_left = middle - left + 1;
  size_t size_right = right - middle;
  std::vector<Inversion> arr_left(size_left);
  std::vector<Inversion> arr_right(size_right);
  for (size_t i = 0; i < size_left; ++i) {
    arr_left[i] = arr[left + i];
  }
  for (size_t i = 0; i < size_right; ++i) {
    arr_right[i] = arr[middle + 1 + i];
  }
  size_t i = 0;
  size_t j = 0;
  size_t k = left;
  while (i < size_left && j < size_right) {
    if (arr_left[i].val <= arr_right[j].val) {
      arr[k++] = arr_left[i];
      result[arr_left[i++].id] += j;
    } else {
      arr[k++] = arr_right[j++];
    }
  }
  while (i < size_left) {
    arr[k++] = arr_left[i];
    result[arr_left[i++].id] += size_right;
  }
  while (j < size_right) {
    arr[k++] = arr_right[j++];
  }
}

void MergeSort(std::vector<Inversion>& arr, int left, int right,
               std::vector<size_t>& res) {
  if (left < right) {
    int middle = (left + right) / 2;
    MergeSort(arr, left, middle, res);
    MergeSort(arr, middle + 1, right, res);
    Merge(arr, left, middle, right, res);
  }
}

int main() {
  size_t n;
  std::cin >> n;
  std::vector<Inversion> arr;
  std::vector<size_t> res(n);
  for (size_t i = 0; i < n; ++i) {
    size_t tmp;
    std::cin >> tmp;
    arr.push_back({i, tmp});
  }
  MergeSort(arr, 0, arr.size() - 1, res);
  for (size_t i = 0; i < n; ++i) {
    std::cout << res[i] << " ";
  }
}
