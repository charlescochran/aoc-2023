#include <algorithm>  // std::sort
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

enum Dir {E = 0b0001, N = 0b0010, W = 0b0100, S = 0b1000};
enum Pipe {V = 0b1010, H = 0b0101, NE = 0b0011, NW = 0b0110, SW = 0b1100, SE = 0b1001};

using Pos = std::pair<unsigned, unsigned>;

std::map<char, Pipe> pipe_map {{'|', V}, {'-', H}, {'L', NE}, {'J', NW}, {'7', SW}, {'F', SE}};
// Map from a row num to corresponding vector of (col num, Pipe) pairs.
// Note: This is for part 2; it only includes pipes non-horizontal pipes in the path.
std::map<unsigned, std::vector<std::pair<unsigned, Pipe>>> path;
std::map<Pos, Pipe> pipes;  // Map from Pos to Pipe (with all pipes in input).
std::pair<Pos, Pipe> start;  // Pos and Pipe of 'S' character in input.

void setup()
{
  std::ifstream f("10-input.txt");
  if (!f.is_open()) { return; };
  unsigned r = 0;
  for (std::string line; std::getline(f, line); )
  {
    for (unsigned c = 0; c < line.size(); c++)
    {
      try
      {
        pipes[{r, c}] = pipe_map.at(line[c]);
      }
      catch (const std::out_of_range& oor)
      {
        if (line[c] == 'S') { start.first = {r, c}; }
      }
    }
    r++;
  }
  f.close();
}

unsigned explore(Pos cur, Dir from_dir)
{
  try
  {
    // If pipe connects to N or S, add it to the path map (for part 2).
    if (((pipes.at(cur) >> 1) & 1) | ((pipes.at(cur) >> 3) & 1))
    {
      path[cur.first].push_back({cur.second, pipes.at(cur)});
    }
    // Recursively explore the next pipe. Once we've reached the start again,
    // all the returns will happen, counting the number of steps taken.
    switch (pipes.at(cur) & (~from_dir))
    {
      case E: return 1 + explore({cur.first, cur.second + 1}, W);
      case N: return 1 + explore({cur.first - 1, cur.second}, S);
      case S: return 1 + explore({cur.first + 1, cur.second}, N);
      case W: return 1 + explore({cur.first, cur.second - 1}, E);
    }
  } catch (const std::out_of_range& oor) {}
  // If we are here, the current position isn't in the pipes map. This is
  // because we've made it back to the start.
  assert(cur == start.first);
  start.second = Pipe(from_dir);  // The first of the starting pipe's two directions
  return 1;  // This 1 represents the final step.
}

void solve() {
  unsigned p1 = 0;
  unsigned p2 = 0;
  // Try exploring the four tiles adjacent to the start pos, stopping once one succeeds.
  std::set<std::pair<Pos, Dir>> neighbors {{{start.first.first, start.first.second + 1}, W},
                                           {{start.first.first - 1, start.first.second}, S},
                                           {{start.first.first + 1, start.first.second}, N},
                                           {{start.first.first, start.first.second - 1}, E}};
  for (const auto& [neighbor, from_dir]: neighbors)
  {
    try
    {
      if (pipes[neighbor] & from_dir)  // If this neighbor is explorable...
      {
        // P1 answer is half the number of steps in the loop.
        p1 = (unsigned)(explore(neighbor, from_dir) / 2);
        // We successfully explored, so the second dir of the starting pipe is
        // the direction toward which we explored.
        unsigned to_dir = (from_dir >> 2) | (from_dir << 2); // Rotate by 2
        start.second = Pipe(start.second | to_dir);
        break;  // Don't explore any other neighbors.
      }
    } catch (const std::out_of_range& oor) {}
  }
  // Part 2: Calculate number of tiles inside the path.
  path[start.first.first].push_back({start.first.second, start.second});  // Add start to path
  for (auto& [r, row] : path)
  {
    std::sort(row.begin(), row.end());
    bool inside = false;
    for (std::vector<std::pair<unsigned, Pipe>>::iterator it = row.begin(); it < row.end(); it++)
    {
      // If this interval is inside the path, increment the P2 answer accordingly.
      if (inside) { p2 += it->first - (it - 1)->first - 1; }
      // Vertical pipes simply toggle whether we are inside.
      if (it->second == V) { inside = !inside; continue; }
      // This is the first in a pair of angular pipes.
      bool north1 = (it->second >> 1) & 1;
      bool south1 = (it->second >> 3) & 1;
      it++;  // Iterate to the matching pipe.
      bool north2 = (it->second >> 1) & 1;
      bool south2 = (it->second >> 3) & 1;
      // If one of the angle pipes points north and the other points south,
      // toggle whether we are inside or not.
      if ((north1 != north2) || (south1 != south2)) { inside = !inside; }
      // Note: the tiles in the middle of an angular pipe pair are never counted
      // toward the total (regardless of the state of inside) since they would
      // be occuppied by horizontal pipes in the path.
    }
  }
  std::cout << "Part 1: " << p1 << std::endl;
  std::cout << "Part 2: " << p2 << std::endl;
}

int main()
{
  auto start_time = std::chrono::high_resolution_clock::now();

  setup();
  solve();

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
  std::cout << "Clock time: " << duration.count() << " us" << std::endl;

  return 0;
}
