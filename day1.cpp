#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


using strings = std::vector<std::string>;


int check_pos(std::string line, int pos, strings digits, strings words)
{
  for (const strings& check : {digits, words})
  {
    for (unsigned int i = 0; i < check.size(); i++)
    {
      if (line.substr(pos, check[i].length()) == check[i])
      {
        return i;
      }
    }
  }
  return -1;
}


void solve(int part) {
  int sum = 0;
  std::ifstream f("1-input.txt");
  std::string line;
  if (f.is_open())
  {
    if (part == 1)
    {
      while (std::getline(f, line))
      {
        sum += 10 * (line[line.find_first_of("1234567890")] - '0');
        sum += line[line.find_last_of("1234567890")] - '0';
      }
    }
    else
    {
      strings digits = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
      strings words = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight",
                       "nine"};
      while (std::getline(f, line))
      {
        for (unsigned int pos = 0; pos < line.length(); pos++)
        {
          int val = check_pos(line, pos, digits, words);
          if (val > -1)
          {
            sum += 10 * val;
            break;
          }
        }
        for (unsigned int pos = line.length() - 1; pos >= 0; pos--)
        {
          int val = check_pos(line, pos, digits, words);
          if (val > -1)
          {
            sum += val;
            break;
          }
        }
      }
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
  std::cout << "Clock time: " << duration.count() << " ms" << std::endl;

  return 0;
}
