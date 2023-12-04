#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <utility>
#include <vector>


std::vector<std::string> setup()
{
  // The input will be read into a vector of strings with a pad of periods
  // around the edges. This pad will make using the parsed input easier to use
  // by allowing the edges to be ignored.
  std::vector<std::string> inp;
  std::ifstream f("3-input.txt");
  std::string line;
  if (f.is_open())
  {
    // Find "width" of input
    std::getline(f, line);
    int width = line.size() + 2;
    f.seekg(0);
    // Add top pad
    inp.push_back(std::string(width, '.'));
    while (std::getline(f, line))
    {
      // Add left and right pad
      inp.push_back("." + line + ".");
    }
    // Add bottom pad
    inp.push_back(std::string(width, '.'));
  }
  f.close();

  return inp;
}


void solve_1(const std::vector<std::string>& inp) {
  int sum = 0;
  std::regex pattern("[0-9]+");
  for (unsigned int r = 1; r < inp.size() - 1; r++)  // For each row
  {
    std::sregex_iterator begin(inp[r].begin(), inp[r].end(), pattern);
    std::sregex_iterator end;  // Default construct
    for (std::sregex_iterator iter = begin; iter != end; iter++)  // For each match in the row
    {
      int start = (*iter).position();
      int length = (*iter).length();
      // Look for symbols in adjacent strings!
      // Note: assuming different numbers are never adjacent, which seems to be
      // the case. This allows us to treat anything other than a period as a
      // symbol.
      std::vector adjacent_strs = {inp[r - 1].substr(start - 1, length + 2),
                                   inp[r + 1].substr(start - 1, length + 2),
                                   inp[r].substr(start - 1, 1),
                                   inp[r].substr(start + length, 1)};
      for (const auto& adjacent_str : adjacent_strs)
      {
        if (adjacent_str != std::string(adjacent_str.length(), '.'))
        {
          // Number by symbol, so count toward sum
          sum += std::stoi((*iter).str());
          break;
        }
      }
    }  // For each match in the row
  }  // For each row
  std::cout << "Part 1: " << sum << std::endl;
}


void solve_2(const std::vector<std::string>& inp)
{
  // Note: I think this solution is needlessly complicated. It would have
  // been better to create a map from '*' character positions to adjacent
  // numbers. Oh well!
  int sum = 0;
  std::regex pattern("[0-9]+");
  // Vector to hold maps, each of which maps the matches in a line to the
  // set of its adjacent coordinates. We only need to handle the current line
  // and the previous two, so older maps will get deleted.
  std::vector<std::map<std::string, std::set<std::pair<int, int>>>> rows;
  rows.push_back(std::map<std::string, std::set<std::pair<int, int>>>());

  for (unsigned int r = 1; r < inp.size() - 1; r++)  // For each row
  {
    std::sregex_iterator begin(inp[r].begin(), inp[r].end(), pattern);
    std::sregex_iterator end;  // Default construct
    for (std::sregex_iterator iter = begin; iter != end; iter++)  // For each match in the row
    {
      int start = (*iter).position();
      int length = (*iter).length();
      // Create set of coordinate tuples adjacent to this match
      std::set<std::pair<int, int>> adjacents = {std::pair<int, int>(r, start - 1),  // Left
                                                 std::pair<int, int>(r, start + length)};  // Right
      for (int j : {r - 1, r + 1})  // Above, below
      {
        for (int i = start - 1; i < start + length + 1; i++)
        {
          adjacents.insert(std::pair<int, int>(j, i));
        }
      }
      // Compare the current match's adjacents with those of each previous match
      // (in this line and the two preceding ones). If the intersection of the
      // adjacents is a '*', a gear has been found.
      // Note: assuming that a '*' is never adjacent to more than two numbers,
      // which seems to be the case.
      for (const std::map<std::string, std::set<std::pair<int, int>>> &row :
           rows) {
        for (const auto& [prev_match, prev_adjacents] : row)
        {
          std::set<std::pair<int, int>> intersect;
          std::set_intersection(prev_adjacents.begin(), prev_adjacents.end(), adjacents.begin(),
                                adjacents.end(), std::inserter(intersect, intersect.begin()));
          for (std::pair<int, int> pos : intersect)
          {
            if (inp[pos.first][pos.second] == '*')
            {
              // Gear found, so increment sum by product of matches
              sum += std::stoi(prev_match) * std::stoi((*iter).str());
            }
          }
        }
      }
      // Update the current row's map entry with this match's adjacents
      rows[0][(*iter).str()] = adjacents;
    }  // For each match in the row
    // Shift rows forward and delete old rows
    rows.insert(rows.begin(), std::map<std::string, std::set<std::pair<int, int>>>());
    if (rows.size() > 3)
    {
      rows.pop_back();
    }
  } // For each row
  std::cout << "Part 2: " << sum << std::endl;
}


int main()
{
  auto start = std::chrono::high_resolution_clock::now();

  std::vector<std::string> inp = setup();
  solve_1(inp);
  solve_2(inp);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Clock time: " << duration.count() << " ms" << std::endl;

  return 0;
}
