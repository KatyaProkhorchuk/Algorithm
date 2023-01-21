#include <algorithm>
#include <iostream>
void Run(const int* arr_pref_min, const int* arr_suf_min) {
  int q_count;
  std::cin >> q_count;
  int left, right;
  while (q_count > 0) {
    std::cin >> left >> right;
    std::cout << std::min(arr_pref_min[left - 1], arr_suf_min[right - 1])
              << std::endl;
    --q_count;
  }
}
int main() {
  int count;
  std::cin >> count;
  int* arr_people = new int[count];

  for (int i = 0; i < count; i++) {
    std::cin >> arr_people[i];
  }
  int* arr_pref_min = new int[count];
  int* arr_suf_min = new int[count];
  arr_pref_min[0] = arr_people[0];
  arr_suf_min[count - 1] = arr_people[count - 1];
  for (int i = 1; i < count; i++) {
    arr_pref_min[i] = std::min(arr_people[i], arr_pref_min[i - 1]);
    arr_suf_min[count - 1 - i] =
        std::min(arr_people[count - 1 - i], arr_suf_min[count - i]);
  }
  Run(arr_pref_min, arr_suf_min);
  delete[] arr_suf_min;
  delete[] arr_pref_min;
  delete[] arr_people;

}
