#include <cassert>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

struct Range {
  unsigned long min, max;

  enum class Overlap { None, Left, Right, Contained, Container };

  Overlap Intersect(const Range &o) {
    if (min >= o.min && max <= o.max)
      return Overlap::Contained;

    if (min < o.min && max > o.max)
      return Overlap::Container;

    if (min < o.min && max >= o.min && max <= o.max)
      return Overlap::Left;

    if (min >= o.min && min <= o.max && max > o.max)
      return Overlap::Right;

    return Overlap::None;
  }
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

  std::size_t count = 0;
  std::vector<Range> ranges;

  for (; iter != lines.end(); ++iter) {
    auto &line = *iter;
    if (!line.size())
      break;

    auto min = std::stoul(line.substr(0, line.find('-')));
    auto max = std::stoul(line.substr(line.find('-') + 1));

    assert(min <= max);

    std::stack<Range> stack;
    stack.push({min, max});

    while (!stack.empty()) {
      bool has_range = true;
      auto cur = stack.top();
      stack.pop();

      assert(cur.min <= cur.max);

      for (auto &range : ranges) {
        auto overlap = cur.Intersect(range);

        switch (overlap) {
        case Range::Overlap::None:
          continue;
        case Range::Overlap::Left:
          cur.max = range.min - 1;
          break;
        case Range::Overlap::Right:
          cur.min = range.max + 1;
          break;
        case Range::Overlap::Contained:
          has_range = false;
          goto EndRange;
        case Range::Overlap::Container:
          has_range = false;
          stack.push({cur.min, range.min - 1});
          stack.push({range.max + 1, cur.max});
          goto EndRange;
        }
      }

    EndRange:
      if (has_range) {
        count += (cur.max - cur.min) + 1;
        ranges.emplace_back(Range{cur.min, cur.max});
      }
    }
  }

  std::cout << count << "\n";

  return 0;
}