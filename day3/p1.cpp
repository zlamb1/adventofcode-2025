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
    int l = 0, r = 0;

    for (std::size_t i = 0; i < line.length() - 1; ++i) {
      auto value = line[i] - '0';
      if (l < value) {
        l = value;
        r = line[i + 1] - '0';
      } else if (r < value)
        r = value;
    }

    auto last = line[line.length() - 1] - '0';
    if (r < last)
      r = last;

    count += l * 10 + r;
  }

  std::cout << count << "\n";

  return 0;
}