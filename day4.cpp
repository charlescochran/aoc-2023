#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <ratio>
#include <regex>
#include <string>
#include <vector>


void solve() {
  int p1 = 0;
  int p2 = 0;
  std::ifstream f("4-input.txt");
  std::string line;
  if (f.is_open())
  {
    int card = 0;  // Which card we are on (but zero-indexed)
    // How many copies of each card we have (originally 1 of each). Will grow as
    // we read the input. Starting size is 10 because we the current card can
    // affect up to 10 future cards.
    std::vector<int> copies(10, 1);

    while (std::getline(f, line)) {
      copies.push_back(1);  // Grow the copies list
      int matches = 0;
      std::vector<std::string> winning;
      std::string num;
      std::istringstream line_stream(line);
      std::getline(line_stream, num, ':');  // Throw away "Card X:"
      // Count the number of matches
      while(std::getline(line_stream, num, ' '))
      {
        if (num.empty())
        {
          continue;
        }
        if (std::find(winning.begin(), winning.end(), num) != winning.end())
        {
          matches += 1;
        }
        else if (winning.size() < 10)
        {
          winning.push_back(num);
        }
      }
      // Increment the copies of the cards we've won
      auto start = copies.begin() + card + 1;
      auto end = std::min(start + matches, copies.end());
      std::transform(start, end, start, [&copies, card](int n) { return n + copies[card]; });

      p1 += pow(2, matches - 1);
      p2 += copies[card];
      card++;
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
  std::cout << "Clock time: " << duration.count() << " us" << std::endl;

  return 0;
}
