#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"WeekLine.h"

namespace StockAnalysisTest {
  class CWeekLineTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) { // ��������ĳ�ʼ������
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
    dl.SetStockName(_T("�ֶ�����"));
    EXPECT_STREQ(dl.GetStockName(), _T("�ֶ�����"));
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
    id.SetStockName(_T("�״��ɷ�"));
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
    id.SetStockName(_T("�״��ɷ�"));
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
    id.SetStockName(_T("�ַ�����"));
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

  TEST_F(CWeekLineTest, TestCreateWeekLine1) {
    CWeekLine weekLine;
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetTime(100100100100);
    pDayLine->SetDay(20200202);
    pDayLine->SetStockCode(_T("sh600000"));
    pDayLine->SetStockName(_T("�ַ�����"));

    pDayLine->SetOpen(1010);
    pDayLine->SetClose(2020);
    pDayLine->SetHigh(3030);
    pDayLine->SetLow(4040);
    pDayLine->SetLastClose(5050);

    pDayLine->SetVolume(1010101010101010);
    pDayLine->SetAmount(2020202020202020);
    pDayLine->SetCurrentValue(303030303030303);
    pDayLine->SetTotalValue(4040404040404040);

    pDayLine->SetOrdinaryBuyVolume(101010100);
    pDayLine->SetAttackBuyVolume(103010100);
    pDayLine->SetStrongBuyVolume(11010100);
    pDayLine->SetOrdinarySellVolume(13500100);
    pDayLine->SetAttackSellVolume(10450100);
    pDayLine->SetStrongSellVolume(105670100);
    pDayLine->SetUnknownVolume(13510100);
    pDayLine->SetCanceledBuyVolume(1034610100);
    pDayLine->SetCanceledSellVolume(105010100);

    pDayLine->SetTransactionNumber(1350100);
    pDayLine->SetTransactionNumberBelow5000(1350100);
    pDayLine->SetTransactionNumberBelow50000(1350100);
    pDayLine->SetTransactionNumberBelow200000(1350100);
    pDayLine->SetTransactionNumberAbove200000(1350100);

    pDayLine->SetAttackBuyBelow50000(1234545);
    pDayLine->SetAttackBuyBelow200000(12345245);
    pDayLine->SetAttackBuyAbove200000(12345145);
    pDayLine->SetAttackSellBelow50000(12345345);
    pDayLine->SetAttackSellBelow200000(123453445);
    pDayLine->SetAttackSellAbove200000(123454556);

    pDayLine->SetOrdinaryBuyNumberBelow5000(123);
    pDayLine->SetOrdinaryBuyNumberBelow10000(1223);
    pDayLine->SetOrdinaryBuyNumberBelow20000(14523);
    pDayLine->SetOrdinaryBuyNumberBelow50000(14523);
    pDayLine->SetOrdinaryBuyNumberBelow100000(165623);
    pDayLine->SetOrdinaryBuyNumberBelow200000(18723);
    pDayLine->SetOrdinaryBuyNumberAbove200000(12673);
    pDayLine->SetOrdinarySellNumberBelow5000(123);
    pDayLine->SetOrdinarySellNumberBelow10000(1223);
    pDayLine->SetOrdinarySellNumberBelow20000(14523);
    pDayLine->SetOrdinarySellNumberBelow50000(14523);
    pDayLine->SetOrdinarySellNumberBelow100000(165623);
    pDayLine->SetOrdinarySellNumberBelow200000(18723);
    pDayLine->SetOrdinarySellNumberAbove200000(12673);

    pDayLine->SetOrdinaryBuyVolumeBelow5000(123);
    pDayLine->SetOrdinaryBuyVolumeBelow10000(1223);
    pDayLine->SetOrdinaryBuyVolumeBelow20000(14523);
    pDayLine->SetOrdinaryBuyVolumeBelow50000(14523);
    pDayLine->SetOrdinaryBuyVolumeBelow100000(165623);
    pDayLine->SetOrdinaryBuyVolumeBelow200000(18723);
    pDayLine->SetOrdinaryBuyVolumeAbove200000(12673);
    pDayLine->SetOrdinarySellVolumeBelow5000(123);
    pDayLine->SetOrdinarySellVolumeBelow10000(1223);
    pDayLine->SetOrdinarySellVolumeBelow20000(14523);
    pDayLine->SetOrdinarySellVolumeBelow50000(14523);
    pDayLine->SetOrdinarySellVolumeBelow100000(165623);
    pDayLine->SetOrdinarySellVolumeBelow200000(18723);
    pDayLine->SetOrdinarySellVolumeAbove200000(12673);

    pDayLine->SetCanceledBuyVolumeBelow5000(12343);
    pDayLine->SetCanceledBuyVolumeBelow10000(124523);
    pDayLine->SetCanceledBuyVolumeBelow20000(145656723);
    pDayLine->SetCanceledBuyVolumeBelow50000(1467523);
    pDayLine->SetCanceledBuyVolumeBelow100000(16765623);
    pDayLine->SetCanceledBuyVolumeBelow200000(1867723);
    pDayLine->SetCanceledBuyVolumeAbove200000(128767673);
    pDayLine->SetCanceledSellVolumeBelow5000(13423);
    pDayLine->SetCanceledSellVolumeBelow10000(1256223);
    pDayLine->SetCanceledSellVolumeBelow20000(14587823);
    pDayLine->SetCanceledSellVolumeBelow50000(1459723);
    pDayLine->SetCanceledSellVolumeBelow100000(67165623);
    pDayLine->SetCanceledSellVolumeBelow200000(1867723);
    pDayLine->SetCanceledSellVolumeAbove200000(1266773);

    weekLine.CreateWeekLine(pDayLine);

    EXPECT_EQ(weekLine.GetFormatedMarketDay(), pDayLine->GetFormatedMarketDay());
    EXPECT_EQ(weekLine.GetFormatedMarketTime(), pDayLine->GetFormatedMarketTime());
    EXPECT_STREQ(weekLine.GetStockCode(), pDayLine->GetStockCode());
    EXPECT_STREQ(weekLine.GetStockName(), pDayLine->GetStockName());

    EXPECT_EQ(weekLine.GetOpen(), pDayLine->GetOpen());
    EXPECT_EQ(weekLine.GetClose(), pDayLine->GetClose());
    EXPECT_EQ(weekLine.GetHigh(), pDayLine->GetHigh());
    EXPECT_EQ(weekLine.GetLow(), pDayLine->GetLow());
    EXPECT_EQ(weekLine.GetLastClose(), pDayLine->GetLastClose());

    EXPECT_EQ(weekLine.GetVolume(), pDayLine->GetVolume());
    EXPECT_EQ(weekLine.GetAmount(), pDayLine->GetAmount());
    EXPECT_EQ(weekLine.GetCurrentValue(), pDayLine->GetCurrentValue());
    EXPECT_EQ(weekLine.GetTotalValue(), pDayLine->GetTotalValue());

    EXPECT_EQ(weekLine.GetOrdinaryBuyVolume(), pDayLine->GetOrdinaryBuyVolume());
    EXPECT_EQ(weekLine.GetAttackBuyVolume(), pDayLine->GetAttackBuyVolume());
    EXPECT_EQ(weekLine.GetStrongBuyVolume(), pDayLine->GetStrongBuyVolume());
    EXPECT_EQ(weekLine.GetOrdinarySellVolume(), pDayLine->GetOrdinarySellVolume());
    EXPECT_EQ(weekLine.GetAttackSellVolume(), pDayLine->GetAttackSellVolume());
    EXPECT_EQ(weekLine.GetStrongSellVolume(), pDayLine->GetStrongSellVolume());
    EXPECT_EQ(weekLine.GetUnknownVolume(), pDayLine->GetUnknownVolume());
    EXPECT_EQ(weekLine.GetCanceledBuyVolume(), pDayLine->GetCanceledBuyVolume());
    EXPECT_EQ(weekLine.GetCanceledSellVolume(), pDayLine->GetCanceledSellVolume());

    EXPECT_EQ(weekLine.GetTransactionNumber(), pDayLine->GetTransactionNumber());
    EXPECT_EQ(weekLine.GetTransactionNumberBelow5000(), pDayLine->GetTransactionNumberBelow5000());
    EXPECT_EQ(weekLine.GetTransactionNumberBelow50000(), pDayLine->GetTransactionNumberBelow50000());
    EXPECT_EQ(weekLine.GetTransactionNumberBelow200000(), pDayLine->GetTransactionNumberBelow200000());
    EXPECT_EQ(weekLine.GetTransactionNumberAbove200000(), pDayLine->GetTransactionNumberAbove200000());

    EXPECT_EQ(weekLine.GetAttackBuyBelow50000(), pDayLine->GetAttackBuyBelow50000());
    EXPECT_EQ(weekLine.GetAttackBuyBelow200000(), pDayLine->GetAttackBuyBelow200000());
    EXPECT_EQ(weekLine.GetAttackBuyAbove200000(), pDayLine->GetAttackBuyAbove200000());
    EXPECT_EQ(weekLine.GetAttackSellBelow50000(), pDayLine->GetAttackSellBelow50000());
    EXPECT_EQ(weekLine.GetAttackSellBelow200000(), pDayLine->GetAttackSellBelow200000());
    EXPECT_EQ(weekLine.GetAttackSellAbove200000(), pDayLine->GetAttackSellAbove200000());

    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow5000(), pDayLine->GetOrdinaryBuyNumberBelow5000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow10000(), pDayLine->GetOrdinaryBuyNumberBelow10000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow20000(), pDayLine->GetOrdinaryBuyNumberBelow20000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow50000(), pDayLine->GetOrdinaryBuyNumberBelow50000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow100000(), pDayLine->GetOrdinaryBuyNumberBelow100000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow200000(), pDayLine->GetOrdinaryBuyNumberBelow200000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberAbove200000(), pDayLine->GetOrdinaryBuyNumberAbove200000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow5000(), pDayLine->GetOrdinarySellNumberBelow5000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow10000(), pDayLine->GetOrdinarySellNumberBelow10000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow20000(), pDayLine->GetOrdinarySellNumberBelow20000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow50000(), pDayLine->GetOrdinarySellNumberBelow50000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow100000(), pDayLine->GetOrdinarySellNumberBelow100000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow200000(), pDayLine->GetOrdinarySellNumberBelow200000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberAbove200000(), pDayLine->GetOrdinarySellNumberAbove200000());

    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow5000(), pDayLine->GetOrdinaryBuyVolumeBelow5000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow10000(), pDayLine->GetOrdinaryBuyVolumeBelow10000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow20000(), pDayLine->GetOrdinaryBuyVolumeBelow20000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow50000(), pDayLine->GetOrdinaryBuyVolumeBelow50000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow100000(), pDayLine->GetOrdinaryBuyVolumeBelow100000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow200000(), pDayLine->GetOrdinaryBuyVolumeBelow200000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeAbove200000(), pDayLine->GetOrdinaryBuyVolumeAbove200000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow5000(), pDayLine->GetOrdinarySellVolumeBelow5000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow10000(), pDayLine->GetOrdinarySellVolumeBelow10000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow20000(), pDayLine->GetOrdinarySellVolumeBelow20000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow50000(), pDayLine->GetOrdinarySellVolumeBelow50000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow100000(), pDayLine->GetOrdinarySellVolumeBelow100000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow200000(), pDayLine->GetOrdinarySellVolumeBelow200000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeAbove200000(), pDayLine->GetOrdinarySellVolumeAbove200000());
  }

  TEST_F(CWeekLineTest, TestCreateWeekLine2) {
    CWeekLine weekLine;
    CDayLinePtr pDayLine1 = make_shared<CDayLine>(), pDayLine2 = make_shared<CDayLine>();
    pDayLine1->SetTime(100100100100);
    pDayLine1->SetDay(20200727);
    pDayLine1->SetStockCode(_T("sh600000"));
    pDayLine1->SetStockName(_T("�ַ�����"));

    pDayLine1->SetOpen(1010);
    pDayLine1->SetClose(2020);
    pDayLine1->SetHigh(3030);
    pDayLine1->SetLow(4040);
    pDayLine1->SetLastClose(5050);

    pDayLine1->SetVolume(1010101010101010);
    pDayLine1->SetAmount(2020202020202020);
    pDayLine1->SetCurrentValue(303030303030303);
    pDayLine1->SetTotalValue(4040404040404040);

    pDayLine1->SetOrdinaryBuyVolume(10101000);
    pDayLine1->SetAttackBuyVolume(103010100);
    pDayLine1->SetStrongBuyVolume(10451100);
    pDayLine1->SetOrdinarySellVolume(13501100);
    pDayLine1->SetAttackSellVolume(10456000);
    pDayLine1->SetStrongSellVolume(10510100);
    pDayLine1->SetUnknownVolume(13510100);
    pDayLine1->SetCanceledBuyVolume(1034610100);
    pDayLine1->SetCanceledSellVolume(105010100);

    pDayLine1->SetTransactionNumber(1350100);
    pDayLine1->SetTransactionNumberBelow5000(1350100);
    pDayLine1->SetTransactionNumberBelow50000(1350100);
    pDayLine1->SetTransactionNumberBelow200000(1350100);
    pDayLine1->SetTransactionNumberAbove200000(1350100);

    pDayLine1->SetAttackBuyBelow50000(1234545);
    pDayLine1->SetAttackBuyBelow200000(12345245);
    pDayLine1->SetAttackBuyAbove200000(12345145);
    pDayLine1->SetAttackSellBelow50000(12345345);
    pDayLine1->SetAttackSellBelow200000(123453445);
    pDayLine1->SetAttackSellAbove200000(123454556);

    pDayLine1->SetAttackBuyBelow50000(1234545);
    pDayLine1->SetAttackBuyBelow200000(12345245);
    pDayLine1->SetAttackBuyAbove200000(12345145);
    pDayLine1->SetAttackSellBelow50000(12345345);
    pDayLine1->SetAttackSellBelow200000(123453445);
    pDayLine1->SetAttackSellAbove200000(123454556);

    pDayLine1->SetOrdinaryBuyNumberBelow5000(123);
    pDayLine1->SetOrdinaryBuyNumberBelow10000(1223);
    pDayLine1->SetOrdinaryBuyNumberBelow20000(14523);
    pDayLine1->SetOrdinaryBuyNumberBelow50000(14523);
    pDayLine1->SetOrdinaryBuyNumberBelow100000(165623);
    pDayLine1->SetOrdinaryBuyNumberBelow200000(18723);
    pDayLine1->SetOrdinaryBuyNumberAbove200000(12673);
    pDayLine1->SetOrdinarySellNumberBelow5000(123);
    pDayLine1->SetOrdinarySellNumberBelow10000(1223);
    pDayLine1->SetOrdinarySellNumberBelow20000(14523);
    pDayLine1->SetOrdinarySellNumberBelow50000(14523);
    pDayLine1->SetOrdinarySellNumberBelow100000(165623);
    pDayLine1->SetOrdinarySellNumberBelow200000(18723);
    pDayLine1->SetOrdinarySellNumberAbove200000(12673);

    pDayLine1->SetOrdinaryBuyVolumeBelow5000(123);
    pDayLine1->SetOrdinaryBuyVolumeBelow10000(1223);
    pDayLine1->SetOrdinaryBuyVolumeBelow20000(14523);
    pDayLine1->SetOrdinaryBuyVolumeBelow50000(14523);
    pDayLine1->SetOrdinaryBuyVolumeBelow100000(165623);
    pDayLine1->SetOrdinaryBuyVolumeBelow200000(18723);
    pDayLine1->SetOrdinaryBuyVolumeAbove200000(12673);
    pDayLine1->SetOrdinarySellVolumeBelow5000(123);
    pDayLine1->SetOrdinarySellVolumeBelow10000(1223);
    pDayLine1->SetOrdinarySellVolumeBelow20000(14523);
    pDayLine1->SetOrdinarySellVolumeBelow50000(14523);
    pDayLine1->SetOrdinarySellVolumeBelow100000(165623);
    pDayLine1->SetOrdinarySellVolumeBelow200000(18723);
    pDayLine1->SetOrdinarySellVolumeAbove200000(12673);

    pDayLine1->SetCanceledBuyVolumeBelow5000(12343);
    pDayLine1->SetCanceledBuyVolumeBelow10000(124523);
    pDayLine1->SetCanceledBuyVolumeBelow20000(145656723);
    pDayLine1->SetCanceledBuyVolumeBelow50000(1467523);
    pDayLine1->SetCanceledBuyVolumeBelow100000(16765623);
    pDayLine1->SetCanceledBuyVolumeBelow200000(1867723);
    pDayLine1->SetCanceledBuyVolumeAbove200000(128767673);
    pDayLine1->SetCanceledSellVolumeBelow5000(13423);
    pDayLine1->SetCanceledSellVolumeBelow10000(1256223);
    pDayLine1->SetCanceledSellVolumeBelow20000(14587823);
    pDayLine1->SetCanceledSellVolumeBelow50000(1459723);
    pDayLine1->SetCanceledSellVolumeBelow100000(67165623);
    pDayLine1->SetCanceledSellVolumeBelow200000(1867723);
    pDayLine1->SetCanceledSellVolumeAbove200000(1266773);

    pDayLine2->SetTime(100100100100);
    pDayLine2->SetDay(20200728); // ��pDayLine1����ͬһ��������
    pDayLine2->SetStockCode(_T("sh600000"));
    pDayLine2->SetStockName(_T("�ַ�����"));

    pDayLine2->SetOpen(1010);
    pDayLine2->SetClose(2020);
    pDayLine2->SetHigh(3030);
    pDayLine2->SetLow(4040);
    pDayLine2->SetLastClose(5050);

    pDayLine2->SetVolume(1010101010101010);
    pDayLine2->SetAmount(2020202020202020);
    pDayLine2->SetCurrentValue(303030303030303);
    pDayLine2->SetTotalValue(4040404040404040);

    pDayLine2->SetOrdinaryBuyVolume(101010100);
    pDayLine2->SetAttackBuyVolume(10301000);
    pDayLine2->SetStrongBuyVolume(104510100);
    pDayLine2->SetOrdinarySellVolume(135010100);
    pDayLine2->SetAttackSellVolume(104560100);
    pDayLine2->SetStrongSellVolume(10510100);
    pDayLine2->SetUnknownVolume(13510100);
    pDayLine2->SetCanceledBuyVolume(1034610100);
    pDayLine2->SetCanceledSellVolume(105010100);

    pDayLine2->SetTransactionNumber(1350100);
    pDayLine2->SetTransactionNumberBelow5000(1350100);
    pDayLine2->SetTransactionNumberBelow50000(1350100);
    pDayLine2->SetTransactionNumberBelow200000(1350100);
    pDayLine2->SetTransactionNumberAbove200000(1350100);

    pDayLine2->SetAttackBuyBelow50000(1234545);
    pDayLine2->SetAttackBuyBelow200000(123145245);
    pDayLine2->SetAttackBuyAbove200000(123453145);
    pDayLine2->SetAttackSellBelow50000(123445345);
    pDayLine2->SetAttackSellBelow200000(1235453445);
    pDayLine2->SetAttackSellAbove200000(1234545566);

    pDayLine2->SetAttackBuyBelow50000(1234545);
    pDayLine2->SetAttackBuyBelow200000(12345245);
    pDayLine2->SetAttackBuyAbove200000(12345145);
    pDayLine2->SetAttackSellBelow50000(12345345);
    pDayLine2->SetAttackSellBelow200000(123453445);
    pDayLine2->SetAttackSellAbove200000(123454556);

    pDayLine2->SetOrdinaryBuyNumberBelow5000(123);
    pDayLine2->SetOrdinaryBuyNumberBelow10000(1223);
    pDayLine2->SetOrdinaryBuyNumberBelow20000(14523);
    pDayLine2->SetOrdinaryBuyNumberBelow50000(14523);
    pDayLine2->SetOrdinaryBuyNumberBelow100000(165623);
    pDayLine2->SetOrdinaryBuyNumberBelow200000(18723);
    pDayLine2->SetOrdinaryBuyNumberAbove200000(12673);
    pDayLine2->SetOrdinarySellNumberBelow5000(123);
    pDayLine2->SetOrdinarySellNumberBelow10000(1223);
    pDayLine2->SetOrdinarySellNumberBelow20000(14523);
    pDayLine2->SetOrdinarySellNumberBelow50000(14523);
    pDayLine2->SetOrdinarySellNumberBelow100000(165623);
    pDayLine2->SetOrdinarySellNumberBelow200000(18723);
    pDayLine2->SetOrdinarySellNumberAbove200000(12673);

    pDayLine2->SetOrdinaryBuyVolumeBelow5000(123);
    pDayLine2->SetOrdinaryBuyVolumeBelow10000(1223);
    pDayLine2->SetOrdinaryBuyVolumeBelow20000(14523);
    pDayLine2->SetOrdinaryBuyVolumeBelow50000(14523);
    pDayLine2->SetOrdinaryBuyVolumeBelow100000(165623);
    pDayLine2->SetOrdinaryBuyVolumeBelow200000(18723);
    pDayLine2->SetOrdinaryBuyVolumeAbove200000(12673);
    pDayLine2->SetOrdinarySellVolumeBelow5000(123);
    pDayLine2->SetOrdinarySellVolumeBelow10000(1223);
    pDayLine2->SetOrdinarySellVolumeBelow20000(14523);
    pDayLine2->SetOrdinarySellVolumeBelow50000(14523);
    pDayLine2->SetOrdinarySellVolumeBelow100000(165623);
    pDayLine2->SetOrdinarySellVolumeBelow200000(18723);
    pDayLine2->SetOrdinarySellVolumeAbove200000(12673);

    pDayLine2->SetCanceledBuyVolumeBelow5000(12343);
    pDayLine2->SetCanceledBuyVolumeBelow10000(124523);
    pDayLine2->SetCanceledBuyVolumeBelow20000(145656723);
    pDayLine2->SetCanceledBuyVolumeBelow50000(1467523);
    pDayLine2->SetCanceledBuyVolumeBelow100000(16765623);
    pDayLine2->SetCanceledBuyVolumeBelow200000(1867723);
    pDayLine2->SetCanceledBuyVolumeAbove200000(128767673);
    pDayLine2->SetCanceledSellVolumeBelow5000(13423);
    pDayLine2->SetCanceledSellVolumeBelow10000(1256223);
    pDayLine2->SetCanceledSellVolumeBelow20000(14587823);
    pDayLine2->SetCanceledSellVolumeBelow50000(1459723);
    pDayLine2->SetCanceledSellVolumeBelow100000(67165623);
    pDayLine2->SetCanceledSellVolumeBelow200000(1867723);
    pDayLine2->SetCanceledSellVolumeAbove200000(1266773);

    weekLine.CreateWeekLine(pDayLine1);
    weekLine.CreateWeekLine(pDayLine2);

    EXPECT_EQ(weekLine.GetFormatedMarketDay(), pDayLine1->GetFormatedMarketDay());
    EXPECT_EQ(weekLine.GetFormatedMarketTime(), pDayLine1->GetFormatedMarketTime());
    EXPECT_STREQ(weekLine.GetStockCode(), pDayLine1->GetStockCode());
    EXPECT_STREQ(weekLine.GetStockName(), pDayLine1->GetStockName());

    EXPECT_EQ(weekLine.GetOpen(), pDayLine1->GetOpen());
    EXPECT_EQ(weekLine.GetClose(), pDayLine2->GetClose());
    EXPECT_EQ(weekLine.GetHigh(), pDayLine1->GetHigh() > pDayLine2->GetHigh() ? pDayLine1->GetHigh() : pDayLine2->GetHigh());
    EXPECT_EQ(weekLine.GetLow(), pDayLine1->GetLow() < pDayLine2->GetLow() ? pDayLine1->GetLow() : pDayLine2->GetLow());
    EXPECT_EQ(weekLine.GetLastClose(), pDayLine1->GetLastClose());

    EXPECT_EQ(weekLine.GetVolume(), pDayLine1->GetVolume() + pDayLine2->GetVolume());
    EXPECT_EQ(weekLine.GetAmount(), pDayLine1->GetAmount() + pDayLine2->GetAmount());
    EXPECT_EQ(weekLine.GetCurrentValue(), pDayLine2->GetCurrentValue());
    EXPECT_EQ(weekLine.GetTotalValue(), pDayLine2->GetTotalValue());

    EXPECT_EQ(weekLine.GetOrdinaryBuyVolume(), pDayLine1->GetOrdinaryBuyVolume() + pDayLine2->GetOrdinaryBuyVolume());
    EXPECT_EQ(weekLine.GetAttackBuyVolume(), pDayLine1->GetAttackBuyVolume() + pDayLine2->GetAttackBuyVolume());
    EXPECT_EQ(weekLine.GetStrongBuyVolume(), pDayLine1->GetStrongBuyVolume() + pDayLine2->GetStrongBuyVolume());
    EXPECT_EQ(weekLine.GetOrdinarySellVolume(), pDayLine1->GetOrdinarySellVolume() + pDayLine2->GetOrdinarySellVolume());
    EXPECT_EQ(weekLine.GetAttackSellVolume(), pDayLine1->GetAttackSellVolume() + pDayLine2->GetAttackSellVolume());
    EXPECT_EQ(weekLine.GetStrongSellVolume(), pDayLine1->GetStrongSellVolume() + pDayLine2->GetStrongSellVolume());
    EXPECT_EQ(weekLine.GetUnknownVolume(), pDayLine1->GetUnknownVolume() + pDayLine2->GetUnknownVolume());
    EXPECT_EQ(weekLine.GetCanceledBuyVolume(), pDayLine1->GetCanceledBuyVolume() + pDayLine2->GetCanceledBuyVolume());
    EXPECT_EQ(weekLine.GetCanceledSellVolume(), pDayLine1->GetCanceledSellVolume() + pDayLine2->GetCanceledSellVolume());

    EXPECT_EQ(weekLine.GetTransactionNumber(), pDayLine1->GetTransactionNumber() + pDayLine2->GetTransactionNumber());
    EXPECT_EQ(weekLine.GetTransactionNumberBelow5000(), pDayLine1->GetTransactionNumberBelow5000() + pDayLine2->GetTransactionNumberBelow50000());
    EXPECT_EQ(weekLine.GetTransactionNumberBelow50000(), pDayLine1->GetTransactionNumberBelow50000() + pDayLine2->GetTransactionNumberBelow50000());
    EXPECT_EQ(weekLine.GetTransactionNumberBelow200000(), pDayLine1->GetTransactionNumberBelow200000() + pDayLine2->GetTransactionNumberBelow200000());
    EXPECT_EQ(weekLine.GetTransactionNumberAbove200000(), pDayLine1->GetTransactionNumberAbove200000() + pDayLine2->GetTransactionNumberAbove200000());

    EXPECT_EQ(weekLine.GetAttackBuyBelow50000(), pDayLine1->GetAttackBuyBelow50000() + pDayLine2->GetAttackBuyBelow50000());
    EXPECT_EQ(weekLine.GetAttackBuyBelow200000(), pDayLine1->GetAttackBuyBelow200000() + pDayLine2->GetAttackBuyBelow200000());
    EXPECT_EQ(weekLine.GetAttackBuyAbove200000(), pDayLine1->GetAttackBuyAbove200000() + pDayLine2->GetAttackBuyAbove200000());
    EXPECT_EQ(weekLine.GetAttackSellBelow50000(), pDayLine1->GetAttackSellBelow50000() + pDayLine2->GetAttackSellBelow50000());
    EXPECT_EQ(weekLine.GetAttackSellBelow200000(), pDayLine1->GetAttackSellBelow200000() + pDayLine2->GetAttackSellBelow200000());
    EXPECT_EQ(weekLine.GetAttackSellAbove200000(), pDayLine1->GetAttackSellAbove200000() + pDayLine2->GetAttackSellAbove200000());

    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow5000(), pDayLine1->GetOrdinaryBuyNumberBelow5000() + pDayLine2->GetOrdinaryBuyNumberBelow5000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow10000(), pDayLine1->GetOrdinaryBuyNumberBelow10000() + pDayLine2->GetOrdinaryBuyNumberBelow10000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow20000(), pDayLine1->GetOrdinaryBuyNumberBelow20000() + pDayLine2->GetOrdinaryBuyNumberBelow20000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow50000(), pDayLine1->GetOrdinaryBuyNumberBelow50000() + pDayLine2->GetOrdinaryBuyNumberBelow50000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow100000(), pDayLine1->GetOrdinaryBuyNumberBelow100000() + pDayLine2->GetOrdinaryBuyNumberBelow100000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberBelow200000(), pDayLine1->GetOrdinaryBuyNumberBelow200000() + pDayLine2->GetOrdinaryBuyNumberBelow200000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyNumberAbove200000(), pDayLine1->GetOrdinaryBuyNumberAbove200000() + pDayLine2->GetOrdinaryBuyNumberAbove200000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow5000(), pDayLine1->GetOrdinarySellNumberBelow5000() + pDayLine2->GetOrdinarySellNumberBelow5000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow10000(), pDayLine1->GetOrdinarySellNumberBelow10000() + pDayLine2->GetOrdinarySellNumberBelow10000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow20000(), pDayLine1->GetOrdinarySellNumberBelow20000() + pDayLine2->GetOrdinarySellNumberBelow20000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow50000(), pDayLine1->GetOrdinarySellNumberBelow50000() + pDayLine2->GetOrdinarySellNumberBelow50000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow100000(), pDayLine1->GetOrdinarySellNumberBelow100000() + pDayLine2->GetOrdinarySellNumberBelow100000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberBelow200000(), pDayLine1->GetOrdinarySellNumberBelow200000() + pDayLine2->GetOrdinarySellNumberBelow200000());
    EXPECT_EQ(weekLine.GetOrdinarySellNumberAbove200000(), pDayLine1->GetOrdinarySellNumberAbove200000() + pDayLine2->GetOrdinarySellNumberAbove200000());

    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow5000(), pDayLine1->GetOrdinaryBuyVolumeBelow5000() + pDayLine2->GetOrdinaryBuyVolumeBelow5000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow10000(), pDayLine1->GetOrdinaryBuyVolumeBelow10000() + pDayLine2->GetOrdinaryBuyVolumeBelow10000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow20000(), pDayLine1->GetOrdinaryBuyVolumeBelow20000() + pDayLine2->GetOrdinaryBuyVolumeBelow20000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow50000(), pDayLine1->GetOrdinaryBuyVolumeBelow50000() + pDayLine2->GetOrdinaryBuyVolumeBelow50000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow100000(), pDayLine1->GetOrdinaryBuyVolumeBelow100000() + pDayLine2->GetOrdinaryBuyVolumeBelow100000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeBelow200000(), pDayLine1->GetOrdinaryBuyVolumeBelow200000() + pDayLine2->GetOrdinaryBuyVolumeBelow200000());
    EXPECT_EQ(weekLine.GetOrdinaryBuyVolumeAbove200000(), pDayLine1->GetOrdinaryBuyVolumeAbove200000() + pDayLine2->GetOrdinaryBuyVolumeAbove200000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow5000(), pDayLine1->GetOrdinarySellVolumeBelow5000() + pDayLine2->GetOrdinarySellVolumeBelow5000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow10000(), pDayLine1->GetOrdinarySellVolumeBelow10000() + pDayLine2->GetOrdinarySellVolumeBelow10000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow20000(), pDayLine1->GetOrdinarySellVolumeBelow20000() + pDayLine2->GetOrdinarySellVolumeBelow20000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow50000(), pDayLine1->GetOrdinarySellVolumeBelow50000() + pDayLine2->GetOrdinarySellVolumeBelow50000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow100000(), pDayLine1->GetOrdinarySellVolumeBelow100000() + pDayLine2->GetOrdinarySellVolumeBelow100000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeBelow200000(), pDayLine1->GetOrdinarySellVolumeBelow200000() + pDayLine2->GetOrdinarySellVolumeBelow200000());
    EXPECT_EQ(weekLine.GetOrdinarySellVolumeAbove200000(), pDayLine1->GetOrdinarySellVolumeAbove200000() + pDayLine2->GetOrdinarySellVolumeAbove200000());

    EXPECT_EQ(weekLine.GetCanceledBuyVolumeBelow5000(), pDayLine1->GetCanceledBuyVolumeBelow5000() + pDayLine2->GetCanceledBuyVolumeBelow5000());
    EXPECT_EQ(weekLine.GetCanceledBuyVolumeBelow10000(), pDayLine1->GetCanceledBuyVolumeBelow10000() + pDayLine2->GetCanceledBuyVolumeBelow10000());
    EXPECT_EQ(weekLine.GetCanceledBuyVolumeBelow20000(), pDayLine1->GetCanceledBuyVolumeBelow20000() + pDayLine2->GetCanceledBuyVolumeBelow20000());
    EXPECT_EQ(weekLine.GetCanceledBuyVolumeBelow50000(), pDayLine1->GetCanceledBuyVolumeBelow50000() + pDayLine2->GetCanceledBuyVolumeBelow50000());
    EXPECT_EQ(weekLine.GetCanceledBuyVolumeBelow100000(), pDayLine1->GetCanceledBuyVolumeBelow100000() + pDayLine2->GetCanceledBuyVolumeBelow100000());
    EXPECT_EQ(weekLine.GetCanceledBuyVolumeBelow200000(), pDayLine1->GetCanceledBuyVolumeBelow200000() + pDayLine2->GetCanceledBuyVolumeBelow200000());
    EXPECT_EQ(weekLine.GetCanceledBuyVolumeAbove200000(), pDayLine1->GetCanceledBuyVolumeAbove200000() + pDayLine2->GetCanceledBuyVolumeAbove200000());
    EXPECT_EQ(weekLine.GetCanceledSellVolumeBelow5000(), pDayLine1->GetCanceledSellVolumeBelow5000() + pDayLine2->GetCanceledSellVolumeBelow5000());
    EXPECT_EQ(weekLine.GetCanceledSellVolumeBelow10000(), pDayLine1->GetCanceledSellVolumeBelow10000() + pDayLine2->GetCanceledSellVolumeBelow10000());
    EXPECT_EQ(weekLine.GetCanceledSellVolumeBelow20000(), pDayLine1->GetCanceledSellVolumeBelow20000() + pDayLine2->GetCanceledSellVolumeBelow20000());
    EXPECT_EQ(weekLine.GetCanceledSellVolumeBelow50000(), pDayLine1->GetCanceledSellVolumeBelow50000() + pDayLine2->GetCanceledSellVolumeBelow50000());
    EXPECT_EQ(weekLine.GetCanceledSellVolumeBelow100000(), pDayLine1->GetCanceledSellVolumeBelow100000() + pDayLine2->GetCanceledSellVolumeBelow100000());
    EXPECT_EQ(weekLine.GetCanceledSellVolumeBelow200000(), pDayLine1->GetCanceledSellVolumeBelow200000() + pDayLine2->GetCanceledSellVolumeBelow200000());
    EXPECT_EQ(weekLine.GetCanceledSellVolumeAbove200000(), pDayLine1->GetCanceledSellVolumeAbove200000() + pDayLine2->GetCanceledSellVolumeAbove200000());
  }
}