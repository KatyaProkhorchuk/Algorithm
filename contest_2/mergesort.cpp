#include <iostream>
#include <vector>

void Merge(std::vector<long int >&arr, int left, int middle, int right) {
  int k_index = left;
  int i_index = left;
  int j_index = middle+1;
  std::vector<long int> res(arr.size());
  // Пока есть элементы в левом и правом прогонах
  while (i_index <= middle && j_index <= right)
  {
    if (arr[i_index] <= arr[j_index]) {
      res[k_index++] = arr[i_index++];
    }
    else {
      res[k_index++] = arr[j_index++];
    }
  }

  // Копируем оставшиеся элементы
  while (i_index <= middle) {
    res[k_index++] = arr[i_index++];
  }
  while (j_index <= right) {
    res[k_index++] = arr[j_index++];
  }
  for (int i = left; i <=right; i++) {
    arr[i] = res[i];
  }
}


void MergeSort(std::vector<long int>&arr, int left,int right){
  if(left==right){
    return;
  }

  int middle = (left+right)/2;
  MergeSort(arr,left,middle);
  MergeSort(arr,middle+1,right);
  Merge(arr,left,middle,right);

}
