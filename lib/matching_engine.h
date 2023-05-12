#ifndef MATCHING_ENGINE_H_
#define MATCHING_ENGINE_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "order.h"
#include "per_instrument_matching_engine.h"

namespace matching_engine {
class MatchingEngine {
private:
  std::unordered_map<std::string, PerInstrumentMatchingEngine> engines_;
  int order_count_;

  std::vector<std::shared_ptr<Order>> PurgeOrders(Side);

public:
  MatchingEngine();
  std::vector<Trade> Match(Side, std::string &&, std::string &&, unsigned int,
                           unsigned int);
  std::vector<std::shared_ptr<Order>> PurgeOrdersSorted();
  bool Cancel(const std::string &, const std::string &);
};
} // namespace matching_engine
#endif