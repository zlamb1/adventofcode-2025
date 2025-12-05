#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Range {
  unsigned long min, max;
};

int main(void) {
  auto f = std::ifstream{"day5/data.txt"};

  if (!f.is_open()) {
    std::cerr << "failed to open data file\n";
    return 1;
  }

  std::vector<std::string> lines;
  std::string line;

  while (std::getline(f, line))
    lines.emplace_back(line);

  auto iter = lines.begin();

  std::vector<Range> ranges;

  for (; iter != lines.end(); ++iter) {
    auto &line = *iter;
    if (!line.size()) {
      ++iter;
      break;
    }

    Range range;
    range.min = std::stoul(line.substr(0, line.find('-')));
    range.max = std::stoul(line.substr(line.find('-') + 1));
    ranges.emplace_back(range);
  }

  std::size_t count = 0;

  for (; iter != lines.end(); ++iter) {
    auto &line = *iter;
    unsigned long value = std::stoul(line);

    for (auto &range : ranges)
      if (value >= range.min && value <= range.max) {
        ++count;
        break;
      }
  }

  std::cout << count << "\n";

  return 0;
}