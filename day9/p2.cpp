#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

struct Tile {
  unsigned long x, y;
};

struct Rect {
  unsigned long l, r, t, b;

  std::size_t area() const { return (r - l + 1) * (b - t + 1); }

  bool intersects(const Rect &o) const {
    return l <= o.r && r >= o.l && t <= o.b && b >= o.t;
  }

  bool contains(unsigned long x, unsigned long y) const {
    return x >= l && x <= r && y >= t && y <= b;
  }
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

  std::size_t width = 0, height = 0;

  while (std::getline(f, line)) {
    Tile tile;
    auto index = line.find(',');

    assert(index != line.npos);

    tile.x = std::stoul(line.substr(0, index));
    tile.y = std::stoul(line.substr(index + 1));
    tiles.push_back(tile);

    width = std::max(width, tile.x + 1);
    height = std::max(height, tile.y + 1);
  }

  char *grid = new char[width * height];

  memset(grid, '.', width * height);
  for (auto &tile : tiles)
    grid[tile.y * width + tile.x] = '#';

  for (std::size_t i = 0; i < tiles.size(); ++i) {
    auto &tile = tiles[i];
    auto &next = i == tiles.size() - 1 ? tiles[0] : tiles[i + 1];

    assert(!(tile.x == next.x && tile.y == next.y));
    assert(tile.x == next.x || tile.y == next.y);

    if (tile.x == next.x) {
      auto max = std::max(tile.y, next.y);
      auto min = std::min(tile.y, next.y);

      grid[max * width + tile.x] = '^';

      for (std::size_t y = min + 1; y < max; ++y) {
        auto index = y * width + tile.x;
        if (grid[index] == '.')
          grid[index] = '^';
      }
    }

    if (tile.y == next.y) {
      auto max = std::max(tile.x, next.x);
      auto min = std::min(tile.x, next.x);

      for (std::size_t x = min + 1; x < max; ++x) {
        auto index = tile.y * width + x;
        if (grid[index] == '.')
          grid[index] = 'X';
      }
    }
  }

  for (std::size_t y = 0; y < height; ++y) {
    bool inside = false;
    for (std::size_t x = 0; x < width; ++x) {
      auto index = y * width + x;

      if (grid[index] == '^')
        inside = !inside;
      else if (inside && grid[index] == '.')
        grid[index] = 'O';
    }
  }

  std::vector<Rect> outside;

  for (std::size_t y = 0; y < height; ++y) {
    for (std::size_t x = 0; x < width; ++x) {
      auto index = y * width + x;

      if (grid[index] != '.')
        continue;

      std::size_t _width = 1;
      std::size_t _height = 1;

      for (std::size_t _x = x + 1; _x < width; ++_x) {
        if (grid[y * width + _x] != '.')
          break;

        ++_width;
      }

      for (std::size_t _y = y + 1; _y < height; ++_y) {
        for (std::size_t _x = x; _x < x + _width; ++_x) {
          if (grid[_y * width + _x] != '.')
            goto EndHeight;
        }

        ++_height;
      }

    EndHeight:
      Rect rect = {x, x + _width - 1, y, y + _height - 1};

      for (auto y = rect.t; y <= rect.b; ++y)
        for (auto x = rect.l; x <= rect.r; ++x)
          grid[y * width + x] = 'R';

      outside.push_back(rect);
    }
  }

  auto cmp = [](const Rect &lhs, const Rect &rhs) {
    return lhs.area() < rhs.area();
  };

  std::priority_queue<Rect, std::vector<Rect>, decltype(cmp)> pq(cmp);

  for (std::size_t i = 0; i < tiles.size(); ++i) {
    for (std::size_t j = i + 1; j < tiles.size(); ++j) {
      const auto &first = tiles[i];
      const auto &second = tiles[j];

      Rect r = {std::min(first.x, second.x), std::max(first.x, second.x),
                std::min(first.y, second.y), std::max(first.y, second.y)};

      pq.emplace(r);
    }
  }

  while (!pq.empty()) {
    auto &rect = pq.top();

    bool valid = true;
    for (auto &outside_rect : outside) {
      if (outside_rect.intersects(rect)) {
        valid = false;
        break;
      }
    }

    if (valid)
      break;

    pq.pop();
  }

  assert(!pq.empty());
  std::cout << pq.top().area() << '\n';

  return 0;
}