#ifndef PER_INSTRUMENT_MATCHING_ENGINE_H_
#define PER_INSTRUMENT_MATCHING_ENGINE_H_

#include <functional>
#include <memory>
#include <queue>
#include <unordered_map>

#include "order.h"
#include "trade.h"

namespace matching_engine {
class PerInstrumentMatchingEngine {
private:
  std::vector<std::shared_ptr<Order>> buy_orders_;
  std::vector<std::shared_ptr<Order>> sell_orders_;
  std::unordered_map<std::string, std::shared_ptr<Order>> order_id_map_;

  template <typename TargetOrderQueueComp, typename UnmatchedOrderQueueComp,
            typename UnmatchPredicate>
  std::vector<Trade> Match(std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>> &,
                           std::vector<std::shared_ptr<Order>> &);

  template <typename TargetOrderQueueComp> void PopOrder(std::vector<std::shared_ptr<Order>> &);
  template <typename TargetOrderQueueComp>
  void PopCancelledOrder(std::vector<std::shared_ptr<Order>> &);

public:
  std::vector<Trade> Match(std::shared_ptr<Order>);
  std::vector<std::shared_ptr<Order>> PurgeOrders(Side);
  bool Cancel(const std::string &);
};
} // namespace matching_engine
#endif