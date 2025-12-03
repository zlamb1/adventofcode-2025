#include <fstream>
#include <iostream>

auto path = "day3/data.txt";

int main(void) {
  auto f = std::ifstream{path};

  if (!f.is_open()) {
    std::cerr << "failed to open '" << path << "'\n";
    return 1;
  }

  std::string line;
  std::size_t count = 0;

  while (std::getline(f, line)) {
    std::size_t num = 0;
    int pos = 0;
    auto len = line.length();

    for (int digit = 0; digit < 12; ++digit) {
      int max = 0, index = pos;
      for (std::size_t i = pos; i < len - (11 - digit); ++i) {
        auto value = line[i] - '0';
        if (value > max) {
          max = value;
          index = i;
        }
      }
      pos = index + 1;
      num = num * 10 + max;
    }

    count += num;
  }

  std::cout << count << "\n";

  return 0;
}