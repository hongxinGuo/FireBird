#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"DayLine.h"

namespace StockAnalysisTest {
  TEST(CDayLineTest, TestGetDay) {
    ASSERT_FALSE(gl_fNormalMode);
    CDayLine dl;
    dl.SetDay(19900101);
    EXPECT_EQ(dl.GetDay(), 19900101);
  }

  TEST(CDayLineTest, TestGetTime) {
    CDayLine dl;
    EXPECT_EQ(dl.GetDay(), 0);
    dl.SetTime(100100100);
    EXPECT_EQ(dl.GetTime(), 100100100);
  }

  TEST(CDayLineTest, TestGetMarket) {
    CDayLine dl;
    EXPECT_EQ(dl.GetMarket(), 0);
    dl.SetMarket(1);
    EXPECT_EQ(dl.GetMarket(), 1);
  }

  TEST(CDayLineTest, TestGetStockCode) {
    CDayLine dl;
    EXPECT_STREQ(dl.GetStockCode(), _T(""));
    dl.SetStockCode(_T("sh600000"));
    EXPECT_STREQ(dl.GetStockCode(), _T("sh600000"));
  }

  TEST(CDayLineTest, TestGetStockName) {
    CDayLine dl;
    EXPECT_STREQ(dl.GetStockName(), _T(""));
    dl.SetStockName(_T("浦东银行"));
    EXPECT_STREQ(dl.GetStockName(), _T("浦东银行"));
  }

  TEST(CDayLineTest, TestGetLastClose) {
    CDayLine dl;
    EXPECT_EQ(dl.GetLastClose(), 0);
    dl.SetLastClose(10100);
    EXPECT_EQ(dl.GetLastClose(), 10100);
  }

  TEST(CDayLineTest, TestGetOpen) {
    ASSERT_FALSE(gl_fNormalMode);
    CDayLine dl;
    EXPECT_EQ(dl.GetOpen(), 0);
    dl.SetOpen(11100);
    EXPECT_EQ(dl.GetOpen(), 11100);
  }

  TEST(CDayLineTest, TestGetHigh) {
    CDayLine dl;
    EXPECT_EQ(dl.GetHigh(), 0);
    dl.SetHigh(11111);
    EXPECT_EQ(dl.GetHigh(), 11111);
  }

  TEST(CDayLineTest, TestGetLow) {
    CDayLine dl;
    EXPECT_EQ(dl.GetLow(), 0);
    dl.SetLow(22222);
    EXPECT_EQ(dl.GetLow(), 22222);
    dl.SetLow(_T("12345"));
    EXPECT_EQ(dl.GetLow(), 12345);
  }

  TEST(CDayLineTest, TestGetClose) {
    CDayLine dl;
    EXPECT_EQ(dl.GetClose(), 0);
    dl.SetClose(33333);
    EXPECT_EQ(dl.GetClose(), 33333);
    dl.SetClose(_T("12345"));
    EXPECT_EQ(dl.GetClose(), 12345);
  }

  TEST(CDayLineTest, TestGetUpDown) {
    CDayLine dl;
    EXPECT_DOUBLE_EQ(dl.GetUpDown(), 0);
    dl.SetUpDown(10.020);
    EXPECT_DOUBLE_EQ(dl.GetUpDown(), 10.020);
    dl.SetUpDown(_T("12.345"));
    EXPECT_DOUBLE_EQ(dl.GetUpDown(), 12.345);
  }

