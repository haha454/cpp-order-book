// test/per_instrument_matching_engine_test.cc
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

  ASSERT_THAT(engine.Match(std::make_shared<Order>(
                  "some id", "some instrument", 1000, 99, 5, Side::kBuy)),
              testing::IsEmpty());
}

TEST(PerInstrumentMatchingEngineTest,
     MatchSellOrderWithTheBuyOrderWithHighestPrice)
{
  PerInstrumentMatchingEngine engine;

  engine.Match(std::make_shared<Order>(
      "some id", "some instrument", 1000, 1, 4, Side::kBuy));
  engine.Match(std::make_shared<Order>(
      "some id 2", "some instrument", 900, 2, 3, Side::kBuy));
  engine.Match(std::make_shared<Order>(
      "some id 3", "some instrument", 800, 3, 10, Side::kBuy));

  EXPECT_EQ(engine.Match(std::make_shared<Order>(
                "some sell id 1", "some instrument", 500, 100, 5, Side::kSell)),
            (std::vector<Trade> {
                {"some sell id 1", "some id", "some instrument", 4, 1000},
                {"some sell id 1", "some id 2", "some instrument", 1, 900}}));
}

TEST(PerInstrumentMatchingEngineTest,
     MatchBuyOrderWithTheSellerOrderWithLowestPrice)
{
  PerInstrumentMatchingEngine engine;

  engine.Match(std::make_shared<Order>(
      "some id", "some instrument", 1000, 1, 10, Side::kSell));
  engine.Match(std::make_shared<Order>(
      "some id 2", "some instrument", 900, 2, 3, Side::kSell));
  engine.Match(std::make_shared<Order>(
      "some id 3", "some instrument", 800, 3, 4, Side::kSell));

  EXPECT_EQ(engine.Match(std::make_shared<Order>(
                "some buy id 1", "some instrument", 2000, 100, 5, Side::kBuy)),
            (std::vector<Trade> {
                {"some buy id 1", "some id 3", "some instrument", 4, 800},
                {"some buy id 1", "some id 2", "some instrument", 1, 900}}));
}

TEST(PerInstrumentMatchingEngineTest, MatchOldestOrderWhenPriceIsTheSame)
{
  PerInstrumentMatchingEngine engine;

  engine.Match(std::make_shared<Order>(
      "some id", "some instrument", 1000, 1, 10, Side::kSell));
  engine.Match(std::make_shared<Order>(
      "some id 2", "some instrument", 1000, 2, 20, Side::kSell));

  EXPECT_EQ(engine.Match(std::make_shared<Order>(
                "some buy id 1", "some instrument", 2000, 100, 15, Side::kBuy)),
            (std::vector<Trade> {
                {"some buy id 1", "some id", "some instrument", 10, 1000},
                {"some buy id 1", "some id 2", "some instrument", 5, 1000}}));
}

TEST(PerInstrumentMatchingEngineTest,
     MatchedOrdersMatchTradesMoreContraOrderQuantity)
{
  PerInstrumentMatchingEngine engine;

  engine.Match(std::make_shared<Order>(
      "some id", "some instrument", 800, 99, 6, Side::kSell));

  EXPECT_EQ(engine.Match(std::make_shared<Order>(
                "some id 2", "some instrument", 900, 100, 5, Side::kBuy)),
            (std::vector<Trade> {
                {"some id 2", "some id", "some instrument", 5, 800}}));
}

TEST(PerInstrumentMatchingEngineTest, MatchedOrdersMatchTradesMoreOrderQuantity)
{
  PerInstrumentMatchingEngine engine;

  engine.Match(std::make_shared<Order>(
      "some id", "some instrument", 800, 99, 5, Side::kSell));

  EXPECT_EQ(engine.Match(std::make_shared<Order>(
                "some id 2", "some instrument", 900, 100, 6, Side::kBuy)),
            (std::vector<Trade> {
                {"some id 2", "some id", "some instrument", 5, 800}}));
}

TEST(PerInstrumentMatchingEngineTest, PurgeOrdersReturnRemainingOrders)
{
  PerInstrumentMatchingEngine engine;

  engine.Match(std::make_shared<Order>(
      "some id", "some instrument", 500, 90, 5, Side::kBuy));
  engine.Match(std::make_shared<Order>(
      "some id 2", "some instrument", 400, 91, 3, Side::kBuy));
  engine.Match(std::make_shared<Order>(
      "some id 3", "some instrument", 900, 92, 3, Side::kSell));

  EXPECT_PRED2(
      AreOrderPointerVectorSame,
      engine.PurgeOrders(Side::kBuy),
      (std::vector<std::shared_ptr<Order>> {
          std::make_shared<Order>(
              "some id", "some instrument", 500, 90, 5, Side::kBuy),
          std::make_shared<Order>(
              "some id 2", "some instrument", 400, 91, 3, Side::kBuy)}));
  EXPECT_PRED2(AreOrderPointerVectorSame,
               engine.PurgeOrders(Side::kSell),
               (std::vector<std::shared_ptr<Order>> {std::make_shared<Order>(
                   "some id 3", "some instrument", 900, 92, 3, Side::kSell)}));
}

TEST(PerInstrumentMatchingEngineTest,
     PurgeOrdersReturnNoOrdersWhenAllAreMatched)
{
  PerInstrumentMatchingEngine engine;

  engine.Match(std::make_shared<Order>(
      "some id", "some instrument", 1000, 90, 5, Side::kBuy));
  engine.Match(std::make_shared<Order>(
      "some id 3", "some instrument", 900, 91, 5, Side::kSell));

  EXPECT_THAT(engine.PurgeOrders(Side::kSell), testing::IsEmpty());
  EXPECT_THAT(engine.PurgeOrders(Side::kBuy), testing::IsEmpty());
}

TEST(PerInstrumentMatchingEngineTest, PurgeOrdersAgainReturnNoOrders)
{
  PerInstrumentMatchingEngine engine;

  engine.Match(std::make_shared<Order>(
      "some id", "some instrument", 500, 99, 5, Side::kBuy));
  engine.PurgeOrders(Side::kBuy);

  EXPECT_THAT(engine.PurgeOrders(Side::kSell), testing::IsEmpty());
  EXPECT_THAT(engine.PurgeOrders(Side::kBuy), testing::IsEmpty());
}
}  // namespace matching_engine
