#include"stdafx.h"
#include"pch.h"

#include"DayLine.h"

namespace StockAnalysisTest {
  TEST(CDayLineTest, TestGetDay) {
    CDayLine dl;
    dl.SetDay(19900101);
    EXPECT_EQ(dl.GetDay(), 19900101);
  }

  TEST(CDayLineTest, TestGetTime) {
    CDayLine dl;
    dl.SetTime(100100100);
    EXPECT_EQ(dl.GetTime(), 100100100);
  }

  TEST(CDayLineTest, TestGetMarket) {
    CDayLine dl;
    dl.SetMarket(1);
    EXPECT_EQ(dl.GetMarket(), 1);
  }

  TEST(CDayLineTest, TestGetStockCode) {
    CDayLine dl;
    dl.SetStockCode(_T("sh600000"));
    EXPECT_STREQ(dl.GetStockCode(), _T("sh600000"));
  }

  TEST(CDayLineTest, TestGetStockName) {
    CDayLine dl;
    dl.SetStockName(_T("浦东银行"));
    EXPECT_STREQ(dl.GetStockName(), _T("浦东银行"));
  }

  TEST(CDayLineTest, TestGetLastClose) {
    CDayLine dl;
    dl.SetLastClose(10100);
    EXPECT_EQ(dl.GetLastClose(), 10100);
  }

  TEST(CDayLineTest, TestGetOpen) {
    CDayLine dl;
    dl.SetOpen(11100);
    EXPECT_EQ(dl.GetOpen(), 11100);
  }

  TEST(CDayLineTest, TestGetHigh) {
    CDayLine dl;
    dl.SetHigh(11111);
    EXPECT_EQ(dl.GetHigh(), 11111);
  }

  TEST(CDayLineTest, TestGetLow) {
    CDayLine dl;
    dl.SetLow(22222);
    EXPECT_EQ(dl.GetLow(), 22222);
  }

  TEST(CDayLineTest, TestGetClose) {
    CDayLine dl;
    dl.SetClose(33333);
    EXPECT_EQ(dl.GetClose(), 33333);
  }

  TEST(CDayLineTest, TestGetUpDown) {
    CDayLine dl;
    dl.SetUpDown(10.020);
    EXPECT_DOUBLE_EQ(dl.GetUpDown(), 10.020);
  }

  TEST(CDayLineTest, TestGetUpDownRate) {
    CDayLine dl;
    dl.SetUpDownRate(40.40);
    EXPECT_DOUBLE_EQ(dl.GetUpDownRate(), 40.40);
  }

  TEST(CDayLineTest, TestGetChangeHandRate) {
    CDayLine dl;
    dl.SetChangeHandRate(30.30);
    EXPECT_DOUBLE_EQ(dl.GetChangeHandRate(), 30.30);
  }

  TEST(CDayLineTest, TestGetVolume) {
    CDayLine dl;
    dl.SetVolume(100100100100100100);
    EXPECT_EQ(dl.GetVolume(), 100100100100100100);
  }

  TEST(CDayLineTest, TestGetAmount) {
    CDayLine dl;
    dl.SetAmount(200200200200200200);
    EXPECT_EQ(dl.GetAmount(), 200200200200200200);
  }

  TEST(CDayLineTest, TestGetTotalValue) {
    CDayLine dl;
    dl.SetTotalValue(300300300300300300);
    EXPECT_EQ(dl.GetTotalValue(), 300300300300300300);
  }

  TEST(CDayLineTest, TestGetCurrentValue) {
    CDayLine dl;
    dl.SetCurrentValue(400400400400400400);
    EXPECT_EQ(dl.GetCurrentValue(), 400400400400400400);
  }

  TEST(CDayLineTest, TestGetRelativeStrong) {
    CDayLine dl;
    dl.SetRelativeStrong(50.50);
    EXPECT_DOUBLE_EQ(dl.GetRelativeStrong(), 50.50);
  }

  TEST(CDayLineTest, GetTransactionNumber) {
    CDayLine dl;
    dl.SetTransactionNumber(600600);
    EXPECT_EQ(dl.GetTransactionNumber(), 600600);
  }
/*
  TEST(CDayLineTest, ) {
    CDayLine dl;
    dl
  }
  TEST(CDayLineTest, ) {
    CDayLine dl;
    dl
  }
  TEST(CDayLineTest, ) {
    CDayLine dl;
    dl
  }
  TEST(CDayLineTest, ) {
    CDayLine dl;
    dl
  }
  TEST(CDayLineTest, ) {
    CDayLine dl;
    dl
  }
  TEST(CDayLineTest, ) {
    CDayLine dl;
    dl
  }
  TEST(CDayLineTest, ) {
    CDayLine dl;
    dl
  }
  TEST(CDayLineTest, ) {
    CDayLine dl;
    dl
  }

*/
}