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

  TEST_F(CVirtualStockTest, TestIsUpdateStockProfileDBAndClearFlag) {
    CVirtualStock stock;

    EXPECT_FALSE(stock.IsUpdateStockProfileDB());
    EXPECT_FALSE(stock.IsUpdateStockProfileDBAndClearFlag());
    stock.SetUpdateStockProfileDB(true);
    EXPECT_TRUE(stock.IsUpdateStockProfileDBAndClearFlag());
    EXPECT_FALSE(stock.IsUpdateStockProfileDB());
  }

  TEST_F(CVirtualStockTest, TestIsActive) {
    CVirtualStock stock;
    EXPECT_FALSE(stock.IsActive());
    stock.SetActive(true);
    EXPECT_TRUE(stock.IsActive());
    stock.SetActive(false);
    EXPECT_FALSE(stock.IsActive());
  }

  TEST_F(CVirtualStockTest, TestGetDayLineEndDate) {
    CVirtualStock stock;
    EXPECT_EQ(stock.GetDayLineEndDate(), 19000101);
    stock.SetDayLineEndDate(19980101);
    EXPECT_EQ(stock.GetDayLineEndDate(), 19980101);
  }

  TEST_F(CVirtualStockTest, TestGetDayLineStartDate) {
    CVirtualStock stock;
    EXPECT_EQ(stock.GetDayLineStartDate(), 29900101);
    stock.SetDayLineStartDate(19980101);
    EXPECT_EQ(stock.GetDayLineStartDate(), 19980101);
  }

  TEST_F(CVirtualStockTest, TestGetIPOStatus) {
    CVirtualStock stock;
    EXPECT_TRUE(stock.IsNotChecked());
    stock.SetIPOStatus(255);
    EXPECT_EQ(stock.GetIPOStatus(), 255);
  }

  TEST_F(CVirtualStockTest, TestIsNullStock) {
    CVirtualStock stock;
    stock.SetIPOStatus(__STAKE_NULL__);
    EXPECT_TRUE(stock.IsNullStock());
    stock.SetIPOStatus(__STAKE_NOT_CHECKED__);
    EXPECT_FALSE(stock.IsNullStock());
  }

  TEST_F(CVirtualStockTest, TestIsIPOed) {
    CVirtualStock stock;
    stock.SetIPOStatus(__STAKE_IPOED__);
    EXPECT_TRUE(stock.IsIPOed());
    stock.SetIPOStatus(__STAKE_NOT_CHECKED__);
    EXPECT_FALSE(stock.IsIPOed());
  }

  TEST_F(CVirtualStockTest, TestIsNotChecked) {
    CChinaStock stock;
    stock.SetIPOStatus(__STAKE_NOT_CHECKED__);
    EXPECT_TRUE(stock.IsNotChecked());
    stock.SetIPOStatus(__STAKE_DELISTED__);
    EXPECT_FALSE(stock.IsNotChecked());
  }

  TEST_F(CVirtualStockTest, TestIsDelisted) {
    CVirtualStock stock;
    stock.SetIPOStatus(__STAKE_DELISTED__);
    EXPECT_TRUE(stock.IsDelisted());
    stock.SetIPOStatus(__STAKE_NOT_CHECKED__);
    EXPECT_FALSE(stock.IsDelisted());
  }

  TEST_F(CVirtualStockTest, TestIsDayLineNeedUpdate) {
    CVirtualStock stock;
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    stock.SetDayLineNeedUpdate(false);
    EXPECT_FALSE(stock.IsDayLineNeedUpdate());
    stock.SetDayLineNeedUpdate(true);
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
  }

  TEST_F(CVirtualStockTest, TestIsDayLineNeedProcess) {
    CVirtualStock stock;
    EXPECT_FALSE(stock.IsDayLineNeedProcess());
    stock.SetDayLineNeedProcess(true);
    EXPECT_TRUE(stock.IsDayLineNeedProcess());
    stock.SetDayLineNeedProcess(false);
    EXPECT_FALSE(stock.IsDayLineNeedProcess());
  }

  TEST_F(CVirtualStockTest, TestIsDayLineNeededSaving) {    // 此两个函数是具备同步机制的，这里没有进行测试
    CVirtualStock stock;
    EXPECT_FALSE(stock.IsDayLineNeedSaving());
    stock.SetDayLineNeedSaving(true);
    EXPECT_TRUE(stock.IsDayLineNeedSaving());
    stock.SetDayLineNeedSaving(false);
    EXPECT_FALSE(stock.IsDayLineNeedSaving());
    stock.SetDayLineNeedSaving(true);
    EXPECT_TRUE(stock.IsDayLineNeedSavingAndClearFlag());
    EXPECT_FALSE(stock.IsDayLineNeedSaving());
  }


}