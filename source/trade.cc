#include <ostream>
#include <string>
#include <utility>

#include "trade.h"

namespace matching_engine {
auto Trade::operator==(const Trade& other) const -> bool
{
  return order_id_ == other.order_id_ &&
         contra_order_id_ == other.contra_order_id_ &&
         instrument_ == other.instrument_ && quantity_ == other.quantity_ &&
         price_ == other.price_;
}
auto Trade::Builder() -> TradeBuilder
{ return {}; }

auto TradeBuilder::SetOrderId(const std::string& order_id) -> TradeBuilder&
{
  trade_.order_id_ = order_id;
  return *this;
}
auto TradeBuilder::SetContraOrderId(const std::string& contra_order_id)
    -> TradeBuilder&
{
  trade_.contra_order_id_ = contra_order_id;
  return *this;
}
auto TradeBuilder::SetInstrument(const std::string& instrument) -> TradeBuilder&
{
  trade_.instrument_ = instrument;
  return *this;
}
auto TradeBuilder::SetQuantity(unsigned int quantity) -> TradeBuilder&
{
  trade_.quantity_ = quantity;
  return *this;
}
auto TradeBuilder::SetPrice(unsigned int price) -> TradeBuilder&
{
  trade_.price_ = price;
  return *this;
}
auto TradeBuilder::Build() -> Trade
{ return std::move(trade_); }

auto operator<<(std::ostream& ostream, const Trade& trade) -> std::ostream&
{
  return ostream << "TRADE " << trade.instrument_ << ' ' << trade.order_id_ << ' '
            << trade.contra_order_id_ << ' ' << trade.quantity_ << ' '
            << trade.price_;
}

} // namespace matching_engine
