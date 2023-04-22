#ifndef TRADE_H_
#define TRADE_H_

#include <string>

namespace matching_engine {
class TradeBuilder;

class Trade {
private:
  std::string order_id_, contra_order_id_, instrument_;
  unsigned int quantity_, price_;

  Trade() = default;

  friend class TradeBuilder;
  friend std::ostream &operator<<(std::ostream &, const Trade &);
public:
  bool operator==(const Trade &) const;
  static TradeBuilder Builder();
};

class TradeBuilder {
private:
  Trade trade_;

public:
  TradeBuilder();
  TradeBuilder &SetOrderId(const std::string &);
  TradeBuilder &SetContraOrderId(const std::string &);
  TradeBuilder &SetInstrument(const std::string &);
  TradeBuilder &SetQuantity(unsigned int);
  TradeBuilder &SetPrice(unsigned int);
  Trade Build();
};
} // namespace matching_engine
#endif