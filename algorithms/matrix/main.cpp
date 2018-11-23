#include <vector>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <iostream>
#include <utility>

using matrix = std::vector<std::vector<int>>;

matrix random_matrix(int n) {
  matrix m;
  m.resize(n);
  for (auto &row: m) {
    row.reserve(n);
    for (int i = 0; i < n; i++) {
      row.push_back(rand());
    }
  }
  return m;
}

bool equal(matrix const &a, matrix const &b) {
  if (a.size() != b.size()) { return false; }
  const auto height = a.size();
  bool any_fault = false;
  for (auto i = 0; i < height; i++) {
    if (a[i].size() != b[i].size()) {
      any_fault = true;
      break;
    }
    const auto width = a[i].size();
    for (auto j = 0; j < width; j++) {
      if (a[i][j] != b[i][j]) {
        any_fault = true;
        break;
      }
    }
    if (any_fault) { break; }
  }
  return !any_fault;
}

matrix rot90(matrix const &m) {
  matrix r = m;
  const auto height = m.size();
  const auto width = m[0].size();
  for (auto i = 0; i < height; i++) {
    for (auto j = 0; j < width; j++) {
      r[j][height - 1 - i] = m[i][j];
    }
  }
  return r;
}

matrix flip_v(matrix const &m) {
  matrix r = m;
  const auto height = m.size();
  const auto width = m[0].size();
  for (auto i = 0; i < height/2; i++) {
    for (auto j = 0; j < width; j++) {
      std::swap(r[i][j], r[height - 1 - i][j]);
    }
  }
  return r;
}

matrix flip_h(matrix const &m) {
  matrix r = m;
  const auto height = m.size();
  const auto width = m[0].size();
  for (auto i = 0; i < height; i++) {
    for (auto j = 0; j < width/2; j++) {
      std::swap(r[i][j], r[i][width - 1 - j]);
    }
  }
  return r;
}

matrix flip_diag_lt_rb(matrix const &m) {
  matrix r = m;
  const auto height = m.size();
  const auto width = m[0].size();
  for (auto i = 1; i < height; i++) {
    for (auto j = 0; j < i; j++) {
      std::swap(r[i][j], r[j][i]);
    }
  }
  return r;
}

matrix flip_diag_lb_rt(matrix const &m) {
  matrix r = m;
  const auto height = m.size();
  const auto width = m[0].size();
  for (auto i = 0; i < height - 1; i++) {
    for (auto j = 0; j < width - 1 - i; j++) {
      std::swap(r[i][j], r[width - 1 - j][height - 1 - i]);
    }
  }
  return r;
}

#define ROT_270(x) (rot90(rot90(rot90(x))))
#define ROT_360(x) (rot90(rot90(rot90(rot90(x)))))

int main() {
  srand(time(0));

  for (int i = 1; i < 20; i++) {
    auto m = random_matrix(i);
    if (!equal(m, ROT_360(m))) {
      throw std::logic_error("Rotation doesn't work");
    }
    if (!equal(m, flip_v(flip_v(m)))) {
      throw std::logic_error("Vertical flipping doesn't work");
    }
    if (!equal(m, flip_h(flip_h(m)))) {
      throw std::logic_error("Horizontal flipping doesn't work");
    }
    if (!equal(flip_diag_lt_rb(m), flip_h(rot90(m)))) {
      throw std::logic_error("LeftTop to RightBottom diagonal flipping doesn't work");
    }
    if (!equal(flip_diag_lb_rt(m), flip_h(ROT_270(m)))) {
      throw std::logic_error("LeftBottom to RightTop diagonal flipping doesn't work");
    }
  }

  std::cout << "Done" << std::endl;
  
  return 0;
}
