#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"WeekLine.h"

namespace StockAnalysisTest {
  class CChinaStockHistoryDataTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) { // 本测试类的初始化函数
      ASSERT_FALSE(gl_fNormalMode);
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetStockCode();
    }

    static void TearDownTestSuite() {
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetStockCode();
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
    }
  };

  TEST_F(CChinaStockHistoryDataTest, TestGetDate) {
    ASSERT_FALSE(gl_fNormalMode);
    CChinaStockHistoryData dl;
    dl.SetDate(__CHINA_MARKET_BEGIN_DATE__);
    EXPECT_EQ(dl.GetFormatedMarketDate(), __CHINA_MARKET_BEGIN_DATE__);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetTime) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetFormatedMarketDate(), 0);
    dl.SetTime(100100100);
    EXPECT_EQ(dl.GetFormatedMarketTime(), 100100100);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetMarket) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetMarket(), 0);
    dl.SetMarket(1);
    EXPECT_EQ(dl.GetMarket(), 1);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetStockCode) {
    CChinaStockHistoryData dl;
    EXPECT_STREQ(dl.GetStockCode(), _T(""));
    dl.SetStockCode(_T("sh600000"));
    EXPECT_STREQ(dl.GetStockCode(), _T("sh600000"));
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetStockName) {
    CChinaStockHistoryData dl;
    EXPECT_STREQ(dl.GetStockName(), _T(""));
    dl.SetStockName(_T("浦东银行"));
    EXPECT_STREQ(dl.GetStockName(), _T("浦东银行"));
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetLastClose) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetLastClose(), 0);
    dl.SetLastClose(10100);
    EXPECT_EQ(dl.GetLastClose(), 10100);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOpen) {
    ASSERT_FALSE(gl_fNormalMode);
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOpen(), 0);
    dl.SetOpen(11100);
    EXPECT_EQ(dl.GetOpen(), 11100);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetHigh) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetHigh(), 0);
    dl.SetHigh(11111);
    EXPECT_EQ(dl.GetHigh(), 11111);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetLow) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetLow(), 0);
    dl.SetLow(22222);
    EXPECT_EQ(dl.GetLow(), 22222);
    dl.SetLow(_T("12345"));
    EXPECT_EQ(dl.GetLow(), 12345);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetClose) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetClose(), 0);
    dl.SetClose(33333);
    EXPECT_EQ(dl.GetClose(), 33333);
    dl.SetClose(_T("12345"));
    EXPECT_EQ(dl.GetClose(), 12345);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetUpDown) {
    CChinaStockHistoryData dl;
    EXPECT_DOUBLE_EQ(dl.GetUpDown(), 0);
    dl.SetUpDown(10.020);
    EXPECT_DOUBLE_EQ(dl.GetUpDown(), 10.020);
    dl.SetUpDown(_T("12.345"));
    EXPECT_DOUBLE_EQ(dl.GetUpDown(), 12.345);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetUpDownRate) {
    CChinaStockHistoryData dl;
    EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 0);
    dl.SetUpDownRate(40.40);
    EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 40.40);
    dl.SetUpDownRate(_T("12.345"));
    EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 12.345);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetChangeHandRate) {
    ASSERT_FALSE(gl_fNormalMode);
    CChinaStockHistoryData dl;
    EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 0);
    dl.SetChangeHandRate(30.30);
    EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 30.30);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetVolume) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetVolume(), 0);
    dl.SetVolume(100100100100100100);
    EXPECT_EQ(dl.GetVolume(), 100100100100100100);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetAmount) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetAmount(), 0);
    dl.SetAmount(200200200200200200);
    EXPECT_EQ(dl.GetAmount(), 200200200200200200);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetTotalValue) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetTotalValue(), 0);
    CString str = _T("3.13e+11");
    dl.SetTotalValue(str.GetBuffer());
    EXPECT_DOUBLE_EQ((double)(dl.GetTotalValue()), 3.13e+11);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetCurrentValue) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetCurrentValue(), 0);
    CString str = _T("3.15e+11");
    dl.SetCurrentValue(str.GetBuffer());
    EXPECT_DOUBLE_EQ((double)(dl.GetCurrentValue()), 3.15e+11);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetRS) {
    CChinaStockHistoryData dl;
    EXPECT_DOUBLE_EQ(dl.GetRS(), 0);
    dl.SetRS(50.50);
    EXPECT_DOUBLE_EQ(dl.GetRS(), 50.50);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetRSIndex) {
    CChinaStockHistoryData dl;
    EXPECT_DOUBLE_EQ(dl.GetRSIndex(), 0);
    dl.SetRSIndex(50.5023);
    EXPECT_DOUBLE_EQ(dl.GetRSIndex(), 50.5023);
  }
  TEST_F(CChinaStockHistoryDataTest, TestGetRSBackup) {
    CChinaStockHistoryData dl;
    EXPECT_DOUBLE_EQ(dl.GetRSBackup(), 0);
    dl.SetRSBackup(50.506);
    EXPECT_DOUBLE_EQ(dl.GetRSBackup(), 50.506);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetRSLogarithm) {
    CChinaStockHistoryData dl;
    EXPECT_DOUBLE_EQ((double)dl.GetRSLogarithm(), 0.0);
    dl.SetRSLogarithm(50.50);
    EXPECT_DOUBLE_EQ((double)dl.GetRSLogarithm(), 50.50);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetTransactionNumber) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetTransactionNumber(), 0);
    dl.SetTransactionNumber(600600);
    EXPECT_EQ(dl.GetTransactionNumber(), 600600);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetTransactionNumberBelow5000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetTransactionNumberBelow5000(), 0);
    dl.SetTransactionNumberBelow5000(10);
    EXPECT_EQ(dl.GetTransactionNumberBelow5000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetTransactionNumberBelow50000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetTransactionNumberBelow50000(), 0);
    dl.SetTransactionNumberBelow50000(10);
    EXPECT_EQ(dl.GetTransactionNumberBelow50000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetTransactionNumberBelow200000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetTransactionNumberBelow200000(), 0);
    dl.SetTransactionNumberBelow200000(10);
    EXPECT_EQ(dl.GetTransactionNumberBelow200000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetTransactionNumberAbove200000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetTransactionNumberAbove200000(), 0);
    dl.SetTransactionNumberAbove200000(10);
    EXPECT_EQ(dl.GetTransactionNumberAbove200000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinaryBuyVolume) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolume(), 0);
    dl.SetOrdinaryBuyVolume(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolume(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetAttackBuyVolume) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetAttackBuyVolume(), 0);
    dl.SetAttackBuyVolume(10);
    EXPECT_EQ(dl.GetAttackBuyVolume(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetStrongBuyVolume) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetStrongBuyVolume(), 0);
    dl.SetStrongBuyVolume(10);
    EXPECT_EQ(dl.GetStrongBuyVolume(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetStrongSellVolume) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetStrongSellVolume(), 0);
    dl.SetStrongSellVolume(10);
    EXPECT_EQ(dl.GetStrongSellVolume(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetAttackSellVolume) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetAttackSellVolume(), 0);
    dl.SetAttackSellVolume(10);
    EXPECT_EQ(dl.GetAttackSellVolume(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetUnknownVolume) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetUnknownVolume(), 0);
    dl.SetUnknownVolume(10);
    EXPECT_EQ(dl.GetUnknownVolume(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetCanceledBuyVolume) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetCanceledBuyVolume(), 0);
    dl.SetCanceledBuyVolume(10);
    EXPECT_EQ(dl.GetCanceledBuyVolume(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetCanceledSellVolume) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetCanceledSellVolume(), 0);
    dl.SetCanceledSellVolume(10);
    EXPECT_EQ(dl.GetCanceledSellVolume(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinarySellVolume) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinarySellVolume(), 0);
    dl.SetOrdinarySellVolume(10);
    EXPECT_EQ(dl.GetOrdinarySellVolume(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetAttackBuyBelow50000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetAttackBuyBelow50000(), 0);
    dl.SetAttackBuyBelow50000(10);
    EXPECT_EQ(dl.GetAttackBuyBelow50000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetAttackBuyBelow200000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetAttackBuyBelow200000(), 0);
    dl.SetAttackBuyBelow200000(10);
    EXPECT_EQ(dl.GetAttackBuyBelow200000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetAttackBuyAbove200000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetAttackBuyAbove200000(), 0);
    dl.SetAttackBuyAbove200000(10);
    EXPECT_EQ(dl.GetAttackBuyAbove200000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetAttackSellAbove200000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetAttackSellAbove200000(), 0);
    dl.SetAttackSellAbove200000(10);
    EXPECT_EQ(dl.GetAttackSellAbove200000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetAttackSellBelow200000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetAttackSellBelow200000(), 0);
    dl.SetAttackSellBelow200000(10);
    EXPECT_EQ(dl.GetAttackSellBelow200000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetAttackSellBelow50000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetAttackSellBelow50000(), 0);
    dl.SetAttackSellBelow50000(10);
    EXPECT_EQ(dl.GetAttackSellBelow50000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinaryBuyVolumeBelow5000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow5000(), 0);
    dl.SetOrdinaryBuyVolumeBelow5000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow5000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinaryBuyVolumeBelow10000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow10000(), 0);
    dl.SetOrdinaryBuyVolumeBelow10000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow10000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinaryBuyVolumeBelow20000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow20000(), 0);
    dl.SetOrdinaryBuyVolumeBelow20000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow20000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinaryBuyVolumeBelow50000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow50000(), 0);
    dl.SetOrdinaryBuyVolumeBelow50000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow50000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinaryBuyVolumeBelow100000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow100000(), 0);
    dl.SetOrdinaryBuyVolumeBelow100000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow100000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinaryBuyVolumeBelow200000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow200000(), 0);
    dl.SetOrdinaryBuyVolumeBelow200000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow200000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinaryBuyVolumeAbove200000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeAbove200000(), 0);
    dl.SetOrdinaryBuyVolumeAbove200000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeAbove200000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinarySellVolumeBelow5000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow5000(), 0);
    dl.SetOrdinarySellVolumeBelow5000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow5000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinarySellVolumeBelow10000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow10000(), 0);
    dl.SetOrdinarySellVolumeBelow10000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow10000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinarySellVolumeBelow20000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow20000(), 0);
    dl.SetOrdinarySellVolumeBelow20000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow20000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinarySellVolumeBelow50000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow50000(), 0);
    dl.SetOrdinarySellVolumeBelow50000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow50000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinarySellVolumeBelow100000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow100000(), 0);
    dl.SetOrdinarySellVolumeBelow100000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow100000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinarySellVolumeBelow200000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow200000(), 0);
    dl.SetOrdinarySellVolumeBelow200000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow200000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinarySellVolumeAbove200000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeAbove200000(), 0);
    dl.SetOrdinarySellVolumeAbove200000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeAbove200000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinaryBuyNumberBelow5000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow5000(), 0);
    dl.SetOrdinaryBuyNumberBelow5000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow5000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinaryBuyNumberBelow10000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow10000(), 0);
    dl.SetOrdinaryBuyNumberBelow10000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow10000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinaryBuyNumberBelow20000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow20000(), 0);
    dl.SetOrdinaryBuyNumberBelow20000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow20000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinaryBuyNumberBelow50000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow50000(), 0);
    dl.SetOrdinaryBuyNumberBelow50000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow50000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinaryBuyNumberBelow100000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow100000(), 0);
    dl.SetOrdinaryBuyNumberBelow100000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow100000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinaryBuyNumberBelow200000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow200000(), 0);
    dl.SetOrdinaryBuyNumberBelow200000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow200000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinaryBuyNumberAbove200000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberAbove200000(), 0);
    dl.SetOrdinaryBuyNumberAbove200000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberAbove200000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinarySellNumberBelow5000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow5000(), 0);
    dl.SetOrdinarySellNumberBelow5000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow5000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinarySellNumberBelow10000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow10000(), 0);
    dl.SetOrdinarySellNumberBelow10000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow10000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinarySellNumberBelow20000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow20000(), 0);
    dl.SetOrdinarySellNumberBelow20000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow20000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinarySellNumberBelow50000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow50000(), 0);
    dl.SetOrdinarySellNumberBelow50000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow50000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinarySellNumberBelow100000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow100000(), 0);
    dl.SetOrdinarySellNumberBelow100000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow100000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinarySellNumberBelow200000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow200000(), 0);
    dl.SetOrdinarySellNumberBelow200000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow200000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGetOrdinarySellNumberAbove200000) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberAbove200000(), 0);
    dl.SetOrdinarySellNumberAbove200000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberAbove200000(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGet3RS) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.Get3RS(), 0);
    dl.Set3RS(10);
    EXPECT_EQ(dl.Get3RS(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGet5RS) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.Get5RS(), 0);
    dl.Set5RS(10);
    EXPECT_EQ(dl.Get5RS(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGet10RS) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.Get10RS(), 0);
    dl.Set10RS(10);
    EXPECT_EQ(dl.Get10RS(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGet30RS) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.Get30RS(), 0);
    dl.Set30RS(10);
    EXPECT_EQ(dl.Get30RS(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGet60RS) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.Get60RS(), 0);
    dl.Set60RS(10);
    EXPECT_EQ(dl.Get60RS(), 10);
  }

  TEST_F(CChinaStockHistoryDataTest, TestGet120RS) {
    CChinaStockHistoryData dl;
    EXPECT_EQ(dl.Get120RS(), 0);
    dl.Set120RS(10);
    EXPECT_EQ(dl.Get120RS(), 10);
  }
}