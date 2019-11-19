#include "pch.h"

#include"globedef.h"
#include"StockBasicInfo.h"

namespace StockAnalysisTest {
  TEST(StockBasicInfoTest, TestInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    CStockBasicInfo id;
    EXPECT_EQ(id.GetMarket(), 0);
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    EXPECT_STREQ(id.GetStockName(), _T(""));
    EXPECT_EQ(id.GetOffset(), -1);
    EXPECT_EQ(id.GetDayLineEndDay(), 19900101);
    EXPECT_EQ(id.GetIPOStatus(), __STOCK_NOT_CHECKED__);
    id.SetMarket(1);
    id.SetStockCode(_T("abcde"));
    id.SetStockName(_T("dcba"));
    id.SetOffset(1);
    id.SetDayLineEndDay(20020202);
    id.SetIPOStatus(0);
    EXPECT_EQ(id.GetMarket(), 1);
    EXPECT_STREQ(id.GetStockCode(), _T("abcde"));
    EXPECT_STREQ(id.GetStockName(), _T("dcba"));
    EXPECT_EQ(id.GetOffset(), 1);
    EXPECT_EQ(id.GetDayLineEndDay(), 20020202);
    EXPECT_EQ(id.GetIPOStatus(), 0);
    id.Reset();
    EXPECT_EQ(id.GetMarket(), 0);
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    EXPECT_STREQ(id.GetStockName(), _T(""));
    EXPECT_EQ(id.GetOffset(), -1);
    EXPECT_EQ(id.GetDayLineEndDay(), 19900101);
    EXPECT_EQ(id.GetIPOStatus(), __STOCK_NOT_CHECKED__);
  }

  TEST(StockBasicInfoTest, TestGetMarket) {
    CStockBasicInfo id;
    EXPECT_EQ(id.GetMarket(), 0);
    id.SetMarket(__SHANGHAI_MARKET__);
    EXPECT_EQ(id.GetMarket(), __SHANGHAI_MARKET__);
    id.SetMarket(__SHENZHEN_MARKET__);
    EXPECT_EQ(id.GetMarket(), __SHENZHEN_MARKET__);
  }

  TEST(StockBasicInfoTest, TestGetStockCode) {
    CStockBasicInfo id;
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    id.SetStockCode(_T("sh600000"));
    EXPECT_STREQ(id.GetStockCode(), _T("sh600000"));
  }

  TEST(StockBasicInfoTest, TestGetStockName) {
    CStockBasicInfo id;
    EXPECT_STREQ(id.GetStockName(), _T(""));
    id.SetStockName(_T("sh600000"));
    EXPECT_STREQ(id.GetStockName(), _T("sh600000"));
  }
  TEST(StockBasicInfoTest, TestGetOffset) {
    CStockBasicInfo id;
    EXPECT_EQ(id.GetOffset(), -1);
    id.SetOffset(10101);
    EXPECT_EQ(id.GetOffset(), 10101);
  }

  TEST(StockBasicInfoTest, TestGetDayLineEndDay) {
    CStockBasicInfo id;
    EXPECT_EQ(id.GetDayLineEndDay(), 19900101);
    id.SetDayLineEndDay(19980101);
    EXPECT_EQ(id.GetDayLineEndDay(), 19980101);
  }

  TEST(StockBasicInfoTest, TestGetDayLineStartDay) {
    CStockBasicInfo id;
    EXPECT_EQ(id.GetDayLineStartDay(), 19900101);
    id.SetDayLineStartDay(19980101);
    EXPECT_EQ(id.GetDayLineStartDay(), 19980101);
  }

  TEST(StockBasicInfoTest, TestGetIPOStatus) {
    CStockBasicInfo id;
    EXPECT_EQ(id.GetIPOStatus(), 128);
    id.SetIPOStatus(255);
    EXPECT_EQ(id.GetIPOStatus(), 255);
  }

