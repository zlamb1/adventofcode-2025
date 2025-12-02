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
      auto len = str.length();
      long current_len = len >> 1;

      for (; current_len > 0; current_len--) {
        if ((len % current_len) != 0)
          continue;

        auto tmp = len / current_len;

        for (std::size_t i = 0; i < tmp; ++i) {
          if (str.substr(0, current_len) !=
              str.substr(i * current_len, current_len)) {
            break;
          } else if (i == tmp - 1) {
            counter += min - 1;
            current_len = 0;
          }
        }
      }
    }
  }

  std::cout << counter << "\n";

  return 0;
}