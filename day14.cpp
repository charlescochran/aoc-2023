#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

enum Direction {N = 0, W = 1, S = 2, E = 3};

// Rock: {r, c, is_square}
using Rock = std::tuple<unsigned, unsigned, bool>;
using Rocks = std::set<Rock>;
// Group: {coordinate, is_square}
// A Group represents either a row or col of Rocks
using Group = std::vector<std::pair<unsigned, bool>>;
// State: {rock_positions_hash, load}
using State = std::pair<unsigned, unsigned>;

Rocks rocks;             // The rock positions. Updated as rocks move.
unsigned height, width;  // Height/width of input; calculated in setup().

void setup()
{
  std::ifstream f("14-input.txt");
  if (!f.is_open()) { return; };

  std::string line;
  std::getline(f, line);
  width = line.size();
  f.seekg(0);
  unsigned r = 0;
  while (std::getline(f, line))
  {
    for (unsigned c = 0; c < line.size(); c++)
    {
      switch (line[c])
      {
        case 'O': rocks.emplace(r, c, false); break;
        case '#': rocks.emplace(r, c, true); break;
      }
    }
    r++;
  }
  height = r;
  f.close();
}

Group tilt_group(Group& in, bool reverse, unsigned len)
{
  // Return a new Group with positions found by "tilting" the given one. If
  // reverse is true, move positions up instead of down. The len parameter
  // represents the theoretical length of the group.
  Group out;
  // If reverse is true, reverse the Group before starting to make sure Rocks
  // are processed in the correct order.
  if (reverse) { std::reverse(in.begin(), in.end()); }

  for (const auto& [j, is_square] : in)
  {
    // Square rocks simply stay in the same place.
    if (is_square) {
      out.emplace_back(j, is_square);
    }
    // The first rock in the group is round; it will move as far as possible.
    else if (out.empty()) {
      out.emplace_back(reverse ? len - 1 : 0, false);
    }
    // Put this round rock next to the last one.
    else
    {
      out.emplace_back(out.rbegin()->first + (reverse ? -1 : 1), false);
    }
  }
  // If necessary, undo the original reversal.
  if (reverse) { std::reverse(out.begin(), out.end()); }

  return out;
}

Rocks tilt(const Rocks& in, Direction dir)
{
  // Return a new set of rock positions, found by "tilting" the given one in the
  // provided direction.
  // Note: RVO should prevent expensive copy-on-return.
  Rocks out;
  if (dir == N || dir == S)
  {
    for (unsigned col = 0; col < width; col++)
    {
      Group group;  // Form a group from the column.
      for (const auto& [r, c, is_square] : in)
      {
        if (c == col) { group.emplace_back(r, is_square); }
      }
      // Tilt the group and put the results in the output set.
      for (const auto& [j, is_square] : tilt_group(group, dir == S, height))
      {
        out.emplace(j, col, is_square);
      }
    }
  }
  else
  {
    for (unsigned row = 0; row < height; row++)
    {
      Group group;  // Form a group from the row.
      for (const auto& [r, c, is_square] : in)
      {
        if (r == row) { group.emplace_back(c, is_square); }
      }
      // Tilt the group and put the results in the output set.
      for (const auto& [j, is_square] : tilt_group(group, dir == E, width))
      {
        out.emplace(row, j, is_square);
      }
    }
  }
  return out;
}

unsigned calc_load(const Rocks& in)
{
  // Calculate the load from the given rock positions.
  unsigned sum = 0;
  for (const auto& [r, c, is_square] : in)
  {
    if (!is_square) {
      sum += height - r;
    }
  }
  return sum;
}

unsigned hash_rocks(const Rocks& in) {
  // Convert the given rock positions into a number, faciliating comparison with
  // other rock positions.
  unsigned hash = 0;
  for (const auto& [r, c, is_square] : in)
  {
    // Ignore square rocks since they never move.
    if (is_square) { continue; }
    hash += c + width * r;
  }
  return hash;
}

void solve()
{
  // Part 1: tilt the rocks north and calculate the load.
  rocks = tilt(rocks, N);
  unsigned p1 = calc_load(rocks);

  // Part 2: Cyclically tilt the Rocks N, W, S, and E, memoizing the results and
  // looking for a repeated pattern.
  unsigned p2;
  std::vector<State> States;
  // Don't worry; we will break out of this loop!
  for (unsigned cycle = 0; cycle < 1000000000; cycle++)
  {
    // Perform one cycle of tilts.
    rocks = tilt(rocks, N);
    rocks = tilt(rocks, W);
    rocks = tilt(rocks, S);
    rocks = tilt(rocks, E);
    // Compare the resulting Rock positions to the previous ones.
    unsigned hash = hash_rocks(rocks);
    const auto& found = std::find_if(States.begin(),
                                     States.end(),
                                     [hash](const State& state) { return state.first == hash; });
    // If a match is found, we've identified a loop...
    if (found != States.end()) {
      unsigned loop_start = (found - States.begin());
      unsigned loop_len = cycle - loop_start;
      // Extrapolating, we see that the final load value must equal one inside
      // the loop. Report it as the answer!
      p2 = States[loop_start + ((1000000000 - loop_start - 1) % loop_len)].second;
      break;
    }
    // Otherwise, save this State and continue with the next cycle.
    States.emplace_back(hash, calc_load(rocks));
  }

  std::cout << "Part 1: " << p1 << std::endl;
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
