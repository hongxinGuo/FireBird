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

  TEST_F(CWeekLineTest, TestGetDay) {
    ASSERT_FALSE(gl_fNormalMode);
    CWeekLine dl;
    dl.SetDay(__CHINA_MARKET_BEGIN_DAY__);
    EXPECT_EQ(dl.GetFormatedMarketDay(), __CHINA_MARKET_BEGIN_DAY__);
  }

  TEST_F(CWeekLineTest, TestGetTime) {
    CWeekLine dl;
    EXPECT_EQ(dl.GetFormatedMarketDay(), 0);
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

  TEST_F(CWeekLineTest, TestGetRelativeStrong) {
    CWeekLine dl;
    EXPECT_DOUBLE_EQ(dl.GetRelativeStrong(), 0);
    dl.SetRelativeStrong(50.50);
    EXPECT_DOUBLE_EQ(dl.GetRelativeStrong(), 50.50);
  }

  TEST_F(CWeekLineTest, TestGetRelativeStrongIndex) {
    CWeekLine dl;
    EXPECT_DOUBLE_EQ(dl.GetRelativeStrongIndex(), 0);
    dl.SetRelativeStrongIndex(50.5023);
    EXPECT_DOUBLE_EQ(dl.GetRelativeStrongIndex(), 50.5023);
  }
  TEST_F(CWeekLineTest, TestGetRelativeStrongBackup) {
    CWeekLine dl;
    EXPECT_DOUBLE_EQ(dl.GetRelativeStrongBackup(), 0);
    dl.SetRelativeStrongBackup(50.506);
    EXPECT_DOUBLE_EQ(dl.GetRelativeStrongBackup(), 50.506);
  }

  TEST_F(CWeekLineTest, TestGetRSLogarithm) {
    CWeekLine dl;
    EXPECT_DOUBLE_EQ((double)dl.GetRelativeStrongLogarithm(), 0.0);
    dl.SetRSLogarithm(50.50);
    EXPECT_DOUBLE_EQ((double)dl.GetRelativeStrongLogarithm(), 50.50);
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
    CWeekLine id, id2;

    id.SetDay(21101101);
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
    id.SetRelativeStrong(14.5);
    id.SetRelativeStrongIndex(15.6);
    id.SetRelativeStrongBackup(20.9);

    ASSERT(!gl_fNormalMode);
    setWeekLineBasicInfo.m_strFilter = _T("[Day] = 21101101");
    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.m_pDatabase->BeginTrans();
    id.AppendData(&setWeekLineBasicInfo);
    setWeekLineBasicInfo.m_pDatabase->CommitTrans();
    setWeekLineBasicInfo.Close();

    setWeekLineBasicInfo.m_strFilter = _T("[Day] = 21101101");
    setWeekLineBasicInfo.Open();
    id2.LoadBasicData(&setWeekLineBasicInfo);
    EXPECT_EQ(setWeekLineBasicInfo.m_Day, id.GetFormatedMarketDay());
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
    EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_RelativeStrong), id.GetRelativeStrong());
    EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_RelativeStrongIndex), id.GetRelativeStrongIndex());
    EXPECT_DOUBLE_EQ(atof(setWeekLineBasicInfo.m_RelativeStrongBackup), id.GetRelativeStrongBackup());
    setWeekLineBasicInfo.Close();

    EXPECT_EQ(id2.GetFormatedMarketDay(), id.GetFormatedMarketDay());
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
    EXPECT_DOUBLE_EQ(id2.GetRelativeStrong(), id.GetRelativeStrong());
    EXPECT_DOUBLE_EQ(id2.GetRelativeStrongIndex(), id.GetRelativeStrongIndex());
    EXPECT_DOUBLE_EQ(id2.GetRelativeStrongBackup(), id.GetRelativeStrongBackup());

    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.m_pDatabase->BeginTrans();
    while (!setWeekLineBasicInfo.IsEOF()) {
      setWeekLineBasicInfo.Delete();
      setWeekLineBasicInfo.MoveNext();
    }
    setWeekLineBasicInfo.m_pDatabase->CommitTrans();
    setWeekLineBasicInfo.Close();
  }

  TEST_F(CWeekLineTest, TestLoadData) {
    CSetWeekLineBasicInfo setWeekLineBasicInfo;
    CWeekLine id, idLoaded;

    id.SetDay(21101101);
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
    id.SetRelativeStrong(14.5);
    id.SetRelativeStrongIndex(135.6);
    id.SetRelativeStrongBackup(120.9);

    ASSERT(!gl_fNormalMode);
    setWeekLineBasicInfo.m_strFilter = _T("[Day] = 21101101");
    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.m_pDatabase->BeginTrans();
    id.AppendData(&setWeekLineBasicInfo);
    setWeekLineBasicInfo.m_pDatabase->CommitTrans();
    setWeekLineBasicInfo.Close();

    setWeekLineBasicInfo.m_strFilter = _T("[Day] = 21101101");
    setWeekLineBasicInfo.Open();
    idLoaded.LoadBasicData(&setWeekLineBasicInfo);
    EXPECT_EQ(idLoaded.GetFormatedMarketDay(), id.GetFormatedMarketDay());
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
    EXPECT_DOUBLE_EQ(idLoaded.GetRelativeStrong(), id.GetRelativeStrong());
    EXPECT_DOUBLE_EQ(idLoaded.GetRelativeStrongIndex(), id.GetRelativeStrongIndex());
    EXPECT_DOUBLE_EQ(idLoaded.GetRelativeStrongBackup(), id.GetRelativeStrongBackup());
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
    id.SetDay(__CHINA_MARKET_BEGIN_DAY__);
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
    setWeekLineBasicInfo.m_strFilter = _T("[Day] = 19900101");
    setWeekLineBasicInfo.Open();
    setWeekLineBasicInfo.AddNew();
    id.SaveData(&setWeekLineBasicInfo);
    setWeekLineBasicInfo.Update();
    setWeekLineBasicInfo.Close();

    setWeekLineBasicInfo.Open();
    id2.LoadBasicData(&setWeekLineBasicInfo);
    EXPECT_EQ(id.GetFormatedMarketDay(), id2.GetFormatedMarketDay());
    EXPECT_STREQ(id.GetStockCode(), id2.GetStockCode());
    EXPECT_EQ(id.GetOpen(), id2.GetOpen());

    setWeekLineBasicInfo.Delete();
    setWeekLineBasicInfo.Close();
  }
}