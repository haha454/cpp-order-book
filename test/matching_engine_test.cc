#include <memory>
#include <vector>

#include "matching_engine.h"

#include "gtest/gtest.h"
#include "order.h"
#include "order_assert.h"
#include "trade.h"

namespace matching_engine
{
TEST(MatchingEngineTest, MatchOrdersOfDifferentInstruments)
{
  MatchingEngine engine;

  engine.Match(Side::kBuy, "some id 11", "some instrument 1", 5, 100);
  engine.Match(Side::kSell, "some id 21", "some instrument 2", 4, 1000);

  EXPECT_EQ(engine.Match(Side::kSell, "some id 16", "some instrument 1", 3, 90),
            (std::vector<Trade> {
                {"some id 16", "some id 11", "some instrument 1", 3, 100}}));
  EXPECT_EQ(
      engine.Match(Side::kBuy, "some id 26", "some instrument 2", 2, 1100),
      (std::vector<Trade> {
          {"some id 26", "some id 21", "some instrument 2", 2, 1000}}));
}

TEST(MatchingEngineTest, PurgeOrdersReturnOrdersWithSellPriorityAndSequenceKept)
{
  MatchingEngine engine;

  engine.Match(Side::kBuy, "some id 11", "some instrument 1", 5, 100);
  engine.Match(Side::kSell, "some id 21", "some instrument 2", 4, 1000);
  engine.Match(Side::kBuy, "some id 16", "some instrument 1", 1, 99);
  engine.Match(Side::kSell, "some id 26", "some instrument 2", 2, 999);

  EXPECT_PRED2(
      AreOrderPointerVectorSame,
      engine.PurgeOrdersSorted(),
      (std::vector {
          std::make_shared<Order>(
              "some id 21", "some instrument 2", 1000, 2, 4, Side::kSell),
          std::make_shared<Order>(
              "some id 26", "some instrument 2", 999, 4, 2, Side::kSell),
          std::make_shared<Order>(
              "some id 11", "some instrument 1", 100, 1, 5, Side::kBuy),
          std::make_shared<Order>(
              "some id 16", "some instrument 1", 99, 3, 1, Side::kBuy)}));
}
}  // namespace matching_engine
