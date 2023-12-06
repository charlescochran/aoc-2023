#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


double quadform(long a, long b, long c)
{
  // Return the smaller of the two quadform solutions
  return (b - std::sqrt(std::pow(b, 2) - 4 * a * c)) / (2 * a);
}


void solve() {
  long p1 = 1, p2 = 1;
  // Parse the input
  std::vector<std::vector<long>> inp;
  std::ifstream f("6-input.txt");
  if (!f.is_open()) { return; };
  for (std::string line; std::getline(f, line); )
  {
    std::string val;
    inp.push_back(std::vector<long>());
    std::istringstream line_stream(line);
    for (std::string item; std::getline(line_stream, item, ' '); )
    {
      try
      {
        inp.back().push_back(std::stol(item));
        val += item;
      }
      catch (const std::invalid_argument& ex) {}
    }
    inp.back().push_back(std::stol(val));
  }
  // The input provides the total_time and distance.
  //           (1) velocity = charge_time
  //           (2) discharge_time = total_time - charge_time
  //           (3) distance = velocity * discharge_time
  // Substituting: distance = charge_time * (total_time - charge_time)
  //  Rearranging: charge_time^2 - (total_time * charge_time) + distance = 0

  // The two solutions of this (found using the quadratic equation) with the
  // input data represent the two charge times that will exactly tie the record
  // distance; everything in between them will beat it. We will call the
  // smaller of the two solutions the record_charge_time.

  // There are total_time + 1 possible charge times, of which 2 *
  // (record_charge_time + 1) do not beat the record. Subtracting, we find the
  // number that do.
  for (unsigned i = 0; i < inp[0].size(); i++)
  {
    double record_charge_time = quadform(1, inp[0][i], inp[1][i]);
    if (i < inp[0].size() - 1)
    {
      p1 *= inp[0][i] + 1 - (2 * (std::floor(record_charge_time + 1)));
    }
    else
    {
      p2 = inp[0][i] + 1 - (2 * (std::floor(record_charge_time + 1)));
    }
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
