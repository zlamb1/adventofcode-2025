#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "expected path\n";
    return 1;
  }

  auto path = std::string{argv[1]};
  path += "/data.txt";
  auto f = std::ifstream{path};

  if (!f.is_open()) {
    std::cerr << "failed to open '" << path << "'\n";
    return 1;
  }

  std::string range;
  std::size_t counter = 0;

  while (std::getline(f, range, ',')) {
    unsigned long long min, max, pos;

    pos = range.find('-');
    min = std::stoull(range.substr(0, pos));
    max = std::stoull(range.substr(pos + 1));

    while (min <= max) {
      auto str = std::to_string(min++);
      auto half_len = str.length() >> 1;

      if ((str.length() % 2) == 1)
        continue;

      if (str.substr(0, half_len) == str.substr(half_len, half_len))
        counter += min - 1;
    }
  }

  std::cout << counter << "\n";

  return 0;
}