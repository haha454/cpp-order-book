#include "order.h"

#include <istream>
#include <memory>
#include <ostream>
#include <string>

namespace matching_engine {
bool Order::operator==(const Order &other) const {
  return side_ == other.side_ && id_ == other.id_ &&
         instrument_ == other.instrument_ && quantity_ == other.quantity_ &&
         price_ == other.price_ && timestamp_ == other.timestamp_;
}
bool Order::operator!=(const Order &other) const { return !(*this == other); }
OrderBuilder Order::Builder() { return OrderBuilder(); }
void Order::ReduceQuantity(unsigned int delta) { quantity_ -= delta; }
Side Order::GetSide() const { return side_; }
const std::string &Order::GetId() const { return id_; }
const std::string &Order::GetInstrument() const { return instrument_; }
unsigned int Order::GetQuantity() const { return quantity_; }
unsigned int Order::GetPrice() const { return price_; }
bool Order::IsCancelled() const { return is_cancelled_; }
void Order::Cancel() { is_cancelled_ = true; }

OrderBuilder::OrderBuilder() : order_{std::make_shared<Order>()} {}
OrderBuilder &OrderBuilder::SetSide(Side side) {
  order_->side_ = side;
  return *this;
}
OrderBuilder &OrderBuilder::SetId(std::string &&id) {
  order_->id_ = std::move(id);
  return *this;
}
OrderBuilder &OrderBuilder::SetInstrument(std::string &&instrument) {
  order_->instrument_ = std::move(instrument);
  return *this;
}
OrderBuilder &OrderBuilder::SetQuantity(int quantity) {
  order_->quantity_ = quantity;
  return *this;
}
OrderBuilder &OrderBuilder::SetPrice(int price) {
  order_->price_ = price;
  return *this;
}
OrderBuilder &OrderBuilder::SetTimestamp(int timestamp) {
  order_->timestamp_ = timestamp;
  return *this;
}
std::shared_ptr<Order> OrderBuilder::Build() { return order_; }

bool OrderLesserPrice::operator()(std::shared_ptr<const Order> lhs,
                                  std::shared_ptr<const Order> rhs) const {
  return lhs->price_ < rhs->price_ ||
         (lhs->price_ == rhs->price_ && lhs->timestamp_ > rhs->timestamp_);
}

bool OrderGreaterPrice::operator()(std::shared_ptr<const Order> lhs,
                                   std::shared_ptr<const Order> rhs) const {
  return lhs->price_ > rhs->price_ ||
         (lhs->price_ == rhs->price_ && lhs->timestamp_ > rhs->timestamp_);
}

bool OrderSmallerTimestampWithSellPriority::operator()(
    std::shared_ptr<const Order> lhs, std::shared_ptr<const Order> rhs) const {
  return (lhs->side_ == Side::kSell && rhs->side_ == Side::kBuy) ||
         (lhs->side_ == rhs->side_ && lhs->timestamp_ < rhs->timestamp_);
}

std::ostream &operator<<(std::ostream &os, const Side &side) {
  return os << (side == matching_engine::Side::kBuy ? "BUY" : "SELL");
}

std::ostream &operator<<(std::ostream &os, std::shared_ptr<const Order> order) {
  return os << order->id_ << ' ' << order->side_ << ' ' << order->instrument_
            << ' ' << order->quantity_ << ' ' << order->price_;
}

std::istream &operator>>(std::istream &is, Side &side) {
  std::string raw_side;
  is >> raw_side;
  if ("BUY" == raw_side) {
    side = Side::kBuy;
    return is;
  } else if ("SELL" == raw_side) {
    side = Side::kSell;
    return is;
  } else {
    is.setstate(std::ios_base::failbit);
    return is;
  }
}

} // namespace matching_engine
