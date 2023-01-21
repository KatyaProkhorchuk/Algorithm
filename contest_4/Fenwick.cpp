#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
class SegmentTree {
 public:
  SegmentTree(){};
  SegmentTree(size_t n) {
    SizeUpdate(n);
    values_.resize(size_);
//    CreateSegmentTree(0);
//    build(0, 0, size_);
  }
//
//  void CreateSegmentTree(size_t index) {
//    if (index < size_) {
//      CreateSegmentTree(2 * index + 1);
//      CreateSegmentTree(2 * index + 2);
//      values_[index]=
//          GetValues(index * 2 + 1) + GetValues(index * 2 + 2);
//    }
//  }
  void SizeUpdate(size_t n) {
//    size_ = 2 << (int64_t) (std::log2(n - 1) + 1);
//    --size_;
    size_ = 2 * n - 1;

  }
  size_t Size() const { return size_; }
  int64_t GetValues(size_t index) {
    if (index < size_) {
      return values_[index];
    }
    return 0;
  }

  void UpdateSum(int64_t index) {
    for (; index > 0;) {
      index = (index - 1) / 2;
      values_[index] =
          GetValues(index * 2 + 1) + GetValues(index * 2 + 2);
    }
  }
  void UpdateTree(int64_t index) {
    ++values_[index + (size_ / 2)];
    UpdateSum(index + (size_ / 2));
  }

  void MinusTree(int64_t index) {
    --values_[index + (size_ / 2)];
    UpdateSum(index + (size_ / 2));
  }
  double Rsq(int64_t left, int64_t right, size_t index, size_t check_left, size_t check_right) {
    if (right <= check_left || check_right<= left) {
      return 0;
    }
    if (left <= check_left && right >= check_right) {
      return values_[index];
    }
    size_t middle = check_left + (check_right - check_left) / 2;
    return Rsq(left,right,index * 2 + 1,check_left,middle)
    + Rsq(left,right,index * 2 + 2,middle,check_right);
  }
  float Find(size_t index) {
    return values_[index + size_/2];
  }
  ~SegmentTree() {}
 private:
  size_t size_ = 0;
  std::vector<int64_t> values_;
};

std::vector<int64_t> InputArray(int64_t n) {
  std::vector<int64_t> input;
  for (int64_t i = 0; i < n; ++i) {
    int64_t val = 0;
    std::cin >> val;
    input.push_back(val);
  }
  return input;
}

void Run(size_t n, std::vector<size_t> position){
  SegmentTree tree = SegmentTree(n);
  size_t count_user = 0;
  for(size_t i = 0; i < n; ++i){
    std::string str;
    std::cin >> str;
    if(str == "CHEER") {
      int value = 0;
      std::cin>>value;
      if (count_user == 0){
        std::cout << 0 << std::endl;
      } else {
        if(count_user == 1) {
          std::cout << 1 << std::endl;
        } else {
          if(position[value] != 0) {
//            std::cout << tree.Rsq(0, position[value]  - 1, 0,0,tree.Size()/2) << " "<< count_user<<"\n ";
            std::cout << tree.Rsq(0, position[value] - 1, 0, 0,tree.Size() - 1) / (count_user - 1) << std::endl;
          }
          else {
            std::cout << 0 << std::endl;
          }
        }
      }
    }
    if (str == "RUN") {
      int user = 0, page = 0;
      std::cin >> user >> page;
      if (position[user] == 0){
        position[user] = page - 1; //запоминаем индекс в дереве
        ++count_user;
       } else {
        tree.MinusTree(position[user]);
        position[user] = page - 1;
      }
      tree.UpdateTree(page - 1);
    }
  }
}

int main(){
  size_t n = 0;
  std::cin >> n;
  const int64_t count_user = 100000;
  std::vector<size_t> position(count_user);
  Run(n, position);
}