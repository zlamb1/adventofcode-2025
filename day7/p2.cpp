#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Beam {
  std::size_t x, y;

  Beam(std::size_t x, std::size_t y) {
    this->x = x;
    this->y = y;
  }

  Beam(const Beam &b) {
    x = b.x;
    y = b.y;
  }

  Beam &operator=(const Beam &b) {
    x = b.x;
    y = b.y;
    return *this;
  }

  bool operator==(const Beam &o) const { return x == o.x && y == o.y; }
};

namespace std {

template <> struct hash<Beam> {
  std::size_t operator()(const Beam &b) const noexcept {
    return (23 * 31 + b.x) * 31 + b.y;
  }
};

} // namespace std

static std::size_t width, height;
static std::unordered_map<Beam, std::size_t> m_timelines;

std::size_t timelines(std::vector<std::string> rows, std::size_t x,
                      std::size_t y) {
  std::size_t t = 0;
  char ch;

  Beam key = {x, y};
  if (m_timelines.count(key))
    return m_timelines[key];

  if (y >= height - 1) {
    t = 1;
    goto End;
  }

  y += 1;
  ch = rows[y][x];

  if (ch == '^') {
    if (x)
      t += timelines(rows, x - 1, y);
    if (x < width - 1)
      t += timelines(rows, x + 1, y);
  } else
    return timelines(rows, x, y);

End:
  m_timelines[key] = t;
  return t;
}

int main(void) {
  auto f = std::ifstream{"day7/data.txt"};

  if (!f.is_open()) {
    std::cerr << "failed to open data file\n";
    return 1;
  }

  std::vector<std::string> rows;
  std::string row;
  Beam start{0, 0};

  bool first_row = true;
  bool find_start = true;

  while (std::getline(f, row)) {
    if (first_row)
      first_row = false;
    else
      assert(rows[0].length() == row.length());

    std::size_t index;
    if (find_start) {
      if ((index = row.find('S')) != row.npos) {
        find_start = false;
        start = {index, rows.size()};
      }
    } else
      assert(row.find('S') == row.npos);

    rows.emplace_back(row);
  }

  width = rows[0].length();
  height = rows.size();

  assert(width);
  assert(height);

  std::cout << timelines(rows, start.x, start.y) << "\n";

  return 0;
}