#include <iostream>

void InputArray(int*& array, size_t array_size) {
  for (size_t i = 0; i < array_size; ++i) {
    std::cin >> array[i];
  }
}

int CycleSearch(const int* arr, size_t begin, size_t end) {
  size_t middle = 0;
  while (end - begin > 1) {
    middle = begin + (end - begin) / 2;
    if (arr[begin] < arr[middle]) {
      begin = middle;
    }
    if (arr[end] > arr[middle]) {
      end = middle;
    }
  }
  size_t result = begin;
  if (arr[end] < arr[begin]) {
    result = end;
  } else {
    if (arr[begin] == arr[end]) {
      if (arr[begin] > arr[0]) {
        result = 0;
      }
    }
  }
  return result;
}

int main() {
  size_t array_size = 0;
  std::cin >> array_size;
  int* array = new int[array_size];
  InputArray(array,array_size);
  std::cout << CycleSearch(array, 0, array_size - 1);
  delete[] array;
}
