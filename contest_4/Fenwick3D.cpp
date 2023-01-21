#include <iomanip>
#include <iostream>
#include <vector>
struct Coordinate {
  int x{0};
  int y{0};
  int z{0};
};
std::istream& operator>>(std::istream& in, Coordinate& a) {
  in >> a.x >> a.y >> a.z;
  return in;
}
using Vector3d = std::vector<std::vector<std::vector<int>>>;
class Fenwick {
 public:
  Fenwick(int size);
  int Sum(int x, int y, int z);
  void Update(int x, int y, int z, const int& delta);
 private:
  int size_;
  Vector3d data_;
};
Fenwick::Fenwick(int size) {
  data_.resize(
      size, std::vector<std::vector<int>>(size, std::vector<int>(size, 0)));
  size_ = size;
}
int Fenwick::Sum(int x, int y, int z) {
  int sum = 0;
  for (int i = x; i >= 0; i = (i & (i + 1)) - 1) {
    for (int j = y; j >= 0; j = (j & (j + 1)) - 1) {
      for (int k = z; k >= 0; k = (k & (k + 1)) - 1) {
        sum += data_[i][j][k];
      }
    }
  }
  return sum;
}
void Fenwick::Update(int x, int y, int z, const int& delta) {
  for (int i = x; i < size_; i = (i | (i + 1))) {
    for (int j = y; j < size_; j = (j | (j + 1))) {
      for (int k = z; k < size_; k = (k | (k + 1))) {
        data_[i][j][k] += delta;
      }
    }
  }
}

void Count_Stars(int n) {
  Fenwick tree(n + 1);
  Coordinate a;
  Coordinate b;
  int op = 0;
  int delta = 0;
  while (true) {
    std::cin >> op;
    if (op == 1) {
      std::cin >> a >> delta;
      tree.Update(a.x + 1, a.y + 1, a.z + 1, delta);
    } else if (op == 2) {
      std::cin >> a >> b;
      size_t sum = 0;
      sum += tree.Sum(b.x + 1, b.y + 1, b.z + 1);
      sum += tree.Sum(b.x + 1, a.y, a.z);
      sum += tree.Sum(a.x, b.y + 1, a.z);
      sum += tree.Sum(a.x, a.y, b.z + 1);
      sum -= tree.Sum(a.x, a.y, a.z);
      sum -= tree.Sum(b.x + 1, b.y + 1, a.z);
      sum -= tree.Sum(a.x, b.y + 1, b.z + 1);
      sum -= tree.Sum(b.x + 1, a.y, b.z + 1);
      std::cout << sum << std::endl;
    } else if (op == 3) {
      break;
    }
  }

}

int main() {
  size_t n = 0;
  std::cin >> n;
  Count_Stars(n);
}