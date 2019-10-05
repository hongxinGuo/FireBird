#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"DayLine.h"

using namespace testing;

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
  }

  TEST(CDayLineTest, TestGetClose) {
    CDayLine dl;
    EXPECT_EQ(dl.GetClose(), 0);
    dl.SetClose(33333);
    EXPECT_EQ(dl.GetClose(), 33333);
  }

  TEST(CDayLineTest, TestGetUpDown) {
    CDayLine dl;
    EXPECT_DOUBLE_EQ(dl.GetUpDown(), 0);
    dl.SetUpDown(10.020);
    EXPECT_DOUBLE_EQ(dl.GetUpDown(), 10.020);
  }

  TEST(CDayLineTest, TestGetUpDownRate) {
    CDayLine dl;
    EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 0);
    dl.SetUpDownRate(40.40);
    EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 40.40);
  }

  TEST(CDayLineTest, TestGetChangeHandRate) {
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
    dl.SetTotalValue(300300300300300300);
    EXPECT_EQ(dl.GetTotalValue(), 300300300300300300);
  }

  TEST(CDayLineTest, TestGetCurrentValue) {
    CDayLine dl;
    EXPECT_EQ(dl.GetCurrentValue(), 0);
    dl.SetCurrentValue(400400400400400400);
    EXPECT_EQ(dl.GetCurrentValue(), 400400400400400400);
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
}