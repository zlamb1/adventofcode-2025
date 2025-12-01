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
      if (dial + amount >= 100) {
        amount -= 100 - dial;
        dial = 0;
        dial += amount;
        dial %= 100;
        password += 1 + (amount / 100);
      } else if (!dial && !amount)
        ++password;
      else
        dial += amount;
    } else {
      if (amount >= dial) {
        amount -= dial;
        if (dial) {
          dial = 0;
          ++password;
        }
        dial -= amount;
        dial %= 100;
        if (dial < 0)
          dial += 100;
        password += amount / 100;
      } else
        dial -= amount;
    }
  }

  std::cout << password << std::endl;
}