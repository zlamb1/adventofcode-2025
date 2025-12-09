#include <cassert>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

struct Box {
  unsigned long x, y, z;
  std::shared_ptr<std::unordered_set<Box *>> circuit = nullptr;

  unsigned long distance(const Box &o) const {
    return std::sqrt(
        std::pow(static_cast<long long>(x) - static_cast<long long>(o.x), 2) +
        std::pow(static_cast<long long>(y) - static_cast<long long>(o.y), 2) +
        std::pow(static_cast<long long>(z) - static_cast<long long>(o.z), 2));
  }

  bool operator==(const Box &o) const {
    return x == o.x && y == o.y && z == o.z;
  }
};

template <> struct std::hash<Box> {
  std::size_t operator()(const Box &b) const noexcept {
    return ((23 * 31 + b.x) * 31 + b.y) * 31 + b.z;
  }
};

std::ostream &operator<<(std::ostream &os, const Box &b) {
  os << b.x << ", " << b.y << ", " << b.z << "\n";
  return os;
}

struct Pair {
  Box *first, *second;
  unsigned long distance;

  Pair(Box *first, Box *second)
      : first(first), second(second), distance(first->distance(*second)) {}
};

struct Distance {
  bool operator()(const Pair &lhs, const Pair &rhs) {
    return lhs.distance >= rhs.distance;
  }
};

int main() {
  auto f = std::ifstream{"day8/data.txt"};

  if (!f.is_open()) {
    std::cerr << "failed to open data file\n";
    return 1;
  }

  std::vector<Box> boxes;
  std::string line;

  while (std::getline(f, line)) {
    Box box;

    auto index = line.find(',');
    assert(index != line.npos);

    box.x = std::stoul(line.substr(0, index));

    auto index2 = line.find(',', index + 1);
    assert(index2 != line.npos);

    box.y = std::stoul(line.substr(index + 1, index2 - (index + 1)));
    box.z = std::stoul(line.substr(index2 + 1));

    boxes.push_back(box);
  }

  for (auto &box : boxes) {
    box.circuit = std::make_shared<std::unordered_set<Box *>>();
    box.circuit->emplace(&box);
  }

  std::priority_queue<Pair, std::vector<Pair>, Distance> pqueue;

  for (std::size_t i = 0; i < boxes.size(); ++i)
    for (std::size_t j = i + 1; j < boxes.size(); ++j)
      pqueue.emplace(&boxes[i], &boxes[j]);

  while (1) {
    auto pair = pqueue.top();
    auto first = pair.first;
    auto second = pair.second;
    pqueue.pop();

    if (first->circuit == second->circuit)
      continue;

    auto circuit = second->circuit;
    for (auto box : *circuit) {
      box->circuit = first->circuit;
    }

    first->circuit->merge(*circuit);
    circuit->clear();

    if (first->circuit->size() == boxes.size()) {
      std::cout << first->x * second->x << "\n";
      break;
    }
  }

  return 0;
}