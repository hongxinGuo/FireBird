#include"stdafx.h"

#include"pch.h"
#include"globedef.h"

#include"Stock.h"

using namespace testing;

namespace StockAnalysisTest {
  TEST(StockTest, TestInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    CStock stock;
    EXPECT_EQ(stock.m_vDayLine.size(), 0);
    EXPECT_FALSE(stock.IsActive());
    EXPECT_STREQ(stock.GetStockCode(), _T(""));
    EXPECT_STREQ(stock.GetStockName(), _T(""));
    EXPECT_EQ(stock.GetOffset(), -1);
    EXPECT_EQ(stock.GetTransactionTime(), 0);
    EXPECT_EQ(stock.GetLastClose(), 0);
    EXPECT_EQ(stock.GetOpen(), 0);
    EXPECT_EQ(stock.GetHigh(), 0);
    EXPECT_EQ(stock.GetLow(), 0);
    EXPECT_EQ(stock.GetNew(), 0);
    EXPECT_EQ(stock.GetVolume(), 0);
    EXPECT_EQ(stock.GetAmount(), 0);
    EXPECT_EQ(stock.GetPBuy(0), 0);
    EXPECT_EQ(stock.GetVBuy(0), 0);
    EXPECT_EQ(stock.GetPBuy(1), 0);
    EXPECT_EQ(stock.GetVBuy(1), 0);
    EXPECT_EQ(stock.GetPBuy(2), 0);
    EXPECT_EQ(stock.GetVBuy(2), 0);
    EXPECT_EQ(stock.GetPBuy(3), 0);
    EXPECT_EQ(stock.GetVBuy(3), 0);
    EXPECT_EQ(stock.GetPBuy(4), 0);
    EXPECT_EQ(stock.GetVBuy(4), 0);
    EXPECT_EQ(stock.GetPSell(0), 0);
    EXPECT_EQ(stock.GetVSell(0), 0);
    EXPECT_EQ(stock.GetPSell(1), 0);
    EXPECT_EQ(stock.GetVSell(1), 0);
    EXPECT_EQ(stock.GetPSell(2), 0);
    EXPECT_EQ(stock.GetVSell(2), 0);
    EXPECT_EQ(stock.GetPSell(3), 0);
    EXPECT_EQ(stock.GetVSell(3), 0);
    EXPECT_EQ(stock.GetPSell(4), 0);
    EXPECT_EQ(stock.GetVSell(4), 0);
    EXPECT_EQ(stock.GetAttackBuyAmount(), 0);
    EXPECT_EQ(stock.GetAttackSellAmount(), 0);
    EXPECT_DOUBLE_EQ(stock.GetRelativeStrong(), 0);
    EXPECT_FALSE(stock.IsDayLineLoaded());
    EXPECT_FALSE(stock.IsActive());
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    EXPECT_FALSE(stock.IsInquiringOnce());
    EXPECT_FALSE(stock.IsChoiced());
    EXPECT_FALSE(stock.IsMinLineUpdated());
    EXPECT_FALSE(stock.IsDayLineUpdated());
    EXPECT_FALSE(stock.HaveFirstRTData());
  }
  TEST(StockTest, TestGetMarket) {
    CStock stock;
    EXPECT_EQ(stock.GetMarket(), 0);
    stock.SetMarket(1);
    EXPECT_EQ(stock.GetMarket(), 1);
  }

  TEST(StockTest, TestGetStockCode) {
    CStock stock;
    EXPECT_STREQ(stock.GetStockCode(), _T(""));
    stock.SetStockCode(_T("sh600000"));
    EXPECT_STREQ(stock.GetStockCode(), _T("sh600000"));
  }

  TEST(StockTest, TestGetStockName) {
    CStock stock;
    EXPECT_STREQ(stock.GetStockName(), _T(""));
    stock.SetStockName(_T("浦东银行"));
    EXPECT_STREQ(stock.GetStockName(), _T("浦东银行"));
  }

  TEST(StockTest, TestGetOffset) {
    CStock stock;
    EXPECT_EQ(stock.GetOffset(), -1);
    stock.SetOffset(_T(101010));
    EXPECT_EQ(stock.GetOffset(), 101010);
  }

  TEST(StockTest, TestGetDayLineStartDay) {
    CStock stock;
    EXPECT_EQ(stock.GetDayLineStartDay(), 19900101);
    stock.SetDayLineStartDay(100100100);
    EXPECT_EQ(stock.GetDayLineStartDay(), 100100100);
  }

  TEST(StockTest, TestGetDayLineEndDay) {
    CStock stock;
    EXPECT_EQ(stock.GetDayLineEndDay(), 19900101);
    stock.SetDayLineEndDay(100100100);
    EXPECT_EQ(stock.GetDayLineEndDay(), 100100100);
  }

