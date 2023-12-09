#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>  // std::lcm()
#include <set>
#include <sstream>
#include <string>
#include <vector>

void solve()
{
  // Map from source string to pair of dest strings: (i.e. "AAA" -> ("BBB", "CCC"))
  std::map<std::string, std::pair<std::string, std::string>> network;
  std::string instructions;  // Instructions string (i.e. "RLLRRLRRR")
  std::set<std::string> nodes;  // Which (unfinished) nodes we are currently on
  std::set<std::string> next;  // Which (unfinished) nodes we will be on next step
  std::set<unsigned> counts; // How many steps it took each node to reach its goal
  unsigned c = 0;  // Counter

  std::ifstream f("8-input.txt");
  if (!f.is_open()) { return; }
  std::getline(f, instructions);  // Read the instructions string
  f.ignore(10000, '\n');  // Throw away blank line in input
  for (std::string line; std::getline(f, line); )
  {
    std::string src, dest1, dest2;
    std::stringstream line_stream(line);
    line_stream >> src >> dest1 >> dest1 >> dest2;  // dest1 initially gets "="
    network[src] = {dest1.substr(1, dest1.length() - 2), dest2.substr(0, dest2.length() - 1)};
    if (src[2] == 'A') { nodes.insert(src); }  // Find starting nodes
  }
  f.close();

  while (nodes.size() > 0)
  {
    for (std::string node : nodes)
    {
      if (node[2] == 'Z')  // This node has reached its goal, so record the count
      {
        // Part 1: if we've reached ZZZ, assume it was from AAA and print count
        if (node == "ZZZ")
        {
          std::cout << "Part 1: " << c << std::endl;
        }
        counts.insert(c);
      }
      else  // This node has not reached its goal, so follow the network
      {
        char instruction = instructions[c % instructions.size()];
        next.insert((instruction == 'L') ? network[node].first : network[node].second);
      }
    }
    nodes = std::move(next);
    next.clear();
    c++;
  }
  // Part 2: output the LCM of the recorded counts
  unsigned long p2 = 1;
  for (unsigned count : counts) { p2 = std::lcm(p2, count); }
  std::cout << "Part 2: " << p2 << std::endl;
}

int main()
{
  auto start = std::chrono::high_resolution_clock::now();

  solve();

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Clock time: " << duration.count() << " us" << std::endl;

  return 0;
}
