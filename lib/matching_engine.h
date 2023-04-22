#ifndef MATCHING_ENGINE_H_
#define MATCHING_ENGINE_H_

#include <string>
#include <unordered_map>

#include "order.h"
#include "per_instrument_matching_engine.h"

namespace matching_engine {
class MatchingEngine {
private:
  std::unordered_map<std::string, PerInstrumentMatchingEngine> engines_;
  int order_count_;

  std::vector<Order> PurgeOrders(Side);
public:
  MatchingEngine();
  std::vector<Trade> Match(Side, std::string&& ,std::string&&, unsigned int, unsigned int);
  std::vector<Order> PurgeOrdersSorted();
};
} // namespace matching_engine
#endif