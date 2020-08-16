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
    EXPECT_EQ(ThreadChoice10RSStrong1StockSet(&market), (UINT)101);
    EXPECT_TRUE(market.IsUpdateOptionDB());
    EXPECT_EQ(market.GetUpdatedDayFor10DayRS1(), market.GetFormatedMarketDay());
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
    EXPECT_EQ(ThreadChoice10RSStrong2StockSet(&market), (UINT)102);
    EXPECT_TRUE(market.IsUpdateOptionDB());
    EXPECT_EQ(market.GetUpdatedDayFor10DayRS2(), market.GetFormatedMarketDay());
  }

  class CThreadChoice10RSStrongStockSet : public ::testing::Test
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
    CRSReference RSReference;
  };

  TEST_F(CThreadChoice10RSStrongStockSet, TestThreadChoice10RSStrongStockSet) {
    size_t lInformationSize = gl_systemMessage.GetInformationDequeSize();
    EXPECT_CALL(market, Choice10RSStrongStockSet(&RSReference, 2))
      .Times(1)
      .WillOnce(Return(true));
    EXPECT_EQ(ThreadChoice10RSStrongStockSet(&market, &RSReference, 2), (UINT)103);
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), lInformationSize + 2);
  }
}