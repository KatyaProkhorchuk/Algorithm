#include <iostream>
#include <vector>

struct Segment {
  int from;
  int to;
};

bool operator<=(const Segment& first, const Segment& second) {
  return first.from <= second.from;
}
bool operator<(const Segment& first, const Segment& second) {
  return first.from < second.from;
}

void Merge(std::vector<Segment>& arr, int left, int middle,
           int right) {
  int i_index = left;
  int j_index = middle + 1;
  std::vector<Segment> res;
  while (i_index <= middle && j_index <= right) {
    if (arr[i_index] < arr[j_index]) {
      res.emplace_back(arr[i_index++]);
//      ++i_index;
    } else {
      res.emplace_back(arr[j_index++]);
//      ++j_index;
    }
  }
  while (i_index <= middle) {
    res.emplace_back(arr[i_index++]);
//    ++i_index;
  }
  while (j_index <= right) {
    res.emplace_back(arr[j_index++]);
//    ++j_index;
  }
  for (int i = left; i <= right; ++i) {
    arr[i] = res[i - left];
  }
}

void MergeSort(std::vector<Segment>& arr, int left, int right) {
  if (left < right) {
    int middle = (left + right) / 2;
    MergeSort(arr, left, middle);
    MergeSort(arr, middle + 1, right);
    Merge(arr, left, middle, right);
  }
}

std::vector<Segment> ReadPoints() {
  std::vector<Segment> points;
  int segment_count;
  std::cin >> segment_count;
  for (int i = 0; i < segment_count; ++i) {
    int x;
    int y;
    std::cin >> x >> y;
    points.push_back({x, y});
  }
  return points;
}

std::vector<Segment> FindAudioSegments(const std::vector<Segment>& segments) {
  std::vector<Segment> result;
  int left = segments[0].from;
  int right = segments[0].to;
  for (size_t i = 1; i < segments.size(); ++i) {
    if (right < segments[i].from) {
      result.push_back({left, right});
      left = segments[i].from;
      right = segments[i].to;
    } else {
      if (right < segments[i].to) {
        right = segments[i].to;
      }
    }
  }
  result.push_back({left, right});
  return result;
}

void PrintResults(const std::vector<Segment>& segments) {
  std::cout << segments.size() << std::endl;
  for (auto[from, to] : segments) {
    printf("%d %d\n", from, to);
  }
}

int main() {
  auto points = ReadPoints();
  MergeSort(points, 0, points.size() - 1);
  auto segments = FindAudioSegments(points);
  PrintResults(segments);
}
