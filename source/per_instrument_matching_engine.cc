#include <functional>
#include <iostream>
#include <memory>
#include <queue>

#include "per_instrument_matching_engine.h"

#include <cassert>

#include "trade.h"
#include "order.h"

namespace matching_engine {

template <typename TargetOrderQueueComp>
auto PerInstrumentMatchingEngine::PopOrder(
    std::vector<std::shared_ptr<Order>> &target_orders) -> void {
  assert(target_orders.size());
  std::pop_heap(target_orders.begin(), target_orders.end(),
                TargetOrderQueueComp());
  assert(order_id_map_.erase(target_orders.back()->GetId()));
  target_orders.pop_back();
}

template <typename TargetOrderQueueComp>
auto PerInstrumentMatchingEngine::PopCancelledOrder(
    std::vector<std::shared_ptr<Order>> &target_orders) -> void {
  while (!target_orders.empty() && target_orders.front()->IsCancelled()) {
    PopOrder<TargetOrderQueueComp>(target_orders);
  }
}

template <typename TargetOrderQueueComp, typename UnmatchedOrderQueueComp,
          typename UnmatchPredicate>
auto PerInstrumentMatchingEngine::Match(
    std::shared_ptr<Order> order,
    std::vector<std::shared_ptr<Order>> &target_orders,
    std::vector<std::shared_ptr<Order>> &unmatched_orders) -> std::vector<Trade> {
  std::vector<Trade> trades;
  PopCancelledOrder<TargetOrderQueueComp>(target_orders);
  while (!target_orders.empty() && order->GetQuantity() &&
         !UnmatchPredicate()(order, target_orders.front())) {
    auto &contra_order = target_orders.front();
    auto trade_quantity =
        std::min(order->GetQuantity(), contra_order->GetQuantity());
    trades.push_back(Trade::Builder()
                         .SetOrderId(order->GetId())
                         .SetContraOrderId(contra_order->GetId())
                         .SetInstrument(order->GetInstrument())
                         .SetQuantity(trade_quantity)
                         .SetPrice(contra_order->GetPrice())
                         .Build());
    order->ReduceQuantity(trade_quantity);
    contra_order->ReduceQuantity(trade_quantity);
    if (!contra_order->GetQuantity()) {
      PopOrder<TargetOrderQueueComp>(target_orders);
    }
    PopCancelledOrder<TargetOrderQueueComp>(target_orders);
  }

  if (order->GetQuantity()) {
    unmatched_orders.push_back(order);
    order_id_map_[unmatched_orders.back()->GetId()] = order;
    std::push_heap(unmatched_orders.begin(), unmatched_orders.end(),
                   UnmatchedOrderQueueComp());
  }

  return trades;
}

auto PerInstrumentMatchingEngine::Match(std::shared_ptr<Order> order) -> std::vector<Trade> {
  switch (order->GetSide()) {
  case Side::kBuy:
    return Match<OrderGreaterPrice, OrderLesserPrice, OrderLesserPrice>(
        std::move(order), sell_orders_, buy_orders_);
  case Side::kSell:
    return Match<OrderLesserPrice, OrderGreaterPrice, OrderGreaterPrice>(
        std::move(order), buy_orders_, sell_orders_);
  }
  std::cerr << "unknown side" << '\n';
  return {};
}

auto PerInstrumentMatchingEngine::PurgeOrders(Side side) -> std::vector<std::shared_ptr<Order>> {
  switch (side) {
  case Side::kBuy:
    return std::move(buy_orders_);
  case Side::kSell:
    return std::move(sell_orders_);
  }
  std::cerr << "unknown side" << '\n';
  return {};
}

auto PerInstrumentMatchingEngine::Cancel(const std::string &order_id) -> bool {
  if (!order_id_map_.contains(order_id)) {
    return false;
  }

  order_id_map_[order_id]->Cancel();
  return true;
}
} // namespace matching_engine
