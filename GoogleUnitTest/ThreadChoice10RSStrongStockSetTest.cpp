#include"pch.h"

#include"globedef.h"
#include"MockChinaMarket.h"

#include"thread.h"

using namespace testing;

namespace StockAnalysisTest {
  class CThreadChoice10RSStrong1StockSet : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
    }

    static void TearDownTestSuite(void) {
    }

    virtual void SetUp(void) override {
    }

    virtual void TearDown(void) override {
    }
    CMockChinaMarket market;
  };

  TEST_F(CThreadChoice10RSStrong1StockSet, TestThreadChoice10RSStrong1StockSet) {
    EXPECT_CALL(market, Choice10RSStrong1StockSet())
      .Times(1)
      .WillOnce(Return(true));
    EXPECT_EQ(ThreadChoice10RSStrong1StockSet(&market), (UINT)24);
    EXPECT_TRUE(market.IsUpdateOptionDB());
    EXPECT_EQ(market.GetUpdatedDayFor10DayRS1(), market.GetDay());
  }

  class CThreadChoice10RSStrong2StockSet : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
    }

    static void TearDownTestSuite(void) {
    }

    virtual void SetUp(void) override {
    }

    virtual void TearDown(void) override {
    }
    CMockChinaMarket market;
  };

  TEST_F(CThreadChoice10RSStrong2StockSet, TestThreadChoice10RSStrong2StockSet) {
    EXPECT_CALL(market, Choice10RSStrong2StockSet())
      .Times(1)
      .WillOnce(Return(true));
    EXPECT_EQ(ThreadChoice10RSStrong2StockSet(&market), (UINT)23);
    EXPECT_TRUE(market.IsUpdateOptionDB());
    EXPECT_EQ(market.GetUpdatedDayFor10DayRS2(), market.GetDay());
  }
}