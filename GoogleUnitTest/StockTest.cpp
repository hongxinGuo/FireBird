#include"stdafx.h"

#include"pch.h"
#include"globedef.h"

#include"Stock.h"
#include"ChinaMarket.h"

static CSinaRTWebData m_SinaRTWebData; // 新浪实时数据采集
static CTengxunRTWebData m_TengxunRTWebData; // 腾讯实时数据采集
static CNeteaseDayLineWebData m_NeteaseDayLineWebData; // 网易日线历史数据

namespace StockAnalysisTest {
  class CStockTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
    }

    virtual void TearDown(void) override {
      // clearup
    }

    static void SetUpTestCase() { // 本测试类的初始化函数
      ASSERT_FALSE(gl_fNormalMode);
    }

    static void TearDownTestCase() {
    }
  };

  TEST_F(CStockTest, TestInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    CStock stock;
    EXPECT_EQ(stock.GetDayLineSize(), 0);
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
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    EXPECT_FALSE(stock.IsDayLineNeedProcess());
    EXPECT_FALSE(stock.IsDayLineNeedSaving());
    EXPECT_FALSE(stock.IsChoiced());
    EXPECT_FALSE(stock.IsMinLineUpdated());
    EXPECT_FALSE(stock.IsDayLineUpdated());
    EXPECT_FALSE(stock.HaveFirstRTData());
  }
  TEST_F(CStockTest, TestGetMarket) {
    CStock stock;
    EXPECT_EQ(stock.GetMarket(), 0);
    stock.SetMarket(1);
    EXPECT_EQ(stock.GetMarket(), 1);
  }

  TEST_F(CStockTest, TestGetStockCode) {
    CStock stock;
    EXPECT_STREQ(stock.GetStockCode(), _T(""));
    stock.SetStockCode(_T("sh600000"));
    EXPECT_STREQ(stock.GetStockCode(), _T("sh600000"));
  }

  TEST_F(CStockTest, TestGetStockName) {
    CStock stock;
    EXPECT_STREQ(stock.GetStockName(), _T(""));
    stock.SetStockName(_T("浦东银行"));
    EXPECT_STREQ(stock.GetStockName(), _T("浦东银行"));
  }

  TEST_F(CStockTest, TestGetOffset) {
    CStock stock;
    EXPECT_EQ(stock.GetOffset(), -1);
    stock.SetOffset(_T(101010));
    EXPECT_EQ(stock.GetOffset(), 101010);
  }

  TEST_F(CStockTest, TestGetDayLineStartDay) {
    CStock stock;
    EXPECT_EQ(stock.GetDayLineStartDay(), __CHINA_MARKET_BEGIN_DAY__);
    stock.SetDayLineStartDay(100100100);
    EXPECT_EQ(stock.GetDayLineStartDay(), 100100100);
  }

  TEST_F(CStockTest, TestGetDayLineEndDay) {
    CStock stock;
    EXPECT_EQ(stock.GetDayLineEndDay(), __CHINA_MARKET_BEGIN_DAY__);
    stock.SetDayLineEndDay(100100100);
    EXPECT_EQ(stock.GetDayLineEndDay(), 100100100);
  }

  TEST_F(CStockTest, TestGetIPOStatus) {
    CStock stock;
    EXPECT_EQ(stock.GetIPOStatus(), __STOCK_NOT_CHECKED__);
    stock.SetIPOStatus(100100100);
    EXPECT_EQ(stock.GetIPOStatus(), 100100100);
  }

  TEST_F(CStockTest, TestGetTransactionTime) {
    CStock stock;
    EXPECT_EQ(stock.GetTransactionTime(), 0);
    stock.SetTransactionTime(100100100100);
    EXPECT_EQ(stock.GetTransactionTime(), 100100100100);
  }

  TEST_F(CStockTest, TestGetLastClose) {
    CStock stock;
    EXPECT_EQ(stock.GetLastClose(), 0);
    stock.SetLastClose(100100100);
    EXPECT_EQ(stock.GetLastClose(), 100100100);
  }

  TEST_F(CStockTest, TestGetOpen) {
    CStock stock;
    EXPECT_EQ(stock.GetOpen(), 0);
    stock.SetOpen(100100100);
    EXPECT_EQ(stock.GetOpen(), 100100100);
  }

  TEST_F(CStockTest, TestGetHigh) {
    CStock stock;
    EXPECT_EQ(stock.GetHigh(), 0);
    stock.SetHigh(100100100);
    EXPECT_EQ(stock.GetHigh(), 100100100);
  }

  TEST_F(CStockTest, TestGetLow) {
    CStock stock;
    EXPECT_EQ(stock.GetLow(), 0);
    stock.SetLow(100100100);
    EXPECT_EQ(stock.GetLow(), 100100100);
  }

  TEST_F(CStockTest, TestGetNew) {
    CStock stock;
    EXPECT_EQ(stock.GetNew(), 0);
    stock.SetNew(100100100);
    EXPECT_EQ(stock.GetNew(), 100100100);
  }

  TEST_F(CStockTest, TestGetAmount) {
    CStock stock;
    EXPECT_EQ(stock.GetAmount(), 0);
    stock.SetAmount(100100100);
    EXPECT_EQ(stock.GetAmount(), 100100100);
  }

  TEST_F(CStockTest, TestGetVolume) {
    CStock stock;
    EXPECT_EQ(stock.GetVolume(), 0);
    stock.SetVolume(100100100);
    EXPECT_EQ(stock.GetVolume(), 100100100);
  }

  TEST_F(CStockTest, TestUpdateStatus) {
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
    EXPECT_EQ(stock.GetTransactionTime(), 100100100100);   // UpdateCurrentStatus函数不更新交易时间
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

  TEST_F(CStockTest, TestGetAttackBuyAmount) {
    CStock id;
    EXPECT_EQ(id.GetAttackBuyAmount(), 0);
    id.SetAttackBuyAmount(10101010);
    EXPECT_EQ(id.GetAttackBuyAmount(), 10101010);
  }

  TEST_F(CStockTest, TestGetAttackSellAmount) {
    CStock id;
    EXPECT_EQ(id.GetAttackSellAmount(), 0);
    id.SetAttackSellAmount(10101010);
    EXPECT_EQ(id.GetAttackSellAmount(), 10101010);
  }

  TEST_F(CStockTest, TestGetOrdinaryBuyVolume) {
    CStock id;
    EXPECT_EQ(id.GetOrdinaryBuyVolume(), 0);
    id.SetOrdinaryBuyVolume(10101010);
    EXPECT_EQ(id.GetOrdinaryBuyVolume(), 10101010);
  }

  TEST_F(CStockTest, TestGetOrdinarySellVolume) {
    CStock id;
    EXPECT_EQ(id.GetOrdinarySellVolume(), 0);
    id.SetOrdinarySellVolume(10101010);
    EXPECT_EQ(id.GetOrdinarySellVolume(), 10101010);
  }

  TEST_F(CStockTest, TestGetAttackBuyVolume) {
    CStock id;
    EXPECT_EQ(id.GetAttackBuyVolume(), 0);
    id.SetAttackBuyVolume(10101010);
    EXPECT_EQ(id.GetAttackBuyVolume(), 10101010);
  }

  TEST_F(CStockTest, TestGetStrongBuyVolume) {
    CStock id;
    EXPECT_EQ(id.GetStrongBuyVolume(), 0);
    id.SetStrongBuyVolume(10101010);
    EXPECT_EQ(id.GetStrongBuyVolume(), 10101010);
  }

  TEST_F(CStockTest, TestGetCurrentAttackBuy) {
    CStock id;
    EXPECT_EQ(id.GetCurrentAttackBuy(), 0);
    id.SetCurrentAttackBuy(10101010);
    EXPECT_EQ(id.GetCurrentAttackBuy(), 10101010);
  }

  TEST_F(CStockTest, TestGetCurrentStrongBuy) {
    CStock id;
    EXPECT_EQ(id.GetCurrentStrongBuy(), 0);
    id.SetCurrentStrongBuy(10101010);
    EXPECT_EQ(id.GetCurrentStrongBuy(), 10101010);
  }

  TEST_F(CStockTest, TestGetAttackSellVolume) {
    CStock id;
    EXPECT_EQ(id.GetAttackSellVolume(), 0);
    id.SetAttackSellVolume(10101010);
    EXPECT_EQ(id.GetAttackSellVolume(), 10101010);
  }

  TEST_F(CStockTest, TestGetStrongSellVolume) {
    CStock id;
    EXPECT_EQ(id.GetStrongSellVolume(), 0);
    id.SetStrongSellVolume(10101010);
    EXPECT_EQ(id.GetStrongSellVolume(), 10101010);
  }

  TEST_F(CStockTest, TestGetCurrentAttackSell) {
    CStock id;
    EXPECT_EQ(id.GetCurrentAttackSell(), 0);
    id.SetCurrentAttackSell(10101010);
    EXPECT_EQ(id.GetCurrentAttackSell(), 10101010);
  }

  TEST_F(CStockTest, TestGetCurrentStrongSell) {
    CStock id;
    EXPECT_EQ(id.GetCurrentStrongSell(), 0);
    id.SetCurrentStrongSell(10101010);
    EXPECT_EQ(id.GetCurrentStrongSell(), 10101010);
  }

  TEST_F(CStockTest, TestGetUnknownVolume) {
    CStock id;
    EXPECT_EQ(id.GetUnknownVolume(), 0);
    id.SetUnknownVolume(10101010);
    EXPECT_EQ(id.GetUnknownVolume(), 10101010);
  }

  TEST_F(CStockTest, TestGetCurrentUnknown) {
    CStock id;
    EXPECT_EQ(id.GetCurrentUnknown(), 0);
    id.SetCurrentUnknown(10101010);
    EXPECT_EQ(id.GetCurrentUnknown(), 10101010);
  }

  TEST_F(CStockTest, TestGetCancelBuyVolume) {
    CStock id;
    EXPECT_EQ(id.GetCancelBuyVolume(), 0);
    id.SetCancelBuyVolume(10101010);
    EXPECT_EQ(id.GetCancelBuyVolume(), 10101010);
  }
  TEST_F(CStockTest, TestGetCancelSellVolume) {
    CStock id;
    EXPECT_EQ(id.GetCancelSellVolume(), 0);
    id.SetCancelSellVolume(10101010);
    EXPECT_EQ(id.GetCancelSellVolume(), 10101010);
  }

  TEST_F(CStockTest, TestReadOnlyFunctions) {
  }

  TEST_F(CStockTest, TestGetRelativeStrong) {
    CStock stock;

    EXPECT_DOUBLE_EQ(stock.GetRelativeStrong(), 0);
    stock.SetRelativeStrong(1.1);
    EXPECT_DOUBLE_EQ(stock.GetRelativeStrong(), 1.1);
  }

  TEST_F(CStockTest, TestIsActive) {
    CStock stock;
    EXPECT_FALSE(stock.IsActive());
    stock.SetActive(true);
    EXPECT_TRUE(stock.IsActive());
    stock.SetActive(false);
    EXPECT_FALSE(stock.IsActive());
  }

  TEST_F(CStockTest, TestIsDayLineNeedUpdate) {
    CStock stock;
    long lNumberOfStock = gl_ChinaStockMarket.GetDayLineNeedUpdateNumber();
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    stock.SetDayLineNeedUpdate(false);
    EXPECT_FALSE(stock.IsDayLineNeedUpdate());
    EXPECT_EQ(lNumberOfStock, gl_ChinaStockMarket.GetDayLineNeedUpdateNumber() + 1);
    stock.SetDayLineNeedUpdate(true);
    EXPECT_TRUE(stock.IsDayLineNeedUpdate());
    EXPECT_EQ(lNumberOfStock, gl_ChinaStockMarket.GetDayLineNeedUpdateNumber());
  }

  TEST_F(CStockTest, TestIsDayLineNeedProcess) {
    CStock stock;
    long lNumberOfStock = gl_ChinaStockMarket.GetDayLineNeedProcessNumber();
    EXPECT_FALSE(stock.IsDayLineNeedProcess());
    stock.SetDayLineNeedProcess(true);
    EXPECT_TRUE(stock.IsDayLineNeedProcess());
    EXPECT_EQ(lNumberOfStock + 1, gl_ChinaStockMarket.GetDayLineNeedProcessNumber());
    stock.SetDayLineNeedProcess(false);
    EXPECT_FALSE(stock.IsDayLineNeedProcess());
    EXPECT_EQ(lNumberOfStock, gl_ChinaStockMarket.GetDayLineNeedProcessNumber());
  }

  TEST_F(CStockTest, TestIsChoiced) {
    CStock stock;
    EXPECT_FALSE(stock.IsChoiced());
    stock.SetChoiced(true);
    EXPECT_TRUE(stock.IsChoiced());
    stock.SetChoiced(false);
    EXPECT_FALSE(stock.IsChoiced());
  }

  TEST_F(CStockTest, TestIsMinLineUpdated) {
    CStock stock;
    EXPECT_FALSE(stock.IsMinLineUpdated());
    stock.SetMinLineUpdated(true);
    EXPECT_TRUE(stock.IsMinLineUpdated());
    stock.SetMinLineUpdated(false);
    EXPECT_FALSE(stock.IsMinLineUpdated());
  }

  TEST_F(CStockTest, TestIsDayLineUpdated) {
    CStock stock;
    EXPECT_FALSE(stock.IsDayLineUpdated());
    stock.SetDayLineUpdated(true);
    EXPECT_TRUE(stock.IsDayLineUpdated());
    stock.SetDayLineUpdated(false);
    EXPECT_FALSE(stock.IsDayLineUpdated());
  }

  TEST_F(CStockTest, TestIsDayLineLoaded) {
    CStock stock;
    EXPECT_FALSE(stock.IsDayLineLoaded());
    stock.SetDayLineLoaded(true);
    EXPECT_TRUE(stock.IsDayLineLoaded());
    stock.SetDayLineLoaded(false);
    EXPECT_FALSE(stock.IsDayLineLoaded());
  }

  TEST_F(CStockTest, TestHaveFirstRTData) {
    CStock stock;
    EXPECT_FALSE(stock.HaveFirstRTData());
    stock.SetHavingFirstRTData(true);
    EXPECT_TRUE(stock.HaveFirstRTData());
    stock.SetHavingFirstRTData(false);
    EXPECT_TRUE(stock.HaveFirstRTData());
  }

  TEST_F(CStockTest, TestIsStartCalculating) {
    CStock stock;
    EXPECT_FALSE(stock.HaveFirstRTData());
    EXPECT_FALSE(stock.SetHavingFirstRTData(false)); // 不允许外部设置停止计算标识（内部可以）
    EXPECT_FALSE(stock.HaveFirstRTData());
    EXPECT_TRUE(stock.SetHavingFirstRTData(true));
    EXPECT_TRUE(stock.HaveFirstRTData());
    EXPECT_FALSE(stock.SetHavingFirstRTData(true));  // 不允许再次设置开始计算标识
    EXPECT_TRUE(stock.HaveFirstRTData());
  }

  TEST_F(CStockTest, TestIsDayNeededSaving) {    // 此两个函数是具备同步机制的，这里没有进行测试
    CStock stock;
    int iNumberOfSave = gl_ChinaStockMarket.GetDayLineNeedSaveNumber();
    stock.SetDayLineNeedSaving(true);
    EXPECT_EQ(iNumberOfSave + 1, gl_ChinaStockMarket.GetDayLineNeedSaveNumber());
    EXPECT_TRUE(stock.IsDayLineNeedSaving());
    stock.SetDayLineNeedSaving(false);
    EXPECT_EQ(iNumberOfSave, gl_ChinaStockMarket.GetDayLineNeedSaveNumber());
    EXPECT_FALSE(stock.IsDayLineNeedSaving());
    stock.SetDayLineNeedSaving(true);
    EXPECT_TRUE(stock.IsDayLineNeedSavingAndClearFlag());
    EXPECT_FALSE(stock.IsDayLineNeedSaving());
    EXPECT_EQ(iNumberOfSave, gl_ChinaStockMarket.GetDayLineNeedSaveNumber());
  }

  TEST_F(CStockTest, TestTransferNeteaseDayLineWebDataToBuffer) {
    CString str = _T("abcedfg\r\n");
    m_NeteaseDayLineWebData.__TESTSetBuffer(str);
    CStock stock;
    EXPECT_FALSE(stock.IsDayLineNeedProcess());
    EXPECT_EQ(stock.GetDayLineBufferLength(), 0);
    stock.TransferNeteaseDayLineWebDataToBuffer(&m_NeteaseDayLineWebData);
    EXPECT_EQ(stock.GetDayLineBufferLength(), str.GetLength());
    EXPECT_TRUE(stock.IsDayLineNeedProcess());
    stock.SetDayLineNeedProcess(false); // 将此标识还原为初始状态。
  }

  TEST_F(CStockTest, TestTodayDataIsActive) {
    CStock stock;
    EXPECT_FALSE(stock.IsTodayDataActive());
    stock.SetActive(true);
    EXPECT_FALSE(stock.IsTodayDataActive());
    stock.SetHigh(1010);
    EXPECT_TRUE(stock.IsTodayDataActive());
    stock.SetHigh(0);
    stock.SetLow(1);
    EXPECT_TRUE(stock.IsTodayDataActive());
    stock.SetLow(0);
    stock.SetAmount(10000);
    EXPECT_TRUE(stock.IsTodayDataActive());
    stock.SetAmount(0);
    stock.SetVolume(100);
    EXPECT_TRUE(stock.IsTodayDataActive());
    stock.SetActive(false);
    EXPECT_FALSE(stock.IsTodayDataActive());
  }

  TEST_F(CStockTest, TestIsTodayDataChanged) {
    CStock stock;
    EXPECT_FALSE(stock.IsTodayDataChanged());
    stock.SetHigh(10);
    EXPECT_TRUE(stock.IsTodayDataChanged());
    stock.SetHigh(0);
    stock.SetLow(10);
    EXPECT_TRUE(stock.IsTodayDataChanged());
    stock.SetLow(0);
    stock.SetVolume(10);
    EXPECT_TRUE(stock.IsTodayDataChanged());
    stock.SetVolume(0);
    stock.SetAmount(10);
    EXPECT_TRUE(stock.IsTodayDataChanged());
  }

  TEST_F(CStockTest, TestGetCurrentGuadanTransactionPrice) {
    CStock id;

    EXPECT_DOUBLE_EQ(id.GetCurrentGuadanTransactionPrice(), 0);
    id.SetCurrentGuadanTransactionPrice(10.01);
    EXPECT_DOUBLE_EQ(id.GetCurrentGuadanTransactionPrice(), 10.01);
  }

  TEST_F(CStockTest, TestSetCheckingDayLineStatus) {
    CStock id;
    EXPECT_TRUE(id.IsDayLineNeedUpdate());
    id.SetDayLineEndDay(gl_ChinaStockMarket.GetDay());
    id.SetCheckingDayLineStatus();
    EXPECT_FALSE(id.IsDayLineNeedUpdate());
    id.SetDayLineNeedUpdate(true);
    id.SetDayLineEndDay(gl_ChinaStockMarket.GetLastTradeDay());
    id.SetCheckingDayLineStatus();
    EXPECT_FALSE(id.IsDayLineNeedUpdate());
    id.SetDayLineNeedUpdate(true);
    id.SetDayLineEndDay(gl_ChinaStockMarket.GetLastTradeDay() - 1);
    id.SetIPOStatus(__STOCK_NULL__);
    id.SetCheckingDayLineStatus();
    EXPECT_FALSE(id.IsDayLineNeedUpdate());
    id.SetDayLineNeedUpdate(true);
    id.SetIPOStatus(__STOCK_DELISTED__);
    id.SetCheckingDayLineStatus();
    if (gl_ChinaStockMarket.GetDayOfWeek() == 1) EXPECT_TRUE(id.IsDayLineNeedUpdate());
    else EXPECT_FALSE(id.IsDayLineNeedUpdate());
    id.SetDayLineNeedUpdate(true);
    id.SetDayLineEndDay(__CHINA_MARKET_BEGIN_DAY__);
    id.SetCheckingDayLineStatus();
    EXPECT_TRUE(id.IsDayLineNeedUpdate());
  }

  TEST_F(CStockTest, TestRTDataDeque) {    // 此三个函数是具备同步机制的，这里没有进行测试
    CRTDataPtr pData = make_shared<CRTData>();
    pData->SetStockCode(_T("sh600008"));
    CStock stock;
    EXPECT_EQ(stock.GetRTDataQueueSize(), 0);
    stock.PushRTData(pData);
    EXPECT_EQ(stock.GetRTDataQueueSize(), 1);
    pData = stock.GetRTDataAtHead();
    EXPECT_EQ(stock.GetRTDataQueueSize(), 1);
    EXPECT_STREQ(pData->GetStockCode(), _T("sh600008"));
    CRTDataPtr pData2 = stock.PopRTData();
    EXPECT_EQ(stock.GetRTDataQueueSize(), 0);
  }

  TEST_F(CStockTest, TestGetGuadan) {
    CStock stock;
    EXPECT_FALSE(stock.HaveGuadan(10000));
    stock.SetGuadan(10000, 10000);
    EXPECT_TRUE(stock.HaveGuadan(10000));
    EXPECT_EQ(stock.GetGuadan(10000), 10000);
  }

  TEST_F(CStockTest, TestClearRTDataDeque) {
    CStock stock;
    CRTDataPtr pRTData;
    for (int i = 0; i < 10; i++) {
      pRTData = make_shared<CRTData>();
      pRTData->SetLastClose(i);
      stock.PushRTData(pRTData);
    }
    EXPECT_EQ(stock.GetRTDataQueueSize(), 10);
    stock.ClearRTDataDeque();
    EXPECT_EQ(stock.GetRTDataQueueSize(), 0);
  }

  TEST_F(CStockTest, TestSaveTempInfo) {
    CSetDayLineToday setDayLineToday;
    CStockPtr pStock = make_shared<CStock>();

    pStock->SetHavingFirstRTData(true);
    pStock->SetMarket(__SHANGHAI_MARKET__);
    pStock->SetStockCode(_T("sh600000"));
    pStock->SetTransactionTime(FormatToTTime(20191101));
    pStock->SetLastClose(101010);
    pStock->SetOpen(202020);
    pStock->SetHigh(303030);
    pStock->SetLow(404040);
    pStock->SetNew(505050);
    pStock->SetVolume(60606060606060);
    pStock->SetAmount(707070707070707);
    pStock->SetUpDown(pStock->GetOpen() - pStock->GetNew());
    pStock->SetUpDownRate((double)pStock->GetUpDown() / pStock->GetLastClose() * 100);
    pStock->SetCurrentValue(808080808080808);
    pStock->SetTotalValue(909090909090909);

    pStock->SetTransactionNumber(123435);
    pStock->SetTransactionNumberBelow5000(45346456);
    pStock->SetTransactionNumberBelow50000(5698);
    pStock->SetTransactionNumberBelow200000(67687);
    pStock->SetTransactionNumberAbove200000(6876);
    pStock->SetOrdinaryBuyVolume(435);
    pStock->SetOrdinarySellVolume(560985);
    pStock->SetAttackBuyBelow50000(54509);
    pStock->SetAttackBuyBelow200000(45049);
    pStock->SetAttackBuyAbove200000(34508);
    pStock->SetAttackSellBelow50000(45896);
    pStock->SetAttackSellBelow200000(56457);
    pStock->SetAttackSellAbove200000(8767);
    pStock->SetAttackBuyAmount(123456654);
    pStock->SetAttackBuyVolume(23423534);
    pStock->SetAttackSellAmount(4353454);
    pStock->SetAttackSellVolume(94589489);
    pStock->SetCancelBuyVolume(435245);
    pStock->SetCancelSellVolume(45648698);
    pStock->SetUnknownVolume(4895747);
    pStock->SetStrongBuyVolume(453456);
    pStock->SetStrongSellVolume(98976);

    ASSERT(!gl_fNormalMode);
    ASSERT(gl_fTestMode);
    setDayLineToday.Open();
    setDayLineToday.m_pDatabase->BeginTrans();
    while (!setDayLineToday.IsEOF()) {
      setDayLineToday.Delete();
      setDayLineToday.MoveNext();
    }
    setDayLineToday.m_pDatabase->CommitTrans();
    setDayLineToday.m_pDatabase->BeginTrans();
    setDayLineToday.AddNew();

    pStock->SaveTempInfo(setDayLineToday);
    setDayLineToday.Update();
    setDayLineToday.m_pDatabase->CommitTrans();
    setDayLineToday.Close();

    setDayLineToday.Open();
    EXPECT_EQ(setDayLineToday.m_Day, 20191101);
    EXPECT_EQ(setDayLineToday.m_Market, pStock->GetMarket());
    EXPECT_STREQ(setDayLineToday.m_StockCode, pStock->GetStockCode());
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_LastClose), (double)pStock->GetLastClose() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_Open), (double)pStock->GetOpen() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_High), (double)pStock->GetHigh() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_Low), (double)pStock->GetLow() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_Close), (double)pStock->GetNew() / 1000);
    EXPECT_EQ(atoll(setDayLineToday.m_Volume), pStock->GetVolume());
    EXPECT_EQ(atoll(setDayLineToday.m_Amount), pStock->GetAmount());
    EXPECT_EQ(atof(setDayLineToday.m_UpAndDown), (double)pStock->GetUpDown() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLineToday.m_UpDownRate), pStock->GetUpDownRate());
    EXPECT_EQ(atoll(setDayLineToday.m_CurrentValue), pStock->GetCurrentValue());
    EXPECT_EQ(atoll(setDayLineToday.m_TotalValue), pStock->GetTotalValue());

    EXPECT_EQ(atol(setDayLineToday.m_AttackBuyAbove200000), pStock->GetAttackBuyAbove200000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackBuyBelow200000), pStock->GetAttackBuyBelow200000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackBuyBelow50000), pStock->GetAttackBuyBelow50000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackBuyVolume), pStock->GetAttackBuyVolume());
    EXPECT_EQ(atol(setDayLineToday.m_AttackSellAbove200000), pStock->GetAttackSellAbove200000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackSellBelow200000), pStock->GetAttackSellBelow200000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackSellBelow50000), pStock->GetAttackSellBelow50000());
    EXPECT_EQ(atol(setDayLineToday.m_AttackSellVolume), pStock->GetAttackSellVolume());

    EXPECT_EQ(atol(setDayLineToday.m_OrdinaryBuyVolume), pStock->GetOrdinaryBuyVolume());
    EXPECT_EQ(atol(setDayLineToday.m_OrdinarySellVolume), pStock->GetOrdinarySellVolume());
    EXPECT_EQ(atol(setDayLineToday.m_CancelBuyVolume), pStock->GetCancelBuyVolume());
    EXPECT_EQ(atol(setDayLineToday.m_CancelSellVolume), pStock->GetCancelSellVolume());
    EXPECT_EQ(atol(setDayLineToday.m_StrongBuyVolume), pStock->GetStrongBuyVolume());
    EXPECT_EQ(atol(setDayLineToday.m_StrongSellVolume), pStock->GetStrongSellVolume());
    EXPECT_EQ(atol(setDayLineToday.m_UnknownVolume), pStock->GetUnknownVolume());

    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumber), pStock->GetTransactionNumber());
    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumberAbove200000), pStock->GetTransactionNumberAbove200000());
    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumberBelow200000), pStock->GetTransactionNumberBelow200000());
    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumberBelow50000), pStock->GetTransactionNumberBelow50000());
    EXPECT_EQ(atol(setDayLineToday.m_TransactionNumberBelow5000), pStock->GetTransactionNumberBelow5000());
    setDayLineToday.Close();
  }

  TEST_F(CStockTest, TestSaveTodayInfo) {
    CSetDayLine setDayLine;
    CSetDayLineInfo setDayLineInfo;
    CStockPtr pStock = make_shared<CStock>();
    long lDay = 21090101;
    time_t tt = FormatToTTime(lDay);
    CString strDay = _T("21090101"); // 最好设置此日期为未来，以防止误操作实际数据库

    pStock->SetMarket(__SHANGHAI_MARKET__);
    pStock->SetStockCode(_T("sh600000"));
    pStock->SetTransactionTime(tt); // 此处设置固定的日期，而存储时使用的是当前日期，故而需要与gl_systemTime.FormatToDay()作比较
    pStock->SetLastClose(101010);
    pStock->SetOpen(202020);
    pStock->SetHigh(303030);
    pStock->SetLow(404040);
    pStock->SetNew(505050);
    pStock->SetVolume(606060606060);
    pStock->SetAmount(707070707070707);
    pStock->SetUpDown(pStock->GetOpen() - pStock->GetNew());
    pStock->SetUpDownRate((double)pStock->GetUpDown() / pStock->GetLastClose() * 100);
    pStock->SetCurrentValue(8080808080808);
    pStock->SetTotalValue(9090909090909);

    pStock->SetTransactionNumber(123435);
    pStock->SetTransactionNumberBelow5000(45346456);
    pStock->SetTransactionNumberBelow50000(5698);
    pStock->SetTransactionNumberBelow200000(67687);
    pStock->SetTransactionNumberAbove200000(6876);
    pStock->SetOrdinaryBuyVolume(435);
    pStock->SetOrdinarySellVolume(560985);
    pStock->SetAttackBuyBelow50000(54509);
    pStock->SetAttackBuyBelow200000(45049);
    pStock->SetAttackBuyAbove200000(34508);
    pStock->SetAttackSellBelow50000(45896);
    pStock->SetAttackSellBelow200000(56457);
    pStock->SetAttackSellAbove200000(8767);
    pStock->SetAttackBuyAmount(123456678);
    pStock->SetAttackBuyVolume(23423534);
    pStock->SetAttackSellAmount(4353454);
    pStock->SetAttackSellVolume(94589489);
    pStock->SetCancelBuyVolume(435245);
    pStock->SetCancelSellVolume(45648698);
    pStock->SetUnknownVolume(4895747);
    pStock->SetStrongBuyVolume(453456);
    pStock->SetStrongSellVolume(98976);

    ASSERT(!gl_fNormalMode);
    ASSERT(gl_fTestMode);
    setDayLine.m_strFilter = _T("[Day] =");
    setDayLine.m_strFilter += strDay;
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    setDayLine.AddNew();
    pStock->SaveBasicInfo(setDayLine);
    setDayLine.Update();
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();

    setDayLineInfo.m_strFilter = _T("[Day] =");
    setDayLineInfo.m_strFilter += strDay;
    setDayLineInfo.Open();
    setDayLineInfo.m_pDatabase->BeginTrans();
    setDayLineInfo.AddNew();
    pStock->SaveCalculatedInfo(setDayLineInfo);
    setDayLineInfo.Update();
    setDayLineInfo.m_pDatabase->CommitTrans();
    setDayLineInfo.Close();

    setDayLine.m_strFilter = _T("[Day] =");
    setDayLine.m_strFilter += strDay;
    setDayLine.Open();
    EXPECT_EQ(setDayLine.m_Day, FormatToDay(pStock->GetTransactionTime()));
    EXPECT_EQ(setDayLine.m_Market, pStock->GetMarket());
    EXPECT_STREQ(setDayLine.m_StockCode, pStock->GetStockCode());
    EXPECT_DOUBLE_EQ(atof(setDayLine.m_LastClose), (double)pStock->GetLastClose() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLine.m_Open), (double)pStock->GetOpen() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLine.m_High), (double)pStock->GetHigh() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLine.m_Low), (double)pStock->GetLow() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLine.m_Close), (double)pStock->GetNew() / 1000);
    EXPECT_EQ(atoll(setDayLine.m_Volume), pStock->GetVolume());
    EXPECT_EQ(atoll(setDayLine.m_Amount), pStock->GetAmount());
    EXPECT_EQ(atof(setDayLine.m_UpAndDown), (double)pStock->GetUpDown() / 1000);
    EXPECT_DOUBLE_EQ(atof(setDayLine.m_UpDownRate), pStock->GetUpDownRate());
    EXPECT_EQ(atoll(setDayLine.m_CurrentValue), pStock->GetCurrentValue());
    EXPECT_EQ(atoll(setDayLine.m_TotalValue), pStock->GetTotalValue());
    setDayLine.Close();
    setDayLine.m_strFilter = _T("[Day] =");
    setDayLine.m_strFilter += strDay;
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    while (!setDayLine.IsEOF()) {
      setDayLine.Delete();
      setDayLine.MoveNext();
    }
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();

    setDayLineInfo.m_strFilter = _T("[Day] =");
    setDayLineInfo.m_strFilter += strDay;
    setDayLineInfo.Open();
    EXPECT_EQ(setDayLineInfo.m_Day, FormatToDay(pStock->GetTransactionTime()));
    EXPECT_EQ(setDayLineInfo.m_Market, pStock->GetMarket());
    EXPECT_STREQ(setDayLineInfo.m_StockCode, pStock->GetStockCode());
    EXPECT_EQ(atol(setDayLineInfo.m_AttackBuyAbove200000), pStock->GetAttackBuyAbove200000());
    EXPECT_EQ(atol(setDayLineInfo.m_AttackBuyBelow200000), pStock->GetAttackBuyBelow200000());
    EXPECT_EQ(atol(setDayLineInfo.m_AttackBuyBelow50000), pStock->GetAttackBuyBelow50000());
    EXPECT_EQ(atol(setDayLineInfo.m_AttackBuyVolume), pStock->GetAttackBuyVolume());
    EXPECT_EQ(atol(setDayLineInfo.m_AttackSellAbove200000), pStock->GetAttackSellAbove200000());
    EXPECT_EQ(atol(setDayLineInfo.m_AttackSellBelow200000), pStock->GetAttackSellBelow200000());
    EXPECT_EQ(atol(setDayLineInfo.m_AttackSellBelow50000), pStock->GetAttackSellBelow50000());
    EXPECT_EQ(atol(setDayLineInfo.m_AttackSellVolume), pStock->GetAttackSellVolume());

    EXPECT_EQ(atol(setDayLineInfo.m_OrdinaryBuyVolume), pStock->GetOrdinaryBuyVolume());
    EXPECT_EQ(atol(setDayLineInfo.m_OrdinarySellVolume), pStock->GetOrdinarySellVolume());
    EXPECT_EQ(atol(setDayLineInfo.m_CancelBuyVolume), pStock->GetCancelBuyVolume());
    EXPECT_EQ(atol(setDayLineInfo.m_CancelSellVolume), pStock->GetCancelSellVolume());
    EXPECT_EQ(atol(setDayLineInfo.m_StrongBuyVolume), pStock->GetStrongBuyVolume());
    EXPECT_EQ(atol(setDayLineInfo.m_StrongSellVolume), pStock->GetStrongSellVolume());
    EXPECT_EQ(atol(setDayLineInfo.m_UnknownVolume), pStock->GetUnknownVolume());

    EXPECT_EQ(atol(setDayLineInfo.m_TransactionNumber), pStock->GetTransactionNumber());
    EXPECT_EQ(atol(setDayLineInfo.m_TransactionNumberAbove200000), pStock->GetTransactionNumberAbove200000());
    EXPECT_EQ(atol(setDayLineInfo.m_TransactionNumberBelow200000), pStock->GetTransactionNumberBelow200000());
    EXPECT_EQ(atol(setDayLineInfo.m_TransactionNumberBelow50000), pStock->GetTransactionNumberBelow50000());
    EXPECT_EQ(atol(setDayLineInfo.m_TransactionNumberBelow5000), pStock->GetTransactionNumberBelow5000());
    setDayLineInfo.Close();
    setDayLineInfo.m_strFilter = _T("[Day] =");
    setDayLineInfo.m_strFilter += strDay;
    setDayLineInfo.Open();
    setDayLineInfo.m_pDatabase->BeginTrans();
    while (!setDayLineInfo.IsEOF()) {
      setDayLineInfo.Delete();
      setDayLineInfo.MoveNext();
    }
    setDayLineInfo.m_pDatabase->CommitTrans();
    setDayLineInfo.Close();
  }

  TEST_F(CStockTest, TestSaveDayLine) {
    CSetDayLine setDayLine;
    CDayLinePtr pid;
    CDayLine id;
    CStockPtr pStock = gl_ChinaStockMarket.GetStockPtr(_T("sh600008"));
    EXPECT_FALSE(gl_ChinaStockMarket.IsUpdateStockCodeDB());
    gl_systemTime.__Test_SetDay(20190101);

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDay(21101201);
      pid->SetMarket(__SHANGHAI_MARKET__);
      pid->SetStockCode(_T("sh600008"));
      pid->SetStockName(_T("首创股份"));
      pid->SetLastClose(34235345);
      pid->SetOpen(1000000 + i);
      pid->SetHigh(45234543);
      pid->SetLow(3452345);
      pid->SetClose(452435);
      pid->SetVolume(34523454);
      pid->SetAmount(3245235345);
      pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
      pid->SetUpDownRate(123.45);
      pid->SetTotalValue(234523452345);
      pid->SetCurrentValue(234145345245);
      pid->SetChangeHandRate(54.321);
      pid->SetRelativeStrong(14.5);
      pStock->PushDayLinePtr(pid);
    }
    pStock->SetDayLineEndDay(10190101);
    pStock->SetStockCode(_T("sh600008"));
    ASSERT(!gl_fNormalMode);
    pStock->SaveDayLine();
    EXPECT_EQ(pStock->GetDayLineEndDay(), 21101201);
    EXPECT_TRUE(gl_ChinaStockMarket.IsUpdateStockCodeDB());

    setDayLine.m_strFilter = _T("[Day] = 21101201");
    setDayLine.Open();
    for (int i = 0; i < 10; i++) {
      id.LoadData(setDayLine);
      pid = pStock->GetDayLinePtr(i);
      EXPECT_EQ(setDayLine.m_Day, pid->GetDay());
      EXPECT_EQ(setDayLine.m_Market, pid->GetMarket());
      EXPECT_STREQ(setDayLine.m_StockCode, pid->GetStockCode());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_LastClose) * 1000, pid->GetLastClose());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_Open) * 1000, pid->GetOpen());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_High) * 1000, pid->GetHigh());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_Low) * 1000, pid->GetLow());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_Close) * 1000, pid->GetClose());
      EXPECT_EQ(atoll(setDayLine.m_Volume), pid->GetVolume());
      EXPECT_EQ(atoll(setDayLine.m_Amount), pid->GetAmount());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_UpAndDown), pid->GetUpDown());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_UpDownRate), pid->GetUpDownRate());
      EXPECT_EQ(atoll(setDayLine.m_TotalValue), pid->GetTotalValue());
      EXPECT_EQ(atoll(setDayLine.m_CurrentValue), pid->GetCurrentValue());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_ChangeHandRate), pid->GetChangeHandRate());
      EXPECT_DOUBLE_EQ(atof(setDayLine.m_RelativeStrong), pid->GetRelativeStrong());
      setDayLine.MoveNext();
    }
    setDayLine.Close();

    setDayLine.m_strFilter = _T("[Day] = 21101201");
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    while (!setDayLine.IsEOF()) {
      setDayLine.Delete();
      setDayLine.MoveNext();
    }
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();
  }

  TEST_F(CStockTest, TestLoadDayLine) {
    CSetDayLine setDayLine;
    CDayLinePtr pid;
    CDayLinePtr pDayLine = nullptr;
    CStock id;
    CStockPtr pStock = gl_ChinaStockMarket.GetStockPtr(_T("sh600008"));

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDay(21101201);
      pid->SetMarket(__SHANGHAI_MARKET__);
      pid->SetStockCode(_T("sh600008"));
      pid->SetStockName(_T("首创股份"));
      pid->SetLastClose(34235345);
      pid->SetOpen(1000000 + i);
      pid->SetHigh(45234543);
      pid->SetLow(3452345);
      pid->SetClose(452435);
      pid->SetVolume(34523454);
      pid->SetAmount(3245235345);
      pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
      pid->SetUpDownRate(123.45);
      pid->SetTotalValue(234523452345);
      pid->SetCurrentValue(234145345245);
      pid->SetChangeHandRate(54.321);
      pid->SetRelativeStrong(14.5);
      pStock->PushDayLinePtr(pid);
    }
    pStock->SetStockCode(_T("sh600008"));
    pStock->SetDayLineEndDay(10190101);
    ASSERT(!gl_fNormalMode);
    pStock->SaveDayLine();
    EXPECT_EQ(pStock->GetDayLineEndDay(), 21101201);

    setDayLine.m_strFilter = _T("[Day] = 21101201");
    setDayLine.Open();
    id.LoadDayLine(setDayLine);
    for (int i = 0; i < 10; i++) {
      pid = id.GetDayLinePtr(i);
      pDayLine = pStock->GetDayLinePtr(i);
      EXPECT_EQ(pDayLine->GetDay(), pid->GetDay());
      EXPECT_EQ(pDayLine->GetMarket(), pid->GetMarket());
      EXPECT_STREQ(pDayLine->GetStockCode(), pid->GetStockCode());
      EXPECT_EQ(pDayLine->GetLastClose(), pid->GetLastClose());
      EXPECT_EQ(pDayLine->GetOpen(), pid->GetOpen());
      EXPECT_EQ(pDayLine->GetHigh(), pid->GetHigh());
      EXPECT_EQ(pDayLine->GetLow(), pid->GetLow());
      EXPECT_EQ(pDayLine->GetClose(), pid->GetClose());
      EXPECT_EQ(pDayLine->GetVolume(), pid->GetVolume());
      EXPECT_EQ(pDayLine->GetAmount(), pid->GetAmount());
      EXPECT_DOUBLE_EQ(pDayLine->GetUpDown(), pid->GetUpDown());
      EXPECT_DOUBLE_EQ(pDayLine->GetUpDownRate(), pid->GetUpDownRate());
      EXPECT_EQ(pDayLine->GetTotalValue(), pid->GetTotalValue());
      EXPECT_EQ(pDayLine->GetCurrentValue(), pid->GetCurrentValue());
      EXPECT_DOUBLE_EQ(pDayLine->GetChangeHandRate(), pid->GetChangeHandRate());
      EXPECT_DOUBLE_EQ(pDayLine->GetRelativeStrong(), pid->GetRelativeStrong());
    }
    setDayLine.Close();

    setDayLine.m_strFilter = _T("[Day] = 21101201");
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    while (!setDayLine.IsEOF()) {
      setDayLine.Delete();
      setDayLine.MoveNext();
    }
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();
  }

  TEST_F(CStockTest, TestUpdateDayLineStartEndDay) {
    CDayLinePtr pid;
    CStock id;
    CStockPtr pStock = gl_ChinaStockMarket.GetStockPtr(_T("sh600008"));

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDay(__CHINA_MARKET_BEGIN_DAY__ + i * 100000);
      pid->SetMarket(__SHANGHAI_MARKET__);
      pid->SetStockCode(_T("sh600008"));
      pid->SetStockName(_T("首创股份"));
      pid->SetLastClose(34235345);
      pid->SetOpen(1000000 + i);
      pid->SetHigh(45234543);
      pid->SetLow(3452345);
      pid->SetClose(452435);
      pid->SetVolume(34523454);
      pid->SetAmount(3245235345);
      pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
      pid->SetUpDownRate(123.45);
      pid->SetTotalValue(234523452345);
      pid->SetCurrentValue(234145345245);
      pid->SetChangeHandRate(54.321);
      pid->SetRelativeStrong(14.5);
      pStock->PushDayLinePtr(pid);
    }
    pStock->SetStockCode(_T("sh600008"));
    pStock->SetDayLineStartDay(19900102);
    pStock->SetDayLineEndDay(20800100);
    ASSERT(!gl_fNormalMode);
    pStock->UpdateDayLineStartEndDay();
    EXPECT_EQ(pStock->GetDayLineEndDay(), __CHINA_MARKET_BEGIN_DAY__ + 9 * 100000);
    EXPECT_EQ(pStock->GetDayLineStartDay(), 19900102);
    EXPECT_TRUE(gl_ChinaStockMarket.IsUpdateStockCodeDB());
  }

  TEST_F(CStockTest, TestUpdateDayLineStartEndDay2) {
    CDayLinePtr pid;
    CStock id;
    CStockPtr pStock = gl_ChinaStockMarket.GetStockPtr(_T("sh600008"));

    for (int i = 0; i < 10; i++) {
      pid = make_shared<CDayLine>();
      pid->SetDay(__CHINA_MARKET_BEGIN_DAY__ + i * 100000);
      pid->SetMarket(__SHANGHAI_MARKET__);
      pid->SetStockCode(_T("sh600008"));
      pid->SetStockName(_T("首创股份"));
      pid->SetLastClose(34235345);
      pid->SetOpen(1000000 + i);
      pid->SetHigh(45234543);
      pid->SetLow(3452345);
      pid->SetClose(452435);
      pid->SetVolume(34523454);
      pid->SetAmount(3245235345);
      pid->SetUpDown(((double)pid->GetClose() - pid->GetLastClose()) / 1000);
      pid->SetUpDownRate(123.45);
      pid->SetTotalValue(234523452345);
      pid->SetCurrentValue(234145345245);
      pid->SetChangeHandRate(54.321);
      pid->SetRelativeStrong(14.5);
      pStock->PushDayLinePtr(pid);
    }
    pStock->SetStockCode(_T("sh600008"));
    pStock->SetDayLineStartDay(19900100);
    pStock->SetDayLineEndDay(20800102);
    ASSERT(!gl_fNormalMode);
    pStock->UpdateDayLineStartEndDay();
    EXPECT_EQ(pStock->GetDayLineEndDay(), 20800102);
    EXPECT_EQ(pStock->GetDayLineStartDay(), 19900100);
    EXPECT_TRUE(gl_ChinaStockMarket.IsUpdateStockCodeDB());
  }

  TEST_F(CStockTest, TestSetTodayActive) {
    CStockPtr pStock = gl_ChinaStockMarket.GetStockPtr(_T("sh600001")); // 这个股票退市了，故而可以作为测试对象
    EXPECT_FALSE(pStock->IsActive());
    CString strStockName = pStock->GetStockName();
    WORD wMarket = pStock->GetMarket();
    long lTotalActiveStock = gl_ChinaStockMarket.GetTotalActiveStock();
    pStock->SetTodayActive(__SHENZHEN_MARKET__, _T("sh600002"), _T("梨园"));
    EXPECT_TRUE(pStock->IsActive());
    EXPECT_FALSE(pStock->IsDayLineLoaded());
    EXPECT_EQ(pStock->GetMarket(), __SHENZHEN_MARKET__);
    EXPECT_STREQ(pStock->GetStockCode(), _T("sh600002"));
    EXPECT_STREQ(pStock->GetStockName(), _T("梨园"));
    EXPECT_EQ(gl_ChinaStockMarket.GetTotalActiveStock(), lTotalActiveStock + 1);

    // clearup
    pStock->SetActive(false);
    pStock->SetMarket(wMarket);
    pStock->SetStockCode(_T("sh600001"));
    pStock->SetStockName(strStockName);
    gl_ChinaStockMarket.SetTotalActiveStock(lTotalActiveStock);
  }

  TEST_F(CStockTest, TestIncreaseCurrentPos) {
    CStock id;
    INT64 l = id.GetCurrentPos();
    char* p = id.GetCurrentPosPtr();
    id.IncreaseCurrentPos();
    EXPECT_EQ(l + 1, id.GetCurrentPos());
    EXPECT_EQ(p + 1, id.GetCurrentPosPtr());
    id.IncreaseCurrentPos(10);
    EXPECT_EQ(l + 11, id.GetCurrentPos());
    EXPECT_EQ(p + 11, id.GetCurrentPosPtr());
    id.ResetCurrentPos();
    EXPECT_EQ(l, id.GetCurrentPos());
    EXPECT_EQ(p, id.GetCurrentPosPtr());
    EXPECT_EQ(p, id.GetDayLineBufferPtr());
  }

  TEST_F(CStockTest, TestSkipNeteaseDayLineFirstInformationLine) {
    CStock id;
    CString str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r\n");
    id.__TestSetDayLineBuffer(str.GetLength(), str.GetBuffer());
    id.ResetCurrentPos();
    EXPECT_TRUE(id.SkipNeteaseDayLineInformationHeader());
    str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\n"); // 缺少\r
    id.__TestSetDayLineBuffer(str.GetLength(), str.GetBuffer());
    id.ResetCurrentPos();
    EXPECT_FALSE(id.SkipNeteaseDayLineInformationHeader());
    str = _T("日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,换手率,成交量,成交金额,总市值,流通市值\r"); // 缺少\n
    id.__TestSetDayLineBuffer(str.GetLength(), str.GetBuffer());
    id.ResetCurrentPos();
    EXPECT_FALSE(id.SkipNeteaseDayLineInformationHeader());
  }

  TEST_F(CStockTest, TestIsVolumeConsisitence) {
    CStock id;
    EXPECT_TRUE(id.IsVolumeConsistence());
    id.SetVolume(10000);
    EXPECT_FALSE(id.IsVolumeConsistence());
    id.SetUnknownVolume(10000);
    EXPECT_TRUE(id.IsVolumeConsistence());
  }
}