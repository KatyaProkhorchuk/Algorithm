#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

class SquareForAStar {
 public:
  SquareForAStar(std::vector<int>& arr, int n) : size_(n) {
    for (int i = 0; i < size_ - 1; ++i) {
      if (arr[size_ - 1 - i] == 0) {
        index_zero_ = 8 - i;
        ++position_;
      }
      sum_ *= 10;
      sum_ += arr[n - 1 - i];
    }
    parent_ = nullptr;
  }
  void SetStepCount(int value) { step_count_ = value; }
  int GetSum() const { return sum_; }

  //расстояние до pos
  int Distance(int number, int pos) {
    number = (number == 0) ? 9 : number;
    if (pos == number) {
      return 0;
    }
    // 'manhattan distance'
    return abs(((number - 1) % 3) - ((pos - 1) % 3)) +
           abs(((number - 1) / 3) - ((pos - 1) / 3));
  }
  int Heuristic() {
    int calc = 0;
    int decimal = 1;
    int num = sum_;
    for (int i = size_ - 1; i >= 0; --i, decimal *= 10) {
      calc += Distance((num / decimal) % 10, i);
    }
    return calc + step_count_;
  }
  SquareForAStar(int new_sum) {
    if (new_sum != 0) {
      sum_ = new_sum;
      for (index_zero_ = 0; new_sum % 10 != 0; new_sum /= 10, ++index_zero_) {
      }
    } else {
      sum_ = 0;
      index_zero_ = 0;
    }
    parent_ = nullptr;
  }
  SquareForAStar& Up() {
    //вычисляем sum_ при сдвиге вверх
    if (index_zero_ <= 5) {
      int decimal = 1;
      int tmp_sum = sum_;
      for (int i = 0; i < index_zero_ + 3; ++i, decimal *= 10, tmp_sum /= 10) {
      }
      tmp_sum %= 10;
      int new_sum = sum_ - tmp_sum * decimal;
      for (int i = 0; i < 3; ++i, decimal /= 10) {
      }
      new_sum = new_sum + decimal * tmp_sum;
      SquareForAStar* res = new SquareForAStar(new_sum);
      return *res;
    }
    SquareForAStar* res = new SquareForAStar(0);
    return *res;
  }
  SquareForAStar& Down() {
    if (index_zero_ >= 3) {
      int decimal = 1;
      int tmp_sum = sum_;
      for (int i = 0; i < index_zero_ - 3; ++i, decimal *= 10, tmp_sum /= 10) {
      }
      tmp_sum %= 10;
      int new_sum = sum_ - tmp_sum * decimal;
      for (int i = 0; i < 3; ++i, decimal *= 10) {
      }
      new_sum = new_sum + decimal * tmp_sum;
      SquareForAStar* res = new SquareForAStar(new_sum);
      return *res;
    }
    SquareForAStar* res = new SquareForAStar(0);
    return *res;
  }
  SquareForAStar& Right() {
    if (index_zero_ % 3 != 0) {
      int decimal = 1;
      int tmp_sum = sum_;
      for (int i = 0; i < index_zero_ - 1; ++i, decimal *= 10, tmp_sum /= 10) {
      }
      tmp_sum %= 10;
      int new_sum = sum_ - tmp_sum * decimal;
      new_sum += (10 * decimal * tmp_sum);
      SquareForAStar* res = new SquareForAStar(new_sum);
      return *res;
    }
    SquareForAStar* res = new SquareForAStar(0);
    return *res;
  }
  SquareForAStar& Left() {
    if (index_zero_ % 3 != 2) {
      int decimal = 1;
      int tmp_sum = sum_;
      for (int i = 0; i < index_zero_ + 1; ++i, decimal *= 10, tmp_sum /= 10) {
      }
      tmp_sum %= 10;
      int new_sum = sum_ - tmp_sum * decimal;
      new_sum += (decimal * tmp_sum / 10);
      SquareForAStar* res = new SquareForAStar(new_sum);
      return *res;
    }
    SquareForAStar* res = new SquareForAStar(0);
    return *res;
  }
  int GetStepCount() { return step_count_; }
  void SetParent(SquareForAStar* vertex) { parent_ = vertex; }
  SquareForAStar* GetParent() { return parent_; }
  char GetMove() { return move_; }
  int Variant() {
    int decimal = 1;
    int count = 0;
    int dig = 0;
    int tmp_decimal = 0;
    for (int i = 0; i < 9; ++i) {
      dig = (sum_ / decimal) % 10;
      tmp_decimal = decimal * 10;
      for (int j = i + 1; j < 9; ++j) {
        if (dig != 0 && dig < (sum_ / tmp_decimal) % 10 &&
            (sum_ / tmp_decimal) % 10 != 0) {
          ++count;
        }
        tmp_decimal *= 10;
      }
      decimal *= 10;
    }

    return count;
  }
  void GetNew(std::vector<SquareForAStar*>& square) {
    square.resize(4);
    square[0] = &Up();
    square[1] = &Down();
    square[2] = &Right();
    square[3] = &Left();
    square[0]->move_ = 'U';
    square[1]->move_ = 'D';
    square[2]->move_ = 'R';
    square[3]->move_ = 'L';
  }

