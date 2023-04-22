#ifndef ORDER_H_
#define ORDER_H_

#include <string>

namespace matching_engine {
class OrderBuilder;

enum class Side { kBuy, kSell };
std::istream &operator>>(std::istream &is, Side &side);

class Order {
private:
  Side side_;
  std::string id_, instrument_;
  unsigned int quantity_, price_, timestamp_;

  Order() = default;
  friend class OrderBuilder;
  friend class OrderLesserPrice;
  friend class OrderGreaterPrice;
  friend class OrderSmallerTimestampWithSellPriority;
  friend std::ostream &operator<<(std::ostream &, const Order &);

public:
  bool operator==(const Order &) const;
  static OrderBuilder Builder();
  void ReduceQuantity(unsigned int);
  Side GetSide() const;
  const std::string &GetId() const;
  const std::string &GetInstrument() const;
  unsigned int GetQuantity() const;
  unsigned int GetPrice() const;
};

class OrderBuilder {
private:
  Order order_;

public:
  OrderBuilder();
  OrderBuilder &SetSide(Side);
  OrderBuilder &SetId(std::string &&);
  OrderBuilder &SetInstrument(std::string &&);
  OrderBuilder &SetQuantity(int);
  OrderBuilder &SetPrice(int);
  OrderBuilder &SetTimestamp(int);
  Order Build();
};

class OrderLesserPrice {
public:
  bool operator()(const Order &, const Order &) const;
};

class OrderGreaterPrice {
public:
  bool operator()(const Order &, const Order &) const;
};

class OrderSmallerTimestampWithSellPriority {
public:
  bool operator()(const Order &, const Order &) const;
};
} // namespace matching_engine
#endif