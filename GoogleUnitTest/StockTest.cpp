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
    EXPECT_EQ(stock.GetTransactionNumber(), 0);
    EXPECT_EQ(stock.GetTransactionNumberBelow5000(), 0);
    EXPECT_EQ(stock.GetTransactionNumberBelow50000(), 0);
    EXPECT_EQ(stock.GetTransactionNumberBelow200000(), 0);
    EXPECT_EQ(stock.GetTransactionNumberAbove200000(), 0);
    EXPECT_EQ(stock.GetOrdinaryBuyVolume(), 0);
    EXPECT_EQ(stock.GetOrdinarySellVolume(), 0);
    EXPECT_EQ(stock.GetAttackBuyVolume(), 0);
    EXPECT_EQ(stock.GetAttackSellVolume(), 0);
    EXPECT_EQ(stock.GetStrongBuyVolume(), 0);
    EXPECT_EQ(stock.GetStrongSellVolume(), 0);
    EXPECT_EQ(stock.GetCancelBuyVolume(), 0);
    EXPECT_EQ(stock.GetCancelSellVolume(), 0);
    EXPECT_EQ(stock.GetCurrentAttackBuy(), 0);
    EXPECT_EQ(stock.GetCurrentAttackSell(), 0);
    EXPECT_EQ(stock.GetCurrentStrongBuy(), 0);
    EXPECT_EQ(stock.GetCurrentStrongSell(), 0);
    EXPECT_EQ(stock.GetUnknownVolume(), 0);
    EXPECT_EQ(stock.GetCurrentUnknown(), 0);
    long l = stock.GetCurrentTransactionType();
    EXPECT_EQ(stock.GetCurrentTransactionType(), 0);
    EXPECT_EQ(stock.GetCurrentTransationVolume(), 0);
    EXPECT_EQ(stock.GetCancelSellVolume(), 0);
    EXPECT_EQ(stock.GetCancelBuyVolume(), 0);
    EXPECT_EQ(stock.GetAttackBuyBelow50000(), 0);
    EXPECT_EQ(stock.GetAttackBuyBelow200000(), 0);
    EXPECT_EQ(stock.GetAttackBuyAbove200000(), 0);
    EXPECT_EQ(stock.GetAttackSellBelow50000(), 0);
    EXPECT_EQ(stock.GetAttackSellBelow200000(), 0);
    EXPECT_EQ(stock.GetAttackSellAbove200000(), 0);
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

  TEST(StockTest, TestGetCode) {
    CStock stock;
    EXPECT_EQ(stock.GetCode(), 0);
    stock.SetCode(600000);
    EXPECT_EQ(stock.GetCode(), 600000);
  }

  TEST(StockTest, TestIsActive) {
    CStock stock;
    EXPECT_FALSE(stock.IsActive());
    stock.SetActive(true);
    EXPECT_TRUE(stock.IsActive());
    stock.SetActive(false);
    EXPECT_FALSE(stock.IsActive());
  }

  TEST(StockTest, TestGetTime) {
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

  TEST(StockTest, TestGetAttackBuyAmount) {
    CStock stock;
    EXPECT_EQ(stock.GetAttackBuyAmount(), 0);
    stock.SetAttackBuyAmount(100100100);
    EXPECT_EQ(stock.GetAttackBuyAmount(), 100100100);
  }

  TEST(StockTest, TestGetAttackSellAmount) {
    CStock stock;
    EXPECT_EQ(stock.GetAttackSellAmount(), 0);
    stock.SetAttackSellAmount(100100100);
    EXPECT_EQ(stock.GetAttackSellAmount(), 100100100);
  }

  TEST(StockTest, TestGetOrdinaryBuyVolume) {
    CStock stock;
    EXPECT_EQ(stock.GetOrdinaryBuyVolume(), 0);
    stock.SetOrdinaryBuyVolume(100100100);
    EXPECT_EQ(stock.GetOrdinaryBuyVolume(), 100100100);
  }

  TEST(StockTest, TestGetOrdinarySellVolume) {
    CStock stock;
    EXPECT_EQ(stock.GetOrdinarySellVolume(), 0);
    stock.SetOrdinarySellVolume(100100100);
    EXPECT_EQ(stock.GetOrdinarySellVolume(), 100100100);
  }

  TEST(StockTest, TestGetAttackBuyVolume) {
    CStock stock;
    EXPECT_EQ(stock.GetAttackBuyVolume(), 0);
    stock.SetAttackBuyVolume(100100100);
    EXPECT_EQ(stock.GetAttackBuyVolume(), 100100100);
  }

  TEST(StockTest, TestGetStrongBuyVolume) {
    CStock stock;
    EXPECT_EQ(stock.GetStrongBuyVolume(), 0);
    stock.SetStrongBuyVolume(100100100);
    EXPECT_EQ(stock.GetStrongBuyVolume(), 100100100);
  }

  TEST(StockTest, TestGetCurrentAttackBuy) {
    CStock stock;
    EXPECT_EQ(stock.GetCurrentAttackBuy(), 0);
    stock.SetCurrentAttackBuy(100100100);
    EXPECT_EQ(stock.GetCurrentAttackBuy(), 100100100);
  }

  TEST(StockTest, TestGetCurrentStrongBuy) {
    CStock stock;
    EXPECT_EQ(stock.GetCurrentStrongBuy(), 0);
    stock.SetCurrentStrongBuy(100100100);
    EXPECT_EQ(stock.GetCurrentStrongBuy(), 100100100);
  }

  TEST(StockTest, TestGetAttackSellVolume) {
    CStock stock;
    EXPECT_EQ(stock.GetAttackSellVolume(), 0);
    stock.SetAttackSellVolume(100100100);
    EXPECT_EQ(stock.GetAttackSellVolume(), 100100100);
  }

  TEST(StockTest, TestGetStrongSellVolume) {
    CStock stock;
    EXPECT_EQ(stock.GetStrongSellVolume(), 0);
    stock.SetStrongSellVolume(100100100);
    EXPECT_EQ(stock.GetStrongSellVolume(), 100100100);
  }

  TEST(StockTest, TestGetCurrentAttackSell) {
    CStock stock;
    EXPECT_EQ(stock.GetCurrentAttackSell(), 0);
    stock.SetCurrentAttackSell(100100100);
    EXPECT_EQ(stock.GetCurrentAttackSell(), 100100100);
  }

  TEST(StockTest, TestGetCurrentStrongSell) {
    CStock stock;
    EXPECT_EQ(stock.GetCurrentStrongSell(), 0);
    stock.SetCurrentStrongSell(100100100);
    EXPECT_EQ(stock.GetCurrentStrongSell(), 100100100);
  }

  TEST(StockTest, TestGetUnknownVolume) {
    CStock stock;
    EXPECT_EQ(stock.GetUnknownVolume(), 0);
    stock.SetUnknownVolume(100100100);
    EXPECT_EQ(stock.GetUnknownVolume(), 100100100);
  }

  TEST(StockTest, TestGetCurrentUnknown) {
    CStock stock;
    EXPECT_EQ(stock.GetCurrentUnknown(), 0);
    stock.SetCurrentUnknown(100100100);
    EXPECT_EQ(stock.GetCurrentUnknown(), 100100100);
  }

  TEST(StockTest, TestGetCancelBuyVolume) {
    CStock stock;
    EXPECT_EQ(stock.GetCancelBuyVolume(), 0);
    stock.SetCancelBuyVolume(100100100);
    EXPECT_EQ(stock.GetCancelBuyVolume(), 100100100);
  }

  TEST(StockTest, TestGetCancelSellVolume) {
    CStock stock;
    EXPECT_EQ(stock.GetCancelSellVolume(), 0);
    stock.SetCancelSellVolume(100100100);
    EXPECT_EQ(stock.GetCancelSellVolume(), 100100100);
  }

  TEST(StockTest, TestGetTransactionNumber) {
    CStock stock;
    EXPECT_EQ(stock.GetTransactionNumber(), 0);
    stock.SetTransactionNumber(100100100);
    EXPECT_EQ(stock.GetTransactionNumber(), 100100100);
  }

  TEST(StockTest, TestGetTransactionNumberBelow5000) {
    CStock stock;
    EXPECT_EQ(stock.GetTransactionNumberBelow5000(), 0);
    stock.SetTransactionNumberBelow5000(100100100);
    EXPECT_EQ(stock.GetTransactionNumberBelow5000(), 100100100);
  }

  TEST(StockTest, TestGetTransactionNumberBelow50000) {
    CStock stock;
    EXPECT_EQ(stock.GetTransactionNumberBelow50000(), 0);
    stock.SetTransactionNumberBelow50000(100100100);
    EXPECT_EQ(stock.GetTransactionNumberBelow50000(), 100100100);
  }

  TEST(StockTest, TestGetTransactionNumberBelow200000) {
    CStock stock;
    EXPECT_EQ(stock.GetTransactionNumberBelow200000(), 0);
    stock.SetTransactionNumberBelow200000(100100100);
    EXPECT_EQ(stock.GetTransactionNumberBelow200000(), 100100100);
  }

  TEST(StockTest, TestGetTransactionNumberAbove200000) {
    CStock stock;
    EXPECT_EQ(stock.GetTransactionNumberAbove200000(), 0);
    stock.SetTransactionNumberAbove200000(100100100);
    EXPECT_EQ(stock.GetTransactionNumberAbove200000(), 100100100);
  }

  TEST(StockTest, TestGetAttackBuyBelow50000) {
    CStock stock;
    EXPECT_EQ(stock.GetAttackBuyBelow50000(), 0);
    stock.SetAttackBuyBelow50000(100100100);
    EXPECT_EQ(stock.GetAttackBuyBelow50000(), 100100100);
  }

  TEST(StockTest, TestGetAttackBuyBelow200000) {
    CStock stock;
    EXPECT_EQ(stock.GetAttackBuyBelow200000(), 0);
    stock.SetAttackBuyBelow200000(100100100);
    EXPECT_EQ(stock.GetAttackBuyBelow200000(), 100100100);
  }

  TEST(StockTest, TestGetAttackBuyAbove200000) {
    CStock stock;
    EXPECT_EQ(stock.GetAttackBuyAbove200000(), 0);
    stock.SetAttackBuyAbove200000(100100100);
    EXPECT_EQ(stock.GetAttackBuyAbove200000(), 100100100);
  }

  TEST(StockTest, TestGetAttackSellBelow50000) {
    CStock stock;
    EXPECT_EQ(stock.GetAttackSellBelow50000(), 0);
    stock.SetAttackSellBelow50000(100100100);
    EXPECT_EQ(stock.GetAttackSellBelow50000(), 100100100);
  }

  TEST(StockTest, TestGetAttackSellBelow200000) {
    CStock stock;
    EXPECT_EQ(stock.GetAttackSellBelow200000(), 0);
    stock.SetAttackSellBelow200000(100100100);
    EXPECT_EQ(stock.GetAttackSellBelow200000(), 100100100);
  }

  TEST(StockTest, TestGetAttackSellAbove200000) {
    CStock stock;
    EXPECT_EQ(stock.GetAttackSellAbove200000(), 0);
    stock.SetAttackSellAbove200000(100100100);
    EXPECT_EQ(stock.GetAttackSellAbove200000(), 100100100);
  }

  TEST(StockTest, TestReadOnlyFunctions) {
    CStock stock;

    EXPECT_DOUBLE_EQ(stock.GetRelativeStrong(), 0);
  }

  TEST(StockTest, TestGetGuaDan) {
    CStock stock;
    EXPECT_FALSE(stock.HaveGuaDan(10000));
    stock.SetGuaDan(10000, 10000);
    EXPECT_TRUE(stock.HaveGuaDan(10000));
    EXPECT_EQ(stock.GetGuaDan(10000), 10000);
  }

  TEST(StockTest, TestIsChoiced) {
    CStock stock;
    EXPECT_FALSE(stock.IsChoiced());
    stock.SetChoicedFlag(true);
    EXPECT_TRUE(stock.IsChoiced());
    stock.SetChoicedFlag(false);
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

  TEST(StockTest, TestIsStartCalculating) {
    CStock stock;
    EXPECT_FALSE(stock.IsStartCalculating());
    EXPECT_FALSE(stock.SetStartCalculating(false)); // 不允许外部设置停止计算标识（内部可以）
    EXPECT_FALSE(stock.IsStartCalculating());
    EXPECT_TRUE(stock.SetStartCalculating(true));
    EXPECT_TRUE(stock.IsStartCalculating());
    EXPECT_FALSE(stock.SetStartCalculating(true));  // 不允许再次设置开始计算标识
    EXPECT_TRUE(stock.IsStartCalculating());
  }

  TEST(StockTest, TestIsDayNeededSaving) {    // 此两个函数是具备同步机制的，这里没有进行测试
    CStock stock;
    stock.SetDayLineNeedSavingFlag(true);
    EXPECT_TRUE(stock.IsDayLineNeedSaving());
    stock.SetDayLineNeedSavingFlag(false);
    EXPECT_FALSE(stock.IsDayLineNeedSaving());
  }

  TEST(StockTest, TestUpdateCurrentStatus) {
    CStock stock;
    CStockRTDataPtr pRTData;

    pRTData = make_shared<CStockRTData>();
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
    stock.UpdataCurrentStatus(pRTData);
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

  TEST(StockTest, TestRTDataDeque) {    // 此三个函数是具备同步机制的，这里没有进行测试
    CStockRTDataPtr pData = make_shared<CStockRTData>();
    pData->SetCode(600000);
    CStock stock;
    EXPECT_EQ(stock.GetRTDataDequeSize(), 0);
    stock.PushRTData(pData);
    EXPECT_EQ(stock.GetRTDataDequeSize(), 1);
    CStockRTDataPtr pData2 = stock.PopRTData();
    EXPECT_EQ(pData2->GetCode(), 600000);
    EXPECT_EQ(stock.GetRTDataDequeSize(), 0);
  }
}