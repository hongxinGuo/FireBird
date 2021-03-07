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

  TEST_F(CVirtualStockTest, TestGetSymbol) {
    CVirtualStock stock;
    EXPECT_STREQ(stock.GetSymbol(), _T(""));
    stock.SetSymbol(_T("600000.SS"));
    EXPECT_STREQ(stock.GetSymbol(), _T("600000.SS"));
  }

  TEST_F(CVirtualStockTest, TestGetTransactionTime) {
    CVirtualStock stock;
    EXPECT_EQ(stock.GetTransactionTime(), 0);
    stock.SetTransactionTime(1010101010);
    EXPECT_EQ(stock.GetTransactionTime(), 1010101010);
  }

  TEST_F(CVirtualStockTest, TestGetLastClose) {
    CVirtualStock stock;
    EXPECT_EQ(stock.GetLastClose(), 0);
    stock.SetLastClose(10101010);
    EXPECT_EQ(stock.GetLastClose(), 10101010);
  }

  TEST_F(CVirtualStockTest, TestGetOpen) {
    CVirtualStock stock;
    EXPECT_EQ(stock.GetOpen(), 0);
    stock.SetOpen(10101);
    EXPECT_EQ(stock.GetOpen(), 10101);
  }

  TEST_F(CVirtualStockTest, TestGetHigh) {
    CVirtualStock stock;
    EXPECT_EQ(stock.GetHigh(), 0);
    stock.SetHigh(19980101);
    EXPECT_EQ(stock.GetHigh(), 19980101);
  }

  TEST_F(CVirtualStockTest, TestGetLow) {
    CVirtualStock stock;
    EXPECT_EQ(stock.GetLow(), 0);
    stock.SetLow(19980101);
    EXPECT_EQ(stock.GetLow(), 19980101);
  }

  TEST_F(CVirtualStockTest, TestGetNew) {
    CVirtualStock stock;
    EXPECT_EQ(stock.GetNew(), 0);
    stock.SetNew(10101010);
    EXPECT_EQ(stock.GetNew(), 10101010);
  }

  TEST_F(CVirtualStockTest, TestGetUpDown) {
    CVirtualStock stock;
    EXPECT_EQ(stock.GetUpDown(), 0);
    stock.SetUpDown(10101010);
    EXPECT_EQ(stock.GetUpDown(), 10101010);
  }

  TEST_F(CVirtualStockTest, TestGetAmount) {
    CVirtualStock stock;
    EXPECT_EQ(stock.GetAmount(), 0);
    stock.SetAmount(1010101010101010);
    EXPECT_EQ(stock.GetAmount(), 1010101010101010);
  }

  TEST_F(CVirtualStockTest, TestGetVolume) {
    CVirtualStock stock;
    EXPECT_EQ(stock.GetVolume(), 0);
    stock.SetVolume(10101010);
    EXPECT_EQ(stock.GetVolume(), 10101010);
  }

  TEST_F(CVirtualStockTest, TestGetTotalValue) {
    CVirtualStock stock;
    EXPECT_EQ(stock.GetTotalValue(), 0);
    stock.SetTotalValue(10101010);
    EXPECT_EQ(stock.GetTotalValue(), 10101010);
  }

  TEST_F(CVirtualStockTest, TestGetCurrentValue) {
    CVirtualStock stock;
    EXPECT_EQ(stock.GetCurrentValue(), 0);
    stock.SetCurrentValue(10101010);
    EXPECT_EQ(stock.GetCurrentValue(), 10101010);
  }

  TEST_F(CVirtualStockTest, TestGetChangeHandRate) {
    CVirtualStock stock;
    EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 0.0);
    stock.SetChangeHandRate(1.1);
    EXPECT_DOUBLE_EQ(stock.GetChangeHandRate(), 1.1);
  }

  TEST_F(CVirtualStockTest, TestGetUpDownRate) {
    CVirtualStock stock;
    EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 0.0);
    stock.SetUpDownRate(1.1);
    EXPECT_DOUBLE_EQ(stock.GetUpDownRate(), 1.1);
  }

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

    EXPECT_FALSE(stock.IsUpdateProfileDB());
    stock.SetUpdateProfileDB(true);
    EXPECT_TRUE(stock.IsUpdateProfileDB());
    stock.SetUpdateProfileDB(false);
    EXPECT_FALSE(stock.IsUpdateProfileDB());
  }

  TEST_F(CVirtualStockTest, TestIsUpdateProfileDBAndClearFlag) {
    CVirtualStock stock;

    EXPECT_FALSE(stock.IsUpdateProfileDB());
    EXPECT_FALSE(stock.IsUpdateProfileDBAndClearFlag());
    stock.SetUpdateProfileDB(true);
    EXPECT_TRUE(stock.IsUpdateProfileDBAndClearFlag());
    EXPECT_FALSE(stock.IsUpdateProfileDB());
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
    CVirtualStock stock;
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