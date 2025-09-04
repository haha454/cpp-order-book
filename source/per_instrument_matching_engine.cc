#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include "per_instrument_matching_engine.h"

#include "order.h"
#include "trade.h"

namespace matching_engine
{

template<typename TargetOrderQueueComp>
auto PerInstrumentMatchingEngine::PopOrder(
    std::vector<std::shared_ptr<Order>>& target_orders) -> void
{
  assert(!target_orders.empty());
  std::pop_heap(
      target_orders.begin(), target_orders.end(), TargetOrderQueueComp());
  assert(order_id_map_.erase(target_orders.back()->id));
  target_orders.pop_back();
}

template<typename TargetOrderQueueComp>
auto PerInstrumentMatchingEngine::PopCancelledOrder(
    std::vector<std::shared_ptr<Order>>& target_orders) -> void
{
  while (!target_orders.empty() && target_orders.front()->is_cancelled) {
    PopOrder<TargetOrderQueueComp>(target_orders);
  }
}

template<typename TargetOrderQueueComp,
         typename UnmatchedOrderQueueComp,
         typename UnmatchPredicate>
auto PerInstrumentMatchingEngine::Match(
    const std::shared_ptr<Order>& order,
    std::vector<std::shared_ptr<Order>>& target_orders,
    std::vector<std::shared_ptr<Order>>& unmatched_orders) -> std::vector<Trade>
{
  std::vector<Trade> trades;
  PopCancelledOrder<TargetOrderQueueComp>(target_orders);
  while (!target_orders.empty() && order->quantity
         && !UnmatchPredicate()(order, target_orders.front()))
  {
    auto& contra_order = target_orders.front();
    auto trade_quantity = std::min(order->quantity, contra_order->quantity);
    trades.emplace_back(order->id,
                        contra_order->id,
                        order->instrument,
                        trade_quantity,
                        contra_order->price);
    order->ReduceQuantity(trade_quantity);
    contra_order->ReduceQuantity(trade_quantity);
    if (!contra_order->quantity) {
      PopOrder<TargetOrderQueueComp>(target_orders);
    }
    PopCancelledOrder<TargetOrderQueueComp>(target_orders);
  }

  if (order->quantity) {
    unmatched_orders.push_back(order);
    order_id_map_[unmatched_orders.back()->id] = order;
    std::push_heap(unmatched_orders.begin(),
                   unmatched_orders.end(),
                   UnmatchedOrderQueueComp());
  }

  return trades;
}

auto PerInstrumentMatchingEngine::Match(const std::shared_ptr<Order>& order)
    -> std::vector<Trade>
{
  switch (order->side) {
    case Side::kBuy:
      return Match<OrderGreaterPrice, OrderLesserPrice, OrderLesserPrice>(
          order, sell_orders_, buy_orders_);
    case Side::kSell:
      return Match<OrderLesserPrice, OrderGreaterPrice, OrderGreaterPrice>(
          order, buy_orders_, sell_orders_);
  }
  std::cerr << "unknown side" << '\n';
  return {};
}

auto PerInstrumentMatchingEngine::PurgeOrders(Side side)
    -> std::vector<std::shared_ptr<Order>>
{
  switch (side) {
    case Side::kBuy:
      return std::move(buy_orders_);
    case Side::kSell:
      return std::move(sell_orders_);
  }
  std::cerr << "unknown side" << '\n';
  return {};
}

auto PerInstrumentMatchingEngine::Cancel(const std::string& order_id) -> bool
{
  if (!order_id_map_.contains(order_id)) {
    return false;
  }

  order_id_map_[order_id]->Cancel();
  return true;
}
}  // namespace matching_engine
