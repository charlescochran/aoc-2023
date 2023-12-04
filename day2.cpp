#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>


// Global regex patterns
const std::vector<std::regex> patterns = {std::regex("([0-9]+) red"),
                                          std::regex("([0-9]+) green"),
                                          std::regex("([0-9]+) blue")};


int calc_score(const std::string& line, int id, int part)
{
  std::vector<int> maximums = {0, 0, 0};  // R, G, B
  if (part == 1)
  {
    // In part 1, the maximums are given.
    maximums = {12, 13, 14};
  }
  // For red, then green, then blue...
  for (unsigned int i = 0; i < patterns.size(); i++)
  {
    std::sregex_iterator begin(line.begin(), line.end(), patterns[i]);
    std::sregex_iterator end;  // default construct
    // Find each of the color values...
    for (std::sregex_iterator iter = begin; iter != end; iter++)
    {
      int val = std::stoi((*iter)[1]);
      // If color val does not exceed maximum for that color, continue.
      if (val < maximums[i])
      {
        continue;
      }
      if (part == 1)
      {
        // In part 1, this game is impossible, so return 0 to avoid incrementing
        // the sum.
        return 0;
      }
      // In part 2, we simply need to update the maximum.
      maximums[i] = val;
    }
  }
  if (part == 1)
  {
    // In part 1, if we made it here, return the game ID.
    return id;
  }
  // In part 2, return the product of the maximums.
  return maximums[0] * maximums[1] * maximums[2];
}


void solve(int part) {
  int sum = 0;
  int id = 0;
  std::ifstream f("2-input.txt");
  std::string line;
  if (f.is_open())
  {
    while (std::getline(f, line))
    {
      sum += calc_score(line, ++id, part);
    }
  }
  f.close();
  std::cout << "Part " << part << ": " << sum << std::endl;
}


int main()
{
  auto start = std::chrono::high_resolution_clock::now();

  solve(1);
  solve(2);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Clock time: " << duration.count() << " us" << std::endl;

  return 0;
}
