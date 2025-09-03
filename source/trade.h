#ifndef TRADE_H
#define TRADE_H

#include <string>
#include <ostream>

namespace matching_engine
{

struct Trade
{
  std::string order_id, contra_order_id, instrument;
  unsigned int quantity {}, price {};

  Trade(std::string order_id,
        std::string contra_order_id,
        std::string instrument,
        unsigned int quantity,
        unsigned int price)
      : order_id(std::move(order_id))
      , contra_order_id(std::move(contra_order_id))
      , instrument(std::move(instrument))
      , quantity(quantity)
      , price(price)
  {
  }

  auto operator==(const Trade& other) const -> bool
  {
    return order_id == other.order_id
        && contra_order_id == other.contra_order_id
        && instrument == other.instrument && quantity == other.quantity
        && price == other.price;
  }
};

inline auto operator<<(std::ostream& ostream, const Trade& trade)
    -> std::ostream&
{
  return ostream << "TRADE " << trade.instrument << ' ' << trade.order_id << ' '
                 << trade.contra_order_id << ' ' << trade.quantity << ' '
                 << trade.price;
}
}  // namespace matching_engine
#endif
