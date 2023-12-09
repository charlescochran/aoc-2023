#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <vector>

int extrapolate(std::vector<int> row)
{
  int back = row.back();  // Save last value in row
  // Replace row with the deltas between each of its elements
  std::adjacent_difference(row.begin(), row.end(), row.begin());
  // If all elements equal, extrapolate that value
  if (std::all_of(row.begin(), row.end(), [](int v) { return v == 0; }))
  {
    return back;
  }
  // Otherwise recurse, ignoring the first delta (which will simply be equal to
  // the first value)
  return back + extrapolate(std::vector<int>(row.begin() + 1, row.end()));
}

void solve()
{
  std::vector<std::vector<int>> rows;
  std::ifstream f("9-input.txt");
  for (std::string line; std::getline(f, line); )
  {
    std::istringstream line_stream(line);
    rows.emplace_back(std::istream_iterator<int>(line_stream), std::istream_iterator<int>());
  }

  int p1 = 0, p2 = 0;
  for (std::vector<int>& row : rows)
  {
    p1 += extrapolate(row);
    std::reverse(row.begin(), row.end());  // Part 2 is not so bad!
    p2 += extrapolate(row);
  }
  std::cout << "Part 1: " << p1 << std::endl;
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
