#include <istream>
#include <memory>
#include <ostream>
#include <string>

#include "order.h"

namespace matching_engine
{
auto Order::operator==(const Order& other) const -> bool
{
  return side_ == other.side_ && id_ == other.id_
      && instrument_ == other.instrument_ && quantity_ == other.quantity_
      && price_ == other.price_ && timestamp_ == other.timestamp_;
}

auto Order::operator!=(const Order& other) const -> bool
{
  return !(*this == other);
}

auto Order::Builder() -> OrderBuilder
{
  return {};
}

auto Order::ReduceQuantity(unsigned int delta) -> void
{
  quantity_ -= delta;
}

auto Order::GetSide() const -> Side
{
  return side_;
}

auto Order::GetId() const -> const std::string&
{
  return id_;
}

auto Order::GetInstrument() const -> const std::string&
{
  return instrument_;
}

auto Order::GetQuantity() const -> unsigned int
{
  return quantity_;
}

auto Order::GetPrice() const -> unsigned int
{
  return price_;
}

auto Order::IsCancelled() const -> bool
{
  return is_cancelled_;
}

auto Order::Cancel() -> void
{
  is_cancelled_ = true;
}

OrderBuilder::OrderBuilder()
    : order_ {std::make_shared<Order>()}
{
}

auto OrderBuilder::SetSide(Side side) -> OrderBuilder&
{
  order_->side_ = side;
  return *this;
}

auto OrderBuilder::SetId(std::string&& order_id) -> OrderBuilder&
{
  order_->id_ = std::move(order_id);
  return *this;
}

auto OrderBuilder::SetInstrument(std::string&& instrument) -> OrderBuilder&
{
  order_->instrument_ = std::move(instrument);
  return *this;
}

auto OrderBuilder::SetQuantity(unsigned int quantity) -> OrderBuilder&
{
  order_->quantity_ = quantity;
  return *this;
}

auto OrderBuilder::SetPrice(unsigned int price) -> OrderBuilder&
{
  order_->price_ = price;
  return *this;
}

auto OrderBuilder::SetTimestamp(unsigned int timestamp) -> OrderBuilder&
{
  order_->timestamp_ = timestamp;
  return *this;
}

auto OrderBuilder::Build() -> std::shared_ptr<Order>
{
  return order_;
}

auto OrderLesserPrice::operator()(const std::shared_ptr<const Order>& lhs,
                                  const std::shared_ptr<const Order>& rhs) const
    -> bool
{
  return lhs->price_ < rhs->price_
      || (lhs->price_ == rhs->price_ && lhs->timestamp_ > rhs->timestamp_);
}

auto OrderGreaterPrice::operator()(
    const std::shared_ptr<const Order>& lhs,
    const std::shared_ptr<const Order>& rhs) const -> bool
{
  return lhs->price_ > rhs->price_
      || (lhs->price_ == rhs->price_ && lhs->timestamp_ > rhs->timestamp_);
}

auto OrderSmallerTimestampWithSellPriority::operator()(
    const std::shared_ptr<const Order>& lhs,
    const std::shared_ptr<const Order>& rhs) const -> bool
{
  return (lhs->side_ == Side::kSell && rhs->side_ == Side::kBuy)
      || (lhs->side_ == rhs->side_ && lhs->timestamp_ < rhs->timestamp_);
}

namespace
{
auto operator<<(std::ostream& ostream, const Side& side) -> std::ostream&
{
  return ostream << (side == Side::kBuy ? "BUY" : "SELL");
}
}  // namespace

auto operator<<(std::ostream& ostream,
                const std::shared_ptr<const Order>& order) -> std::ostream&
{
  return ostream << order->id_ << ' ' << order->side_ << ' '
                 << order->instrument_ << ' ' << order->quantity_ << ' '
                 << order->price_;
}

auto operator>>(std::istream& istream, Side& side) -> std::istream&
{
  std::string raw_side;
  istream >> raw_side;
  if ("BUY" == raw_side) {
    side = Side::kBuy;
    return istream;
  }
  if ("SELL" == raw_side) {
    side = Side::kSell;
    return istream;
  }
  istream.setstate(std::ios_base::failbit);
  return istream;
}

}  // namespace matching_engine
