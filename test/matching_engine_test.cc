#include "../lib/matching_engine.h"

#include <vector>

#include "../lib/order.h"
#include "../lib/trade.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace matching_engine {
TEST(MatchingEngineTest, MatchOrdersOfDifferentInstruments) {
  MatchingEngine engine;

  engine.Match(Side::kBuy, "some id 11", "some instrument 1", 5, 100);
  engine.Match(Side::kSell, "some id 21", "some instrument 2", 4, 1000);

  EXPECT_EQ(engine.Match(Side::kSell, "some id 16", "some instrument 1", 3, 90),
            (std::vector<Trade>{Trade::Builder()
                                    .SetOrderId("some id 16")
                                    .SetContraOrderId("some id 11")
                                    .SetInstrument("some instrument 1")
                                    .SetQuantity(3)
                                    .SetPrice(100)
                                    .Build()}));
  EXPECT_EQ(
      engine.Match(Side::kBuy, "some id 26", "some instrument 2", 2, 1100),
      (std::vector<Trade>{Trade::Builder()
                              .SetOrderId("some id 26")
                              .SetContraOrderId("some id 21")
                              .SetInstrument("some instrument 2")
                              .SetQuantity(2)
                              .SetPrice(1000)
                              .Build()}));
}

TEST(MatchingEngineTest,
     PurgeOrdersReturnOrdersWithSellPriorityAndSequenceKept) {
  MatchingEngine engine;

  engine.Match(Side::kBuy, "some id 11", "some instrument 1", 5, 100);
  engine.Match(Side::kSell, "some id 21", "some instrument 2", 4, 1000);
  engine.Match(Side::kBuy, "some id 16", "some instrument 1", 1, 99);
  engine.Match(Side::kSell, "some id 26", "some instrument 2", 2, 999);

  EXPECT_EQ(engine.PurgeOrdersSorted(),
            (std::vector<Order>{Order::Builder()
                                    .SetSide(Side::kSell)
                                    .SetId("some id 21")
                                    .SetInstrument("some instrument 2")
                                    .SetQuantity(4)
                                    .SetPrice(1000)
                                    .SetTimestamp(2)
                                    .Build(),
                                Order::Builder()
                                    .SetSide(Side::kSell)
                                    .SetId("some id 26")
                                    .SetInstrument("some instrument 2")
                                    .SetQuantity(2)
                                    .SetPrice(999)
                                    .SetTimestamp(4)
                                    .Build(),
                                Order::Builder()
                                    .SetSide(Side::kBuy)
                                    .SetId("some id 11")
                                    .SetInstrument("some instrument 1")
                                    .SetQuantity(5)
                                    .SetPrice(100)
                                    .SetTimestamp(1)
                                    .Build(),
                                Order::Builder()
                                    .SetSide(Side::kBuy)
                                    .SetId("some id 16")
                                    .SetInstrument("some instrument 1")
                                    .SetQuantity(1)
                                    .SetPrice(99)
                                    .SetTimestamp(3)
                                    .Build()}));
}
} // namespace matching_engine