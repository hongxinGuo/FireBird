#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"WeekLine.h"

namespace StockAnalysisTest {
  class CWeekLineTest : public ::testing::Test
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

  TEST_F(CWeekLineTest, TestGetDate) {
    ASSERT_FALSE(gl_fNormalMode);
    CWeekLine dl;
    dl.SetDate(__CHINA_MARKET_BEGIN_DATE__);
    EXPECT_EQ(dl.GetFormatedMarketDate(), __CHINA_MARKET_BEGIN_DATE__);
  }

  TEST_F(CWeekLineTest, TestGetTime) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetFormatedMarketDate(), 0);
    dl.SetTime(100100100);
    EXPECT_EQ(dl.GetFormatedMarketTime(), 100100100);
  }

  TEST_F(CWeekLineTest, TestGetMarket) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetMarket(), 0);
    dl.SetMarket(1);
    EXPECT_EQ(dl.GetMarket(), 1);
  }

  TEST_F(CWeekLineTest, TestGetStockCode) {
    CWeekLine dl;
    EXPECT_STREQ(dl.GetStockCode(), _T(""));
    dl.SetStockCode(_T("sh600000"));
    EXPECT_STREQ(dl.GetStockCode(), _T("sh600000"));
  }

  TEST_F(CWeekLineTest, TestGetStockName) {
    CWeekLine dl;
    EXPECT_STREQ(dl.GetStockName(), _T(""));
    dl.SetStockName(_T("浦东银行"));
    EXPECT_STREQ(dl.GetStockName(), _T("浦东银行"));
  }

  TEST_F(CWeekLineTest, TestGetLastClose) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetLastClose(), 0);
    dl.SetLastClose(10100);
    EXPECT_EQ(dl.GetLastClose(), 10100);
  }

  TEST_F(CWeekLineTest, TestGetOpen) {
    ASSERT_FALSE(gl_fNormalMode);
    CWeekLine dl;
    EXPECT_EQ(dl.GetOpen(), 0);
    dl.SetOpen(11100);
    EXPECT_EQ(dl.GetOpen(), 11100);
  }

  TEST_F(CWeekLineTest, TestGetHigh) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetHigh(), 0);
    dl.SetHigh(11111);
    EXPECT_EQ(dl.GetHigh(), 11111);
  }

  TEST_F(CWeekLineTest, TestGetLow) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetLow(), 0);
    dl.SetLow(22222);
    EXPECT_EQ(dl.GetLow(), 22222);
    dl.SetLow(_T("12345"));
    EXPECT_EQ(dl.GetLow(), 12345);
  }

  TEST_F(CWeekLineTest, TestGetClose) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetClose(), 0);
    dl.SetClose(33333);
    EXPECT_EQ(dl.GetClose(), 33333);
    dl.SetClose(_T("12345"));
    EXPECT_EQ(dl.GetClose(), 12345);
  }

  TEST_F(CWeekLineTest, TestGetUpDown) {
    CWeekLine dl;
    EXPECT_DOUBLE_EQ(dl.GetUpDown(), 0);
    dl.SetUpDown(10.020);
    EXPECT_DOUBLE_EQ(dl.GetUpDown(), 10.020);
    dl.SetUpDown(_T("12.345"));
    EXPECT_DOUBLE_EQ(dl.GetUpDown(), 12.345);
  }

  TEST_F(CWeekLineTest, TestGetUpDownRate) {
    CWeekLine dl;
    EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 0);
    dl.SetUpDownRate(40.40);
    EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 40.40);
    dl.SetUpDownRate(_T("12.345"));
    EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 12.345);
  }

  TEST_F(CWeekLineTest, TestGetChangeHandRate) {
    ASSERT_FALSE(gl_fNormalMode);
    CWeekLine dl;
    EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 0);
    dl.SetChangeHandRate(30.30);
    EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 30.30);
  }

  TEST_F(CWeekLineTest, TestGetVolume) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetVolume(), 0);
    dl.SetVolume(100100100100100100);
    EXPECT_EQ(dl.GetVolume(), 100100100100100100);
  }

  TEST_F(CWeekLineTest, TestGetAmount) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetAmount(), 0);
    dl.SetAmount(200200200200200200);
    EXPECT_EQ(dl.GetAmount(), 200200200200200200);
  }

  TEST_F(CWeekLineTest, TestGetTotalValue) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetTotalValue(), 0);
    CString str = _T("3.13e+11");
    dl.SetTotalValue(str.GetBuffer());
    EXPECT_DOUBLE_EQ((double)(dl.GetTotalValue()), 3.13e+11);
  }

  TEST_F(CWeekLineTest, TestGetCurrentValue) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetCurrentValue(), 0);
    CString str = _T("3.15e+11");
    dl.SetCurrentValue(str.GetBuffer());
    EXPECT_DOUBLE_EQ((double)(dl.GetCurrentValue()), 3.15e+11);
  }

  TEST_F(CWeekLineTest, TestGetRS) {
    CWeekLine dl;
    EXPECT_DOUBLE_EQ(dl.GetRS(), 0);
    dl.SetRS(50.50);
    EXPECT_DOUBLE_EQ(dl.GetRS(), 50.50);
  }

  TEST_F(CWeekLineTest, TestGetRSIndex) {
    CWeekLine dl;
    EXPECT_DOUBLE_EQ(dl.GetRSIndex(), 0);
    dl.SetRSIndex(50.5023);
    EXPECT_DOUBLE_EQ(dl.GetRSIndex(), 50.5023);
  }
  TEST_F(CWeekLineTest, TestGetRSBackup) {
    CWeekLine dl;
    EXPECT_DOUBLE_EQ(dl.GetRSBackup(), 0);
    dl.SetRSBackup(50.506);
    EXPECT_DOUBLE_EQ(dl.GetRSBackup(), 50.506);
  }

  TEST_F(CWeekLineTest, TestGetRSLogarithm) {
    CWeekLine dl;
    EXPECT_DOUBLE_EQ((double)dl.GetRSLogarithm(), 0.0);
    dl.SetRSLogarithm(50.50);
    EXPECT_DOUBLE_EQ((double)dl.GetRSLogarithm(), 50.50);
  }

  TEST_F(CWeekLineTest, TestGetTransactionNumber) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetTransactionNumber(), 0);
    dl.SetTransactionNumber(600600);
    EXPECT_EQ(dl.GetTransactionNumber(), 600600);
  }

  TEST_F(CWeekLineTest, TestGetTransactionNumberBelow5000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetTransactionNumberBelow5000(), 0);
    dl.SetTransactionNumberBelow5000(10);
    EXPECT_EQ(dl.GetTransactionNumberBelow5000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetTransactionNumberBelow50000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetTransactionNumberBelow50000(), 0);
    dl.SetTransactionNumberBelow50000(10);
    EXPECT_EQ(dl.GetTransactionNumberBelow50000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetTransactionNumberBelow200000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetTransactionNumberBelow200000(), 0);
    dl.SetTransactionNumberBelow200000(10);
    EXPECT_EQ(dl.GetTransactionNumberBelow200000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetTransactionNumberAbove200000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetTransactionNumberAbove200000(), 0);
    dl.SetTransactionNumberAbove200000(10);
    EXPECT_EQ(dl.GetTransactionNumberAbove200000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinaryBuyVolume) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolume(), 0);
    dl.SetOrdinaryBuyVolume(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolume(), 10);
  }

  TEST_F(CWeekLineTest, TestGetAttackBuyVolume) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetAttackBuyVolume(), 0);
    dl.SetAttackBuyVolume(10);
    EXPECT_EQ(dl.GetAttackBuyVolume(), 10);
  }

  TEST_F(CWeekLineTest, TestGetStrongBuyVolume) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetStrongBuyVolume(), 0);
    dl.SetStrongBuyVolume(10);
    EXPECT_EQ(dl.GetStrongBuyVolume(), 10);
  }

  TEST_F(CWeekLineTest, TestGetStrongSellVolume) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetStrongSellVolume(), 0);
    dl.SetStrongSellVolume(10);
    EXPECT_EQ(dl.GetStrongSellVolume(), 10);
  }

  TEST_F(CWeekLineTest, TestGetAttackSellVolume) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetAttackSellVolume(), 0);
    dl.SetAttackSellVolume(10);
    EXPECT_EQ(dl.GetAttackSellVolume(), 10);
  }

  TEST_F(CWeekLineTest, TestGetUnknownVolume) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetUnknownVolume(), 0);
    dl.SetUnknownVolume(10);
    EXPECT_EQ(dl.GetUnknownVolume(), 10);
  }

  TEST_F(CWeekLineTest, TestGetCanceledBuyVolume) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetCanceledBuyVolume(), 0);
    dl.SetCanceledBuyVolume(10);
    EXPECT_EQ(dl.GetCanceledBuyVolume(), 10);
  }

  TEST_F(CWeekLineTest, TestGetCanceledSellVolume) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetCanceledSellVolume(), 0);
    dl.SetCanceledSellVolume(10);
    EXPECT_EQ(dl.GetCanceledSellVolume(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinarySellVolume) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinarySellVolume(), 0);
    dl.SetOrdinarySellVolume(10);
    EXPECT_EQ(dl.GetOrdinarySellVolume(), 10);
  }

  TEST_F(CWeekLineTest, TestGetAttackBuyBelow50000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetAttackBuyBelow50000(), 0);
    dl.SetAttackBuyBelow50000(10);
    EXPECT_EQ(dl.GetAttackBuyBelow50000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetAttackBuyBelow200000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetAttackBuyBelow200000(), 0);
    dl.SetAttackBuyBelow200000(10);
    EXPECT_EQ(dl.GetAttackBuyBelow200000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetAttackBuyAbove200000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetAttackBuyAbove200000(), 0);
    dl.SetAttackBuyAbove200000(10);
    EXPECT_EQ(dl.GetAttackBuyAbove200000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetAttackSellAbove200000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetAttackSellAbove200000(), 0);
    dl.SetAttackSellAbove200000(10);
    EXPECT_EQ(dl.GetAttackSellAbove200000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetAttackSellBelow200000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetAttackSellBelow200000(), 0);
    dl.SetAttackSellBelow200000(10);
    EXPECT_EQ(dl.GetAttackSellBelow200000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetAttackSellBelow50000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetAttackSellBelow50000(), 0);
    dl.SetAttackSellBelow50000(10);
    EXPECT_EQ(dl.GetAttackSellBelow50000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinaryBuyVolumeBelow5000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow5000(), 0);
    dl.SetOrdinaryBuyVolumeBelow5000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow5000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinaryBuyVolumeBelow10000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow10000(), 0);
    dl.SetOrdinaryBuyVolumeBelow10000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow10000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinaryBuyVolumeBelow20000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow20000(), 0);
    dl.SetOrdinaryBuyVolumeBelow20000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow20000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinaryBuyVolumeBelow50000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow50000(), 0);
    dl.SetOrdinaryBuyVolumeBelow50000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow50000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinaryBuyVolumeBelow100000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow100000(), 0);
    dl.SetOrdinaryBuyVolumeBelow100000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow100000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinaryBuyVolumeBelow200000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow200000(), 0);
    dl.SetOrdinaryBuyVolumeBelow200000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow200000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinaryBuyVolumeAbove200000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeAbove200000(), 0);
    dl.SetOrdinaryBuyVolumeAbove200000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeAbove200000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinarySellVolumeBelow5000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow5000(), 0);
    dl.SetOrdinarySellVolumeBelow5000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow5000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinarySellVolumeBelow10000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow10000(), 0);
    dl.SetOrdinarySellVolumeBelow10000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow10000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinarySellVolumeBelow20000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow20000(), 0);
    dl.SetOrdinarySellVolumeBelow20000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow20000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinarySellVolumeBelow50000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow50000(), 0);
    dl.SetOrdinarySellVolumeBelow50000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow50000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinarySellVolumeBelow100000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow100000(), 0);
    dl.SetOrdinarySellVolumeBelow100000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow100000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinarySellVolumeBelow200000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow200000(), 0);
    dl.SetOrdinarySellVolumeBelow200000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow200000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinarySellVolumeAbove200000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeAbove200000(), 0);
    dl.SetOrdinarySellVolumeAbove200000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeAbove200000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinaryBuyNumberBelow5000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow5000(), 0);
    dl.SetOrdinaryBuyNumberBelow5000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow5000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinaryBuyNumberBelow10000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow10000(), 0);
    dl.SetOrdinaryBuyNumberBelow10000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow10000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinaryBuyNumberBelow20000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow20000(), 0);
    dl.SetOrdinaryBuyNumberBelow20000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow20000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinaryBuyNumberBelow50000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow50000(), 0);
    dl.SetOrdinaryBuyNumberBelow50000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow50000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinaryBuyNumberBelow100000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow100000(), 0);
    dl.SetOrdinaryBuyNumberBelow100000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow100000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinaryBuyNumberBelow200000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow200000(), 0);
    dl.SetOrdinaryBuyNumberBelow200000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow200000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinaryBuyNumberAbove200000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberAbove200000(), 0);
    dl.SetOrdinaryBuyNumberAbove200000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberAbove200000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinarySellNumberBelow5000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow5000(), 0);
    dl.SetOrdinarySellNumberBelow5000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow5000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinarySellNumberBelow10000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow10000(), 0);
    dl.SetOrdinarySellNumberBelow10000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow10000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinarySellNumberBelow20000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow20000(), 0);
    dl.SetOrdinarySellNumberBelow20000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow20000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinarySellNumberBelow50000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow50000(), 0);
    dl.SetOrdinarySellNumberBelow50000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow50000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinarySellNumberBelow100000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow100000(), 0);
    dl.SetOrdinarySellNumberBelow100000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow100000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinarySellNumberBelow200000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow200000(), 0);
    dl.SetOrdinarySellNumberBelow200000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow200000(), 10);
  }

  TEST_F(CWeekLineTest, TestGetOrdinarySellNumberAbove200000) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberAbove200000(), 0);
    dl.SetOrdinarySellNumberAbove200000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberAbove200000(), 10);
  }

  TEST_F(CWeekLineTest, TestGet3RS) {
    CWeekLine dl;
    EXPECT_EQ(dl.Get3RS(), 0);
    dl.Set3RS(10);
    EXPECT_EQ(dl.Get3RS(), 10);
  }

  TEST_F(CWeekLineTest, TestGet5RS) {
    CWeekLine dl;
    EXPECT_EQ(dl.Get5RS(), 0);
    dl.Set5RS(10);
    EXPECT_EQ(dl.Get5RS(), 10);
  }

  TEST_F(CWeekLineTest, TestGet10RS) {
    CWeekLine dl;
    EXPECT_EQ(dl.Get10RS(), 0);
    dl.Set10RS(10);
    EXPECT_EQ(dl.Get10RS(), 10);
  }

  TEST_F(CWeekLineTest, TestGet30RS) {
    CWeekLine dl;
    EXPECT_EQ(dl.Get30RS(), 0);
    dl.Set30RS(10);
    EXPECT_EQ(dl.Get30RS(), 10);
  }

  TEST_F(CWeekLineTest, TestGet60RS) {
    CWeekLine dl;
    EXPECT_EQ(dl.Get60RS(), 0);
    dl.Set60RS(10);
    EXPECT_EQ(dl.Get60RS(), 10);
  }

  TEST_F(CWeekLineTest, TestGet120RS) {
    CWeekLine dl;
    EXPECT_EQ(dl.Get120RS(), 0);
    dl.Set120RS(10);
    EXPECT_EQ(dl.Get120RS(), 10);
  }

  TEST_F(CWeekLineTest, TestSaveData) {
    CSetWeekLineBasicInfo setWeekLineBasicInfo;
    CSetWeekLineExtendInfo setWeekLineExtendInfo;
    CWeekLine id, id2;

    id.SetDate(21101101);
    id.SetMarket(__SHANGHAI_MARKET__);
    id.SetStockCode(_T("sh600008"));
    id.SetStockName(_T("首创股份"));
    id.SetLastClose(34235345);
    id.SetOpen(343452435);
    id.SetHigh(45234543);
    id.SetLow(3452345);
    id.SetClose(452435);
    id.SetVolume(34523454);
    id.SetAmount(3245235345);
    id.SetUpDown(12345);
    id.SetUpDownRate(123.45);
    id.SetTotalValue(234523452345);
    id.SetCurrentValue(234145345245);
    id.SetChangeHandRate(54.321);
    id.SetRS(14.5);
    id.SetRSIndex(15.6);
    id.SetRSBackup(20.9);

    id.SetTransactionNumber(100390);
    id.SetTransactionNumberBelow5000(10034390);
    id.SetTransactionNumberBelow50000(10034390);
    id.SetTransactionNumberBelow200000(10033490);
    id.SetTransactionNumberAbove200000(10034590);

    id.SetCanceledBuyVolume(6);
    id.SetCanceledSellVolume(7);
    id.SetAttackBuyVolume(8);
    id.SetAttackSellVolume(9);
    id.SetStrongBuyVolume(10);
    id.SetStrongSellVolume(11);
    id.SetUnknownVolume(12);
    id.SetOrdinaryBuyVolume(13);
    id.SetOrdinarySellVolume(14);
    id.SetAttackBuyBelow50000(15);
    id.SetAttackBuyBelow200000(16);
    id.SetAttackBuyAbove200000(17);
    id.SetAttackSellBelow50000(18);
    id.SetAttackBuyBelow200000(19);
    id.SetAttackBuyAbove200000(20);

    id.SetOrdinaryBuyVolumeBelow5000(30);
    id.SetOrdinaryBuyVolumeBelow10000(31);
    id.SetOrdinaryBuyVolumeBelow20000(32);
    id.SetOrdinaryBuyVolumeBelow50000(33);
    id.SetOrdinaryBuyVolumeBelow100000(34);
    id.SetOrdinaryBuyVolumeBelow200000(35);
    id.SetOrdinaryBuyVolumeAbove200000(36);
    id.SetOrdinarySellVolumeBelow5000(37);
    id.SetOrdinarySellVolumeBelow10000(38);
    id.SetOrdinarySellVolumeBelow20000(39);
    id.SetOrdinarySellVolumeBelow50000(40);
    id.SetOrdinarySellVolumeBelow100000(41);
    id.SetOrdinarySellVolumeBelow200000(42);
    id.SetOrdinarySellVolumeAbove200000(43);
    id.SetOrdinaryBuyNumberBelow5000(44);
    id.SetOrdinaryBuyNumberBelow10000(45);
    id.SetOrdinaryBuyNumberBelow20000(46);
    id.SetOrdinaryBuyNumberBelow50000(47);
    id.SetOrdinaryBuyNumberBelow100000(48);
    id.SetOrdinaryBuyNumberBelow200000(49);
    id.SetOrdinaryBuyNumberAbove200000(50);
    id.SetOrdinarySellNumberBelow5000(51);
    id.SetOrdinarySellNumberBelow10000(52);
    id.SetOrdinarySellNumberBelow20000(53);
    id.SetOrdinarySellNumberBelow50000(54);
    id.SetOrdinarySellNumberBelow100000(55);
    id.SetOrdinarySellNumberBelow200000(56);
    id.SetOrdinarySellNumberAbove200000(57);

    id.SetCanceledBuyVolumeBelow5000(64);
    id.SetCanceledBuyVolumeBelow10000(65);
    id.SetCanceledBuyVolumeBelow20000(66);
    id.SetCanceledBuyVolumeBelow50000(67);
    id.SetCanceledBuyVolumeBelow100000(68);
    id.SetCanceledBuyVolumeBelow200000(69);
    id.SetCanceledBuyVolumeAbove200000(70);
    id.SetCanceledSellVolumeBelow5000(71);
    id.SetCanceledSellVolumeBelow10000(72);
    id.SetCanceledSellVolumeBelow20000(73);
    id.SetCanceledSellVolumeBelow50000(74);
    id.SetCanceledSellVolumeBelow100000(75);
    id.SetCanceledSellVolumeBelow200000(76);
    id.SetCanceledSellVolumeAbove200000(77);

    ASSERT(!gl_fNormalMode);
    setWeekLineBasicInfo.m_strFilter = _T("[ID] = 1");
    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.m_pDatabase->BeginTrans();
    id.AppendData(&setWeekLineBasicInfo);
    setWeekLineBasicInfo.m_pDatabase->CommitTrans();
    setWeekLineBasicInfo.Close();

    setWeekLineExtendInfo.m_strFilter = _T("[ID] = 1");
    setWeekLineExtendInfo.Open();
    setWeekLineExtendInfo.m_pDatabase->BeginTrans();
    id.AppendData(&setWeekLineExtendInfo);
    setWeekLineExtendInfo.m_pDatabase->CommitTrans();
    setWeekLineExtendInfo.Close();

    setWeekLineBasicInfo.m_strFilter = _T("[Date] = 21101101");
    setWeekLineBasicInfo.Open();
    id2.LoadBasicData(&setWeekLineBasicInfo);
    EXPECT_EQ(setWeekLineBasicInfo.m_Date, id.GetFormatedMarketDate());
    EXPECT_EQ(setWeekLineBasicInfo.m_Market, id.GetMarket());
    EXPECT_STREQ(setWeekLineBasicInfo.m_StockCode, id.GetStockCode());
    //EXPECT_STREQ(setWeekLineBasicInfo.m_StockName, id.GetStockName());
    EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_LastClose) * 1000, id.GetLastClose());
    EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_Open) * 1000, id.GetOpen());
    EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_High) * 1000, id.GetHigh());
    EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_Low) * 1000, id.GetLow());
    EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_Close) * 1000, id.GetClose());
    EXPECT_EQ(atoll(setWeekLineBasicInfo.m_Volume), id.GetVolume());
    EXPECT_EQ(atoll(setWeekLineBasicInfo.m_Amount), id.GetAmount());
    EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_UpAndDown), id.GetUpDown());
    EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_UpDownRate), id.GetUpDownRate());
    EXPECT_EQ(atoll(setWeekLineBasicInfo.m_TotalValue), id.GetTotalValue());
    EXPECT_EQ(atoll(setWeekLineBasicInfo.m_CurrentValue), id.GetCurrentValue());
    EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_ChangeHandRate), id.GetChangeHandRate());
    EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_RS), id.GetRS());
    EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_RSIndex), id.GetRSIndex());
    EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_RSBackup), id.GetRSBackup());
    setWeekLineBasicInfo.Close();

    EXPECT_EQ(id2.GetFormatedMarketDate(), id.GetFormatedMarketDate());
    EXPECT_EQ(id2.GetMarket(), id.GetMarket());
    EXPECT_STREQ(id2.GetStockCode(), id.GetStockCode());
    //EXPECT_STREQ(setWeekLineBasicInfo.m_StockName, id.GetStockName());
    EXPECT_DOUBLE_EQ(id2.GetLastClose(), id.GetLastClose());
    EXPECT_DOUBLE_EQ(id2.GetOpen(), id.GetOpen());
    EXPECT_DOUBLE_EQ(id2.GetHigh(), id.GetHigh());
    EXPECT_DOUBLE_EQ(id2.GetLow(), id.GetLow());
    EXPECT_DOUBLE_EQ(id2.GetClose(), id.GetClose());
    EXPECT_EQ(id2.GetVolume(), id.GetVolume());
    EXPECT_EQ(id2.GetAmount(), id.GetAmount());
    EXPECT_DOUBLE_EQ(id2.GetUpDown(), id.GetUpDown());
    EXPECT_DOUBLE_EQ(id2.GetUpDownRate(), id.GetUpDownRate());
    EXPECT_EQ(id2.GetTotalValue(), id.GetTotalValue());
    EXPECT_EQ(id2.GetCurrentValue(), id.GetCurrentValue());
    EXPECT_DOUBLE_EQ(id2.GetChangeHandRate(), id.GetChangeHandRate());
    EXPECT_DOUBLE_EQ(id2.GetRS(), id.GetRS());
    EXPECT_DOUBLE_EQ(id2.GetRSIndex(), id.GetRSIndex());
    EXPECT_DOUBLE_EQ(id2.GetRSBackup(), id.GetRSBackup());

    setWeekLineExtendInfo.m_strFilter = _T("[Date] = 21101101");
    setWeekLineExtendInfo.Open();
    id2.LoadExtendData(&setWeekLineExtendInfo);
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_TransactionNumber), id2.GetTransactionNumber());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_TransactionNumberBelow5000), id2.GetTransactionNumberBelow5000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_TransactionNumberBelow50000), id2.GetTransactionNumberBelow50000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_TransactionNumberBelow200000), id2.GetTransactionNumberBelow200000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_TransactionNumberAbove200000), id2.GetTransactionNumberAbove200000());

    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_CanceledBuyVolume), id2.GetCanceledBuyVolume());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_CanceledSellVolume), id2.GetCanceledSellVolume());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_AttackBuyVolume), id2.GetAttackBuyVolume());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_AttackSellVolume), id2.GetAttackSellVolume());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_StrongBuyVolume), id2.GetStrongBuyVolume());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_StrongSellVolume), id2.GetStrongSellVolume());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_UnknownVolume), id2.GetUnknownVolume());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinaryBuyVolume), id2.GetOrdinaryBuyVolume());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinarySellVolume), id2.GetOrdinarySellVolume());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_AttackBuyBelow50000), id2.GetAttackBuyBelow50000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_AttackBuyBelow200000), id2.GetAttackBuyBelow200000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_AttackBuyAbove200000), id2.GetAttackBuyAbove200000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_AttackSellBelow50000), id2.GetAttackSellBelow50000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_AttackSellBelow200000), id2.GetAttackSellBelow200000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_AttackSellAbove200000), id2.GetAttackSellAbove200000());

    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinaryBuyVolumeBelow5000), id2.GetOrdinaryBuyVolumeBelow5000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinaryBuyVolumeBelow10000), id2.GetOrdinaryBuyVolumeBelow10000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinaryBuyVolumeBelow20000), id2.GetOrdinaryBuyVolumeBelow20000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinaryBuyVolumeBelow50000), id2.GetOrdinaryBuyVolumeBelow50000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinaryBuyVolumeBelow100000), id2.GetOrdinaryBuyVolumeBelow100000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinaryBuyVolumeBelow200000), id2.GetOrdinaryBuyVolumeBelow200000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinaryBuyVolumeAbove200000), id2.GetOrdinaryBuyVolumeAbove200000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinarySellVolumeBelow5000), id2.GetOrdinarySellVolumeBelow5000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinarySellVolumeBelow10000), id2.GetOrdinarySellVolumeBelow10000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinarySellVolumeBelow20000), id2.GetOrdinarySellVolumeBelow20000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinarySellVolumeBelow50000), id2.GetOrdinarySellVolumeBelow50000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinarySellVolumeBelow100000), id2.GetOrdinarySellVolumeBelow100000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinarySellVolumeBelow200000), id2.GetOrdinarySellVolumeBelow200000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinarySellVolumeAbove200000), id2.GetOrdinarySellVolumeAbove200000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinaryBuyNumberBelow5000), id2.GetOrdinaryBuyNumberBelow5000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinaryBuyNumberBelow10000), id2.GetOrdinaryBuyNumberBelow10000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinaryBuyNumberBelow20000), id2.GetOrdinaryBuyNumberBelow20000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinaryBuyNumberBelow50000), id2.GetOrdinaryBuyNumberBelow50000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinaryBuyNumberBelow100000), id2.GetOrdinaryBuyNumberBelow100000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinaryBuyNumberBelow200000), id2.GetOrdinaryBuyNumberBelow200000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinaryBuyNumberAbove200000), id2.GetOrdinaryBuyNumberAbove200000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinarySellNumberBelow5000), id2.GetOrdinarySellNumberBelow5000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinarySellNumberBelow10000), id2.GetOrdinarySellNumberBelow10000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinarySellNumberBelow20000), id2.GetOrdinarySellNumberBelow20000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinarySellNumberBelow50000), id2.GetOrdinarySellNumberBelow50000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinarySellNumberBelow100000), id2.GetOrdinarySellNumberBelow100000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinarySellNumberBelow200000), id2.GetOrdinarySellNumberBelow200000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_OrdinarySellNumberAbove200000), id2.GetOrdinarySellNumberAbove200000());

    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_CanceledBuyVolumeBelow5000), id2.GetCanceledBuyVolumeBelow5000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_CanceledBuyVolumeBelow10000), id2.GetCanceledBuyVolumeBelow10000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_CanceledBuyVolumeBelow20000), id2.GetCanceledBuyVolumeBelow20000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_CanceledBuyVolumeBelow50000), id2.GetCanceledBuyVolumeBelow50000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_CanceledBuyVolumeBelow100000), id2.GetCanceledBuyVolumeBelow100000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_CanceledBuyVolumeBelow200000), id2.GetCanceledBuyVolumeBelow200000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_CanceledBuyVolumeAbove200000), id2.GetCanceledBuyVolumeAbove200000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_CanceledSellVolumeBelow5000), id2.GetCanceledSellVolumeBelow5000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_CanceledSellVolumeBelow10000), id2.GetCanceledSellVolumeBelow10000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_CanceledSellVolumeBelow20000), id2.GetCanceledSellVolumeBelow20000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_CanceledSellVolumeBelow50000), id2.GetCanceledSellVolumeBelow50000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_CanceledSellVolumeBelow100000), id2.GetCanceledSellVolumeBelow100000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_CanceledSellVolumeBelow200000), id2.GetCanceledSellVolumeBelow200000());
    EXPECT_EQ(atoll(setWeekLineExtendInfo.m_CanceledSellVolumeAbove200000), id2.GetCanceledSellVolumeAbove200000());
    setWeekLineExtendInfo.Close();

    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.m_pDatabase->BeginTrans();
    while (!setWeekLineBasicInfo.IsEOF()) {
      setWeekLineBasicInfo.Delete();
      setWeekLineBasicInfo.MoveNext();
    }
    setWeekLineBasicInfo.m_pDatabase->CommitTrans();
    setWeekLineBasicInfo.Close();

    setWeekLineExtendInfo.Open();
    setWeekLineExtendInfo.m_pDatabase->BeginTrans();
    while (!setWeekLineExtendInfo.IsEOF()) {
      setWeekLineExtendInfo.Delete();
      setWeekLineExtendInfo.MoveNext();
    }
    setWeekLineExtendInfo.m_pDatabase->CommitTrans();
    setWeekLineExtendInfo.Close();
  }

  TEST_F(CWeekLineTest, TestLoadData) {
    CSetWeekLineBasicInfo setWeekLineBasicInfo;
    CWeekLine id, idLoaded;

    id.SetDate(21101101);
    id.SetMarket(__SHANGHAI_MARKET__);
    id.SetStockCode(_T("sh600008"));
    id.SetStockName(_T("首创股份"));
    id.SetLastClose(34235345);
    id.SetOpen(343452435);
    id.SetHigh(45234543);
    id.SetLow(3452345);
    id.SetClose(452435);
    id.SetVolume(34523454);
    id.SetAmount(3245235345);
    id.SetUpDown(((double)id.GetClose() - id.GetLastClose()) / 1000);
    id.SetUpDownRate(123.45);
    id.SetTotalValue(234523452345);
    id.SetCurrentValue(234145345245);
    id.SetChangeHandRate(54.321);
    id.SetRS(14.5);
    id.SetRSIndex(135.6);
    id.SetRSBackup(120.9);

    ASSERT(!gl_fNormalMode);
    setWeekLineBasicInfo.m_strFilter = _T("[ID] = 1");
    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.m_pDatabase->BeginTrans();
    id.AppendData(&setWeekLineBasicInfo);
    setWeekLineBasicInfo.m_pDatabase->CommitTrans();
    setWeekLineBasicInfo.Close();

    setWeekLineBasicInfo.m_strFilter = _T("[Date] = 21101101");
    setWeekLineBasicInfo.Open();
    idLoaded.LoadBasicData(&setWeekLineBasicInfo);
    EXPECT_EQ(idLoaded.GetFormatedMarketDate(), id.GetFormatedMarketDate());
    EXPECT_EQ(idLoaded.GetMarket(), id.GetMarket());
    EXPECT_STREQ(idLoaded.GetStockCode(), id.GetStockCode());
    //EXPECT_STREQ(setWeekLineBasicInfo.m_StockName, id.GetStockName());
    EXPECT_EQ(idLoaded.GetLastClose(), id.GetLastClose());
    EXPECT_EQ(idLoaded.GetOpen(), id.GetOpen());
    EXPECT_EQ(idLoaded.GetHigh(), id.GetHigh());
    EXPECT_EQ(idLoaded.GetLow(), id.GetLow());
    EXPECT_EQ(idLoaded.GetClose(), id.GetClose());
    EXPECT_EQ(idLoaded.GetVolume(), id.GetVolume());
    EXPECT_EQ(idLoaded.GetAmount(), id.GetAmount());
    EXPECT_DOUBLE_EQ(idLoaded.GetUpDown(), id.GetUpDown());
    EXPECT_DOUBLE_EQ(idLoaded.GetUpDownRate(), id.GetUpDownRate());
    EXPECT_EQ(idLoaded.GetTotalValue(), id.GetTotalValue());
    EXPECT_EQ(idLoaded.GetCurrentValue(), id.GetCurrentValue());
    EXPECT_DOUBLE_EQ(idLoaded.GetChangeHandRate(), id.GetChangeHandRate());
    EXPECT_DOUBLE_EQ(idLoaded.GetRS(), id.GetRS());
    EXPECT_DOUBLE_EQ(idLoaded.GetRSIndex(), id.GetRSIndex());
    EXPECT_DOUBLE_EQ(idLoaded.GetRSBackup(), id.GetRSBackup());
    setWeekLineBasicInfo.Close();

    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.m_pDatabase->BeginTrans();
    while (!setWeekLineBasicInfo.IsEOF()) {
      setWeekLineBasicInfo.Delete();
      setWeekLineBasicInfo.MoveNext();
    }
    setWeekLineBasicInfo.m_pDatabase->CommitTrans();
    setWeekLineBasicInfo.Close();
  }

  TEST_F(CWeekLineTest, TestLoadWeekLine) {
    CWeekLine id, id2;
    CSetWeekLineBasicInfo setWeekLineBasicInfo;
    id.SetDate(__CHINA_MARKET_BEGIN_DATE__);
    id.SetMarket(__SHANGHAI_MARKET__);
    id.SetStockCode(_T("sh600000"));
    id.SetStockName(_T("浦发银行"));
    id.SetLastClose(1010);
    id.SetOpen(1100);
    id.SetHigh(1200);
    id.SetLow(1000);
    id.SetClose(1150);
    id.SetVolume(100000);
    id.SetAmount(100000000);
    setWeekLineBasicInfo.m_strFilter = _T("[ID] = 1");
    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.AddNew();
    id.SaveData(&setWeekLineBasicInfo);
    setWeekLineBasicInfo.Update();
    setWeekLineBasicInfo.Close();

    setWeekLineBasicInfo.m_strFilter = _T("[Date] = 19900101");
    setWeekLineBasicInfo.Open();
    id2.LoadBasicData(&setWeekLineBasicInfo);
    EXPECT_EQ(id.GetFormatedMarketDate(), id2.GetFormatedMarketDate());
    EXPECT_STREQ(id.GetStockCode(), id2.GetStockCode());
    EXPECT_EQ(id.GetOpen(), id2.GetOpen());

    setWeekLineBasicInfo.Delete();
    setWeekLineBasicInfo.Close();
  }

  TEST_F(CWeekLineTest, TestCreateWeekLine1) {
    CWeekLine weekLine;
    CDLPtr pDL = make_shared<CDL>();
    pDL->SetTime(100100100100);
    pDL->SetDate(20200202);
    pDL->SetStockCode(_T("sh600000"));
    pDL->SetStockName(_T("浦发银行"));

    pDL->SetOpen(1010);
    pDL->SetClose(2020);
    pDL->SetHigh(3030);
    pDL->SetLow(4040);
    pDL->SetLastClose(5050);
    pDL->SetUpDown((double)(pDL->GetClose() - pDL->GetLastClose()) / 1000);
    pDL->SetVolume(1010101010101010);
    pDL->SetAmount(2020202020202020);
    pDL->SetCurrentValue(303030303030303);
    pDL->SetTotalValue(4040404040404040);

    pDL->SetOrdinaryBuyVolume(101010100);
    pDL->SetAttackBuyVolume(103010100);
    pDL->SetStrongBuyVolume(11010100);
    pDL->SetOrdinarySellVolume(13500100);
    pDL->SetAttackSellVolume(10450100);
    pDL->SetStrongSellVolume(105670100);
    pDL->SetUnknownVolume(13510100);
    pDL->SetCanceledBuyVolume(1034610100);
    pDL->SetCanceledSellVolume(105010100);

    pDL->SetTransactionNumber(1350100);
    pDL->SetTransactionNumberBelow5000(1350100);
    pDL->SetTransactionNumberBelow50000(1350100);
    pDL->SetTransactionNumberBelow200000(1350100);
    pDL->SetTransactionNumberAbove200000(1350100);

    pDL->SetAttackBuyBelow50000(1234545);
    pDL->SetAttackBuyBelow200000(12345245);
    pDL->SetAttackBuyAbove200000(12345145);
    pDL->SetAttackSellBelow50000(12345345);
    pDL->SetAttackSellBelow200000(123453445);
    pDL->SetAttackSellAbove200000(123454556);

    pDL->SetOrdinaryBuyNumberBelow5000(123);
    pDL->SetOrdinaryBuyNumberBelow10000(1223);
    pDL->SetOrdinaryBuyNumberBelow20000(14523);
    pDL->SetOrdinaryBuyNumberBelow50000(14523);
    pDL->SetOrdinaryBuyNumberBelow100000(165623);
    pDL->SetOrdinaryBuyNumberBelow200000(18723);
    pDL->SetOrdinaryBuyNumberAbove200000(12673);
    pDL->SetOrdinarySellNumberBelow5000(123);
    pDL->SetOrdinarySellNumberBelow10000(1223);
    pDL->SetOrdinarySellNumberBelow20000(14523);
    pDL->SetOrdinarySellNumberBelow50000(14523);
    pDL->SetOrdinarySellNumberBelow100000(165623);
    pDL->SetOrdinarySellNumberBelow200000(18723);
    pDL->SetOrdinarySellNumberAbove200000(12673);

    pDL->SetOrdinaryBuyVolumeBelow5000(123);
    pDL->SetOrdinaryBuyVolumeBelow10000(1223);
    pDL->SetOrdinaryBuyVolumeBelow20000(14523);
    pDL->SetOrdinaryBuyVolumeBelow50000(14523);
    pDL->SetOrdinaryBuyVolumeBelow100000(165623);
    pDL->SetOrdinaryBuyVolumeBelow200000(18723);
    pDL->SetOrdinaryBuyVolumeAbove200000(12673);
    pDL->SetOrdinarySellVolumeBelow5000(123);
    pDL->SetOrdinarySellVolumeBelow10000(1223);
    pDL->SetOrdinarySellVolumeBelow20000(14523);
    pDL->SetOrdinarySellVolumeBelow50000(14523);
    pDL->SetOrdinarySellVolumeBelow100000(165623);
    pDL->SetOrdinarySellVolumeBelow200000(18723);
    pDL->SetOrdinarySellVolumeAbove200000(12673);

    pDL->SetCanceledBuyVolumeBelow5000(12343);
    pDL->SetCanceledBuyVolumeBelow10000(124523);
    pDL->SetCanceledBuyVolumeBelow20000(145656723);
    pDL->SetCanceledBuyVolumeBelow50000(1467523);
    pDL->SetCanceledBuyVolumeBelow100000(16765623);
    pDL->SetCanceledBuyVolumeBelow200000(1867723);
    pDL->SetCanceledBuyVolumeAbove200000(128767673);
    pDL->SetCanceledSellVolumeBelow5000(13423);
    pDL->SetCanceledSellVolumeBelow10000(1256223);
    pDL->SetCanceledSellVolumeBelow20000(14587823);
    pDL->SetCanceledSellVolumeBelow50000(1459723);
    pDL->SetCanceledSellVolumeBelow100000(67165623);
    pDL->SetCanceledSellVolumeBelow200000(1867723);
    pDL->SetCanceledSellVolumeAbove200000(1266773);

    weekLine.UpdateWeekLine(pDL);

    EXPECT_EQ(weekLine.GetFormatedMarketDate(), GetCurrentMonday(pDL->GetFormatedMarketDate())) << "周线日期总是当周的星期一";
    EXPECT_EQ(weekLine.GetFormatedMarketTime(), pDL->GetFormatedMarketTime());
    EXPECT_STREQ(weekLine.GetStockCode(), pDL->GetStockCode());
    EXPECT_STREQ(weekLine.GetStockName(), pDL->GetStockName());

    EXPECT_EQ(weekLine.GetOpen(), pDL->GetOpen());
    EXPECT_EQ(weekLine.GetClose(), pDL->GetClose());
    EXPECT_EQ(weekLine.GetHigh(), pDL->GetHigh());
    EXPECT_EQ(weekLine.GetLow(), pDL->GetLow());
    EXPECT_EQ(weekLine.GetLastClose(), pDL->GetLastClose());
    EXPECT_DOUBLE_EQ(weekLine.GetUpDown(), pDL->GetUpDown());

    EXPECT_EQ(weekLine.GetVolume(), pDL->GetVolume());
    EXPECT_EQ(weekLine.GetAmount(), pDL->GetAmount());
    EXPECT_EQ(weekLine.GetCurrentValue(), pDL->GetCurrentValue());
    EXPECT_EQ(weekLine.GetTotalValue(), pDL->GetTotalValue());

    EXPECT_EQ(weekLine.GetOrdinaryBuyVolume(), pDL->GetOrdinaryBuyVolume());
    EXPECT_EQ(weekLine.GetAttackBuyVolume(), pDL->GetAttackBuyVolume());
    EXPECT_EQ(weekLine.GetStrongBuyVolume(), pDL->GetStrongBuyVolume());
    EXPECT_EQ(weekLine.GetOrdinarySellVolume(), pDL->GetOrdinarySellVolume());
    EXPECT_EQ(weekLine.GetAttackSellVolume(), pDL->GetAttackSellVolume());
    EXPECT_EQ(weekLine.GetStrongSellVolume(), pDL->GetStrongSellVolume());
    EXPECT_EQ(weekLine.GetUnknownVolume(), pDL->GetUnknownVolume());
    EXPECT_EQ(weekLine.GetCanceledBuyVolume(), pDL->GetCanceledBuyVolume());
    EXPECT_EQ(weekLine.GetCanceledSellVolume(), pDL->GetCanceledSellVolume());

    EXPECT_EQ(weekLine.GetTransactionNumber(), pDL->GetTransactionNumber());
    EXPECT_EQ(weekLine.GetTransactionNumberBelow5000(), pDL->GetTransactionNumberBelow5000());
    EXPECT_EQ(weekLine.GetTransactionNumberBelow50000(), pDL->GetTransactionNumberBelow50000());
    EXPECT_EQ(weekLine.GetTransactionNumberBelow200000(), pDL->GetTransactionNumberBelow200000());
    EXPECT_EQ(weekLine.GetTransactionNumberAbove200000(), pDL->GetTransactionNumberAbove200000());

    EXPECT_EQ(weekLine.GetAttackBuyBelow50000(), pDL->GetAttackBuyBelow50000());
    EXPECT_EQ(weekLine.GetAttackBuyBelow200000(), pDL->GetAttackBuyBelow200000());
    EXPECT_EQ(weekLine.GetAttackBuyAbove200000(), pDL->GetAttackBuyAbove200000());
    EXPECT_EQ(weekLine.GetAttackSellBelow50000(), pDL->GetAttackSellBelow50000());
    EXPECT_EQ(weekLine.GetAttackSellBelow200000(), pDL->GetAttackSellBelow200000());
    EXPECT_EQ(weekLine.GetAttackSellAbove200000(), pDL->GetAttackSellAbove200000());

    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow5000(), pDL->GetOrdinaryBuyNumberBelow5000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow10000(), pDL->GetOrdinaryBuyNumberBelow10000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow20000(), pDL->GetOrdinaryBuyNumberBelow20000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow50000(), pDL->GetOrdinaryBuyNumberBelow50000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow100000(), pDL->GetOrdinaryBuyNumberBelow100000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow200000(), pDL->GetOrdinaryBuyNumberBelow200000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberAbove200000(), pDL->GetOrdinaryBuyNumberAbove200000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow5000(), pDL->GetOrdinarySellNumberBelow5000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow10000(), pDL->GetOrdinarySellNumberBelow10000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow20000(), pDL->GetOrdinarySellNumberBelow20000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow50000(), pDL->GetOrdinarySellNumberBelow50000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow100000(), pDL->GetOrdinarySellNumberBelow100000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow200000(), pDL->GetOrdinarySellNumberBelow200000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberAbove200000(), pDL->GetOrdinarySellNumberAbove200000());

    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow5000(), pDL->GetOrdinaryBuyVolumeBelow5000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow10000(), pDL->GetOrdinaryBuyVolumeBelow10000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow20000(), pDL->GetOrdinaryBuyVolumeBelow20000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow50000(), pDL->GetOrdinaryBuyVolumeBelow50000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow100000(), pDL->GetOrdinaryBuyVolumeBelow100000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow200000(), pDL->GetOrdinaryBuyVolumeBelow200000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeAbove200000(), pDL->GetOrdinaryBuyVolumeAbove200000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow5000(), pDL->GetOrdinarySellVolumeBelow5000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow10000(), pDL->GetOrdinarySellVolumeBelow10000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow20000(), pDL->GetOrdinarySellVolumeBelow20000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow50000(), pDL->GetOrdinarySellVolumeBelow50000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow100000(), pDL->GetOrdinarySellVolumeBelow100000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow200000(), pDL->GetOrdinarySellVolumeBelow200000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeAbove200000(), pDL->GetOrdinarySellVolumeAbove200000());
  }

  TEST_F(CWeekLineTest, TestCreateWeekLine2) {
    CWeekLine weekLine;
    CDLPtr pDL1 = make_shared<CDL>(), pDL2 = make_shared<CDL>();
    pDL1->SetTime(100100100100);
    pDL1->SetDate(20200727);
    pDL1->SetStockCode(_T("sh600000"));
    pDL1->SetStockName(_T("浦发银行"));

    pDL1->SetOpen(1010);
    pDL1->SetClose(2020);
    pDL1->SetHigh(3030);
    pDL1->SetLow(4040);
    pDL1->SetLastClose(5050);
    pDL1->SetUpDown((double)(pDL1->GetClose() - pDL1->GetLastClose()) / 1000);

    pDL1->SetVolume(1010101010101010);
    pDL1->SetAmount(2020202020202020);
    pDL1->SetCurrentValue(303030303030303);
    pDL1->SetTotalValue(4040404040404040);

    pDL1->SetOrdinaryBuyVolume(10101000);
    pDL1->SetAttackBuyVolume(103010100);
    pDL1->SetStrongBuyVolume(10451100);
    pDL1->SetOrdinarySellVolume(13501100);
    pDL1->SetAttackSellVolume(10456000);
    pDL1->SetStrongSellVolume(10510100);
    pDL1->SetUnknownVolume(13510100);
    pDL1->SetCanceledBuyVolume(1034610100);
    pDL1->SetCanceledSellVolume(105010100);

    pDL1->SetTransactionNumber(1350100);
    pDL1->SetTransactionNumberBelow5000(1350100);
    pDL1->SetTransactionNumberBelow50000(1350100);
    pDL1->SetTransactionNumberBelow200000(1350100);
    pDL1->SetTransactionNumberAbove200000(1350100);

    pDL1->SetAttackBuyBelow50000(1234545);
    pDL1->SetAttackBuyBelow200000(12345245);
    pDL1->SetAttackBuyAbove200000(12345145);
    pDL1->SetAttackSellBelow50000(12345345);
    pDL1->SetAttackSellBelow200000(123453445);
    pDL1->SetAttackSellAbove200000(123454556);

    pDL1->SetAttackBuyBelow50000(1234545);
    pDL1->SetAttackBuyBelow200000(12345245);
    pDL1->SetAttackBuyAbove200000(12345145);
    pDL1->SetAttackSellBelow50000(12345345);
    pDL1->SetAttackSellBelow200000(123453445);
    pDL1->SetAttackSellAbove200000(123454556);

    pDL1->SetOrdinaryBuyNumberBelow5000(123);
    pDL1->SetOrdinaryBuyNumberBelow10000(1223);
    pDL1->SetOrdinaryBuyNumberBelow20000(14523);
    pDL1->SetOrdinaryBuyNumberBelow50000(14523);
    pDL1->SetOrdinaryBuyNumberBelow100000(165623);
    pDL1->SetOrdinaryBuyNumberBelow200000(18723);
    pDL1->SetOrdinaryBuyNumberAbove200000(12673);
    pDL1->SetOrdinarySellNumberBelow5000(123);
    pDL1->SetOrdinarySellNumberBelow10000(1223);
    pDL1->SetOrdinarySellNumberBelow20000(14523);
    pDL1->SetOrdinarySellNumberBelow50000(14523);
    pDL1->SetOrdinarySellNumberBelow100000(165623);
    pDL1->SetOrdinarySellNumberBelow200000(18723);
    pDL1->SetOrdinarySellNumberAbove200000(12673);

    pDL1->SetOrdinaryBuyVolumeBelow5000(123);
    pDL1->SetOrdinaryBuyVolumeBelow10000(1223);
    pDL1->SetOrdinaryBuyVolumeBelow20000(14523);
    pDL1->SetOrdinaryBuyVolumeBelow50000(14523);
    pDL1->SetOrdinaryBuyVolumeBelow100000(165623);
    pDL1->SetOrdinaryBuyVolumeBelow200000(18723);
    pDL1->SetOrdinaryBuyVolumeAbove200000(12673);
    pDL1->SetOrdinarySellVolumeBelow5000(123);
    pDL1->SetOrdinarySellVolumeBelow10000(1223);
    pDL1->SetOrdinarySellVolumeBelow20000(14523);
    pDL1->SetOrdinarySellVolumeBelow50000(14523);
    pDL1->SetOrdinarySellVolumeBelow100000(165623);
    pDL1->SetOrdinarySellVolumeBelow200000(18723);
    pDL1->SetOrdinarySellVolumeAbove200000(12673);

    pDL1->SetCanceledBuyVolumeBelow5000(12343);
    pDL1->SetCanceledBuyVolumeBelow10000(124523);
    pDL1->SetCanceledBuyVolumeBelow20000(145656723);
    pDL1->SetCanceledBuyVolumeBelow50000(1467523);
    pDL1->SetCanceledBuyVolumeBelow100000(16765623);
    pDL1->SetCanceledBuyVolumeBelow200000(1867723);
    pDL1->SetCanceledBuyVolumeAbove200000(128767673);
    pDL1->SetCanceledSellVolumeBelow5000(13423);
    pDL1->SetCanceledSellVolumeBelow10000(1256223);
    pDL1->SetCanceledSellVolumeBelow20000(14587823);
    pDL1->SetCanceledSellVolumeBelow50000(1459723);
    pDL1->SetCanceledSellVolumeBelow100000(67165623);
    pDL1->SetCanceledSellVolumeBelow200000(1867723);
    pDL1->SetCanceledSellVolumeAbove200000(1266773);

    pDL2->SetTime(100100100100);
    pDL2->SetDate(20200728); // 与pDL1处于同一个星期中
    pDL2->SetStockCode(_T("sh600000"));
    pDL2->SetStockName(_T("浦发银行"));

    pDL2->SetOpen(1010);
    pDL2->SetClose(2020);
    pDL2->SetHigh(3030);
    pDL2->SetLow(4040);
    pDL2->SetLastClose(5050);
    pDL2->SetUpDown((double)(pDL2->GetClose() - pDL2->GetLastClose()) / 1000);

    pDL2->SetVolume(1010101010101010);
    pDL2->SetAmount(2020202020202020);
    pDL2->SetCurrentValue(303030303030303);
    pDL2->SetTotalValue(4040404040404040);

    pDL2->SetOrdinaryBuyVolume(101010100);
    pDL2->SetAttackBuyVolume(10301000);
    pDL2->SetStrongBuyVolume(104510100);
    pDL2->SetOrdinarySellVolume(135010100);
    pDL2->SetAttackSellVolume(104560100);
    pDL2->SetStrongSellVolume(10510100);
    pDL2->SetUnknownVolume(13510100);
    pDL2->SetCanceledBuyVolume(1034610100);
    pDL2->SetCanceledSellVolume(105010100);

    pDL2->SetTransactionNumber(1350100);
    pDL2->SetTransactionNumberBelow5000(1350100);
    pDL2->SetTransactionNumberBelow50000(1350100);
    pDL2->SetTransactionNumberBelow200000(1350100);
    pDL2->SetTransactionNumberAbove200000(1350100);

    pDL2->SetAttackBuyBelow50000(1234545);
    pDL2->SetAttackBuyBelow200000(123145245);
    pDL2->SetAttackBuyAbove200000(123453145);
    pDL2->SetAttackSellBelow50000(123445345);
    pDL2->SetAttackSellBelow200000(1235453445);
    pDL2->SetAttackSellAbove200000(1234545566);

    pDL2->SetAttackBuyBelow50000(1234545);
    pDL2->SetAttackBuyBelow200000(12345245);
    pDL2->SetAttackBuyAbove200000(12345145);
    pDL2->SetAttackSellBelow50000(12345345);
    pDL2->SetAttackSellBelow200000(123453445);
    pDL2->SetAttackSellAbove200000(123454556);

    pDL2->SetOrdinaryBuyNumberBelow5000(123);
    pDL2->SetOrdinaryBuyNumberBelow10000(1223);
    pDL2->SetOrdinaryBuyNumberBelow20000(14523);
    pDL2->SetOrdinaryBuyNumberBelow50000(14523);
    pDL2->SetOrdinaryBuyNumberBelow100000(165623);
    pDL2->SetOrdinaryBuyNumberBelow200000(18723);
    pDL2->SetOrdinaryBuyNumberAbove200000(12673);
    pDL2->SetOrdinarySellNumberBelow5000(123);
    pDL2->SetOrdinarySellNumberBelow10000(1223);
    pDL2->SetOrdinarySellNumberBelow20000(14523);
    pDL2->SetOrdinarySellNumberBelow50000(14523);
    pDL2->SetOrdinarySellNumberBelow100000(165623);
    pDL2->SetOrdinarySellNumberBelow200000(18723);
    pDL2->SetOrdinarySellNumberAbove200000(12673);

    pDL2->SetOrdinaryBuyVolumeBelow5000(123);
    pDL2->SetOrdinaryBuyVolumeBelow10000(1223);
    pDL2->SetOrdinaryBuyVolumeBelow20000(14523);
    pDL2->SetOrdinaryBuyVolumeBelow50000(14523);
    pDL2->SetOrdinaryBuyVolumeBelow100000(165623);
    pDL2->SetOrdinaryBuyVolumeBelow200000(18723);
    pDL2->SetOrdinaryBuyVolumeAbove200000(12673);
    pDL2->SetOrdinarySellVolumeBelow5000(123);
    pDL2->SetOrdinarySellVolumeBelow10000(1223);
    pDL2->SetOrdinarySellVolumeBelow20000(14523);
    pDL2->SetOrdinarySellVolumeBelow50000(14523);
    pDL2->SetOrdinarySellVolumeBelow100000(165623);
    pDL2->SetOrdinarySellVolumeBelow200000(18723);
    pDL2->SetOrdinarySellVolumeAbove200000(12673);

    pDL2->SetCanceledBuyVolumeBelow5000(12343);
    pDL2->SetCanceledBuyVolumeBelow10000(124523);
    pDL2->SetCanceledBuyVolumeBelow20000(145656723);
    pDL2->SetCanceledBuyVolumeBelow50000(1467523);
    pDL2->SetCanceledBuyVolumeBelow100000(16765623);
    pDL2->SetCanceledBuyVolumeBelow200000(1867723);
    pDL2->SetCanceledBuyVolumeAbove200000(128767673);
    pDL2->SetCanceledSellVolumeBelow5000(13423);
    pDL2->SetCanceledSellVolumeBelow10000(1256223);
    pDL2->SetCanceledSellVolumeBelow20000(14587823);
    pDL2->SetCanceledSellVolumeBelow50000(1459723);
    pDL2->SetCanceledSellVolumeBelow100000(67165623);
    pDL2->SetCanceledSellVolumeBelow200000(1867723);
    pDL2->SetCanceledSellVolumeAbove200000(1266773);

    weekLine.UpdateWeekLine(pDL1);
    weekLine.UpdateWeekLine(pDL2);

    EXPECT_EQ(weekLine.GetFormatedMarketDate(), pDL1->GetFormatedMarketDate());
    EXPECT_EQ(weekLine.GetFormatedMarketTime(), pDL1->GetFormatedMarketTime());
    EXPECT_STREQ(weekLine.GetStockCode(), pDL1->GetStockCode());
    EXPECT_STREQ(weekLine.GetStockName(), pDL1->GetStockName());

    EXPECT_EQ(weekLine.GetOpen(), pDL1->GetOpen());
    EXPECT_EQ(weekLine.GetClose(), pDL2->GetClose());
    EXPECT_EQ(weekLine.GetHigh(), pDL1->GetHigh() > pDL2->GetHigh() ? pDL1->GetHigh() : pDL2->GetHigh());
    EXPECT_EQ(weekLine.GetLow(), pDL1->GetLow() < pDL2->GetLow() ? pDL1->GetLow() : pDL2->GetLow());
    EXPECT_EQ(weekLine.GetLastClose(), pDL1->GetLastClose());
    EXPECT_DOUBLE_EQ(weekLine.GetUpDown(), (double)(pDL2->GetClose() - pDL1->GetLastClose()) / 1000);

    EXPECT_EQ(weekLine.GetVolume(), pDL1->GetVolume() + pDL2->GetVolume());
    EXPECT_EQ(weekLine.GetAmount(), pDL1->GetAmount() + pDL2->GetAmount());
    EXPECT_EQ(weekLine.GetCurrentValue(), pDL2->GetCurrentValue());
    EXPECT_EQ(weekLine.GetTotalValue(), pDL2->GetTotalValue());

    EXPECT_EQ(weekLine.GetOrdinaryBuyVolume(), pDL1->GetOrdinaryBuyVolume() + pDL2->GetOrdinaryBuyVolume());
    EXPECT_EQ(weekLine.GetAttackBuyVolume(), pDL1->GetAttackBuyVolume() + pDL2->GetAttackBuyVolume());
    EXPECT_EQ(weekLine.GetStrongBuyVolume(), pDL1->GetStrongBuyVolume() + pDL2->GetStrongBuyVolume());
    EXPECT_EQ(weekLine.GetOrdinarySellVolume(), pDL1->GetOrdinarySellVolume() + pDL2->GetOrdinarySellVolume());
    EXPECT_EQ(weekLine.GetAttackSellVolume(), pDL1->GetAttackSellVolume() + pDL2->GetAttackSellVolume());
    EXPECT_EQ(weekLine.GetStrongSellVolume(), pDL1->GetStrongSellVolume() + pDL2->GetStrongSellVolume());
    EXPECT_EQ(weekLine.GetUnknownVolume(), pDL1->GetUnknownVolume() + pDL2->GetUnknownVolume());
    EXPECT_EQ(weekLine.GetCanceledBuyVolume(), pDL1->GetCanceledBuyVolume() + pDL2->GetCanceledBuyVolume());
    EXPECT_EQ(weekLine.GetCanceledSellVolume(), pDL1->GetCanceledSellVolume() + pDL2->GetCanceledSellVolume());

    EXPECT_EQ(weekLine.GetTransactionNumber(), pDL1->GetTransactionNumber() + pDL2->GetTransactionNumber());
    EXPECT_EQ(weekLine.GetTransactionNumberBelow5000(), pDL1->GetTransactionNumberBelow5000() + pDL2->GetTransactionNumberBelow50000());
    EXPECT_EQ(weekLine.GetTransactionNumberBelow50000(), pDL1->GetTransactionNumberBelow50000() + pDL2->GetTransactionNumberBelow50000());
    EXPECT_EQ(weekLine.GetTransactionNumberBelow200000(), pDL1->GetTransactionNumberBelow200000() + pDL2->GetTransactionNumberBelow200000());
    EXPECT_EQ(weekLine.GetTransactionNumberAbove200000(), pDL1->GetTransactionNumberAbove200000() + pDL2->GetTransactionNumberAbove200000());

    EXPECT_EQ(weekLine.GetAttackBuyBelow50000(), pDL1->GetAttackBuyBelow50000() + pDL2->GetAttackBuyBelow50000());
    EXPECT_EQ(weekLine.GetAttackBuyBelow200000(), pDL1->GetAttackBuyBelow200000() + pDL2->GetAttackBuyBelow200000());
    EXPECT_EQ(weekLine.GetAttackBuyAbove200000(), pDL1->GetAttackBuyAbove200000() + pDL2->GetAttackBuyAbove200000());
    EXPECT_EQ(weekLine.GetAttackSellBelow50000(), pDL1->GetAttackSellBelow50000() + pDL2->GetAttackSellBelow50000());
    EXPECT_EQ(weekLine.GetAttackSellBelow200000(), pDL1->GetAttackSellBelow200000() + pDL2->GetAttackSellBelow200000());
    EXPECT_EQ(weekLine.GetAttackSellAbove200000(), pDL1->GetAttackSellAbove200000() + pDL2->GetAttackSellAbove200000());

    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow5000(), pDL1->GetOrdinaryBuyNumberBelow5000() + pDL2->GetOrdinaryBuyNumberBelow5000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow10000(), pDL1->GetOrdinaryBuyNumberBelow10000() + pDL2->GetOrdinaryBuyNumberBelow10000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow20000(), pDL1->GetOrdinaryBuyNumberBelow20000() + pDL2->GetOrdinaryBuyNumberBelow20000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow50000(), pDL1->GetOrdinaryBuyNumberBelow50000() + pDL2->GetOrdinaryBuyNumberBelow50000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow100000(), pDL1->GetOrdinaryBuyNumberBelow100000() + pDL2->GetOrdinaryBuyNumberBelow100000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow200000(), pDL1->GetOrdinaryBuyNumberBelow200000() + pDL2->GetOrdinaryBuyNumberBelow200000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberAbove200000(), pDL1->GetOrdinaryBuyNumberAbove200000() + pDL2->GetOrdinaryBuyNumberAbove200000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow5000(), pDL1->GetOrdinarySellNumberBelow5000() + pDL2->GetOrdinarySellNumberBelow5000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow10000(), pDL1->GetOrdinarySellNumberBelow10000() + pDL2->GetOrdinarySellNumberBelow10000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow20000(), pDL1->GetOrdinarySellNumberBelow20000() + pDL2->GetOrdinarySellNumberBelow20000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow50000(), pDL1->GetOrdinarySellNumberBelow50000() + pDL2->GetOrdinarySellNumberBelow50000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow100000(), pDL1->GetOrdinarySellNumberBelow100000() + pDL2->GetOrdinarySellNumberBelow100000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow200000(), pDL1->GetOrdinarySellNumberBelow200000() + pDL2->GetOrdinarySellNumberBelow200000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberAbove200000(), pDL1->GetOrdinarySellNumberAbove200000() + pDL2->GetOrdinarySellNumberAbove200000());

    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow5000(), pDL1->GetOrdinaryBuyVolumeBelow5000() + pDL2->GetOrdinaryBuyVolumeBelow5000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow10000(), pDL1->GetOrdinaryBuyVolumeBelow10000() + pDL2->GetOrdinaryBuyVolumeBelow10000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow20000(), pDL1->GetOrdinaryBuyVolumeBelow20000() + pDL2->GetOrdinaryBuyVolumeBelow20000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow50000(), pDL1->GetOrdinaryBuyVolumeBelow50000() + pDL2->GetOrdinaryBuyVolumeBelow50000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow100000(), pDL1->GetOrdinaryBuyVolumeBelow100000() + pDL2->GetOrdinaryBuyVolumeBelow100000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow200000(), pDL1->GetOrdinaryBuyVolumeBelow200000() + pDL2->GetOrdinaryBuyVolumeBelow200000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeAbove200000(), pDL1->GetOrdinaryBuyVolumeAbove200000() + pDL2->GetOrdinaryBuyVolumeAbove200000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow5000(), pDL1->GetOrdinarySellVolumeBelow5000() + pDL2->GetOrdinarySellVolumeBelow5000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow10000(), pDL1->GetOrdinarySellVolumeBelow10000() + pDL2->GetOrdinarySellVolumeBelow10000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow20000(), pDL1->GetOrdinarySellVolumeBelow20000() + pDL2->GetOrdinarySellVolumeBelow20000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow50000(), pDL1->GetOrdinarySellVolumeBelow50000() + pDL2->GetOrdinarySellVolumeBelow50000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow100000(), pDL1->GetOrdinarySellVolumeBelow100000() + pDL2->GetOrdinarySellVolumeBelow100000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow200000(), pDL1->GetOrdinarySellVolumeBelow200000() + pDL2->GetOrdinarySellVolumeBelow200000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeAbove200000(), pDL1->GetOrdinarySellVolumeAbove200000() + pDL2->GetOrdinarySellVolumeAbove200000());

    EXPECT_EQ(weekLine.GetCanceledBuyVolumeBelow5000(), pDL1->GetCanceledBuyVolumeBelow5000() + pDL2->GetCanceledBuyVolumeBelow5000());
    EXPECT_EQ(weekLine.GetCanceledBuyVolumeBelow10000(), pDL1->GetCanceledBuyVolumeBelow10000() + pDL2->GetCanceledBuyVolumeBelow10000());
    EXPECT_EQ(weekLine.GetCanceledBuyVolumeBelow20000(), pDL1->GetCanceledBuyVolumeBelow20000() + pDL2->GetCanceledBuyVolumeBelow20000());
    EXPECT_EQ(weekLine.GetCanceledBuyVolumeBelow50000(), pDL1->GetCanceledBuyVolumeBelow50000() + pDL2->GetCanceledBuyVolumeBelow50000());
    EXPECT_EQ(weekLine.GetCanceledBuyVolumeBelow100000(), pDL1->GetCanceledBuyVolumeBelow100000() + pDL2->GetCanceledBuyVolumeBelow100000());
    EXPECT_EQ(weekLine.GetCanceledBuyVolumeBelow200000(), pDL1->GetCanceledBuyVolumeBelow200000() + pDL2->GetCanceledBuyVolumeBelow200000());
    EXPECT_EQ(weekLine.GetCanceledBuyVolumeAbove200000(), pDL1->GetCanceledBuyVolumeAbove200000() + pDL2->GetCanceledBuyVolumeAbove200000());
    EXPECT_EQ(weekLine.GetCanceledSellVolumeBelow5000(), pDL1->GetCanceledSellVolumeBelow5000() + pDL2->GetCanceledSellVolumeBelow5000());
    EXPECT_EQ(weekLine.GetCanceledSellVolumeBelow10000(), pDL1->GetCanceledSellVolumeBelow10000() + pDL2->GetCanceledSellVolumeBelow10000());
    EXPECT_EQ(weekLine.GetCanceledSellVolumeBelow20000(), pDL1->GetCanceledSellVolumeBelow20000() + pDL2->GetCanceledSellVolumeBelow20000());
    EXPECT_EQ(weekLine.GetCanceledSellVolumeBelow50000(), pDL1->GetCanceledSellVolumeBelow50000() + pDL2->GetCanceledSellVolumeBelow50000());
    EXPECT_EQ(weekLine.GetCanceledSellVolumeBelow100000(), pDL1->GetCanceledSellVolumeBelow100000() + pDL2->GetCanceledSellVolumeBelow100000());
    EXPECT_EQ(weekLine.GetCanceledSellVolumeBelow200000(), pDL1->GetCanceledSellVolumeBelow200000() + pDL2->GetCanceledSellVolumeBelow200000());
    EXPECT_EQ(weekLine.GetCanceledSellVolumeAbove200000(), pDL1->GetCanceledSellVolumeAbove200000() + pDL2->GetCanceledSellVolumeAbove200000());
  }
}