  TEST(StockBasicInfoTest, TestGetTransactionTime) {
    CStockBasicInfo id;
    EXPECT_EQ(id.GetTransactionTime(), 0);
    id.SetTransactionTime(1010101010);
    EXPECT_EQ(id.GetTransactionTime(), 1010101010);
  }

  TEST(StockBasicInfoTest, TestGetLastClose) {
    CStockBasicInfo id;
    EXPECT_EQ(id.GetLastClose(), 0);
    id.SetLastClose(10101010);
    EXPECT_EQ(id.GetLastClose(), 10101010);
  }
  TEST(StockBasicInfoTest, TestGetOpen) {
    CStockBasicInfo id;
    EXPECT_EQ(id.GetOpen(), 0);
    id.SetOpen(10101);
    EXPECT_EQ(id.GetOpen(), 10101);
  }

  TEST(StockBasicInfoTest, TestGetHigh) {
    CStockBasicInfo id;
    EXPECT_EQ(id.GetHigh(), 0);
    id.SetHigh(19980101);
    EXPECT_EQ(id.GetHigh(), 19980101);
  }

  TEST(StockBasicInfoTest, TestGetLow) {
    CStockBasicInfo id;
    EXPECT_EQ(id.GetLow(), 0);
    id.SetLow(19980101);
    EXPECT_EQ(id.GetLow(), 19980101);
  }

