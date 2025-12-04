#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

int main(void) {
  auto f = std::ifstream("day4/data.txt");

  if (!f.is_open()) {
    std::cerr << "failed to open 'day4/data.txt'\n";
    return 1;
  }

  std::vector<std::string> lines;
  std::string line;

  while (std::getline(f, line)) {
    lines.emplace_back(line);
  }

  if (!lines.size()) {
    std::cerr << "got no input\n";
    return 1;
  }

  std::size_t rows = lines.size();
  std::size_t columns = lines[0].size();
  std::size_t count = 0;

  for (std::size_t r = 0; r < rows; ++r) {
    auto row = lines[r];
    assert(row.length() == columns);
    for (std::size_t c = 0; c < columns; ++c) {
      if (row[c] == '@') {
        int neighbors = 0;

        for (int _r = -1; _r <= 1; ++_r) {
          for (int _c = -1; _c <= 1; ++_c) {
            if ((!_r && !_c) || (!r && _r == -1) || (!c && _c == -1) ||
                (r == rows - 1 && _r == 1) || (c == columns - 1 && _c == 1))
              continue;
            if (lines[(int)r + _r][(int)c + _c] == '@')
              ++neighbors;
          }
        }

        if (neighbors < 4)
          ++count;
      }
    }
  }

  std::cout << count << "\n";

  return 0;
}