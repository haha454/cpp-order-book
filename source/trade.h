#ifndef TRADE_H
#define TRADE_H

#include <string>

namespace matching_engine
{
class TradeBuilder;

class Trade
{
  std::string order_id_, contra_order_id_, instrument_;
  unsigned int quantity_{}, price_{};

  Trade() = default;

  friend class TradeBuilder;
  friend auto operator<<(std::ostream&, const Trade&) -> std::ostream&;

public:
  auto operator==(const Trade&) const -> bool;
  static auto Builder() -> TradeBuilder;
};

class TradeBuilder
{
  Trade trade_;

public:
  TradeBuilder() = default;
  auto SetOrderId(const std::string&) -> TradeBuilder&;
  auto SetContraOrderId(const std::string&) -> TradeBuilder&;
  auto SetInstrument(const std::string&) -> TradeBuilder&;
  auto SetQuantity(unsigned int) -> TradeBuilder&;
  auto SetPrice(unsigned int) -> TradeBuilder&;
  auto Build() -> Trade;
};
}  // namespace matching_engine
#endif
