#include <cassert>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

struct Beam {
  std::size_t x, y;
};

int main(void) {
  auto f = std::ifstream{"day7/data.txt"};

  if (!f.is_open()) {
    std::cerr << "failed to open data file\n";
    return 1;
  }

  std::vector<std::string> rows;
  std::string row;
  Beam start;

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

  auto width = rows[0].length();
  auto height = rows.size();

  assert(width);
  assert(height);

  std::size_t splits = 0;

  std::stack<Beam> beams;
  beams.push(start);

  while (!beams.empty()) {
    auto beam = beams.top();
    beams.pop();

    if (beam.y < height - 1) {
      auto x = beam.x;
      auto y = beam.y + 1;
      auto ch = rows[y][x];

      if (ch == '^') {
        bool split = false;

        if (x && rows[y][x - 1] == '.') {
          split = true;
          rows[y][x - 1] = '|';
          beams.push({x - 1, y});
        }

        if (x < width - 1 && rows[y][x + 1] == '.') {
          split = true;
          rows[y][x + 1] = '|';
          beams.push({x + 1, y});
        }

        if (split)
          ++splits;

      } else {
        rows[y][x] = '|';
        beams.push({x, y});
      }
    }
  }

  std::cout << splits << "\n";

  return 0;
}