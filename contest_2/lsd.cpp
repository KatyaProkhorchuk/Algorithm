#include <iostream>
#include <vector>
void BitArr(std::vector<size_t>& bit, std::vector<uint64_t> arr, int byte) {
  for (size_t i = 0; i < 256; ++i) {
    bit.push_back(0);
  }
  for (size_t i = 0; i < arr.size(); ++i) {
    ++bit[arr[i] >> (8 * byte) & 255];
  }
  for (size_t i = 1; i < 256; ++i) {
    bit[i] += bit[i - 1];
  }
}

void LSD(std::vector<uint64_t>& arr, int count, int byte) {
  std::vector<size_t> bit;
  std::vector<uint64_t> res;
  BitArr(bit, arr, byte);
  for (int i = 0; i < count; ++i) {
    res.push_back(0);
  }
  for (int i = count - 1; i >= 0; --i) {
    res[--bit[arr[i] >> (8 * byte) & 255]] = arr[i];
  }
  for (int i = 0; i < count; ++i) {
    arr[i] = res[i];
  }
}

int main() {
  int count;
  std::cin >> count;
  std::vector<uint64_t> arr;
  uint64_t element;
  for (size_t i = 0; i < count; ++i) {
    std::cin >> element;
    arr.push_back(element);
  }
  for (size_t i = 0; i < sizeof(uint64_t); ++i) {
    LSD(arr, count, i);
  }
  for (int i = 0; i < count; ++i) {
    printf("%lu\n", arr[i]);
  }
  return 0;
}
