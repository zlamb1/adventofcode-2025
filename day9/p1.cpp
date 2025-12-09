#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

struct Tile {
  unsigned long x, y;
};

std::ostream &operator<<(std::ostream &os, const Tile &t) {
  os << t.x << ", " << t.y;
  return os;
}

int main() {
  auto f = std::ifstream{"day9/data.txt"};

  if (!f.is_open()) {
    std::cerr << "failed to open data file\n";
    return 1;
  }

  std::vector<Tile> tiles;
  std::string line;

  while (std::getline(f, line)) {
    Tile tile;
    auto index = line.find(',');
    assert(index != line.npos);
    tile.x = std::stoul(line.substr(0, index));
    tile.y = std::stoul(line.substr(index + 1));
    tiles.push_back(tile);
  }

  std::size_t max_area = 0;

  for (std::size_t i = 0; i < tiles.size(); ++i) {
    for (std::size_t j = i + 1; j < tiles.size(); ++j) {
      auto width = std::max(tiles[i].x, tiles[j].x) -
                   std::min(tiles[i].x, tiles[j].x) + 1;
      auto height = std::max(tiles[i].y, tiles[j].y) -
                    std::min(tiles[i].y, tiles[j].y) + 1;
      if (width * height > max_area)
        max_area = width * height;
    }
  }

  std::cout << max_area << "\n";

  return 0;
}