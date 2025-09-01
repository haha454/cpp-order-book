#ifndef PER_INSTRUMENT_MATCHING_ENGINE_H_
#define PER_INSTRUMENT_MATCHING_ENGINE_H_

#include <functional>
#include <memory>
#include <queue>
#include <unordered_map>

#include "trade.h"
#include "order.h"

namespace matching_engine {
class PerInstrumentMatchingEngine {
private:
  std::vector<std::shared_ptr<Order>> buy_orders_;
  std::vector<std::shared_ptr<Order>> sell_orders_;
  std::unordered_map<std::string, std::shared_ptr<Order>> order_id_map_;

  template <typename TargetOrderQueueComp, typename UnmatchedOrderQueueComp,
            typename UnmatchPredicate>
  auto Match(std::shared_ptr<Order>,
             std::vector<std::shared_ptr<Order>>&,
             std::vector<std::shared_ptr<Order>>&) -> std::vector<Trade>;

  template <typename TargetOrderQueueComp> auto PopOrder(std::vector<std::shared_ptr<Order>>&) -> void;
  template <typename TargetOrderQueueComp>
  auto PopCancelledOrder(std::vector<std::shared_ptr<Order>>&) -> void;

public:
  auto Match(std::shared_ptr<Order>) -> std::vector<Trade>;
  auto PurgeOrders(Side) -> std::vector<std::shared_ptr<Order>>;
  auto Cancel(const std::string&) -> bool;
};
} // namespace matching_engine
#endif