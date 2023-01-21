#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

struct Point {
  Point(int64_t x, int64_t y) : x(x), y(y) {};
  int64_t Len() { return x * x + y * y; }
  int64_t x{0};
  int64_t y{0};
};
Point operator+(const Point p1, const Point p2) {
  return Point(p1.x + p2.x,
               p1.y + p2.y);
}
Point operator-(const Point p1, const Point p2) {
  return Point(p1.x - p2.x,
               p1.y - p2.y);
}
long double operator^(Point p1, Point p2) {
  return static_cast<long double>(p1.x * p2.y - p1.y * p2.x);
}
bool operator<(Point p1, Point p2) {
  return (p1 ^ p2) == 0 ? p1.Len() < p2.Len() : (p1 ^ p2) > 0;
}
long double Square(std::vector<Point> points) {
  long double res = 0.0;
  for (Point& p : points) {
    p.x -= points[0].x;
    p.y -= points[0].y;
  }
  for (size_t i = 0; i < points.size() - 1; ++i) {
    res += std::abs((static_cast<long double>(points[i] ^ points[i + 1])) / 2);
  }
  return res;
}

Point StartPoint(std::vector<Point> p) {
  Point start = p[0];
  for (size_t i = 0; i < p.size(); ++i) {
    if ((start.x > p[i].x) || (start.y > p[i].y && start.x == p[i].x)) {
      start = p[i];
    }
  }
  return start;
}
void Print(std::vector<Point> ans) {
  std::cout << ans.size() << std::endl;
  std::cout << ans[0].x << " " << ans[0].y << std::endl;
  for (int i = ans.size() - 1; i > 0; --i) {
    std::cout << ans[i].x << " " << ans[i].y << std::endl;
  }
  long double square = Square(ans);
//  if (square / 1e9 > 1) {
    std::cout << std::fixed << std::setprecision(1) << square
              << std::endl;
//  } else {
//    std::cout << std::fixed << std::setprecision(1) << square
//              << std::endl;
//  }

}
void GrahamConvexHull(std::vector<Point> points, Point start) {
  std::vector<Point> ans;
  ans.push_back(start);
  for (Point& p : points) {
    p.x -= start.x;
    p.y -= start.y;
  }

  for (size_t i = 1; i < points.size(); ++i) {
    while (ans.size() >= 2
                && ((ans[ans.size() - 1] - ans[ans.size() - 2])^(points[i] - ans.back())) <= 0) {
      ans.pop_back();
    }
    ans.push_back(points[i] + start);
  }
  for (Point& p : points) {
    p.x += start.x;
    p.y += start.y;
  }
  Print(ans);
}
int main() {
  size_t n = 0;
  std::cin >> n;
  std::vector<Point> points(n, {0, 0});
  for (size_t i = 0; i < n; ++i) {
    std::cin >> points[i].x >> points[i].y;
  }
  Point start_point = StartPoint(points);
  std::sort(points.begin(), points.end(), [&](Point a, Point b) {
    return ((a - start_point) ^ (b - start_point)) > 0;
  });
  GrahamConvexHull(points, start_point);
}