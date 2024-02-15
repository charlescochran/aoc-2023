#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

enum Dir {E = 0, N = 1, W = 2, S = 3};

// Map each possible symbol to its effect on the beam direction.
// Format: dir_map[cur_symbol][cur_dir] = {next_dir1, ...}
std::map<char, std::map<Dir, std::vector<Dir>>> dir_map {
  {'/',  {{E, {N   }},
          {N, {E   }},
          {W, {S   }},
          {S, {W   }}}},
  {'\\', {{E, {S   }},  // Escaped backslash symbol
          {N, {W   }},
          {W, {N   }},
          {S, {E   }}}},
  {'-',  {{E, {E   }},
          {N, {E, W}},
          {W, {W   }},
          {S, {E, W}}}},
  {'|',  {{E, {N, S}},
          {N, {N   }},
          {W, {N, S}},
          {S, {S   }}}},
  {'.',  {{E, {E   }},
          {N, {N   }},
          {W, {W   }},
          {S, {S   }}}},
};

std::vector<std::string> lines;  // Global vector of lines in input

using Pos = std::pair<int, int>;

void explore(const Pos pos, const Dir dir, std::set<std::pair<Pos, Dir>>& blacklist) {
  // Recursively simulate a beam at pos, travelling dir

  // Ignore pos/dir pairs we have already seen
  if (blacklist.find({pos, dir}) != blacklist.end()) { return; }
  // Ignore out-of-bounds positions
  if (pos.first < 0 || pos.first >= (int)lines[0].size()) { return; }
  if (pos.second < 0 || pos.second >= (int)lines.size()) { return; }

  blacklist.insert({pos, dir});
  // Recursively explore each of the next positions (based on the symbol at this position)
  for (const Dir next_dir : dir_map[lines[pos.second][pos.first]][dir]) {
    switch(next_dir) {
      case N: explore(std::pair(pos.first, pos.second - 1), next_dir, blacklist); break;
      case W: explore(std::pair(pos.first - 1, pos.second), next_dir, blacklist); break;
      case S: explore(std::pair(pos.first, pos.second + 1), next_dir, blacklist); break;
      case E: explore(std::pair(pos.first + 1, pos.second), next_dir, blacklist); break;
    }
  }
}

unsigned calc_energized(const Pos pos, const Dir dir) {
  // Find number of tiles energized by a beam starting at pos, traveling dir
  std::set<std::pair<Pos, Dir>> blacklist;
  explore(pos, dir, blacklist);
  // Return number of unique positions in the blacklist
  std::set<Pos> seen;
  std::transform(blacklist.begin(),
                 blacklist.end(),
                 std::inserter(seen, seen.begin()),
                 [](const std::pair<Pos, Dir>& pair) { return pair.first; });
  return seen.size();
}

void setup()
{
  std::ifstream f("16-input.txt");
  if (!f.is_open()) { exit(1); }
  for (std::string line; std::getline(f, line); ) {
    lines.push_back(line);
  }
  f.close();
}

void solve() {
  unsigned p2 = 0;
  for (unsigned x = 0; x < lines[0].size(); x++) {
    p2 = std::max(p2, calc_energized({x, 0}, S));                     // Top row
    p2 = std::max(p2, calc_energized({x, lines.size() - 1}, N));      // Bottom row
  }
  for (unsigned y = 0; y < lines.size(); y++) {
    p2 = std::max(p2, calc_energized({0, y}, E));                     // Left col
    p2 = std::max(p2, calc_energized({lines[0].size() - 1, y}, W));   // Right col
  }
  std::cout << "Part 1: " << calc_energized({0, 0}, E) << std::endl;
  std::cout << "Part 2: " << p2 << std::endl;
}


int main()
{
  auto start = std::chrono::high_resolution_clock::now();

  setup();
  solve();

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Clock time: " << duration.count() << " us" << std::endl;

  return 0;
}
