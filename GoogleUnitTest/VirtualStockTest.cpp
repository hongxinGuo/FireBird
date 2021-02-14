#include"pch.h"

#include"globedef.h"
#include"VirtualStock.h"

namespace StockAnalysisTest {
  class CVirtualStockTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
    }

    static void TearDownTestSuite(void) {
      while (gl_ThreadStatus.IsWorkingThreadRunning()) Sleep(1);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
    }
  };

  TEST_F(CVirtualStockTest, TestIsTodayNewStock) {
    CVirtualStock stock;

    EXPECT_FALSE(stock.IsTodayNewStock());
    stock.SetTodayNewStock(true);
    EXPECT_TRUE(stock.IsTodayNewStock());
    stock.SetTodayNewStock(false);
    EXPECT_FALSE(stock.IsTodayNewStock());
  }
  TEST_F(CVirtualStockTest, TestIsUpdateStockProfileDB) {
    CVirtualStock stock;

    EXPECT_FALSE(stock.IsUpdateStockProfileDB());
    stock.SetUpdateStockProfileDB(true);
    EXPECT_TRUE(stock.IsUpdateStockProfileDB());
    stock.SetUpdateStockProfileDB(false);
    EXPECT_FALSE(stock.IsUpdateStockProfileDB());
  }

}