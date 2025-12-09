#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Column {
  std::vector<unsigned long> nums;
  enum class Operator { Add, Multiply } op;
};

int main(void) {
  auto f = std::ifstream{"day6/data.txt"};

  if (!f.is_open()) {
    std::cerr << "could not open data file\n";
    return 1;
  }

  std::vector<std::string> lines;
  std::string line;

  std::vector<Column> columns;

  bool set_width = true;
  std::size_t width = 0;

  while (std::getline(f, line)) {
    if (set_width)
      width = line.length();
    else
      assert(line.length() == width);

    lines.push_back(line);
  }

  bool new_column = true;
  std::size_t column = 0, sum = 0;

  for (std::size_t i = width - 1; 1; --i) {
    unsigned long num = 0;

    for (std::size_t j = 0; j < lines.size() - 1; ++j) {
      if (lines[j][i] == ' ')
        continue;

      num = num * 10 + (lines[j][i] - '0');
    }

    if (new_column) {
      new_column = false;
      columns.push_back({
          {},
          Column::Operator::Add,
      });
    }

    columns[column].nums.push_back(num);

    auto op = lines[lines.size() - 1][i];

    if (op != ' ') {
      new_column = true;
      assert(op == '*' || op == '+');
      columns[column++].op =
          op == '+' ? Column::Operator::Add : Column::Operator::Multiply;

      if (i)
        --i;

      auto &col = columns[column - 1];
      assert(col.nums.size());
      unsigned long _n = col.nums[0];

      for (std::size_t k = 1; k < col.nums.size(); ++k) {
        auto n = col.nums[k];
        switch (col.op) {
        case Column::Operator::Add:
          _n += n;
          break;
        case Column::Operator::Multiply:
          _n *= n;
          break;
        }
      }

      sum += _n;
    }

    if (!i)
      break;
  }

  std::cout << sum << "\n";

  return 0;
}