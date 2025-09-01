#include <algorithm>
#include <iostream>
#include <vector>

#include "matching_engine.h"

#include "order.h"

namespace matching_engine {
MatchingEngine::MatchingEngine() : order_count_{0} {}

auto MatchingEngine::Match(Side side,
                           std::string&& id,
                           std::string&& instrument,
                           unsigned int quantity,
                           unsigned int price) -> std::vector<Trade>
{
  if (!quantity) {
    return {};
  }
  order_count_++;
  auto &engine = engines_[instrument];
  return engine.Match(Order::Builder()
                          .SetSide(side)
                          .SetId(std::move(id))
                          .SetInstrument(std::move(instrument))
                          .SetQuantity(quantity)
                          .SetPrice(price)
                          .SetTimestamp(order_count_)
                          .Build());
}

auto MatchingEngine::PurgeOrders(Side side)
    -> std::vector<std::shared_ptr<Order>>
{
  std::vector<std::shared_ptr<Order>> result;
  for (auto &[_, engine] : engines_) {
    std::vector<std::shared_ptr<Order>> engine_orders =
        engine.PurgeOrders(side);
    result.insert(result.end(), std::make_move_iterator(engine_orders.begin()),
                  std::make_move_iterator(engine_orders.end()));
  }

  return result;
}

auto MatchingEngine::PurgeOrdersSorted() -> std::vector<std::shared_ptr<Order>>
{
  auto result = PurgeOrders(Side::kSell);
  auto buy_orders = PurgeOrders(Side::kBuy);
  result.insert(result.end(), std::make_move_iterator(buy_orders.begin()),
                std::make_move_iterator(buy_orders.end()));

  std::sort(result.begin(), result.end(),
            OrderSmallerTimestampWithSellPriority());

  return result;
}

auto MatchingEngine::Cancel(const std::string& instrument,
                            const std::string& order_id) -> bool
{
  if (!engines_.count(instrument)) {
    return false;
  }

  return engines_[instrument].Cancel(order_id);
}

} // namespace matching_engine