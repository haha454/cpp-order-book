#include <memory>
#include <vector>

#include "per_instrument_matching_engine.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "order.h"
#include "order_assert.h"
#include "trade.h"

namespace matching_engine
{
TEST(PerInstrumentMatchingEngineTest, FirstOrderMatchNoTrade)
{
  PerInstrumentMatchingEngine engine;

  ASSERT_THAT(engine.Match(Order::Builder()
                               .SetSide(Side::kBuy)
                               .SetId("some id")
                               .SetInstrument("some instrument")
                               .SetQuantity(5)
                               .SetPrice(1000)
                               .SetTimestamp(99)
                               .Build()),
              testing::IsEmpty());
}

TEST(PerInstrumentMatchingEngineTest, MatchSellOrderWithTheBuyOrderWithHighestPrice)
{
  PerInstrumentMatchingEngine engine;

  engine.Match(Order::Builder()
                   .SetSide(Side::kBuy)
                   .SetId("some id")
                   .SetInstrument("some instrument")
                   .SetQuantity(4)
                   .SetPrice(1000)
                   .SetTimestamp(1)
                   .Build());
  engine.Match(Order::Builder()
                   .SetSide(Side::kBuy)
                   .SetId("some id 2")
                   .SetInstrument("some instrument")
                   .SetQuantity(3)
                   .SetPrice(900)
                   .SetTimestamp(2)
                   .Build());
  engine.Match(Order::Builder()
                   .SetSide(Side::kBuy)
                   .SetId("some id 3")
                   .SetInstrument("some instrument")
                   .SetQuantity(10)
                   .SetPrice(800)
                   .SetTimestamp(3)
                   .Build());

  EXPECT_EQ(engine.Match(Order::Builder()
                             .SetSide(Side::kSell)
                             .SetId("some sell id 1")
                             .SetInstrument("some instrument")
                             .SetQuantity(5)
                             .SetPrice(500)
                             .SetTimestamp(100)
                             .Build()),
            (std::vector<Trade> {Trade::Builder()
                                     .SetOrderId("some sell id 1")
                                     .SetContraOrderId("some id")
                                     .SetInstrument("some instrument")
                                     .SetQuantity(4)
                                     .SetPrice(1000)
                                     .Build(),
                                 Trade::Builder()
                                     .SetOrderId("some sell id 1")
                                     .SetContraOrderId("some id 2")
                                     .SetInstrument("some instrument")
                                     .SetQuantity(1)
                                     .SetPrice(900)
                                     .Build()}));
}

TEST(PerInstrumentMatchingEngineTest,
     MatchBuyOrderWithTheSellerOrderWithLowestPrice)
{
  PerInstrumentMatchingEngine engine;

  engine.Match(Order::Builder()
                   .SetSide(Side::kSell)
                   .SetId("some id")
                   .SetInstrument("some instrument")
                   .SetQuantity(10)
                   .SetPrice(1000)
                   .SetTimestamp(1)
                   .Build());
  engine.Match(Order::Builder()
                   .SetSide(Side::kSell)
                   .SetId("some id 2")
                   .SetInstrument("some instrument")
                   .SetQuantity(3)
                   .SetPrice(900)
                   .SetTimestamp(2)
                   .Build());
  engine.Match(Order::Builder()
                   .SetSide(Side::kSell)
                   .SetId("some id 3")
                   .SetInstrument("some instrument")
                   .SetQuantity(4)
                   .SetPrice(800)
                   .SetTimestamp(3)
                   .Build());

  EXPECT_EQ(engine.Match(Order::Builder()
                             .SetSide(Side::kBuy)
                             .SetId("some buy id 1")
                             .SetInstrument("some instrument")
                             .SetQuantity(5)
                             .SetPrice(2000)
                             .SetTimestamp(100)
                             .Build()),
            (std::vector<Trade> {Trade::Builder()
                                     .SetOrderId("some buy id 1")
                                     .SetContraOrderId("some id 3")
                                     .SetInstrument("some instrument")
                                     .SetQuantity(4)
                                     .SetPrice(800)
                                     .Build(),
                                 Trade::Builder()
                                     .SetOrderId("some buy id 1")
                                     .SetContraOrderId("some id 2")
                                     .SetInstrument("some instrument")
                                     .SetQuantity(1)
                                     .SetPrice(900)
                                     .Build()}));
}

TEST(PerInstrumentMatchingEngineTest, MatchOldestOrderWhenPriceIsTheSame)
{
  PerInstrumentMatchingEngine engine;

  engine.Match(Order::Builder()
                   .SetSide(Side::kSell)
                   .SetId("some id")
                   .SetInstrument("some instrument")
                   .SetQuantity(10)
                   .SetPrice(1000)
                   .SetTimestamp(1)
                   .Build());
  engine.Match(Order::Builder()
                   .SetSide(Side::kSell)
                   .SetId("some id 2")
                   .SetInstrument("some instrument")
                   .SetQuantity(20)
                   .SetPrice(1000)
                   .SetTimestamp(2)
                   .Build());

  EXPECT_EQ(engine.Match(Order::Builder()
                             .SetSide(Side::kBuy)
                             .SetId("some buy id 1")
                             .SetInstrument("some instrument")
                             .SetQuantity(15)
                             .SetPrice(2000)
                             .SetTimestamp(100)
                             .Build()),
            (std::vector<Trade> {Trade::Builder()
                                     .SetOrderId("some buy id 1")
                                     .SetContraOrderId("some id")
                                     .SetInstrument("some instrument")
                                     .SetQuantity(10)
                                     .SetPrice(1000)
                                     .Build(),
                                 Trade::Builder()
                                     .SetOrderId("some buy id 1")
                                     .SetContraOrderId("some id 2")
                                     .SetInstrument("some instrument")
                                     .SetQuantity(5)
                                     .SetPrice(1000)
                                     .Build()}));
}

TEST(PerInstrumentMatchingEngineTest,
     MatchedOrdersMatchTradesMoreContraOrderQuantity)
{
  PerInstrumentMatchingEngine engine;

  engine.Match(Order::Builder()
                   .SetSide(Side::kSell)
                   .SetId("some id")
                   .SetInstrument("some instrument")
                   .SetQuantity(6)
                   .SetPrice(800)
                   .SetTimestamp(99)
                   .Build());

  EXPECT_EQ(engine.Match(Order::Builder()
                             .SetSide(Side::kBuy)
                             .SetId("some id 2")
                             .SetInstrument("some instrument")
                             .SetQuantity(5)
                             .SetPrice(900)
                             .SetTimestamp(100)
                             .Build()),
            (std::vector<Trade> {Trade::Builder()
                                     .SetOrderId("some id 2")
                                     .SetContraOrderId("some id")
                                     .SetInstrument("some instrument")
                                     .SetQuantity(5)
                                     .SetPrice(800)
                                     .Build()}));
}

TEST(PerInstrumentMatchingEngineTest, MatchedOrdersMatchTradesMoreOrderQuantity)
{
  PerInstrumentMatchingEngine engine;

  engine.Match(Order::Builder()
                   .SetSide(Side::kSell)
                   .SetId("some id")
                   .SetInstrument("some instrument")
                   .SetQuantity(5)
                   .SetPrice(800)
                   .SetTimestamp(99)
                   .Build());

  EXPECT_EQ(engine.Match(Order::Builder()
                             .SetSide(Side::kBuy)
                             .SetId("some id 2")
                             .SetInstrument("some instrument")
                             .SetQuantity(6)
                             .SetPrice(900)
                             .SetTimestamp(100)
                             .Build()),
            (std::vector<Trade> {Trade::Builder()
                                     .SetOrderId("some id 2")
                                     .SetContraOrderId("some id")
                                     .SetInstrument("some instrument")
                                     .SetQuantity(5)
                                     .SetPrice(800)
                                     .Build()}));
}

TEST(PerInstrumentMatchingEngineTest, PurgeOrdersReturnRemainingOrders)
{
  PerInstrumentMatchingEngine engine;

  engine.Match(Order::Builder()
                   .SetSide(Side::kBuy)
                   .SetId("some id")
                   .SetInstrument("some instrument")
                   .SetQuantity(5)
                   .SetPrice(500)
                   .SetTimestamp(90)
                   .Build());
  engine.Match(Order::Builder()
                   .SetSide(Side::kBuy)
                   .SetId("some id 2")
                   .SetInstrument("some instrument")
                   .SetQuantity(3)
                   .SetPrice(400)
                   .SetTimestamp(91)
                   .Build());
  engine.Match(Order::Builder()
                   .SetSide(Side::kSell)
                   .SetId("some id 3")
                   .SetInstrument("some instrument")
                   .SetQuantity(3)
                   .SetPrice(900)
                   .SetTimestamp(92)
                   .Build());

  EXPECT_PRED2(AreOrderPointerVectorSame,
               engine.PurgeOrders(Side::kBuy),
               (std::vector<std::shared_ptr<Order>> {
                   Order::Builder()
                       .SetSide(Side::kBuy)
                       .SetId("some id")
                       .SetInstrument("some instrument")
                       .SetQuantity(5)
                       .SetPrice(500)
                       .SetTimestamp(90)
                       .Build(),
                   Order::Builder()
                       .SetSide(Side::kBuy)
                       .SetId("some id 2")
                       .SetInstrument("some instrument")
                       .SetQuantity(3)
                       .SetPrice(400)
                       .SetTimestamp(91)
                       .Build()}));
  EXPECT_PRED2(AreOrderPointerVectorSame,
               engine.PurgeOrders(Side::kSell),
               (std::vector<std::shared_ptr<Order>> {
                   Order::Builder()
                       .SetSide(Side::kSell)
                       .SetId("some id 3")
                       .SetInstrument("some instrument")
                       .SetQuantity(3)
                       .SetPrice(900)
                       .SetTimestamp(92)
                       .Build()}));
}

TEST(PerInstrumentMatchingEngineTest,
     PurgeOrdersReturnNoOrdersWhenAllAreMatched)
{
  PerInstrumentMatchingEngine engine;

  engine.Match(Order::Builder()
                   .SetSide(Side::kBuy)
                   .SetId("some id")
                   .SetInstrument("some instrument")
                   .SetQuantity(5)
                   .SetPrice(1000)
                   .SetTimestamp(90)
                   .Build());
  engine.Match(Order::Builder()
                   .SetSide(Side::kSell)
                   .SetId("some id 3")
                   .SetInstrument("some instrument")
                   .SetQuantity(5)
                   .SetPrice(900)
                   .SetTimestamp(91)
                   .Build());

  EXPECT_THAT(engine.PurgeOrders(Side::kSell), testing::IsEmpty());
  EXPECT_THAT(engine.PurgeOrders(Side::kBuy), testing::IsEmpty());
}

TEST(PerInstrumentMatchingEngineTest, PurgeOrdersAgainReturnNoOrders)
{
  PerInstrumentMatchingEngine engine;

  engine.Match(Order::Builder()
                   .SetSide(Side::kBuy)
                   .SetId("some id")
                   .SetInstrument("some instrument")
                   .SetQuantity(5)
                   .SetPrice(500)
                   .SetTimestamp(99)
                   .Build());
  engine.PurgeOrders(Side::kBuy);

  EXPECT_THAT(engine.PurgeOrders(Side::kSell), testing::IsEmpty());
  EXPECT_THAT(engine.PurgeOrders(Side::kBuy), testing::IsEmpty());
}
}  // namespace matching_engine
