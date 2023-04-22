#include "per_instrument_matching_engine.h"

#include <functional>
#include <iostream>
#include <queue>
#include <stdexcept>

#include "order.h"
#include "trade.h"

namespace matching_engine {
template <typename TargetOrderQueueComp, typename UnmatchedOrderQueueComp,
          typename UnmatchPredicate>
std::vector<Trade>
PerInstrumentMatchingEngine::Match(Order &&order,
                                   std::vector<Order> &target_orders,
                                   std::vector<Order> &unmatched_orders) {
  std::vector<Trade> trades;
  while (!target_orders.empty() && order.GetQuantity() &&
         !UnmatchPredicate()(order, target_orders.front())) {
    auto &contra_order = target_orders.front();
    auto trade_quantity =
        std::min(order.GetQuantity(), contra_order.GetQuantity());
    trades.push_back(Trade::Builder()
                         .SetOrderId(order.GetId())
                         .SetContraOrderId(contra_order.GetId())
                         .SetInstrument(order.GetInstrument())
                         .SetQuantity(trade_quantity)
                         .SetPrice(contra_order.GetPrice())
                         .Build());
    order.ReduceQuantity(trade_quantity);
    contra_order.ReduceQuantity(trade_quantity);
    if (!contra_order.GetQuantity()) {
      std::pop_heap(target_orders.begin(), target_orders.end(),
                    TargetOrderQueueComp());
      target_orders.pop_back();
    }
  }

  if (order.GetQuantity()) {
    unmatched_orders.push_back(std::move(order));
    std::push_heap(unmatched_orders.begin(), unmatched_orders.end(),
                   UnmatchedOrderQueueComp());
  }

  return trades;
}

std::vector<Trade> PerInstrumentMatchingEngine::Match(Order &&order) {
  switch (order.GetSide()) {
  case Side::kBuy:
    return Match<OrderGreaterPrice, OrderLesserPrice, OrderLesserPrice>(
        std::move(order), sell_orders_, buy_orders_);
  case Side::kSell:
    return Match<OrderLesserPrice, OrderGreaterPrice, OrderGreaterPrice>(
        std::move(order), buy_orders_, sell_orders_);
  }
  std::cerr << "unknown side" << std::endl;
  return {};
}

std::vector<Order> PerInstrumentMatchingEngine::PurgeOrders(Side side) {
  switch (side) {
  case Side::kBuy:
    return std::move(buy_orders_);
  case Side::kSell:
    return std::move(sell_orders_);
  }
  std::cerr << "unknown side" << std::endl;
  return {};
}
} // namespace matching_engine