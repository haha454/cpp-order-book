#ifndef ORDER_H_
#define ORDER_H_

#include <memory>
#include <string>

namespace matching_engine
{
class OrderBuilder;

enum class Side : uint8_t
{
  kBuy,
  kSell
};
auto operator>>(std::istream& istream, Side& side) -> std::istream&;

class Order
{
  Side side_{};
  std::string id_, instrument_;
  unsigned int quantity_{}, price_{}, timestamp_{};
  bool is_cancelled_{};

  friend class OrderBuilder;
  friend class OrderLesserPrice;
  friend class OrderGreaterPrice;
  friend class OrderSmallerTimestampWithSellPriority;
  friend auto operator<<(std::ostream&, const std::shared_ptr<const Order>&)
      -> std::ostream&;

public:
  Order() = default;
  auto operator==(const Order&) const -> bool;
  auto operator!=(const Order&) const -> bool;
  static auto Builder() -> OrderBuilder;
  auto ReduceQuantity(unsigned int) -> void;
  auto GetSide() const -> Side;
  auto GetId() const -> const std::string&;
  auto GetInstrument() const -> const std::string&;
  auto GetQuantity() const -> unsigned int;
  auto GetPrice() const -> unsigned int;
  auto IsCancelled() const -> bool;
  auto Cancel() -> void;
};

class OrderBuilder
{
  std::shared_ptr<Order> order_;

public:
  OrderBuilder();
  auto SetSide(Side) -> OrderBuilder&;
  auto SetId(std::string&&) -> OrderBuilder&;
  auto SetInstrument(std::string&&) -> OrderBuilder&;
  auto SetQuantity(unsigned int) -> OrderBuilder&;
  auto SetPrice(unsigned int) -> OrderBuilder&;
  auto SetTimestamp(unsigned int) -> OrderBuilder&;
  auto Build() -> std::shared_ptr<Order>;
};

class OrderLesserPrice
{
public:
  auto operator()(const std::shared_ptr<const Order>&,
                  const std::shared_ptr<const Order>&) const -> bool;
};

class OrderGreaterPrice
{
public:
  auto operator()(const std::shared_ptr<const Order>&,
                  const std::shared_ptr<const Order>&) const -> bool;
};

class OrderSmallerTimestampWithSellPriority
{
public:
  auto operator()(const std::shared_ptr<const Order>&,
                  const std::shared_ptr<const Order>&) const -> bool;
};
}  // namespace matching_engine
#endif
