#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

enum CardType {HIGH=1, ONE=2, TWO=3, THREE=4, FULL=5, FOUR=6, FIVE=7};
using Hand = std::tuple<std::string, CardType, int>;  // cards, CardType, bid

CardType classify(std::string cards)
{
  // Remove any part 2 jokers (represented by '1')
  cards.erase(std::remove(cards.begin(), cards.end(), '1'), cards.end());
  int jokers = 5 - cards.length();  // Number of jokers
  std::map<char, int> counts;
  // How many copies of the most frequent card are there?
  for (char c : cards) { counts[c]++; }
  int most = std::max_element(counts.begin(),
                              counts.end(),
                              [](auto a, auto b) { return a.second < b.second; })->second;
  // How many unique cards are there?
  switch(std::set(cards.begin(), cards.end()).size())
  {
    case 5: return HIGH;
    case 4: return ONE;
    case 3: return most + jokers == 3 ? THREE : TWO;
    case 2: return most + jokers == 4 ? FOUR : FULL;
    default: return FIVE;  // case 1 (all cards are the same) or 0 (all cards were jokers)
  }
}

bool compare(Hand a, Hand b)
{
  // If CardTypes are the same, lexographically sort based on card strings...
  if (std::get<1>(a) == std::get<1>(b)) { return std::get<0>(a) < std::get<0>(b); }
  // Otherwise, sort based on CardType
  return std::get<1>(a) < std::get<1>(b);
}

void solve(int part) {
  // Parse input
  std::vector<Hand> hands;
  std::ifstream f("7-input.txt");
  if (!f.is_open()) { return; };
  for (std::string line; std::getline(f, line); )
  {
    std::stringstream line_stream(line);
    std::string cards;
    int bid;
    line_stream >> cards >> bid;
    // Make string lexographical compare work properly
    // Note: jokers are weak in part 2, so use '1' instead of 'b'
    std::replace(cards.begin(), cards.end(), 'T', 'a');
    std::replace(cards.begin(), cards.end(), 'J', part == 2 ? '1' : 'b');
    std::replace(cards.begin(), cards.end(), 'Q', 'c');
    std::replace(cards.begin(), cards.end(), 'K', 'd');
    std::replace(cards.begin(), cards.end(), 'A', 'e');
    hands.emplace_back(cards, classify(cards), bid);
  }
  // Sort hands with custom comparator
  std::sort(hands.begin(), hands.end(), compare);
  // Calculate answer
  int sum = 0;
  for (unsigned i = 0; i < hands.size(); i++)
  {
    sum += std::get<2>(hands[i]) * (i + 1);
  }
  std::cout << "Part " << part << ": " << sum << std::endl;
}

int main()
{
  auto start = std::chrono::high_resolution_clock::now();

  solve(1);
  solve(2);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Clock time: " << duration.count() << " us" << std::endl;

  return 0;
}
