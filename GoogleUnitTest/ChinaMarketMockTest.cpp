///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试ChinaMarket的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"Thread.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"

#include"SetStockCode.h"
#include"WebInquirer.h"

#include"MockNeteaseDayLineWebInquiry.h"
#include"MockChinaMarket.h"
using namespace testing;

using namespace std;
#include<memory>

namespace StockAnalysisTest {
  class CChinaMarketMockTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      ASSERT_FALSE(gl_fNormalMode);
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), 12000);
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
    }
    static void TearDownTestSuite(void) {
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetStockCode();
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
      while (gl_WebInquirer.IsReadingWebThreadRunning()) Sleep(1);
      gl_pChinaStockMarket->SetDayLineNeedSaveNumber(0);
    }
    virtual void SetUp(void) override {
      chinaMarket.CalculateTime();
      chinaMarket.ResetNeteaseRTDataInquiringIndex();
      chinaMarket.ResetNeteaseDayLineDataInquiringIndex();
      chinaMarket.ResetSinaRTDataInquiringIndex();
      chinaMarket.ResetTengxunRTDataInquiringIndex();
      chinaMarket.SetSystemReady(true); // 测试市场时，默认系统已经准备好
      chinaMarket.SetPermitResetMarket(true);
      chinaMarket.SetCheckActiveStock(true);
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), 12000);

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      gl_ThreadStatus.SetSavingTempData(false);
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), 12000);

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    CMockChinaMarket chinaMarket;
  };

  CMockChinaMarket* s_pchinaMarket;
  class CChinaMarketMockTest2 : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      ASSERT_FALSE(gl_fNormalMode);
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), 12000);
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
      s_pchinaMarket = new CMockChinaMarket;
    }
    static void TearDownTestSuite(void) {
      delete s_pchinaMarket;
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetStockCode();
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
      while (gl_WebInquirer.IsReadingWebThreadRunning()) Sleep(1);
      gl_pChinaStockMarket->SetDayLineNeedSaveNumber(0);
    }
    virtual void SetUp(void) override {
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), 12000);

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      gl_ThreadStatus.SetSavingTempData(false);
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), 12000);

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }
  };

  TEST_F(CChinaMarketMockTest2, TestSaveDayLineData1) {
    CChinaStockPtr pStock = s_pchinaMarket->GetStock(_T("sh600000"));
    EXPECT_FALSE(pStock->IsDayLineNeedSaving());
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveDayLineOfOneStock(_))
      .Times(0);
    s_pchinaMarket->SaveDayLineData();
  }

  TEST_F(CChinaMarketMockTest2, TestSaveDayLineData2) {
    CChinaStockPtr pStock = s_pchinaMarket->GetStock(_T("sh600000"));

    EXPECT_FALSE(pStock->IsDayLineNeedSaving());
    pStock->SetDayLineNeedSaving(true);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveDayLineOfOneStock(_))
      .Times(0);
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedSaveNumber(), 1);
    s_pchinaMarket->SaveDayLineData();
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedSaveNumber(), 0);
  }

  TEST_F(CChinaMarketMockTest2, TestSaveDayLineData3) {
    CChinaStockPtr pStock = s_pchinaMarket->GetStock(_T("sh600000"));

    EXPECT_FALSE(pStock->IsDayLineNeedSaving());
    pStock->SetDayLineNeedSaving(true);
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDay(19900101);
    pStock->SetDayLineEndDay(20000101);
    pStock->StoreDayLine(pDayLine);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveDayLineOfOneStock(_))
      .Times(0);
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedSaveNumber(), 1);
    s_pchinaMarket->SaveDayLineData();
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedSaveNumber(), 0);

    pStock->UnloadDayLine();
  }

  TEST_F(CChinaMarketMockTest2, TestSaveDayLineData4) {
    CChinaStockPtr pStock = s_pchinaMarket->GetStock(_T("sh600000"));

    pStock->SetDayLineNeedSaving(true);
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDay(19900101);
    pStock->SetDayLineEndDay(20000101);
    pStock->StoreDayLine(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDay(s_pchinaMarket->GetDay());
    pStock->StoreDayLine(pDayLine);
    //pStock->SetDayLineNeedSaving(true);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveDayLineOfOneStock(_))
      .Times(1);
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedSaveNumber(), 1);
    EXPECT_TRUE(s_pchinaMarket->SaveDayLineData());
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedSaveNumber(), 0);
    pStock->UnloadDayLine();
  }

  TEST_F(CChinaMarketMockTest2, TestUpdateTempRTData) {
    gl_ThreadStatus.SetSavingTempData(true);
    EXPECT_FALSE(s_pchinaMarket->UpdateTempRTData());
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    EXPECT_TRUE(s_pchinaMarket->UpdateTempRTData());
    EXPECT_TRUE(gl_ThreadStatus.IsSavingTempData());
    gl_ThreadStatus.SetSavingTempData(true);
  }

  TEST_F(CChinaMarketMockTest2, TestTaskLoadCurrentStockDayLine1) {
    s_pchinaMarket->ResetCurrentStock();
    EXPECT_CALL(*s_pchinaMarket, RunningThreadLoadDayLine(_))
      .Times(0);
    EXPECT_TRUE(s_pchinaMarket->TaskLoadCurrentStockDayLine());
  }

  TEST_F(CChinaMarketMockTest2, TestTaskLoadCurrentStockDayLine2) {
    s_pchinaMarket->ResetCurrentStock();
    s_pchinaMarket->SetCurrentStock(_T("sh600000"));
    s_pchinaMarket->GetCurrentStock()->SetDayLineLoaded(true);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadLoadDayLine(_))
      .Times(0);
    EXPECT_TRUE(s_pchinaMarket->TaskLoadCurrentStockDayLine());
  }

  TEST_F(CChinaMarketMockTest2, TestTaskLoadCurrentStockDayLine3) {
    s_pchinaMarket->ResetCurrentStock();
    s_pchinaMarket->SetCurrentStock(_T("sh600000"));
    s_pchinaMarket->GetCurrentStock()->SetDayLineLoaded(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadLoadDayLine(_))
      .Times(1);
    EXPECT_TRUE(s_pchinaMarket->TaskLoadCurrentStockDayLine());
  }

  TEST_F(CChinaMarketMockTest2, TestTaskUpdateStockCodeDB) {
    s_pchinaMarket->SetUpdateStockCodeDB(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadUpdateStockCodeDB())
      .Times(0);
    EXPECT_FALSE(s_pchinaMarket->TaskUpdateStockCodeDB());
    s_pchinaMarket->SetUpdateStockCodeDB(true);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadUpdateStockCodeDB())
      .Times(1);
    EXPECT_TRUE(s_pchinaMarket->TaskUpdateStockCodeDB());
    EXPECT_FALSE(s_pchinaMarket->IsUpdateStockCodeDB());
  }

  TEST_F(CChinaMarketMockTest2, TestTaskUpdateOptionDB) {
    s_pchinaMarket->SetUpdateOptionDB(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadUpdateOptionDB())
      .Times(0);
    EXPECT_FALSE(s_pchinaMarket->TaskUpdateOptionDB());
    s_pchinaMarket->SetUpdateOptionDB(true);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadUpdateOptionDB())
      .Times(1);
    EXPECT_TRUE(s_pchinaMarket->TaskUpdateOptionDB());
    EXPECT_FALSE(s_pchinaMarket->IsUpdateOptionDB());
  }

  TEST_F(CChinaMarketMockTest2, TestTaskUpdateChoicedStockDB) {
    s_pchinaMarket->SetUpdateChoicedStockDB(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadAppendChoicedStockDB())
      .Times(0);
    EXPECT_FALSE(s_pchinaMarket->TaskUpdateChoicedStockDB());
    s_pchinaMarket->SetUpdateChoicedStockDB(true);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadAppendChoicedStockDB())
      .Times(1);
    EXPECT_TRUE(s_pchinaMarket->TaskUpdateChoicedStockDB());
    EXPECT_TRUE(s_pchinaMarket->IsUpdateChoicedStockDB()) << _T("自选股票更改标识设置后，就再也不重置，这样能够保证系统退出时再次更新数据库");
  }

  TEST_F(CChinaMarketMockTest2, TestTaskSaveChoicedRTData) {
    s_pchinaMarket->SetSystemReady(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveChoicedRTData())
      .Times(0);
    EXPECT_FALSE(s_pchinaMarket->TaskSaveChoicedRTData());
    s_pchinaMarket->SetSystemReady(true);
    s_pchinaMarket->SetRecordRTData(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveChoicedRTData())
      .Times(0);
    EXPECT_FALSE(s_pchinaMarket->TaskSaveChoicedRTData());
    s_pchinaMarket->SetRecordRTData(true);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveChoicedRTData())
      .Times(1);
    EXPECT_TRUE(s_pchinaMarket->TaskSaveChoicedRTData());
    s_pchinaMarket->SetRecordRTData(false);
    s_pchinaMarket->SetSystemReady(false);
  }

  TEST_F(CChinaMarketMockTest2, TestTaskSaveTempDataIntoDB) {
    s_pchinaMarket->SetSystemReady(true);
    s_pchinaMarket->SetMarketOpened(true);
    gl_ThreadStatus.SetCalculatingRTData(false);
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    s_pchinaMarket->TaskSaveTempDataIntoDB(93001);
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    s_pchinaMarket->TaskSaveTempDataIntoDB(113559);
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    s_pchinaMarket->TaskSaveTempDataIntoDB(130001);
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    s_pchinaMarket->TaskSaveTempDataIntoDB(150559);
  }

  TEST_F(CChinaMarketMockTest2, TestTaskChoice10RSStrong2StockSet) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    s_pchinaMarket->__TEST_SetMarketTM(tm_);
    s_pchinaMarket->SetSystemReady(false);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong2StockSet(151000));
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong2StockSet(151001));
    s_pchinaMarket->SetSystemReady(true);
    s_pchinaMarket->SetChoiced10RSStrong2StockSet(true);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong2StockSet(151000));
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong2StockSet(151001));
    s_pchinaMarket->SetSystemReady(true);
    s_pchinaMarket->SetChoiced10RSStrong2StockSet(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadChoice10RSStrong2StockSet)
      .Times(1);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong2StockSet(151000));
    EXPECT_TRUE(s_pchinaMarket->TaskChoice10RSStrong2StockSet(151001));
    EXPECT_TRUE(s_pchinaMarket->IsChoiced10RSStrong2StockSet());

    tm_.tm_wday = 0; // 星期日
    s_pchinaMarket->__TEST_SetMarketTM(tm_);
    s_pchinaMarket->SetTodayStockProcessed(false);
    s_pchinaMarket->SetChoiced10RSStrong2StockSet(false);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong2StockSet(151001));
    EXPECT_FALSE(s_pchinaMarket->IsChoiced10RSStrong2StockSet()) << _T("休息日不处理");
  }

  TEST_F(CChinaMarketMockTest2, TestTaskChoice10RSStrong1StockSet) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    s_pchinaMarket->__TEST_SetMarketTM(tm_);
    s_pchinaMarket->SetSystemReady(false);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong1StockSet(151000));
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong1StockSet(151001));
    s_pchinaMarket->SetSystemReady(true);
    s_pchinaMarket->SetChoiced10RSStrong1StockSet(true);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong1StockSet(151000));
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong1StockSet(151001));
    s_pchinaMarket->SetSystemReady(true);
    s_pchinaMarket->SetChoiced10RSStrong1StockSet(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadChoice10RSStrong1StockSet)
      .Times(1);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong1StockSet(151000));
    EXPECT_TRUE(s_pchinaMarket->TaskChoice10RSStrong1StockSet(151001));
    EXPECT_TRUE(s_pchinaMarket->IsChoiced10RSStrong1StockSet());

    tm_.tm_wday = 0; // 星期日
    s_pchinaMarket->__TEST_SetMarketTM(tm_);
    s_pchinaMarket->SetTodayStockProcessed(false);
    s_pchinaMarket->SetChoiced10RSStrong1StockSet(false);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong1StockSet(151001));
    EXPECT_FALSE(s_pchinaMarket->IsChoiced10RSStrong1StockSet()) << _T("休息日不处理");
  }

  TEST_F(CChinaMarketMockTest2, TestTaskChoice10RSStrongStockSet) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    s_pchinaMarket->__TEST_SetMarketTM(tm_);
    s_pchinaMarket->SetSystemReady(false);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrongStockSet(151000));
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrongStockSet(151001));
    s_pchinaMarket->SetSystemReady(true);
    s_pchinaMarket->SetChoiced10RSStrongStockSet(true);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrongStockSet(151000));
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrongStockSet(151001));
    s_pchinaMarket->SetSystemReady(true);
    s_pchinaMarket->SetChoiced10RSStrongStockSet(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadChoice10RSStrongStockSet)
      .Times(1);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrongStockSet(151000));
    EXPECT_TRUE(s_pchinaMarket->TaskChoice10RSStrongStockSet(151001));
    EXPECT_TRUE(s_pchinaMarket->IsChoiced10RSStrongStockSet());

    tm_.tm_wday = 0; // 星期日
    s_pchinaMarket->__TEST_SetMarketTM(tm_);
    s_pchinaMarket->SetTodayStockProcessed(false);
    s_pchinaMarket->SetChoiced10RSStrongStockSet(false);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrongStockSet(151001));
    EXPECT_FALSE(s_pchinaMarket->IsChoiced10RSStrongStockSet()) << _T("休息日不处理");
  }

  TEST_F(CChinaMarketMockTest2, TestProcessTodayStock) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    s_pchinaMarket->__TEST_SetMarketTM(tm_);
    s_pchinaMarket->SetSystemReady(false);
    s_pchinaMarket->SetTodayStockProcessed(false);
    EXPECT_FALSE(s_pchinaMarket->TaskProcessTodayStock(150359));
    EXPECT_FALSE(s_pchinaMarket->TaskProcessTodayStock(150400));
    EXPECT_FALSE(s_pchinaMarket->IsTodayStockProcessed());
    s_pchinaMarket->SetSystemReady(true);
    s_pchinaMarket->SetTodayStockProcessed(true);
    EXPECT_FALSE(s_pchinaMarket->TaskProcessTodayStock(150359));
    EXPECT_FALSE(s_pchinaMarket->TaskProcessTodayStock(150400));
    EXPECT_TRUE(s_pchinaMarket->IsTodayStockProcessed());
    s_pchinaMarket->SetSystemReady(true);
    s_pchinaMarket->SetTodayStockProcessed(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadProcessTodayStock)
      .Times(1);
    EXPECT_FALSE(s_pchinaMarket->TaskProcessTodayStock(150359));
    EXPECT_TRUE(s_pchinaMarket->TaskProcessTodayStock(150400));
    EXPECT_TRUE(s_pchinaMarket->IsTodayStockProcessed());

    tm_.tm_wday = 0; // 星期日
    s_pchinaMarket->__TEST_SetMarketTM(tm_);
    s_pchinaMarket->SetTodayStockProcessed(false);
    EXPECT_FALSE(s_pchinaMarket->TaskProcessTodayStock(150400));
    EXPECT_FALSE(s_pchinaMarket->IsTodayStockProcessed()) << _T("休息日不处理");
  }

  TEST_F(CChinaMarketMockTest2, TestThreadProcessTodayStock) {
    s_pchinaMarket->CalculateTime();
    s_pchinaMarket->SetNewestTransactionTime(s_pchinaMarket->GetLocalTime());
    long lDay = FormatToDay(s_pchinaMarket->GetNewestTransactionTime());
    EXPECT_CALL(*s_pchinaMarket, ProcessCurrentTradeDayStock(lDay))
      .Times(1)
      .WillOnce(Return(4000));
    EXPECT_CALL(*s_pchinaMarket, RunningThreadCalculateThisDayRS(lDay))
      .Times(1)
      .WillOnce(Return(true));
    s_pchinaMarket->SetSystemReady(true);
    EXPECT_EQ(ThreadProcessTodayStock(s_pchinaMarket), (UINT)14);
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 1);
    if (s_pchinaMarket->GetTime() > 150400) {
      EXPECT_EQ(s_pchinaMarket->GetRelativeStrongEndDay(), lDay);
      EXPECT_TRUE(s_pchinaMarket->IsUpdateStockCodeDB());
      EXPECT_TRUE(s_pchinaMarket->IsUpdateOptionDB());
      EXPECT_TRUE(s_pchinaMarket->IsTodayStockProcessed());
    }
  }

  TEST_F(CChinaMarketMockTest2, TestThreadCalculateDayLineRS) {
    s_pchinaMarket->CalculateTime();
    time_t tStart = s_pchinaMarket->GetMarketTime() - 3600 * 24 * 6; // 从一周前开始计算
    tm _tm;
    gmtime_s(&_tm, &tStart);
    long lStartDay = (_tm.tm_year + 1900) * 10000 + (_tm.tm_mon + 1) * 100 + _tm.tm_mday;
    gl_fExitingCalculatingRS = true; // 中间被打断
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    s_pchinaMarket->SetRelativeStrongEndDay(0);
    s_pchinaMarket->SetUpdateOptionDB(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadCalculateThisDayRS(_))
      .Times(5)
      .WillRepeatedly(Return(true));
    EXPECT_EQ(ThreadCalculateDayLineRS(s_pchinaMarket, lStartDay), (UINT)11);
    EXPECT_FALSE(s_pchinaMarket->IsUpdateOptionDB()) << _T("被打断后不设置此标识");
    EXPECT_EQ(s_pchinaMarket->GetRelativeStrongEndDay(), 0);
    EXPECT_FALSE(gl_fExitingCalculatingRS);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());

    tStart = s_pchinaMarket->GetMarketTime() - 3600 * 24 * 6; // 从一周前开始计算
    gmtime_s(&_tm, &tStart);
    lStartDay = (_tm.tm_year + 1900) * 10000 + (_tm.tm_mon + 1) * 100 + _tm.tm_mday;
    gl_fExitingCalculatingRS = false;
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadCalculateThisDayRS(_))
      .Times(5);
    EXPECT_EQ(ThreadCalculateDayLineRS(s_pchinaMarket, lStartDay), (UINT)11);
    EXPECT_TRUE(s_pchinaMarket->IsUpdateOptionDB());
    EXPECT_EQ(s_pchinaMarket->GetRelativeStrongEndDay(), s_pchinaMarket->GetDay());
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());
  }

  TEST_F(CChinaMarketMockTest2, TestThreadUpdateOptionDB) {
    EXPECT_CALL(*s_pchinaMarket, UpdateOptionDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateOptionDB(s_pchinaMarket), (UINT)20);
  }

  TEST_F(CChinaMarketMockTest2, TestThreadUpdateStockCodeDB) {
    EXPECT_CALL(*s_pchinaMarket, UpdateStockCodeDB)
      .Times(1);
    s_pchinaMarket->SetSystemReady(true);
    gl_ThreadStatus.SetCalculatingRTData(false);
    EXPECT_EQ(ThreadUpdateStockCodeDB(s_pchinaMarket), (UINT)18);
  }

  TEST_F(CChinaMarketMockTest2, TestThreadUpdateChoicedStockDB) {
    EXPECT_CALL(*s_pchinaMarket, AppendChoicedStockDB)
      .Times(1);
    s_pchinaMarket->SetSystemReady(true);
    EXPECT_EQ(ThreadAppendChoicedStockDB(s_pchinaMarket), (UINT)22);
  }

  TEST_F(CChinaMarketMockTest2, TestThreadSaveTempRTData) {
    EXPECT_CALL(*s_pchinaMarket, UpdateTodayTempDB)
      .Times(1);
    s_pchinaMarket->SetSystemReady(true);
    EXPECT_EQ(ThreadSaveTempRTData(s_pchinaMarket), (UINT)13);
  }

  TEST_F(CChinaMarketMockTest2, TestThreadSaveRTData) {
    EXPECT_CALL(*s_pchinaMarket, SaveRTData)
      .Times(1);
    s_pchinaMarket->SetSystemReady(true);
    EXPECT_EQ(ThreadSaveRTData(s_pchinaMarket), (UINT)19);
  }
}