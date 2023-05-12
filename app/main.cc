#include <stdio.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../lib/matching_engine.h"
#include "../lib/order.h"

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
  for (auto const &element : v) {
    os << element << std::endl;
  }

  return os;
}

int main() {
  std::string line, order_id, instrument;

  matching_engine::Side side;
  unsigned int quantity, price;
  matching_engine::MatchingEngine matching_engine;
  while (std::getline(std::cin, line)) {
    if (std::all_of(line.begin(), line.end(), isspace)) {
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
