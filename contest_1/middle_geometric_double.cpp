#include <cmath>
#include <iomanip>
#include <iostream>

void Run(const long double* arr_student, int q_count, int count_student) {
  int left = 0, right = 0;
  long double result = 0;
  double degree = 0;
  while (q_count > 0) {
    std::cin >> left >> right;
    degree = double(count_student) / (right - left + 1);
    if (right - left >= 1 && left != 0) {
      result = arr_student[right] / arr_student[left - 1];
    } else {
      result = arr_student[right];
    }
    result = powl(result, degree);
    std::cout << std::fixed << std::setprecision(10) << result << std::endl;
    q_count--;
  }
}

void StudentsInfo(long double* arr_student, int count_student){
  int q_count = 0;
  std::cin >> q_count;
  for (int i = 0; i < count_student; i++) {
    if (i > 0) {
      arr_student[i] =
          arr_student[i - 1] * powl(arr_student[i], 1.0 / count_student);
    } else {
      arr_student[i] = powl(arr_student[i], 1.0 / count_student);
    }
  }
  Run(arr_student, q_count, count_student);
}

int main() {
  int count_student = 0;
  std::cin >> count_student;
  long double* arr_student = new long double[count_student];
  for (int i = 0; i < count_student; i++) {
    std::cin >> arr_student[i];
  }
  StudentsInfo(arr_student,count_student);
  delete[] arr_student;
}
