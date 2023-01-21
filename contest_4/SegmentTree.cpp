//
// Created by katerina on 14.11.22.
//
#include <cmath>
#include <iostream>
#include <vector>

class SegmentTree {
 public:
  SegmentTree() {};
  SegmentTree(std::vector<int64_t> input_arr);
  void CreateSegmentTree(size_t index);
  void SizeUpdate(size_t n);
  size_t Size() const { return size_; }
  int64_t GetFirstValues(size_t index);
  int64_t GetSecondValues(size_t index);
  void UpdateSum(int64_t index);
  void UpdateTree(int64_t index, int64_t value);
  int64_t Rsq(int64_t left, int64_t right, int64_t index, int64_t check_l,
              int64_t check_r);
  ~SegmentTree() {}
 private:
  size_t size_ = 0;
  std::vector<std::pair<int64_t, int64_t>> tree_;
};

int64_t SegmentTree::GetFirstValues(size_t index) {
  if (index < size_) {
    return tree_[index].first;
  }
  return 0;
}
SegmentTree::SegmentTree(std::vector<int64_t> input_arr) {
  SizeUpdate(input_arr.size());
  tree_.resize(size_);
  for (size_t i = 0; i < input_arr.size(); ++i) {
    tree_[i + (size_ / 2)].first =
        (i % 2 == 0) ? input_arr[i] : -input_arr[i];
    tree_[i + (size_ / 2)].second =
        (i % 2 == 0) ? -input_arr[i] : input_arr[i];
  }
  CreateSegmentTree(0);
}
void SegmentTree::CreateSegmentTree(size_t index) {
  if (index < size_ / 2) {
    CreateSegmentTree(2 * index + 1);
    CreateSegmentTree(2 * index + 2);
    tree_[index].first =
        GetFirstValues(index * 2 + 1) + GetFirstValues(index * 2 + 2);
    tree_[index].second =
        GetSecondValues(index * 2 + 1) + GetSecondValues(index * 2 + 2);
  }
}

void SegmentTree::SizeUpdate(size_t n) {
  size_ = 1 << (int64_t) (std::log2(n) + 2);
  --size_;
}
int64_t SegmentTree::GetSecondValues(size_t index) {
  if (index < size_) {
    return tree_[index].second;
  }
  return 0;
}
void SegmentTree::UpdateSum(int64_t index) {
  for (; index > 0;) {
    index = (index - 1) / 2;
    tree_[index].first =
        GetFirstValues(index * 2 + 1) + GetFirstValues(index * 2 + 2);
    tree_[index].second =
        GetSecondValues(index * 2 + 1) + GetSecondValues(index * 2 + 2);
  }
}
void SegmentTree::UpdateTree(int64_t index, int64_t value) {
  tree_[index + (size_ / 2)].first =
      (index % 2 == 0) ? value : -value;
  tree_[index + (size_ / 2)].second =
      (index % 2 == 0) ? -value : value;
  UpdateSum(index + (size_ / 2));
}
int64_t SegmentTree::Rsq(int64_t left, int64_t right, int64_t index, int64_t check_l,
                         int64_t check_r) {
  if (check_r < left || right < check_l) {
    return 0;
  }
  if (left <= check_l && check_r <= right) {
    return (left % 2 == 0) ? tree_[index].first : tree_[index].second;
  }
  int64_t new_check_right = check_l + (check_r - check_l) / 2;
  int64_t first = Rsq(left, right, 2 * index + 1, check_l, new_check_right);
  int64_t second =
      Rsq(left, right, 2 * index + 2, new_check_right + 1, check_r);
  return first + second;
}
std::vector<int64_t> InputArray(int64_t n) {
  std::vector<int64_t> input;
  for (int64_t i = 0; i < n; ++i) {
    int64_t val = 0;
    std::cin >> val;
    input.push_back(val);
  }
  return input;
}
void AnswerForQuestion(SegmentTree tree) {
  size_t m = 0;
  std::cin >> m;
  int64_t id = 0, l = 0, r = 0;
  for (size_t i = 0; i < m; ++i) {
    std::cin >> id >> l >> r;
    if (id == 1) {
      std::cout << tree.Rsq(l - 1, r - 1, 0, 0, tree.Size() / 2) << std::endl;
    } else if (id == 0) {
      tree.UpdateTree(l - 1, r);
    }
  }
}
int main() {
  size_t n = 0;
  std::cin >> n;
  std::vector<int64_t> values = InputArray(n);
  SegmentTree tree = SegmentTree(values);
  AnswerForQuestion(tree);
  return 0;
}