  TEST(CDayLineTest, TestGetUpDownRate) {
    CDayLine dl;
    EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 0);
    dl.SetUpDownRate(40.40);
    EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 40.40);
    dl.SetUpDownRate(_T("12.345"));
    EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 12.345);
  }

  TEST(CDayLineTest, TestGetChangeHandRate) {
    ASSERT_FALSE(gl_fNormalMode);
    CDayLine dl;
    EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 0);
    dl.SetChangeHandRate(30.30);
    EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 30.30);
  }

  TEST(CDayLineTest, TestGetVolume) {
    CDayLine dl;
    EXPECT_EQ(dl.GetVolume(), 0);
    dl.SetVolume(100100100100100100);
    EXPECT_EQ(dl.GetVolume(), 100100100100100100);
  }

  TEST(CDayLineTest, TestGetAmount) {
    CDayLine dl;
    EXPECT_EQ(dl.GetAmount(), 0);
    dl.SetAmount(200200200200200200);
    EXPECT_EQ(dl.GetAmount(), 200200200200200200);
  }

  TEST(CDayLineTest, TestGetTotalValue) {
    CDayLine dl;
    EXPECT_EQ(dl.GetTotalValue(), 0);
    CString str = _T("3.13e+11");
    dl.SetTotalValue(str.GetBuffer());
    EXPECT_DOUBLE_EQ(dl.GetTotalValue(), 3.13e+11);
  }

  TEST(CDayLineTest, TestGetCurrentValue) {
    CDayLine dl;
    EXPECT_EQ(dl.GetCurrentValue(), 0);
    CString str = _T("3.15e+11");
    dl.SetCurrentValue(str.GetBuffer());
    EXPECT_DOUBLE_EQ(dl.GetCurrentValue(), 3.15e+11);
  }

  TEST(CDayLineTest, TestGetRelativeStrong) {
    CDayLine dl;
    EXPECT_DOUBLE_EQ(dl.GetRelativeStrong(), 0);
    dl.SetRelativeStrong(50.50);
    EXPECT_DOUBLE_EQ(dl.GetRelativeStrong(), 50.50);
  }

  TEST(CDayLineTest, TestGetTransactionNumber) {
    CDayLine dl;
    EXPECT_EQ(dl.GetTransactionNumber(), 0);
    dl.SetTransactionNumber(600600);
    EXPECT_EQ(dl.GetTransactionNumber(), 600600);
  }

  TEST(CDayLineTest, TestGetTransactionNumberBelow5000) {
    CDayLine dl;
    EXPECT_EQ(dl.GetTransactionNumberBelow5000(), 0);
    dl.SetTransactionNumberBelow5000(10);
    EXPECT_EQ(dl.GetTransactionNumberBelow5000(), 10);
  }

  TEST(CDayLineTest, TestGetTransactionNumberBelow50000) {
    CDayLine dl;
    EXPECT_EQ(dl.GetTransactionNumberBelow50000(), 0);
    dl.SetTransactionNumberBelow50000(10);
    EXPECT_EQ(dl.GetTransactionNumberBelow50000(), 10);
  }

  TEST(CDayLineTest, TestGetTransactionNumberBelow200000) {
    CDayLine dl;
    EXPECT_EQ(dl.GetTransactionNumberBelow200000(), 0);
    dl.SetTransactionNumberBelow200000(10);
    EXPECT_EQ(dl.GetTransactionNumberBelow200000(), 10);
  }

  TEST(CDayLineTest, TestGetTransactionNumberAbove200000) {
    CDayLine dl;
    EXPECT_EQ(dl.GetTransactionNumberAbove200000(), 0);
    dl.SetTransactionNumberAbove200000(10);
    EXPECT_EQ(dl.GetTransactionNumberAbove200000(), 10);
  }

  TEST(CDayLineTest, TestGetOrdinaryBuyVolume) {
    CDayLine dl;
    EXPECT_EQ(dl.GetOrdinaryBuyVolume(), 0);
    dl.SetOrdinaryBuyVolume(10);
    EXPECT_EQ(dl.GetOrdinaryBuyVolume(), 10);
  }

  TEST(CDayLineTest, TestGetAttackBuyVolume) {
    CDayLine dl;
    EXPECT_EQ(dl.GetAttackBuyVolume(), 0);
    dl.SetAttackBuyVolume(10);
    EXPECT_EQ(dl.GetAttackBuyVolume(), 10);
  }

  TEST(CDayLineTest, TestGetStrongBuyVolume) {
    CDayLine dl;
    EXPECT_EQ(dl.GetStrongBuyVolume(), 0);
    dl.SetStrongBuyVolume(10);
    EXPECT_EQ(dl.GetStrongBuyVolume(), 10);
  }

  TEST(CDayLineTest, TestGetStrongSellVolume) {
    CDayLine dl;
    EXPECT_EQ(dl.GetStrongSellVolume(), 0);
    dl.SetStrongSellVolume(10);
    EXPECT_EQ(dl.GetStrongSellVolume(), 10);
  }

  TEST(CDayLineTest, TestGetAttackSellVolume) {
    CDayLine dl;
    EXPECT_EQ(dl.GetAttackSellVolume(), 0);
    dl.SetAttackSellVolume(10);
    EXPECT_EQ(dl.GetAttackSellVolume(), 10);
  }

  TEST(CDayLineTest, TestGetUnknownVolume) {
    CDayLine dl;
    EXPECT_EQ(dl.GetUnknownVolume(), 0);
    dl.SetUnknownVolume(10);
    EXPECT_EQ(dl.GetUnknownVolume(), 10);
  }

  TEST(CDayLineTest, TestGetCancelBuyVolume) {
    CDayLine dl;
    EXPECT_EQ(dl.GetCancelBuyVolume(), 0);
    dl.SetCancelBuyVolume(10);
    EXPECT_EQ(dl.GetCancelBuyVolume(), 10);
  }

  TEST(CDayLineTest, TestGetCancelSellVolume) {
    CDayLine dl;
    EXPECT_EQ(dl.GetCancelSellVolume(), 0);
    dl.SetCancelSellVolume(10);
    EXPECT_EQ(dl.GetCancelSellVolume(), 10);
  }

  TEST(CDayLineTest, TestGetOrdinarySellVolume) {
    CDayLine dl;
    EXPECT_EQ(dl.GetOrdinarySellVolume(), 0);
    dl.SetOrdinarySellVolume(10);
    EXPECT_EQ(dl.GetOrdinarySellVolume(), 10);
  }

  TEST(CDayLineTest, TestGetAttackBuyBelow50000) {
    CDayLine dl;
    EXPECT_EQ(dl.GetAttackBuyBelow50000(), 0);
    dl.SetAttackBuyBelow50000(10);
    EXPECT_EQ(dl.GetAttackBuyBelow50000(), 10);
  }

  TEST(CDayLineTest, TestGetAttackBuyBelow200000) {
    CDayLine dl;
    EXPECT_EQ(dl.GetAttackBuyBelow200000(), 0);
    dl.SetAttackBuyBelow200000(10);
    EXPECT_EQ(dl.GetAttackBuyBelow200000(), 10);
  }

  TEST(CDayLineTest, TestGetAttackBuyAbove200000) {
    CDayLine dl;
    EXPECT_EQ(dl.GetAttackBuyAbove200000(), 0);
    dl.SetAttackBuyAbove200000(10);
    EXPECT_EQ(dl.GetAttackBuyAbove200000(), 10);
  }

  TEST(CDayLineTest, TestGetAttackSellAbove200000) {
    CDayLine dl;
    EXPECT_EQ(dl.GetAttackSellAbove200000(), 0);
    dl.SetAttackSellAbove200000(10);
    EXPECT_EQ(dl.GetAttackSellAbove200000(), 10);
  }

  TEST(CDayLineTest, TestGetAttackSellBelow200000) {
    CDayLine dl;
    EXPECT_EQ(dl.GetAttackSellBelow200000(), 0);
    dl.SetAttackSellBelow200000(10);
    EXPECT_EQ(dl.GetAttackSellBelow200000(), 10);
  }

  TEST(CDayLineTest, TestGetAttackSellBelow50000) {
    CDayLine dl;
    EXPECT_EQ(dl.GetAttackSellBelow50000(), 0);
    dl.SetAttackSellBelow50000(10);
    EXPECT_EQ(dl.GetAttackSellBelow50000(), 10);
  }

  TEST(CDayLineTest, TestGet3DayRS) {
    CDayLine dl;
    EXPECT_EQ(dl.Get3DayRS(), 0);
    dl.Set3DayRS(10);
    EXPECT_EQ(dl.Get3DayRS(), 10);
  }

  TEST(CDayLineTest, TestGet5DayRS) {
    CDayLine dl;
    EXPECT_EQ(dl.Get5DayRS(), 0);
    dl.Set5DayRS(10);
    EXPECT_EQ(dl.Get5DayRS(), 10);
  }

  TEST(CDayLineTest, TestGet10DayRS) {
    CDayLine dl;
    EXPECT_EQ(dl.Get10DayRS(), 0);
    dl.Set10DayRS(10);
    EXPECT_EQ(dl.Get10DayRS(), 10);
  }

  TEST(CDayLineTest, TestGet30DayRS) {
    CDayLine dl;
    EXPECT_EQ(dl.Get30DayRS(), 0);
    dl.Set30DayRS(10);
    EXPECT_EQ(dl.Get30DayRS(), 10);
  }

  TEST(CDayLineTest, TestGet60DayRS) {
    CDayLine dl;
    EXPECT_EQ(dl.Get60DayRS(), 0);
    dl.Set60DayRS(10);
    EXPECT_EQ(dl.Get60DayRS(), 10);
  }

  TEST(CDayLineTest, TestGet120DayRS) {
    CDayLine dl;
    EXPECT_EQ(dl.Get120DayRS(), 0);
    dl.Set120DayRS(10);
    EXPECT_EQ(dl.Get120DayRS(), 10);
  }

  TEST(CDayLineTest, TestSaveData) {
    CSetDayLine setDayLine;
    CDayLine id;

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

    ASSERT(!gl_fNormalMode);
    setDayLine.m_strFilter = _T("[Day] = 21101101");
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    id.AppendData(setDayLine);
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();

    setDayLine.m_strFilter = _T("[Day] = 21101101");
    setDayLine.Open();
    EXPECT_EQ(setDayLine.m_Day, id.GetDay());
    EXPECT_EQ(setDayLine.m_Market, id.GetMarket());
    EXPECT_STREQ(setDayLine.m_StockCode, id.GetStockCode());
    //EXPECT_STREQ(setDayLine.m_StockName, id.GetStockName());
    EXPECT_DOUBLE_EQ(atof(setDayLine.m_LastClose) * 1000, id.GetLastClose());
    EXPECT_DOUBLE_EQ(atof(setDayLine.m_Open) * 1000, id.GetOpen());
    EXPECT_DOUBLE_EQ(atof(setDayLine.m_High) * 1000, id.GetHigh());
    EXPECT_DOUBLE_EQ(atof(setDayLine.m_Low) * 1000, id.GetLow());
    EXPECT_DOUBLE_EQ(atof(setDayLine.m_Close) * 1000, id.GetClose());
    EXPECT_DOUBLE_EQ(atoll(setDayLine.m_Volume), id.GetVolume());
    EXPECT_DOUBLE_EQ(atoll(setDayLine.m_Amount), id.GetAmount());
    EXPECT_DOUBLE_EQ(atof(setDayLine.m_UpAndDown), id.GetUpDown());
    EXPECT_DOUBLE_EQ(atof(setDayLine.m_UpDownRate), id.GetUpDownRate());
    EXPECT_DOUBLE_EQ(atoll(setDayLine.m_TotalValue), id.GetTotalValue());
    EXPECT_DOUBLE_EQ(atoll(setDayLine.m_CurrentValue), id.GetCurrentValue());
    EXPECT_DOUBLE_EQ(atof(setDayLine.m_ChangeHandRate), id.GetChangeHandRate());
    EXPECT_DOUBLE_EQ(atof(setDayLine.m_RelativeStrong), id.GetRelativeStrong());
    setDayLine.Close();

    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    while (!setDayLine.IsEOF()) {
      setDayLine.Delete();
      setDayLine.MoveNext();
    }
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();
  }

  TEST(CDayLineTest, TestLoadData) {
    CSetDayLine setDayLine;
    CDayLine id, idLoaded;

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

    ASSERT(!gl_fNormalMode);
    setDayLine.m_strFilter = _T("[Day] = 21101101");
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    id.AppendData(setDayLine);
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();

    setDayLine.m_strFilter = _T("[Day] = 21101101");
    setDayLine.Open();
    idLoaded.LoadData(setDayLine);
    EXPECT_EQ(idLoaded.GetDay(), id.GetDay());
    EXPECT_EQ(idLoaded.GetMarket(), id.GetMarket());
    EXPECT_STREQ(idLoaded.GetStockCode(), id.GetStockCode());
    //EXPECT_STREQ(setDayLine.m_StockName, id.GetStockName());
    EXPECT_DOUBLE_EQ(idLoaded.GetLastClose(), id.GetLastClose());
    EXPECT_DOUBLE_EQ(idLoaded.GetOpen(), id.GetOpen());
    EXPECT_DOUBLE_EQ(idLoaded.GetHigh(), id.GetHigh());
    EXPECT_DOUBLE_EQ(idLoaded.GetLow(), id.GetLow());
    EXPECT_DOUBLE_EQ(idLoaded.GetClose(), id.GetClose());
    EXPECT_DOUBLE_EQ(idLoaded.GetVolume(), id.GetVolume());
    EXPECT_DOUBLE_EQ(idLoaded.GetAmount(), id.GetAmount());
    EXPECT_DOUBLE_EQ(idLoaded.GetUpDown(), id.GetUpDown());
    EXPECT_DOUBLE_EQ(idLoaded.GetUpDownRate(), id.GetUpDownRate());
    EXPECT_DOUBLE_EQ(idLoaded.GetTotalValue(), id.GetTotalValue());
    EXPECT_DOUBLE_EQ(idLoaded.GetCurrentValue(), id.GetCurrentValue());
    EXPECT_DOUBLE_EQ(idLoaded.GetChangeHandRate(), id.GetChangeHandRate());
    EXPECT_DOUBLE_EQ(idLoaded.GetRelativeStrong(), id.GetRelativeStrong());
    setDayLine.Close();

    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    while (!setDayLine.IsEOF()) {
      setDayLine.Delete();
      setDayLine.MoveNext();
    }
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();
  }

  TEST(CDayLineTest, TestIsActive) {
    CDayLine dayLine;
    EXPECT_FALSE(dayLine.IsActive());
    dayLine.SetClose(100);
    EXPECT_FALSE(dayLine.IsActive());
    dayLine.SetOpen(100);
    EXPECT_FALSE(dayLine.IsActive());
    dayLine.SetLastClose(100);
    EXPECT_TRUE(dayLine.IsActive());
  }
}