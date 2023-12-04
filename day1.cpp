#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>


using strings = const std::vector<std::string>;


void solve() {
  int p1 = 0;
  int p2 = 0;
  const std::string digits = "0123456789";
  strings words = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
  std::ifstream f("1-input.txt");
  std::string line;
  if (f.is_open())
  {
    while (std::getline(f, line))
    {
      // Map from position to its associated integer value
      std::map<size_t, unsigned int> positions;
      // Add the first and last digits to the map, if found
      size_t first_digit = line.find_first_of(digits);
      size_t last_digit = line.find_last_of(digits);
      // If first_digit is found, second_digit must also be found
      if (first_digit != std::string::npos)
      {
        positions[first_digit] = line[first_digit] - '0';
        positions[last_digit] = line[last_digit] - '0';
        // Increment part 1 answer
        p1 += 10 * positions[first_digit] + positions[last_digit];
      }
      // Add each word's position to the map, if found
      for (unsigned int i = 0; i < words.size(); i++)
      {
        size_t pos = line.find(words[i]);
        while (pos != std::string::npos)
        {
          positions[pos] = i;
          pos = line.find(words[i], pos + 1);
        }
      }
      // Increment part 2 answer. Note that maps are sorted by key, so
      // positions.begin()->second gives the value associated with the smallest
      // position, while positions.rbegin()->second gives the value of the
      // largest position.
      p2 += 10 * positions.begin()->second + positions.rbegin()->second;
    }
  }
  f.close();
  std::cout << "Part 1: " << p1 << std::endl;
  std::cout << "Part 2: " << p2 << std::endl;
}


int main()
{
  auto start = std::chrono::high_resolution_clock::now();

  solve();

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Clock time: " << duration.count() << " ms" << std::endl;

  return 0;
}
