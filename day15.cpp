#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Entry = std::pair<std::string, int>;
using Box = std::vector<Entry>;
using Map = std::vector<Box>;

unsigned hash(std::string key) {
  unsigned val = 0;
  for (const char& c : key) {
    val = ((val + c) * 17) % 256;
  }
  return val;
}

void update(bool remove, Map& map, const std::string& label, int val=0) {
  int idx = hash(label);
  auto entry = std::find_if(map[idx].begin(),
                            map[idx].end(),
                            [label](const Entry& e) { return e.first == label; });
  if (entry == map[idx].end()) {
    // The entry is not in the map
    if (remove) { return; }
    map[idx].emplace_back(label, val);
  } else {
    // The entry is in the map
    if (remove) {
      map[idx].erase(entry);
    } else {
      entry->second = val;
    }
  }
}

void solve() {
  unsigned p1 = 0, p2 = 0;
  Map map(256, Box());
  std::ifstream f("15-input.txt");
  for (std::string step; std::getline(f, step, ','); ) {
    if (step.back() == '\n') { step.pop_back(); }  // Remove '\n' from final step
    p1 += hash(step);  // Part 1: just accumulate the step's hash value
    if (step.back() == '-') {
      // Remove the label from the map
      update(true, map, step.substr(0, step.length() - 1));
    } else {
      // Add/update the label with the given value (assuming single digit)
      update(false, map, step.substr(0, step.length() - 2), step.back() - '0');
    }
  }
  // Calculate the focusing power
  for (unsigned box = 0; box < map.size(); box++) {
    for (unsigned slot = 0; slot < map[box].size(); slot++) {
      p2 += (box + 1) * (slot + 1) * map[box][slot].second;
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
