#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "matching_engine.h"
#include "order.h"

namespace
{
template<typename T>
auto operator<<(std::ostream& ostream, const std::vector<T>& vec)
    -> std::ostream&
{
  for (auto const& element : vec) {
    ostream << element << std::endl;
  }

  return ostream;
}
}  // namespace

auto main() -> int
{
  std::string line;
  std::string order_id;
  std::string instrument;

  matching_engine::Side side {};
  unsigned int quantity {};
  unsigned int price {};
  matching_engine::MatchingEngine matching_engine;
  while (std::getline(std::cin, line)) {
    if (std::ranges::all_of(line, isspace)) {
      std::cerr << "skip the input line as it contains all spaces" << std::endl;
      continue;
    }

    std::istringstream iss(line);
    std::string instruction;
    iss >> instruction;
    if (instruction == "CANCEL") {
      iss >> instrument >> order_id;
      matching_engine.Cancel(instrument, order_id);
    } else {
      order_id = instruction;
      iss >> side >> instrument >> quantity >> price;
      std::cout << matching_engine.Match(
          side, std::move(order_id), std::move(instrument), quantity, price);
    }
  }

  std::cout << std::endl << matching_engine.PurgeOrdersSorted();

  return 0;
}
