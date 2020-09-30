#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"DayLine.h"

namespace StockAnalysisTest {
  class CStockDLTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) { // 本测试类的初始化函数
      ASSERT_FALSE(gl_fNormalMode);
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetStockCode();
    }

    static void TearDownTestSuite() {
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetStockCode();
      EXPECT_EQ(gl_pChinaStockMarket->GetDLNeedProcessNumber(), 0);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
    }
  };

  TEST_F(CStockDLTest, TestGetDate) {
    ASSERT_FALSE(gl_fNormalMode);
    CDL dl;
    dl.SetDate(__CHINA_MARKET_BEGIN_DAY__);
    EXPECT_EQ(dl.GetFormatedMarketDate(), __CHINA_MARKET_BEGIN_DAY__);
  }

  TEST_F(CStockDLTest, TestGetTime) {
    CDL dl;
    EXPECT_EQ(dl.GetFormatedMarketDate(), 0);
    dl.SetTime(100100100);
    EXPECT_EQ(dl.GetFormatedMarketTime(), 100100100);
  }

  TEST_F(CStockDLTest, TestGetMarket) {
    CDL dl;
    EXPECT_EQ(dl.GetMarket(), 0);
    dl.SetMarket(1);
    EXPECT_EQ(dl.GetMarket(), 1);
  }

  TEST_F(CStockDLTest, TestGetStockCode) {
    CDL dl;
    EXPECT_STREQ(dl.GetStockCode(), _T(""));
    dl.SetStockCode(_T("sh600000"));
    EXPECT_STREQ(dl.GetStockCode(), _T("sh600000"));
  }

  TEST_F(CStockDLTest, TestGetStockName) {
    CDL dl;
    EXPECT_STREQ(dl.GetStockName(), _T(""));
    dl.SetStockName(_T("浦东银行"));
    EXPECT_STREQ(dl.GetStockName(), _T("浦东银行"));
  }

  TEST_F(CStockDLTest, TestGetLastClose) {
    CDL dl;
    EXPECT_EQ(dl.GetLastClose(), 0);
    dl.SetLastClose(10100);
    EXPECT_EQ(dl.GetLastClose(), 10100);
  }

  TEST_F(CStockDLTest, TestGetOpen) {
    ASSERT_FALSE(gl_fNormalMode);
    CDL dl;
    EXPECT_EQ(dl.GetOpen(), 0);
    dl.SetOpen(11100);
    EXPECT_EQ(dl.GetOpen(), 11100);
  }

  TEST_F(CStockDLTest, TestGetHigh) {
    CDL dl;
    EXPECT_EQ(dl.GetHigh(), 0);
    dl.SetHigh(11111);
    EXPECT_EQ(dl.GetHigh(), 11111);
  }

  TEST_F(CStockDLTest, TestGetLow) {
    CDL dl;
    EXPECT_EQ(dl.GetLow(), 0);
    dl.SetLow(22222);
    EXPECT_EQ(dl.GetLow(), 22222);
    dl.SetLow(_T("12345"));
    EXPECT_EQ(dl.GetLow(), 12345);
  }

  TEST_F(CStockDLTest, TestGetClose) {
    CDL dl;
    EXPECT_EQ(dl.GetClose(), 0);
    dl.SetClose(33333);
    EXPECT_EQ(dl.GetClose(), 33333);
    dl.SetClose(_T("12345"));
    EXPECT_EQ(dl.GetClose(), 12345);
  }

  TEST_F(CStockDLTest, TestGetUpDown) {
    CDL dl;
    EXPECT_DOUBLE_EQ(dl.GetUpDown(), 0);
    dl.SetUpDown(10.020);
    EXPECT_DOUBLE_EQ(dl.GetUpDown(), 10.020);
    dl.SetUpDown(_T("12.345"));
    EXPECT_DOUBLE_EQ(dl.GetUpDown(), 12.345);
  }

  TEST_F(CStockDLTest, TestGetUpDownRate) {
    CDL dl;
    EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 0);
    dl.SetUpDownRate(40.40);
    EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 40.40);
    dl.SetUpDownRate(_T("12.345"));
    EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 12.345);
  }

  TEST_F(CStockDLTest, TestGetChangeHandRate) {
    ASSERT_FALSE(gl_fNormalMode);
    CDL dl;
    EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 0);
    dl.SetChangeHandRate(30.30);
    EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 30.30);
  }

  TEST_F(CStockDLTest, TestGetVolume) {
    CDL dl;
    EXPECT_EQ(dl.GetVolume(), 0);
    dl.SetVolume(100100100100100100);
    EXPECT_EQ(dl.GetVolume(), 100100100100100100);
  }

  TEST_F(CStockDLTest, TestGetAmount) {
    CDL dl;
    EXPECT_EQ(dl.GetAmount(), 0);
    dl.SetAmount(200200200200200200);
    EXPECT_EQ(dl.GetAmount(), 200200200200200200);
  }

  TEST_F(CStockDLTest, TestGetTotalValue) {
    CDL dl;
    EXPECT_EQ(dl.GetTotalValue(), 0);
    CString str = _T("3.13e+11");
    dl.SetTotalValue(str.GetBuffer());
    EXPECT_DOUBLE_EQ((double)(dl.GetTotalValue()), 3.13e+11);
  }

  TEST_F(CStockDLTest, TestGetCurrentValue) {
    CDL dl;
    EXPECT_EQ(dl.GetCurrentValue(), 0);
    CString str = _T("3.15e+11");
    dl.SetCurrentValue(str.GetBuffer());
    EXPECT_DOUBLE_EQ((double)(dl.GetCurrentValue()), 3.15e+11);
  }

  TEST_F(CStockDLTest, TestGetRS) {
    CDL dl;
    EXPECT_DOUBLE_EQ(dl.GetRS(), 0);
    dl.SetRS(50.50);
    EXPECT_DOUBLE_EQ(dl.GetRS(), 50.50);
  }

  TEST_F(CStockDLTest, TestGetRSIndex) {
    CDL dl;
    EXPECT_DOUBLE_EQ(dl.GetRSIndex(), 0);
    dl.SetRSIndex(50.5023);
    EXPECT_DOUBLE_EQ(dl.GetRSIndex(), 50.5023);
  }
  TEST_F(CStockDLTest, TestGetRSBackup) {
    CDL dl;
    EXPECT_DOUBLE_EQ(dl.GetRSBackup(), 0);
    dl.SetRSBackup(50.506);
    EXPECT_DOUBLE_EQ(dl.GetRSBackup(), 50.506);
  }

  TEST_F(CStockDLTest, TestGetRSLogarithm) {
    CDL dl;
    EXPECT_DOUBLE_EQ((double)dl.GetRSLogarithm(), 0.0);
    dl.SetRSLogarithm(50.50);
    EXPECT_DOUBLE_EQ((double)dl.GetRSLogarithm(), 50.50);
  }

  TEST_F(CStockDLTest, TestGetTransactionNumber) {
    CDL dl;
    EXPECT_EQ(dl.GetTransactionNumber(), 0);
    dl.SetTransactionNumber(600600);
    EXPECT_EQ(dl.GetTransactionNumber(), 600600);
  }

  TEST_F(CStockDLTest, TestGetTransactionNumberBelow5000) {
    CDL dl;
    EXPECT_EQ(dl.GetTransactionNumberBelow5000(), 0);
    dl.SetTransactionNumberBelow5000(10);
    EXPECT_EQ(dl.GetTransactionNumberBelow5000(), 10);
  }

  TEST_F(CStockDLTest, TestGetTransactionNumberBelow50000) {
    CDL dl;
    EXPECT_EQ(dl.GetTransactionNumberBelow50000(), 0);
    dl.SetTransactionNumberBelow50000(10);
    EXPECT_EQ(dl.GetTransactionNumberBelow50000(), 10);
  }

  TEST_F(CStockDLTest, TestGetTransactionNumberBelow200000) {
    CDL dl;
    EXPECT_EQ(dl.GetTransactionNumberBelow200000(), 0);
    dl.SetTransactionNumberBelow200000(10);
    EXPECT_EQ(dl.GetTransactionNumberBelow200000(), 10);
  }

  TEST_F(CStockDLTest, TestGetTransactionNumberAbove200000) {
    CDL dl;
    EXPECT_EQ(dl.GetTransactionNumberAbove200000(), 0);
    dl.SetTransactionNumberAbove200000(10);
    EXPECT_EQ(dl.GetTransactionNumberAbove200000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinaryBuyVolume) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolume(), 0);
    dl.SetOrdinaryBuyVolume(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolume(), 10);
  }

  TEST_F(CStockDLTest, TestGetAttackBuyVolume) {
    CDL dl;
    EXPECT_EQ(dl.GetAttackBuyVolume(), 0);
    dl.SetAttackBuyVolume(10);
    EXPECT_EQ(dl.GetAttackBuyVolume(), 10);
  }

  TEST_F(CStockDLTest, TestGetStrongBuyVolume) {
    CDL dl;
    EXPECT_EQ(dl.GetStrongBuyVolume(), 0);
    dl.SetStrongBuyVolume(10);
    EXPECT_EQ(dl.GetStrongBuyVolume(), 10);
  }

  TEST_F(CStockDLTest, TestGetStrongSellVolume) {
    CDL dl;
    EXPECT_EQ(dl.GetStrongSellVolume(), 0);
    dl.SetStrongSellVolume(10);
    EXPECT_EQ(dl.GetStrongSellVolume(), 10);
  }

  TEST_F(CStockDLTest, TestGetAttackSellVolume) {
    CDL dl;
    EXPECT_EQ(dl.GetAttackSellVolume(), 0);
    dl.SetAttackSellVolume(10);
    EXPECT_EQ(dl.GetAttackSellVolume(), 10);
  }

  TEST_F(CStockDLTest, TestGetUnknownVolume) {
    CDL dl;
    EXPECT_EQ(dl.GetUnknownVolume(), 0);
    dl.SetUnknownVolume(10);
    EXPECT_EQ(dl.GetUnknownVolume(), 10);
  }

  TEST_F(CStockDLTest, TestGetCanceledBuyVolume) {
    CDL dl;
    EXPECT_EQ(dl.GetCanceledBuyVolume(), 0);
    dl.SetCanceledBuyVolume(10);
    EXPECT_EQ(dl.GetCanceledBuyVolume(), 10);
  }

  TEST_F(CStockDLTest, TestGetCanceledSellVolume) {
    CDL dl;
    EXPECT_EQ(dl.GetCanceledSellVolume(), 0);
    dl.SetCanceledSellVolume(10);
    EXPECT_EQ(dl.GetCanceledSellVolume(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinarySellVolume) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinarySellVolume(), 0);
    dl.SetOrdinarySellVolume(10);
    EXPECT_EQ(dl.GetOrdinarySellVolume(), 10);
  }

  TEST_F(CStockDLTest, TestGetAttackBuyBelow50000) {
    CDL dl;
    EXPECT_EQ(dl.GetAttackBuyBelow50000(), 0);
    dl.SetAttackBuyBelow50000(10);
    EXPECT_EQ(dl.GetAttackBuyBelow50000(), 10);
  }

  TEST_F(CStockDLTest, TestGetAttackBuyBelow200000) {
    CDL dl;
    EXPECT_EQ(dl.GetAttackBuyBelow200000(), 0);
    dl.SetAttackBuyBelow200000(10);
    EXPECT_EQ(dl.GetAttackBuyBelow200000(), 10);
  }

  TEST_F(CStockDLTest, TestGetAttackBuyAbove200000) {
    CDL dl;
    EXPECT_EQ(dl.GetAttackBuyAbove200000(), 0);
    dl.SetAttackBuyAbove200000(10);
    EXPECT_EQ(dl.GetAttackBuyAbove200000(), 10);
  }

  TEST_F(CStockDLTest, TestGetAttackSellAbove200000) {
    CDL dl;
    EXPECT_EQ(dl.GetAttackSellAbove200000(), 0);
    dl.SetAttackSellAbove200000(10);
    EXPECT_EQ(dl.GetAttackSellAbove200000(), 10);
  }

  TEST_F(CStockDLTest, TestGetAttackSellBelow200000) {
    CDL dl;
    EXPECT_EQ(dl.GetAttackSellBelow200000(), 0);
    dl.SetAttackSellBelow200000(10);
    EXPECT_EQ(dl.GetAttackSellBelow200000(), 10);
  }

  TEST_F(CStockDLTest, TestGetAttackSellBelow50000) {
    CDL dl;
    EXPECT_EQ(dl.GetAttackSellBelow50000(), 0);
    dl.SetAttackSellBelow50000(10);
    EXPECT_EQ(dl.GetAttackSellBelow50000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinaryBuyVolumeBelow5000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow5000(), 0);
    dl.SetOrdinaryBuyVolumeBelow5000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow5000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinaryBuyVolumeBelow10000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow10000(), 0);
    dl.SetOrdinaryBuyVolumeBelow10000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow10000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinaryBuyVolumeBelow20000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow20000(), 0);
    dl.SetOrdinaryBuyVolumeBelow20000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow20000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinaryBuyVolumeBelow50000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow50000(), 0);
    dl.SetOrdinaryBuyVolumeBelow50000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow50000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinaryBuyVolumeBelow100000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow100000(), 0);
    dl.SetOrdinaryBuyVolumeBelow100000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow100000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinaryBuyVolumeBelow200000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow200000(), 0);
    dl.SetOrdinaryBuyVolumeBelow200000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeBelow200000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinaryBuyVolumeAbove200000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeAbove200000(), 0);
    dl.SetOrdinaryBuyVolumeAbove200000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolumeAbove200000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinarySellVolumeBelow5000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow5000(), 0);
    dl.SetOrdinarySellVolumeBelow5000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow5000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinarySellVolumeBelow10000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow10000(), 0);
    dl.SetOrdinarySellVolumeBelow10000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow10000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinarySellVolumeBelow20000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow20000(), 0);
    dl.SetOrdinarySellVolumeBelow20000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow20000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinarySellVolumeBelow50000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow50000(), 0);
    dl.SetOrdinarySellVolumeBelow50000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow50000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinarySellVolumeBelow100000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow100000(), 0);
    dl.SetOrdinarySellVolumeBelow100000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow100000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinarySellVolumeBelow200000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow200000(), 0);
    dl.SetOrdinarySellVolumeBelow200000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeBelow200000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinarySellVolumeAbove200000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinarySellVolumeAbove200000(), 0);
    dl.SetOrdinarySellVolumeAbove200000(10);
    EXPECT_EQ(dl.GetOrdinarySellVolumeAbove200000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinaryBuyNumberBelow5000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow5000(), 0);
    dl.SetOrdinaryBuyNumberBelow5000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow5000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinaryBuyNumberBelow10000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow10000(), 0);
    dl.SetOrdinaryBuyNumberBelow10000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow10000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinaryBuyNumberBelow20000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow20000(), 0);
    dl.SetOrdinaryBuyNumberBelow20000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow20000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinaryBuyNumberBelow50000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow50000(), 0);
    dl.SetOrdinaryBuyNumberBelow50000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow50000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinaryBuyNumberBelow100000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow100000(), 0);
    dl.SetOrdinaryBuyNumberBelow100000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow100000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinaryBuyNumberBelow200000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow200000(), 0);
    dl.SetOrdinaryBuyNumberBelow200000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberBelow200000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinaryBuyNumberAbove200000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinaryBuyNumberAbove200000(), 0);
    dl.SetOrdinaryBuyNumberAbove200000(10);
    EXPECT_EQ(dl.GetOrdinaryBuyNumberAbove200000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinarySellNumberBelow5000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow5000(), 0);
    dl.SetOrdinarySellNumberBelow5000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow5000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinarySellNumberBelow10000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow10000(), 0);
    dl.SetOrdinarySellNumberBelow10000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow10000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinarySellNumberBelow20000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow20000(), 0);
    dl.SetOrdinarySellNumberBelow20000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow20000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinarySellNumberBelow50000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow50000(), 0);
    dl.SetOrdinarySellNumberBelow50000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow50000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinarySellNumberBelow100000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow100000(), 0);
    dl.SetOrdinarySellNumberBelow100000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow100000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinarySellNumberBelow200000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow200000(), 0);
    dl.SetOrdinarySellNumberBelow200000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberBelow200000(), 10);
  }

  TEST_F(CStockDLTest, TestGetOrdinarySellNumberAbove200000) {
    CDL dl;
    EXPECT_EQ(dl.GetOrdinarySellNumberAbove200000(), 0);
    dl.SetOrdinarySellNumberAbove200000(10);
    EXPECT_EQ(dl.GetOrdinarySellNumberAbove200000(), 10);
  }

  TEST_F(CStockDLTest, TestGet3DayRS) {
    CDL dl;
    EXPECT_EQ(dl.Get3DayRS(), 0);
    dl.Set3DayRS(10);
    EXPECT_EQ(dl.Get3DayRS(), 10);
  }

  TEST_F(CStockDLTest, TestGet5DayRS) {
    CDL dl;
    EXPECT_EQ(dl.Get5DayRS(), 0);
    dl.Set5DayRS(10);
    EXPECT_EQ(dl.Get5DayRS(), 10);
  }

  TEST_F(CStockDLTest, TestGet10DayRS) {
    CDL dl;
    EXPECT_EQ(dl.Get10DayRS(), 0);
    dl.Set10DayRS(10);
    EXPECT_EQ(dl.Get10DayRS(), 10);
  }

  TEST_F(CStockDLTest, TestGet30DayRS) {
    CDL dl;
    EXPECT_EQ(dl.Get30DayRS(), 0);
    dl.Set30DayRS(10);
    EXPECT_EQ(dl.Get30DayRS(), 10);
  }

  TEST_F(CStockDLTest, TestGet60DayRS) {
    CDL dl;
    EXPECT_EQ(dl.Get60DayRS(), 0);
    dl.Set60DayRS(10);
    EXPECT_EQ(dl.Get60DayRS(), 10);
  }

  TEST_F(CStockDLTest, TestGet120DayRS) {
    CDL dl;
    EXPECT_EQ(dl.Get120DayRS(), 0);
    dl.Set120DayRS(10);
    EXPECT_EQ(dl.Get120DayRS(), 10);
  }

  TEST_F(CStockDLTest, TestSaveData) {
    CSetDLBasicInfo setDLBasicInfo;
    CDL id, id2;

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

    ASSERT(!gl_fNormalMode);
    setDLBasicInfo.m_strFilter = _T("[ID] = 1");
    setDLBasicInfo.Open();
    setDLBasicInfo.m_pDatabase->BeginTrans();
    id.AppendData(&setDLBasicInfo);
    setDLBasicInfo.m_pDatabase->CommitTrans();
    setDLBasicInfo.Close();

    setDLBasicInfo.m_strFilter = _T("[Date] = 21101101");
    setDLBasicInfo.Open();
    id2.LoadBasicData(&setDLBasicInfo);
    EXPECT_EQ(setDLBasicInfo.m_Date, id.GetFormatedMarketDate());
    EXPECT_EQ(setDLBasicInfo.m_Market, id.GetMarket());
    EXPECT_STREQ(setDLBasicInfo.m_StockCode, id.GetStockCode());
    //EXPECT_STREQ(setDLBasicInfo.m_StockName, id.GetStockName());
    EXPECT_DOUBLE_EQ(atof(setDLBasicInfo.m_LastClose) * 1000, id.GetLastClose());
    EXPECT_DOUBLE_EQ(atof(setDLBasicInfo.m_Open) * 1000, id.GetOpen());
    EXPECT_DOUBLE_EQ(atof(setDLBasicInfo.m_High) * 1000, id.GetHigh());
    EXPECT_DOUBLE_EQ(atof(setDLBasicInfo.m_Low) * 1000, id.GetLow());
    EXPECT_DOUBLE_EQ(atof(setDLBasicInfo.m_Close) * 1000, id.GetClose());
    EXPECT_EQ(atoll(setDLBasicInfo.m_Volume), id.GetVolume());
    EXPECT_EQ(atoll(setDLBasicInfo.m_Amount), id.GetAmount());
    EXPECT_DOUBLE_EQ(atof(setDLBasicInfo.m_UpAndDown), id.GetUpDown());
    EXPECT_DOUBLE_EQ(atof(setDLBasicInfo.m_UpDownRate), id.GetUpDownRate());
    EXPECT_EQ(atoll(setDLBasicInfo.m_TotalValue), id.GetTotalValue());
    EXPECT_EQ(atoll(setDLBasicInfo.m_CurrentValue), id.GetCurrentValue());
    EXPECT_DOUBLE_EQ(atof(setDLBasicInfo.m_ChangeHandRate), id.GetChangeHandRate());
    EXPECT_DOUBLE_EQ(atof(setDLBasicInfo.m_RS), id.GetRS());
    EXPECT_DOUBLE_EQ(atof(setDLBasicInfo.m_RSIndex), id.GetRSIndex());
    EXPECT_DOUBLE_EQ(atof(setDLBasicInfo.m_RSBackup), id.GetRSBackup());
    setDLBasicInfo.Close();

    EXPECT_EQ(id2.GetFormatedMarketDate(), id.GetFormatedMarketDate());
    EXPECT_EQ(id2.GetMarket(), id.GetMarket());
    EXPECT_STREQ(id2.GetStockCode(), id.GetStockCode());
    //EXPECT_STREQ(setDLBasicInfo.m_StockName, id.GetStockName());
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

    setDLBasicInfo.Open();
    setDLBasicInfo.m_pDatabase->BeginTrans();
    while (!setDLBasicInfo.IsEOF()) {
      setDLBasicInfo.Delete();
      setDLBasicInfo.MoveNext();
    }
    setDLBasicInfo.m_pDatabase->CommitTrans();
    setDLBasicInfo.Close();
  }

  TEST_F(CStockDLTest, TestLoadData) {
    CSetDLBasicInfo setDLBasicInfo;
    CDL id, idLoaded;

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
    setDLBasicInfo.m_strFilter = _T("[ID] = 1");
    setDLBasicInfo.Open();
    setDLBasicInfo.m_pDatabase->BeginTrans();
    id.AppendData(&setDLBasicInfo);
    setDLBasicInfo.m_pDatabase->CommitTrans();
    setDLBasicInfo.Close();

    setDLBasicInfo.m_strFilter = _T("[Date] = 21101101");
    setDLBasicInfo.Open();
    idLoaded.LoadBasicData(&setDLBasicInfo);
    EXPECT_EQ(idLoaded.GetFormatedMarketDate(), id.GetFormatedMarketDate());
    EXPECT_EQ(idLoaded.GetMarket(), id.GetMarket());
    EXPECT_STREQ(idLoaded.GetStockCode(), id.GetStockCode());
    //EXPECT_STREQ(setDLBasicInfo.m_StockName, id.GetStockName());
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
    setDLBasicInfo.Close();

    setDLBasicInfo.Open();
    setDLBasicInfo.m_pDatabase->BeginTrans();
    while (!setDLBasicInfo.IsEOF()) {
      setDLBasicInfo.Delete();
      setDLBasicInfo.MoveNext();
    }
    setDLBasicInfo.m_pDatabase->CommitTrans();
    setDLBasicInfo.Close();
  }

  TEST_F(CStockDLTest, TestIsActive) {
    CDL dayLine;
    EXPECT_FALSE(dayLine.IsActive());
    dayLine.SetClose(100);
    EXPECT_FALSE(dayLine.IsActive());
    dayLine.SetOpen(100);
    EXPECT_FALSE(dayLine.IsActive());
    dayLine.SetLastClose(100);
    EXPECT_TRUE(dayLine.IsActive());
  }

  TEST_F(CStockDLTest, TestLoadDL) {
    CDL id, id2;
    CSetDLBasicInfo setDLBasicInfo;
    id.SetDate(__CHINA_MARKET_BEGIN_DAY__);
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
    setDLBasicInfo.m_strFilter = _T("[ID] = 1");
    setDLBasicInfo.Open();
    setDLBasicInfo.AddNew();
    id.SaveData(&setDLBasicInfo);
    setDLBasicInfo.Update();
    setDLBasicInfo.Close();

    setDLBasicInfo.m_strFilter = _T("StockCode = 'sh600000'");
    setDLBasicInfo.Open();
    id2.LoadBasicData(&setDLBasicInfo);
    EXPECT_EQ(id.GetFormatedMarketDate(), id2.GetFormatedMarketDate());
    EXPECT_STREQ(id.GetStockCode(), id2.GetStockCode());
    EXPECT_EQ(id.GetOpen(), id2.GetOpen());

    setDLBasicInfo.Delete();
    setDLBasicInfo.Close();
  }
}