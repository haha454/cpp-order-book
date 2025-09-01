#ifndef MATCHING_ENGINE_H_
#define MATCHING_ENGINE_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "order.h"
#include "per_instrument_matching_engine.h"

namespace matching_engine {
class MatchingEngine {

  std::unordered_map<std::string, PerInstrumentMatchingEngine> engines_;
  unsigned int order_count_;

  auto PurgeOrders(Side) -> std::vector<std::shared_ptr<Order>>;

public:
  MatchingEngine();
  auto Match(Side, std::string&&, std::string&&, unsigned int, unsigned int)
      -> std::vector<Trade>;
  auto PurgeOrdersSorted() -> std::vector<std::shared_ptr<Order>>;
  auto Cancel(const std::string&, const std::string&) -> bool;
};
} // namespace matching_engine
#endif