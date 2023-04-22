#ifndef PER_INSTRUMENT_MATCHING_ENGINE_H_
#define PER_INSTRUMENT_MATCHING_ENGINE_H_

#include <functional>
#include <queue>

#include "order.h"
#include "trade.h"

namespace matching_engine {
class PerInstrumentMatchingEngine {
private:
  std::vector<Order> buy_orders_;
  std::vector<Order> sell_orders_;

  template <typename TargetOrderQueueComp, typename UnmatchedOrderQueueComp,
            typename UnmatchPredicate>
  static std::vector<Trade> Match(Order &&, std::vector<Order> &,
                                  std::vector<Order> &);

public:
  std::vector<Trade> Match(Order &&);
  std::vector<Order> PurgeOrders(Side);
};
} // namespace matching_engine
#endif