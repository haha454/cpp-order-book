#include "trade.h"

#include <ostream>
#include <string>

namespace matching_engine {
bool Trade::operator==(const Trade &other) const {
  return order_id_ == other.order_id_ &&
         contra_order_id_ == other.contra_order_id_ &&
         instrument_ == other.instrument_ && quantity_ == other.quantity_ &&
         price_ == other.price_;
}
TradeBuilder Trade::Builder() { return TradeBuilder(); }

TradeBuilder::TradeBuilder() : trade_{} {}
TradeBuilder &TradeBuilder::SetOrderId(const std::string &order_id) {
  trade_.order_id_ = order_id;
  return *this;
}
TradeBuilder &
TradeBuilder::SetContraOrderId(const std::string &contra_order_id) {
  trade_.contra_order_id_ = contra_order_id;
  return *this;
}
TradeBuilder &TradeBuilder::SetInstrument(const std::string &instrument) {
  trade_.instrument_ = instrument;
  return *this;
}
TradeBuilder &TradeBuilder::SetQuantity(unsigned int quantity) {
  trade_.quantity_ = quantity;
  return *this;
}
TradeBuilder &TradeBuilder::SetPrice(unsigned int price) {
  trade_.price_ = price;
  return *this;
}
Trade TradeBuilder::Build() { return std::move(trade_); }

std::ostream &operator<<(std::ostream &os, const Trade &trade) {
  return os << "TRADE " << trade.instrument_ << ' ' << trade.order_id_ << ' '
            << trade.contra_order_id_ << ' ' << trade.quantity_ << ' '
            << trade.price_;
}

} // namespace matching_engine
