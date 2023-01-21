#include <iostream>
int Partition(int* arr, int start, int end) {
  int middle = arr[(start + end) / 2];
  while (start <= end) {
    while (arr[start] < middle) {
      ++start;
    }
    while (middle < arr[end]) {
      --end;
    }
    if (start >= end) {
      break;
    }
    std::swap(arr[start], arr[end]);
    ++start;
    --end;
  }
  return end;
}

int FindOrderStatistics(int* arr, int k, int end) {
  int start = 0;
  int middle;
  while (true) {
    middle = Partition(arr, start, end);
    if (k < middle) {
      end = middle;
    } else if (k > middle) {
      start = middle + 1;
    } else {
      return arr[middle];
    }
  }
}

int* ReadData(int size) {
  const int kDiv = 10004321;
  const int kMul1 = 123;
  const int kMul2 = 45;
  int* arr = new int[size];
  std::cin >> arr[0] >> arr[1];
  for (int i = 2; i < size; ++i) {
    arr[i] = (arr[i - 1] * kMul1 + arr[i - 2] * kMul2) % (kDiv);
  }
  return arr;
}
int main() {
  int size;
  int k_statistics;
  std::cin >> size >> k_statistics;
  auto arr = ReadData(size);
  std::cout << FindOrderStatistics(arr, k_statistics - 1, size - 1)
            << std::endl;
  delete[] arr;
  return 0;
}