  TEST(StockTest, TestGetIPOStatus) {
    CStock stock;
    EXPECT_EQ(stock.GetIPOStatus(), __STOCK_NOT_CHECKED__);
    stock.SetIPOStatus(100100100);
    EXPECT_EQ(stock.GetIPOStatus(), 100100100);
  }

  TEST(StockTest, TestGetTransactionTime) {
    CStock stock;
    EXPECT_EQ(stock.GetTransactionTime(), 0);
    stock.SetTransactionTime(100100100100);
    EXPECT_EQ(stock.GetTransactionTime(), 100100100100);
  }

  TEST(StockTest, TestGetLastClose) {
    CStock stock;
    EXPECT_EQ(stock.GetLastClose(), 0);
    stock.SetLastClose(100100100);
    EXPECT_EQ(stock.GetLastClose(), 100100100);
  }

  TEST(StockTest, TestGetOpen) {
    CStock stock;
    EXPECT_EQ(stock.GetOpen(), 0);
    stock.SetOpen(100100100);
    EXPECT_EQ(stock.GetOpen(), 100100100);
  }

  TEST(StockTest, TestGetHigh) {
    CStock stock;
    EXPECT_EQ(stock.GetHigh(), 0);
    stock.SetHigh(100100100);
    EXPECT_EQ(stock.GetHigh(), 100100100);
  }

  TEST(StockTest, TestGetLow) {
    CStock stock;
    EXPECT_EQ(stock.GetLow(), 0);
    stock.SetLow(100100100);
    EXPECT_EQ(stock.GetLow(), 100100100);
  }

  TEST(StockTest, TestGetNew) {
    CStock stock;
    EXPECT_EQ(stock.GetNew(), 0);
    stock.SetNew(100100100);
    EXPECT_EQ(stock.GetNew(), 100100100);
  }

  TEST(StockTest, TestGetAmount) {
    CStock stock;
    EXPECT_EQ(stock.GetAmount(), 0);
    stock.SetAmount(100100100);
    EXPECT_EQ(stock.GetAmount(), 100100100);
  }

  TEST(StockTest, TestGetVolume) {
    CStock stock;
    EXPECT_EQ(stock.GetVolume(), 0);
    stock.SetVolume(100100100);
    EXPECT_EQ(stock.GetVolume(), 100100100);
  }

