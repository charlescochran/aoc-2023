#include <algorithm>  // std::reverse
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

unsigned hamming(const std::string& str1, const std::string& str2)
{
  // Return the Hamming distance between the input strings (the number of
  // characters that differ).
  unsigned dist = 0;
  for (unsigned i = 0; i < str1.size(); i++)
  {
    dist += (str1[i] != str2[i]);
  }
  return dist;
}

std::vector<unsigned> reflection_score(const std::vector<std::string>& orig)
{
  std::vector<unsigned> scores {0, 0};  // {P1, P2}
  std::vector<std::string> strings = orig; // Create a copy of the input strings
  for (bool reversed : {false, true})
  {
    // A reflection begins with the first string (string[0]) matching an
    // odd-indexed string.
    for (unsigned j = 1; j < strings.size(); j += 2)
    {
      int smudges = 1 - hamming(strings[0], strings[j]);  // How many smudges remaining?
      if (smudges < 0) { continue; }  // This string doesn't match string[0], so move on.
      // We've identified a potential reflection. Now, do all the strings in
      // between string[0] and string[j] match their counterpart?
      unsigned radius = (j + 1) / 2;  // Half the width of the reflection
      bool reflection = true;
      for (unsigned k = 1; k < radius; k++)
      {
        smudges -= hamming(strings[k], strings[j - k]);
        if (smudges < 0)
        {
          reflection = false; // Out of smudges. Not a reflection!
          break;
        }
      }
      if (reflection)
      {
        // If we have a smudge left, this is a P1 reflection, so write to scores[0].
        // If we don't, this is a P2 reflection, so write to scores[1].
        scores[(smudges + 1) % 2] += reversed ? strings.size() - radius : radius;
      }
    }
    if (reversed) { return scores; }
    // If we make it here, no reflection was found the first time... Reverse the
    // order of the strings and try again.
    std::reverse(strings.begin(), strings.end());
  }
  assert(false);
}

void solve()
{
  std::ifstream f("13-input.txt");
  if (!f.is_open()) { return; };

  unsigned p1 = 0, p2 = 0;
  std::vector<std::string> rows, cols;
  std::string line;
  while (true)
  {
    std::getline(f, line);
    if (line.empty()) {
      // We've finished a pattern, so do the calculations.
      std::vector<unsigned> horizontal = reflection_score(rows);
      std::vector<unsigned> vertical = reflection_score(cols);
      p1 += 100 * horizontal[0] + vertical[0];
      p2 += 100 * horizontal[1] + vertical[1];
      if (f.eof()) { break; }
      rows.clear();
      cols.clear();
    }
    else
    {
      // We've are in a pattern, so add this line to the rows and cols vectors.
      rows.push_back(line);
      if (cols.size() == 0) { cols.resize(line.size()); }
      for (unsigned c = 0; c < line.size(); c++) { cols[c] += line[c]; }
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
