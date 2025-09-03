#include <algorithm>
#include <memory>
#include <ranges>
#include <string>
#include <utility>
#include <vector>
#include <iterator>

#include "matching_engine.h"
#include <trade.h>
#include "order.h"

namespace matching_engine
{
MatchingEngine::MatchingEngine()
    : order_count_ {0}
{
}

auto MatchingEngine::Match(Side side,
                           std::string order_id,
                           std::string instrument,
                           unsigned int quantity,
                           unsigned int price) -> std::vector<Trade>
{
  if (quantity == 0U) {
    return {};
  }
  order_count_++;
  auto& engine = engines_[instrument];
  return engine.Match(std::make_shared<Order>(std::move(order_id),
                                              std::move(instrument),
                                              price,
                                              order_count_,
                                              quantity,
                                              side));
}

auto MatchingEngine::PurgeOrders(Side side)
    -> std::vector<std::shared_ptr<Order>>
{
  std::vector<std::shared_ptr<Order>> result;
  for (auto& engine : engines_ | std::views::values) {
    std::vector<std::shared_ptr<Order>> engine_orders =
        engine.PurgeOrders(side);
    result.insert(result.end(),
                  std::make_move_iterator(engine_orders.begin()),
                  std::make_move_iterator(engine_orders.end()));
  }

  return result;
}

auto MatchingEngine::PurgeOrdersSorted() -> std::vector<std::shared_ptr<Order>>
{
  auto result = PurgeOrders(Side::kSell);
  auto buy_orders = PurgeOrders(Side::kBuy);
  result.insert(result.end(),
                std::make_move_iterator(buy_orders.begin()),
                std::make_move_iterator(buy_orders.end()));

  std::ranges::sort(result, OrderSmallerTimestampWithSellPriority());

  return result;
}

auto MatchingEngine::Cancel(const std::string& instrument,
                            const std::string& order_id) -> bool
{
  if (!engines_.contains(instrument)) {
    return false;
  }

  return engines_[instrument].Cancel(order_id);
}

}  // namespace matching_engine
