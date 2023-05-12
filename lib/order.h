#ifndef ORDER_H_
#define ORDER_H_

#include <string>
#include <memory>

namespace matching_engine {
class OrderBuilder;

enum class Side { kBuy, kSell };
std::istream &operator>>(std::istream &is, Side &side);

class Order {
private:
  Side side_;
  std::string id_, instrument_;
  unsigned int quantity_, price_, timestamp_;
  bool is_cancelled_;

  friend class OrderBuilder;
  friend class OrderLesserPrice;
  friend class OrderGreaterPrice;
  friend class OrderSmallerTimestampWithSellPriority;
  friend std::ostream &operator<<(std::ostream &, std::shared_ptr<const Order>);

public:
  Order() = default;
  bool operator==(const Order &) const;
  bool operator!=(const Order &) const;
  static OrderBuilder Builder();
  void ReduceQuantity(unsigned int);
  Side GetSide() const;
  const std::string &GetId() const;
  const std::string &GetInstrument() const;
  unsigned int GetQuantity() const;
  unsigned int GetPrice() const;
  bool IsCancelled() const;
  void Cancel();
};

class OrderBuilder {
private:
  std::shared_ptr<Order> order_;

public:
  OrderBuilder();
  OrderBuilder &SetSide(Side);
  OrderBuilder &SetId(std::string &&);
  OrderBuilder &SetInstrument(std::string &&);
  OrderBuilder &SetQuantity(int);
  OrderBuilder &SetPrice(int);
  OrderBuilder &SetTimestamp(int);
  std::shared_ptr<Order> Build();
};

class OrderLesserPrice {
public:
  bool operator()(std::shared_ptr<const Order>, std::shared_ptr<const Order>) const;
};

class OrderGreaterPrice {
public:
  bool operator()(std::shared_ptr<const Order>, std::shared_ptr<const Order>) const;
};

class OrderSmallerTimestampWithSellPriority {
public:
  bool operator()(std::shared_ptr<const Order>, std::shared_ptr<const Order>) const;
};
} // namespace matching_engine
#endif