  TEST(StockTest, TestUpdateCurrentStatus) {
    CStock stock;
    CRTDataPtr pRTData;

    pRTData = make_shared<CRTData>();
    pRTData->SetTransactionTime(100100100100);
    pRTData->SetLastClose(11111);
    pRTData->SetOpen(22222);
    pRTData->SetNew(33333);
    pRTData->SetHigh(66666);
    pRTData->SetLow(10000);
    pRTData->SetVolume(1001001001001);
    pRTData->SetAmount(2002002002002);
    for (int i = 0; i < 5; i++) {
      pRTData->SetPBuy(i, i + 100);
      pRTData->SetVBuy(i, i + 200);
      pRTData->SetPSell(i, i + 300);
      pRTData->SetVSell(i, i + 400);
    }
    stock.UpdateStatus(pRTData);
    EXPECT_EQ(stock.GetTransactionTime(), 0);   // UpdateCurrentStatus函数不更新交易时间
    EXPECT_EQ(stock.GetLastClose(), 11111);
    EXPECT_EQ(stock.GetOpen(), 22222);
    EXPECT_EQ(stock.GetNew(), 33333);
    EXPECT_EQ(stock.GetHigh(), 66666);
    EXPECT_EQ(stock.GetLow(), 10000);
    EXPECT_EQ(stock.GetVolume(), 1001001001001);
    EXPECT_EQ(stock.GetAmount(), 2002002002002);
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(stock.GetPBuy(i), i + 100);
      EXPECT_EQ(stock.GetVBuy(i), i + 200);
      EXPECT_EQ(stock.GetPSell(i), i + 300);
      EXPECT_EQ(stock.GetVSell(i), i + 400);
    }
  }

  TEST(StockBasicInfoTest, TestGetAttackBuyAmount) {
    CStock id;
    EXPECT_EQ(id.GetAttackBuyAmount(), 0);
    id.SetAttackBuyAmount(10101010);
    EXPECT_EQ(id.GetAttackBuyAmount(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetAttackSellAmount) {
    CStock id;
    EXPECT_EQ(id.GetAttackSellAmount(), 0);
    id.SetAttackSellAmount(10101010);
    EXPECT_EQ(id.GetAttackSellAmount(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetOrdinaryBuyVolume) {
    CStock id;
    EXPECT_EQ(id.GetOrdinaryBuyVolume(), 0);
    id.SetOrdinaryBuyVolume(10101010);
    EXPECT_EQ(id.GetOrdinaryBuyVolume(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetOrdinarySellVolume) {
    CStock id;
    EXPECT_EQ(id.GetOrdinarySellVolume(), 0);
    id.SetOrdinarySellVolume(10101010);
    EXPECT_EQ(id.GetOrdinarySellVolume(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetAttackBuyVolume) {
    CStock id;
    EXPECT_EQ(id.GetAttackBuyVolume(), 0);
    id.SetAttackBuyVolume(10101010);
    EXPECT_EQ(id.GetAttackBuyVolume(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetStrongBuyVolume) {
    CStock id;
    EXPECT_EQ(id.GetStrongBuyVolume(), 0);
    id.SetStrongBuyVolume(10101010);
    EXPECT_EQ(id.GetStrongBuyVolume(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetCurrentAttackBuy) {
    CStock id;
    EXPECT_EQ(id.GetCurrentAttackBuy(), 0);
    id.SetCurrentAttackBuy(10101010);
    EXPECT_EQ(id.GetCurrentAttackBuy(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetCurrentStrongBuy) {
    CStock id;
    EXPECT_EQ(id.GetCurrentStrongBuy(), 0);
    id.SetCurrentStrongBuy(10101010);
    EXPECT_EQ(id.GetCurrentStrongBuy(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetAttackSellVolume) {
    CStock id;
    EXPECT_EQ(id.GetAttackSellVolume(), 0);
    id.SetAttackSellVolume(10101010);
    EXPECT_EQ(id.GetAttackSellVolume(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetStrongSellVolume) {
    CStock id;
    EXPECT_EQ(id.GetStrongSellVolume(), 0);
    id.SetStrongSellVolume(10101010);
    EXPECT_EQ(id.GetStrongSellVolume(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetCurrentAttackSell) {
    CStock id;
    EXPECT_EQ(id.GetCurrentAttackSell(), 0);
    id.SetCurrentAttackSell(10101010);
    EXPECT_EQ(id.GetCurrentAttackSell(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetCurrentStrongSell) {
    CStock id;
    EXPECT_EQ(id.GetCurrentStrongSell(), 0);
    id.SetCurrentStrongSell(10101010);
    EXPECT_EQ(id.GetCurrentStrongSell(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetUnknownVolume) {
    CStock id;
    EXPECT_EQ(id.GetUnknownVolume(), 0);
    id.SetUnknownVolume(10101010);
    EXPECT_EQ(id.GetUnknownVolume(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetCurrentUnknown) {
    CStock id;
    EXPECT_EQ(id.GetCurrentUnknown(), 0);
    id.SetCurrentUnknown(10101010);
    EXPECT_EQ(id.GetCurrentUnknown(), 10101010);
  }

  TEST(StockBasicInfoTest, TestGetCancelBuyVolume) {
    CStock id;
    EXPECT_EQ(id.GetCancelBuyVolume(), 0);
    id.SetCancelBuyVolume(10101010);
    EXPECT_EQ(id.GetCancelBuyVolume(), 10101010);
  }
  TEST(StockBasicInfoTest, TestGetCancelSellVolume) {
    CStock id;
    EXPECT_EQ(id.GetCancelSellVolume(), 0);
    id.SetCancelSellVolume(10101010);
    EXPECT_EQ(id.GetCancelSellVolume(), 10101010);
  }

  TEST(StockTest, TestReadOnlyFunctions) {
    CStock stock;

    EXPECT_DOUBLE_EQ(stock.GetRelativeStrong(), 0);
  }

  TEST(StockTest, TestIsActive) {
    CStock stock;
    EXPECT_FALSE(stock.IsActive());
    stock.SetActive(true);
    EXPECT_TRUE(stock.IsActive());
    stock.SetActive(false);
    EXPECT_FALSE(stock.IsActive());
  }

  TEST(StockTest, TestIsDayLineNeedUpdate) {
    CStock stock;
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    stock.SetDayLineNeedUpdate(false);
    EXPECT_FALSE(stock.IsDayLineNeedUpdate());
    stock.SetDayLineNeedUpdate(true);
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
  }

  TEST(StockTest, TestIsInquiringOnce) {
    CStock stock;
    EXPECT_FALSE(stock.IsInquiringOnce());
    stock.SetInquiringOnce(true);
    EXPECT_TRUE(stock.IsInquiringOnce());
    stock.SetInquiringOnce(false);
    EXPECT_FALSE(stock.IsInquiringOnce());
  }

  TEST(StockTest, TestIsChoiced) {
    CStock stock;
    EXPECT_FALSE(stock.IsChoiced());
    stock.SetChoiced(true);
    EXPECT_TRUE(stock.IsChoiced());
    stock.SetChoiced(false);
    EXPECT_FALSE(stock.IsChoiced());
  }

  TEST(StockTest, TestIsMinLineUpdated) {
    CStock stock;
    EXPECT_FALSE(stock.IsMinLineUpdated());
    stock.SetMinLineUpdated(true);
    EXPECT_TRUE(stock.IsMinLineUpdated());
    stock.SetMinLineUpdated(false);
    EXPECT_FALSE(stock.IsMinLineUpdated());
  }

  TEST(StockTest, TestIsDayLineUpdated) {
    CStock stock;
    EXPECT_FALSE(stock.IsDayLineUpdated());
    stock.SetDayLineUpdated(true);
    EXPECT_TRUE(stock.IsDayLineUpdated());
    stock.SetDayLineUpdated(false);
    EXPECT_FALSE(stock.IsDayLineUpdated());
  }

  TEST(StockTest, TestIsDayLineLoaded) {
    CStock stock;
    EXPECT_FALSE(stock.IsDayLineLoaded());
    stock.SetDayLineLoaded(true);
    EXPECT_TRUE(stock.IsDayLineLoaded());
    stock.SetDayLineLoaded(false);
    EXPECT_FALSE(stock.IsDayLineLoaded());
  }

  TEST(StockTest, TestHaveFirstRTData) {
    CStock stock;
    EXPECT_FALSE(stock.HaveFirstRTData());
    stock.SetHavingFirstRTData(true);
    EXPECT_TRUE(stock.HaveFirstRTData());
    stock.SetHavingFirstRTData(false);
    EXPECT_TRUE(stock.HaveFirstRTData());
  }

  TEST(StockTest, TestIsStartCalculating) {
    CStock stock;
    EXPECT_FALSE(stock.HaveFirstRTData());
    EXPECT_FALSE(stock.SetHavingFirstRTData(false)); // 不允许外部设置停止计算标识（内部可以）
    EXPECT_FALSE(stock.HaveFirstRTData());
    EXPECT_TRUE(stock.SetHavingFirstRTData(true));
    EXPECT_TRUE(stock.HaveFirstRTData());
    EXPECT_FALSE(stock.SetHavingFirstRTData(true));  // 不允许再次设置开始计算标识
    EXPECT_TRUE(stock.HaveFirstRTData());
  }

  TEST(StockTest, TestIsDayNeededSaving) {    // 此两个函数是具备同步机制的，这里没有进行测试
    CStock stock;
    stock.SetDayLineNeedSavingFlag(true);
    EXPECT_TRUE(stock.IsDayLineNeedSaving());
    stock.SetDayLineNeedSavingFlag(false);
    EXPECT_FALSE(stock.IsDayLineNeedSaving());
  }

  TEST(StockTest, TestTodayDataIsActive) {
    CStock stock;
    EXPECT_FALSE(stock.TodayDataIsActive());
    stock.SetActive(true);
    EXPECT_FALSE(stock.TodayDataIsActive());
    stock.SetHigh(1010);
    EXPECT_TRUE(stock.TodayDataIsActive());
    stock.SetHigh(0);
    stock.SetLow(1);
    EXPECT_TRUE(stock.TodayDataIsActive());
    stock.SetLow(0);
    stock.SetAmount(10000);
    EXPECT_TRUE(stock.TodayDataIsActive());
    stock.SetAmount(0);
    stock.SetVolume(100);
    EXPECT_TRUE(stock.TodayDataIsActive());
    stock.SetActive(false);
    EXPECT_FALSE(stock.TodayDataIsActive());
  }

  TEST(StockTest, TestRTDataDeque) {    // 此三个函数是具备同步机制的，这里没有进行测试
    CRTDataPtr pData = make_shared<CRTData>();
    CStock stock;
    EXPECT_EQ(stock.GetRTDataDequeSize(), 0);
    stock.PushRTData(pData);
    EXPECT_EQ(stock.GetRTDataDequeSize(), 1);
    CRTDataPtr pData2 = stock.PopRTData();
    EXPECT_EQ(stock.GetRTDataDequeSize(), 0);
  }

  TEST(StockTest, TestGetGuaDan) {
    CStock stock;
    EXPECT_FALSE(stock.HaveGuaDan(10000));
    stock.SetGuaDan(10000, 10000);
    EXPECT_TRUE(stock.HaveGuaDan(10000));
    EXPECT_EQ(stock.GetGuaDan(10000), 10000);
  }

  TEST(StockTest, TestClearRTDataDeque) {
    CStock stock;
    CRTDataPtr pRTData;
    for (int i = 0; i < 10; i++) {
      pRTData = make_shared<CRTData>();
      pRTData->SetLastClose(i);
      stock.PushRTData(pRTData);
    }
    EXPECT_EQ(stock.GetRTDataDequeSize(), 10);
    stock.ClearRTDataDeque();
    EXPECT_EQ(stock.GetRTDataDequeSize(), 0);
  }
}