 private:
  SquareForAStar* parent_;
  char move_ = '\0';    //  переход
  int step_count_ = 0;  //  кол-во шагов
  int index_zero_ = 0;
  int position_ = 0;
  int size_ = 0;
  int sum_ = 1;  //  текущее состояние
};
using Square = std::pair<int, SquareForAStar*>;
struct Cmp {
  bool operator()(const Square& left, const Square& right) const {
    if (left.first == right.first) {
      return left.second->GetSum() < right.second->GetSum();
    }
    return left.first < right.first;
  }
};

void AStar(SquareForAStar& square_for_a_star, std::string& result) {
  std::set<Square, Cmp> square;
  int h_heuristic = square_for_a_star.Heuristic();
  if (h_heuristic == 0) {
    return;
  }
  std::set<int> way;
  int heuristic;
  square.emplace(h_heuristic, &square_for_a_star);
  int8_t flag = 0;
  const int32_t kSolve = 1123456780;
  while (!square.empty() && flag == 0) {
    Square start = *(square.begin());
    way.insert(start.second->GetSum());
    std::vector<SquareForAStar*> temp;
    SquareForAStar* vertex = start.second;
    vertex->GetNew(temp);
    square.erase(square.begin());
    for (int i = 0; i < 4; ++i) {
      if (temp[i]->GetSum() != vertex->GetSum() && temp[i]->GetSum() != 0 &&
          way.find(temp[i]->GetSum()) == way.end()) {
        temp[i]->SetStepCount(vertex->GetStepCount() + 1);
        temp[i]->SetParent(vertex);
        heuristic = temp[i]->Heuristic();
        if (temp[i]->GetSum() == kSolve) {
          for (; temp[i]->GetParent() != nullptr;
               temp[i] = temp[i]->GetParent()) {
            result += temp[i]->GetMove();
          }
          flag = 1;
          break;
        }
        square.emplace(heuristic, temp[i]);
      }
    }
  }
}
int main() {
  const int kSize = 10;
  std::vector<int> arr(kSize);
  for (int i = 0; i < kSize - 1; ++i) {
    std::cin >> arr[kSize - 1 - i];
  }
  SquareForAStar square_for_a_star(arr, kSize);
  std::cout<<square_for_a_star.Variant()<<' ';
  if (square_for_a_star.Variant() % 2 == 0) {
    std::string res;
    AStar(square_for_a_star, res);
    std::cout << res.size() << std::endl;
    for (int i = res.size() - 1; i >= 0; --i) {
      std::cout << res[i];
    }
  } else {
    std::cout << -1;
  }
  return 0;
}
