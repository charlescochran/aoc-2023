#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <set>

using Holes = std::set<std::pair<int, int>>;


Holes setup(int part)
{
  Holes holes;
  int x = 0, y = 0;
  std::ifstream f("18-input.txt");
  std::string line;
  char dir;
  int len;
  std::string color_str;
  const char dirs[] = {'R', 'D', 'L', 'U'};
  if (f.is_open())
  {
    while (std::getline(f, line))
    {
      std::istringstream line_stream(line);
      line_stream >> dir >> len >> color_str;
      if (part == 2) {
        len = std::stoi(color_str.substr(2, color_str.size() - 4), nullptr, 16);
        dir = dirs[color_str[color_str.size() - 2] - '0'];
      }
      // Dig the edge holes
      for (int i = 0; i < len; i++) {
        switch(dir) {
          case 'R': x++; break;
          case 'L': x--; break;
          case 'U': y++; break;
          case 'D': y--; break;
        }
        holes.insert({y, x});
      }
    }
  }
  f.close();

  return holes;
}

void solve(int part, const Holes& holes) {
  // Scanline-based area algorithm, developed from scratch.
  // ~1 min for part 2. Definitely could be optimized or replaced with a more
  // efficient algorithm (i.e. shoelace/pick).
  unsigned long ans = 0;
  int prev_y = -1;
  int prev_x = -1;
  bool inside = false, above = false, below = false;
  for (auto [y, x] : holes) {
    ans++; // Count this hole
    if (y != prev_y) {  // Starting a new row?
      inside = false;  // Always start on the outside
      above = holes.count({y + 1, x}) > 0;  // Is there a hole above?
      below = holes.count({y - 1, x}) > 0;  // Is there a hole below?
    } else {  // Continuing a row
      if (holes.count({y + 1, x}) > 0) { above = !above; }
      if (holes.count({y - 1, x}) > 0) { below = !below; }
      if (inside) { ans += x - prev_x - 1; }  // Count interior spaces
    }
    // Toggle inside if we've seen a wall both above and below
    if (above && below) {
      above = below = false;
      inside = !inside;
    }
    prev_x = x;
    prev_y = y;
  }
  std::cout << "Part " << part << ": " << ans << std::endl;
}


int main()
{
  auto start = std::chrono::high_resolution_clock::now();

  const Holes& holes1 = setup(1);
  solve(1, holes1);
  const Holes& holes2 = setup(2);
  solve(2, holes2);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Clock time: " << duration.count() << " us" << std::endl;

  return 0;
}