  TEST(StockBasicInfoTest, TestGetNew) {
    CStockBasicInfo id;
    EXPECT_EQ(id.GetNew(), 0);
    id.SetNew(10101010);
    EXPECT_EQ(id.GetNew(), 10101010);
  }
  TEST(StockBasicInfoTest, TestGetAmount) {
    CStockBasicInfo id;
    EXPECT_EQ(id.GetAmount(), 0);
    id.SetAmount(1010101010101010);
    EXPECT_EQ(id.GetAmount(), 1010101010101010);
  }
  TEST(StockBasicInfoTest, TestGetVolume) {
    CStockBasicInfo id;
    EXPECT_EQ(id.GetVolume(), 0);
    id.SetVolume(10101010);
    EXPECT_EQ(id.GetVolume(), 10101010);
  }
  TEST(StockBasicInfoTest, TestGetTotalValue) {
    CStockBasicInfo id;
    EXPECT_EQ(id.GetTotalValue(), 0);
    id.SetTotalValue(10101010);
    EXPECT_EQ(id.GetTotalValue(), 10101010);
  }
  TEST(StockBasicInfoTest, TestGetCurrentValue) {
    CStockBasicInfo id;
    EXPECT_EQ(id.GetCurrentValue(), 0);
    id.SetCurrentValue(10101010);
    EXPECT_EQ(id.GetCurrentValue(), 10101010);
  }
  TEST(StockBasicInfoTest, TestGetPBuy) {
    CStockBasicInfo id;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(id.GetPBuy(i), 0);
      id.SetPBuy(i, 10101010);
      EXPECT_EQ(id.GetPBuy(i), 10101010);
    }
  }

  TEST(StockBasicInfoTest, TestGetVBuy) {
    CStockBasicInfo id;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(id.GetVBuy(i), 0);
      id.SetVBuy(i, 10101010);
      EXPECT_EQ(id.GetVBuy(i), 10101010);
    }
  }

  TEST(StockBasicInfoTest, TestGetPSell) {
    CStockBasicInfo id;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(id.GetPSell(i), 0);
      id.SetPSell(i, 10101010);
      EXPECT_EQ(id.GetPSell(i), 10101010);
    }
  }

  TEST(StockBasicInfoTest, TestGetVSell) {
    CStockBasicInfo id;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(id.GetVSell(i), 0);
      id.SetVSell(i, 10101010);
      EXPECT_EQ(id.GetVSell(i), 10101010);
    }
  }

  TEST(StockBasicInfoTest, TestGetRelativeStrong) {
    CStockBasicInfo id;
    EXPECT_DOUBLE_EQ(id.GetRelativeStrong(), 0.0);
    id.SetRelativeStrong(10101010.0);
    EXPECT_DOUBLE_EQ(id.GetRelativeStrong(), 10101010.0);
  }

  TEST(StockBasicInfoTest, TestUpdateStatus) {
    ASSERT_FALSE(gl_fNormalMode);
    CStockBasicInfo id;
    EXPECT_EQ(id.GetMarket(), 0);
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    EXPECT_STREQ(id.GetStockName(), _T(""));
    EXPECT_EQ(id.GetOffset(), -1);
    EXPECT_EQ(id.GetDayLineEndDay(), 19900101);
    EXPECT_EQ(id.GetIPOStatus(), __STOCK_NOT_CHECKED__);
    id.SetMarket(1);
    id.SetStockCode(_T("abcde"));
    id.SetStockName(_T("dcba"));
    id.SetOffset(1);
    id.SetDayLineEndDay(20020202);
    id.SetIPOStatus(0);
    EXPECT_EQ(id.GetMarket(), 1);
    EXPECT_STREQ(id.GetStockCode(), _T("abcde"));
    EXPECT_STREQ(id.GetStockName(), _T("dcba"));
    EXPECT_EQ(id.GetOffset(), 1);
    EXPECT_EQ(id.GetDayLineEndDay(), 20020202);
    EXPECT_EQ(id.GetIPOStatus(), 0);
    id.Reset();
    EXPECT_EQ(id.GetMarket(), 0);
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    EXPECT_STREQ(id.GetStockName(), _T(""));
    EXPECT_EQ(id.GetOffset(), -1);
    EXPECT_EQ(id.GetDayLineEndDay(), 19900101);
    EXPECT_EQ(id.GetIPOStatus(), __STOCK_NOT_CHECKED__);

    CRTDataPtr  pRTData = make_shared<CRTData>();
    time_t tt;
    time(&tt);
    long lDaySource = gl_systemTime.GetDay(tt);

    pRTData->SetTransactionTime(tt);
    pRTData->SetLastClose(10101010);
    pRTData->SetOpen(20202020);
    pRTData->SetHigh(30303030);
    pRTData->SetLow(40404040);
    pRTData->SetNew(50505050);
    pRTData->SetVolume(6060606060606060);
    pRTData->SetAmount(7070707070707070);
    for (int i = 0; i < 5; i++) {
      pRTData->SetPBuy(i, i * 12345);
      pRTData->SetVBuy(i, i * 23456);
      pRTData->SetPSell(i, i * 34567);
      pRTData->SetVSell(i, i * 45678);
    }
    id.UpdateStatus(pRTData);
    long lDayDest = gl_systemTime.GetDay(id.GetTransactionTime());
    EXPECT_EQ(lDaySource, lDayDest);
    EXPECT_EQ(id.GetMarket(), 0);
    EXPECT_STREQ(id.GetStockCode(), _T(""));
    EXPECT_STREQ(id.GetStockName(), _T(""));
    EXPECT_EQ(id.GetOffset(), -1);
    EXPECT_EQ(id.GetDayLineEndDay(), 19900101);
    EXPECT_EQ(id.GetIPOStatus(), __STOCK_NOT_CHECKED__);

    EXPECT_EQ(id.GetLastClose(), 10101010);
    EXPECT_EQ(id.GetOpen(), 20202020);
    EXPECT_EQ(id.GetHigh(), 30303030);
    EXPECT_EQ(id.GetLow(), 40404040);
    EXPECT_EQ(id.GetNew(), 50505050);
    EXPECT_EQ(id.GetVolume(), 6060606060606060);
    EXPECT_EQ(id.GetAmount(), 7070707070707070);
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(id.GetPBuy(i), i * 12345);
      EXPECT_EQ(id.GetVBuy(i), i * 23456);
      EXPECT_EQ(id.GetPSell(i), i * 34567);
      EXPECT_EQ(id.GetVSell(i), i * 45678);
    }
  }
}