#ifndef ORDER_H_
#define ORDER_H_

#include <memory>
#include <string>
#include <istream>

namespace matching_engine
{
enum class Side : uint8_t
{
  kBuy,
  kSell
};

inline auto operator>>(std::istream& istream, Side& side) -> std::istream&
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

struct Order
{
  Order(std::string id,
        std::string instrument,
        unsigned int price,
        unsigned int timestamp,
        unsigned int quantity,
        Side side)
      : id(std::move(id))
      , instrument(std::move(instrument))
      , price(price)
      , timestamp(timestamp)
      , quantity(quantity)
      , side(side)
  {
  }

  std::string id, instrument;
  unsigned int price {}, timestamp {};
  unsigned int quantity {};
  Side side;
  bool is_cancelled {};

  auto operator==(const Order& other) const -> bool
  {
    return side == other.side && id == other.id
        && instrument == other.instrument && quantity == other.quantity
        && price == other.price && timestamp == other.timestamp;
  }

  auto operator!=(const Order& other) const -> bool
  {
    return !(*this == other);
  }

  auto ReduceQuantity(unsigned int delta) -> void { quantity -= delta; }

  auto Cancel() -> void { is_cancelled = false; }
};

class OrderLesserPrice
{
public:
  auto operator()(const std::shared_ptr<const Order>& lhs,
                  const std::shared_ptr<const Order>& rhs) const -> bool
  {
    return lhs->price < rhs->price
        || (lhs->price == rhs->price && lhs->timestamp > rhs->timestamp);
  }
};

class OrderGreaterPrice
{
public:
  auto operator()(const std::shared_ptr<const Order>& lhs,
                  const std::shared_ptr<const Order>& rhs) const -> bool
  {
    return lhs->price > rhs->price
        || (lhs->price == rhs->price && lhs->timestamp > rhs->timestamp);
  }
};

class OrderSmallerTimestampWithSellPriority
{
public:
  auto operator()(const std::shared_ptr<const Order>& lhs,
                  const std::shared_ptr<const Order>& rhs) const -> bool
  {
    return (lhs->side == Side::kSell && rhs->side == Side::kBuy)
        || (lhs->side == rhs->side && lhs->timestamp < rhs->timestamp);
  }
};
}  // namespace matching_engine
#endif
