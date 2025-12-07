#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Column {
  std::vector<unsigned long> nums;
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

  while (std::getline(f, line))
    lines.push_back(line);

  for (std::size_t i = 0; i < lines.size() - 1; ++i) {
    auto &line = lines[i];
    std::size_t column = 0;

    for (std::size_t j = 0; j < line.length(); ++j) {
      if (line[j] != ' ') {
        unsigned long num;
        auto index = line.substr(j).find(' ');

        if (index == line.npos) {
          num = std::stoul(line.substr(j));
          j = line.length();
        } else {
          num = std::stoul(line.substr(j, index));
          j += index;
        }

        if (!i)
          columns.push_back({{num}});
        else {
          assert(column < columns.size());
          columns[column++].nums.push_back(num);
        }
      }
    }
  }

  auto &lastline = lines[lines.size() - 1];
  std::size_t column = 0, sum = 0;

  for (std::size_t i = 0; i < lastline.length(); ++i) {
    if (lastline[i] != ' ') {
      assert(column < columns.size());

      switch (lastline[i]) {
      case '*':
      case '+': {
        auto &vec = columns[column++].nums;
        unsigned long num;
        assert(vec.size());
        num = vec[0];

        for (std::size_t j = 1; j < vec.size(); ++j) {
          if (lastline[i] == '*')
            num *= vec[j];
          else
            num += vec[j];
        }

        sum += num;
        break;
      }
      default:
        std::cerr << "unknown operator: '" << lastline[i] << "'\n";
        return 1;
      }
    }
  }

  std::cout << sum << "\n";

  return 0;
}