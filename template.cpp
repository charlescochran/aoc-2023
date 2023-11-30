#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>


std::tuple<std::vector<std::vector<int>>> setup()
{
  std::vector<std::vector<int>> inp;
  std::ifstream f("X-input.txt");
  std::string line;
  if (f.is_open())
  {
    while (std::getline(f, line))
    {
      std::vector<int> row;
      std::string item;
      std::istringstream line_stream(line);
      while(std::getline(line_stream, item, ' '))
      {
        row.push_back(std::stoi(item));
      }
      inp.push_back(row);
    }
  }
  f.close();

  // Check inp read correctly...
  for (const auto& row : inp)
  {
    for (const auto& item : row)
    {
      std::cout << item << " ";
    }
    std::cout << std::endl;
  }

  return inp;
}


void solve(std::vector<std::vector<int>> inp) {
  int p1 = 0;
  int p2 = 0;
  std::cout << "Part 1: " << p1 << std::endl;
  std::cout << "Part 2: " << p2 << std::endl;
}


int main()
{
  auto start = std::chrono::high_resolution_clock::now();

  auto [inp] = setup();
  solve(inp);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Clock time: " << duration.count() << " ms" << std::endl;

  return 0;
}
