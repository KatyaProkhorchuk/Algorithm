#include <iostream>

bool BinarySearch(int* begin, int* end, int target) {
  int* middle = begin + (end - begin) / 2;

  if (end - begin == 1) {
    return *begin == target;
  }
  if (*middle < target) {
    return BinarySearch(middle, end, target);
  }
  if (*middle > target) {
    return BinarySearch(begin, middle, target);
  }
  return *middle == target;
}

void InputArray(int*& array, size_t array_size) {
  for (size_t i = 0; i < array_size; ++i) {
    std::cin >> array[i];
  }
}

void Run(int*& array) {
  int q = 0;
  int begin = 0;
  int end = 0;
  int target = 0;
  std::cin >> q;
  for (size_t count = 0; count < q; ++count) {
    std::cin >> begin >> end >> target;
    if (BinarySearch(array + begin, array + end, target)) {
      std::cout << "YES" << std::endl;
    } else {
      std::cout << "NO" << std::endl;
    }
  }
}

int main() {
  size_t array_size = 0;
  std::cin >> array_size;
  int* array = new int[array_size];
  InputArray(array, array_size);
  Run(array);
  delete[] array;
}
