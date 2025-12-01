#include <fstream>
#include <iostream>

int main(int argc, const char *argv[]) {
  if (argc < 2)
    return 1;

  std::string path = argv[1];
  path += "/data.txt";

  auto f = std::fstream(path);

  if (!f.is_open()) {
    std::cerr << "failed to open " << path << std::endl;
    return 1;
  }

  std::string line;

  int dial = 50;
  int password = 0;

  while (std::getline(f, line)) {
    bool right = false;
    int amount;

    if (line[0] == 'R')
      right = true;

    amount = std::stoi(line.substr(1));

    if (right) {
      dial += amount;
      dial %= 100;
    } else {
      dial -= amount;
      dial %= 100;
      if (dial < 0) {
        dial += 100;
      }
    }

    if (!dial) {
      ++password;
    } else if (dial < 0) {
      dial %= 99;
      dial *= -1;
    }
  }

  std::cout << password << std::endl;
}