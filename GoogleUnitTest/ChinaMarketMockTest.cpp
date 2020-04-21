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

  TEST_F(CChinaMarketMockTest, TestSaveDayLineData1) {
    CChinaStockPtr pStock = chinaMarket.GetStock(_T("sh600000"));
    EXPECT_FALSE(pStock->IsDayLineNeedSaving());
    EXPECT_CALL(chinaMarket, RunningThreadSaveDayLineOfOneStock(_))
      .Times(0);
    chinaMarket.SaveDayLineData();
  }

  TEST_F(CChinaMarketMockTest, TestSaveDayLineData2) {
    CChinaStockPtr pStock = chinaMarket.GetStock(_T("sh600000"));

    EXPECT_FALSE(pStock->IsDayLineNeedSaving());
    pStock->SetDayLineNeedSaving(true);
    EXPECT_CALL(chinaMarket, RunningThreadSaveDayLineOfOneStock(_))
      .Times(0);
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedSaveNumber(), 1);
    chinaMarket.SaveDayLineData();
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedSaveNumber(), 0);
  }

  TEST_F(CChinaMarketMockTest, TestSaveDayLineData3) {
    CChinaStockPtr pStock = chinaMarket.GetStock(_T("sh600000"));

    EXPECT_FALSE(pStock->IsDayLineNeedSaving());
    pStock->SetDayLineNeedSaving(true);
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDay(19900101);
    pStock->SetDayLineEndDay(20000101);
    pStock->StoreDayLine(pDayLine);
    EXPECT_CALL(chinaMarket, RunningThreadSaveDayLineOfOneStock(_))
      .Times(0);
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedSaveNumber(), 1);
    chinaMarket.SaveDayLineData();
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedSaveNumber(), 0);

    pStock->UnloadDayLine();
  }

  TEST_F(CChinaMarketMockTest, TestSaveDayLineData4) {
    CChinaStockPtr pStock = chinaMarket.GetStock(_T("sh600000"));

    pStock->SetDayLineNeedSaving(true);
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDay(19900101);
    pStock->SetDayLineEndDay(20000101);
    pStock->StoreDayLine(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDay(chinaMarket.GetDay());
    pStock->StoreDayLine(pDayLine);
    //pStock->SetDayLineNeedSaving(true);
    EXPECT_CALL(chinaMarket, RunningThreadSaveDayLineOfOneStock(_))
      .Times(1);
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedSaveNumber(), 1);
    EXPECT_TRUE(chinaMarket.SaveDayLineData());
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedSaveNumber(), 0);
    pStock->UnloadDayLine();
  }

  TEST_F(CChinaMarketMockTest, TestUpdateTempRTData) {
    gl_ThreadStatus.SetSavingTempData(true);
    EXPECT_FALSE(chinaMarket.UpdateTempRTData());
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(chinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    EXPECT_TRUE(chinaMarket.UpdateTempRTData());
    EXPECT_TRUE(gl_ThreadStatus.IsSavingTempData());
    gl_ThreadStatus.SetSavingTempData(true);
  }

  TEST_F(CChinaMarketMockTest, TestTaskLoadCurrentStockDayLine1) {
    chinaMarket.ResetCurrentStock();
    EXPECT_CALL(chinaMarket, RunningThreadLoadDayLine(_))
      .Times(0);
    EXPECT_TRUE(chinaMarket.TaskLoadCurrentStockDayLine());
  }

  TEST_F(CChinaMarketMockTest, TestTaskLoadCurrentStockDayLine2) {
    chinaMarket.ResetCurrentStock();
    chinaMarket.SetCurrentStock(_T("sh600000"));
    chinaMarket.GetCurrentStock()->SetDayLineLoaded(true);
    EXPECT_CALL(chinaMarket, RunningThreadLoadDayLine(_))
      .Times(0);
    EXPECT_TRUE(chinaMarket.TaskLoadCurrentStockDayLine());
  }

  TEST_F(CChinaMarketMockTest, TestTaskLoadCurrentStockDayLine3) {
    chinaMarket.ResetCurrentStock();
    chinaMarket.SetCurrentStock(_T("sh600000"));
    chinaMarket.GetCurrentStock()->SetDayLineLoaded(false);
    EXPECT_CALL(chinaMarket, RunningThreadLoadDayLine(_))
      .Times(1);
    EXPECT_TRUE(chinaMarket.TaskLoadCurrentStockDayLine());
  }

  TEST_F(CChinaMarketMockTest, TestTaskUpdateStockCodeDB) {
    chinaMarket.SetUpdateStockCodeDB(false);
    EXPECT_CALL(chinaMarket, RunningThreadUpdateStockCodeDB())
      .Times(0);
    EXPECT_FALSE(chinaMarket.TaskUpdateStockCodeDB());
    chinaMarket.SetUpdateStockCodeDB(true);
    EXPECT_CALL(chinaMarket, RunningThreadUpdateStockCodeDB())
      .Times(1);
    EXPECT_TRUE(chinaMarket.TaskUpdateStockCodeDB());
    EXPECT_FALSE(chinaMarket.IsUpdateStockCodeDB());
  }

  TEST_F(CChinaMarketMockTest, TestTaskUpdateOptionDB) {
    chinaMarket.SetUpdateOptionDB(false);
    EXPECT_CALL(chinaMarket, RunningThreadUpdateOptionDB())
      .Times(0);
    EXPECT_FALSE(chinaMarket.TaskUpdateOptionDB());
    chinaMarket.SetUpdateOptionDB(true);
    EXPECT_CALL(chinaMarket, RunningThreadUpdateOptionDB())
      .Times(1);
    EXPECT_TRUE(chinaMarket.TaskUpdateOptionDB());
    EXPECT_FALSE(chinaMarket.IsUpdateOptionDB());
  }

  TEST_F(CChinaMarketMockTest, TestTaskUpdateChoicedStockDB) {
    chinaMarket.SetUpdateChoicedStockDB(false);
    EXPECT_CALL(chinaMarket, RunningThreadAppendChoicedStockDB())
      .Times(0);
    EXPECT_FALSE(chinaMarket.TaskUpdateChoicedStockDB());
    chinaMarket.SetUpdateChoicedStockDB(true);
    EXPECT_CALL(chinaMarket, RunningThreadAppendChoicedStockDB())
      .Times(1);
    EXPECT_TRUE(chinaMarket.TaskUpdateChoicedStockDB());
    EXPECT_TRUE(chinaMarket.IsUpdateChoicedStockDB()) << _T("自选股票更改标识设置后，就再也不重置，这样能够保证系统退出时再次更新数据库");
  }

  TEST_F(CChinaMarketMockTest, TestTaskSaveChoicedRTData) {
    chinaMarket.SetSystemReady(false);
    EXPECT_CALL(chinaMarket, RunningThreadSaveChoicedRTData())
      .Times(0);
    EXPECT_FALSE(chinaMarket.TaskSaveChoicedRTData());
    chinaMarket.SetSystemReady(true);
    chinaMarket.SetRecordRTData(false);
    EXPECT_CALL(chinaMarket, RunningThreadSaveChoicedRTData())
      .Times(0);
    EXPECT_FALSE(chinaMarket.TaskSaveChoicedRTData());
    chinaMarket.SetRecordRTData(true);
    EXPECT_CALL(chinaMarket, RunningThreadSaveChoicedRTData())
      .Times(1);
    EXPECT_TRUE(chinaMarket.TaskSaveChoicedRTData());
    chinaMarket.SetRecordRTData(false);
    chinaMarket.SetSystemReady(false);
  }

  TEST_F(CChinaMarketMockTest, TestTaskSaveTempDataIntoDB) {
    chinaMarket.SetSystemReady(true);
    chinaMarket.SetMarketOpened(true);
    gl_ThreadStatus.SetCalculatingRTData(false);
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(chinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    chinaMarket.TaskSaveTempDataIntoDB(93001);
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(chinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    chinaMarket.TaskSaveTempDataIntoDB(113559);
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(chinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    chinaMarket.TaskSaveTempDataIntoDB(130001);
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(chinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    chinaMarket.TaskSaveTempDataIntoDB(150559);
  }

  TEST_F(CChinaMarketMockTest, TestTaskChoice10RSStrongStockSet) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    chinaMarket.__TEST_SetMarketTM(tm_);
    chinaMarket.SetSystemReady(false);
    EXPECT_FALSE(chinaMarket.TaskChoice10RSStrong2StockSet(151000));
    EXPECT_FALSE(chinaMarket.TaskChoice10RSStrong2StockSet(151001));
    chinaMarket.SetSystemReady(true);
    chinaMarket.SetChoiced10RSStrongStockSet(true);
    EXPECT_FALSE(chinaMarket.TaskChoice10RSStrong2StockSet(151000));
    EXPECT_FALSE(chinaMarket.TaskChoice10RSStrong2StockSet(151001));
    chinaMarket.SetSystemReady(true);
    chinaMarket.SetChoiced10RSStrongStockSet(false);
    EXPECT_CALL(chinaMarket, RunningThreadChoice10RSStrong2StockSet)
      .Times(1);
    EXPECT_FALSE(chinaMarket.TaskChoice10RSStrong2StockSet(151000));
    EXPECT_TRUE(chinaMarket.TaskChoice10RSStrong2StockSet(151001));
    EXPECT_TRUE(chinaMarket.IsChoiced10RSStrongStockSet());

    tm_.tm_wday = 0; // 星期日
    chinaMarket.__TEST_SetMarketTM(tm_);
    chinaMarket.SetTodayStockProcessed(false);
    chinaMarket.SetChoiced10RSStrongStockSet(false);
    EXPECT_FALSE(chinaMarket.TaskChoice10RSStrong2StockSet(151001));
    EXPECT_FALSE(chinaMarket.IsChoiced10RSStrongStockSet()) << _T("休息日不处理");
  }

  TEST_F(CChinaMarketMockTest, TestProcessTodayStock) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    chinaMarket.__TEST_SetMarketTM(tm_);
    chinaMarket.SetSystemReady(false);
    chinaMarket.SetTodayStockProcessed(false);
    EXPECT_FALSE(chinaMarket.TaskProcessTodayStock(150359));
    EXPECT_FALSE(chinaMarket.TaskProcessTodayStock(150400));
    EXPECT_FALSE(chinaMarket.IsTodayStockProcessed());
    chinaMarket.SetSystemReady(true);
    chinaMarket.SetTodayStockProcessed(true);
    EXPECT_FALSE(chinaMarket.TaskProcessTodayStock(150359));
    EXPECT_FALSE(chinaMarket.TaskProcessTodayStock(150400));
    EXPECT_TRUE(chinaMarket.IsTodayStockProcessed());
    chinaMarket.SetSystemReady(true);
    chinaMarket.SetTodayStockProcessed(false);
    EXPECT_CALL(chinaMarket, RunningThreadProcessTodayStock)
      .Times(1);
    EXPECT_FALSE(chinaMarket.TaskProcessTodayStock(150359));
    EXPECT_TRUE(chinaMarket.TaskProcessTodayStock(150400));
    EXPECT_TRUE(chinaMarket.IsTodayStockProcessed());

    tm_.tm_wday = 0; // 星期日
    chinaMarket.__TEST_SetMarketTM(tm_);
    chinaMarket.SetTodayStockProcessed(false);
    EXPECT_FALSE(chinaMarket.TaskProcessTodayStock(150400));
    EXPECT_FALSE(chinaMarket.IsTodayStockProcessed()) << _T("休息日不处理");
  }

  TEST_F(CChinaMarketMockTest, TestThreadUpdateOptionDB) {
    EXPECT_CALL(chinaMarket, UpdateOptionDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateOptionDB(&chinaMarket), (UINT)20);
  }

  TEST_F(CChinaMarketMockTest, TestThreadUpdateStockCodeDB) {
    EXPECT_CALL(chinaMarket, UpdateStockCodeDB)
      .Times(1);
    chinaMarket.SetSystemReady(true);
    gl_ThreadStatus.SetCalculatingRTData(false);
    EXPECT_EQ(ThreadUpdateStockCodeDB(&chinaMarket), (UINT)18);
  }

  TEST_F(CChinaMarketMockTest, TestThreadUpdateChoicedStockDB) {
    EXPECT_CALL(chinaMarket, AppendChoicedStockDB)
      .Times(1);
    chinaMarket.SetSystemReady(true);
    EXPECT_EQ(ThreadAppendChoicedStockDB(&chinaMarket), (UINT)22);
  }

  TEST_F(CChinaMarketMockTest, TestThreadSaveTempRTData) {
    EXPECT_CALL(chinaMarket, UpdateTodayTempDB)
      .Times(1);
    chinaMarket.SetSystemReady(true);
    EXPECT_EQ(ThreadSaveTempRTData(&chinaMarket), (UINT)13);
  }

  TEST_F(CChinaMarketMockTest, TestThreadSaveRTData) {
    EXPECT_CALL(chinaMarket, SaveRTData)
      .Times(1);
    chinaMarket.SetSystemReady(true);
    EXPECT_EQ(ThreadSaveRTData(&chinaMarket), (UINT)19);
  }

  TEST_F(CChinaMarketMockTest, TestThreadProcessTodayStock) {
    chinaMarket.CalculateTime();
    chinaMarket.SetNewestTransactionTime(chinaMarket.GetLocalTime());
    long lDay = FormatToDay(chinaMarket.GetNewestTransactionTime());
    EXPECT_CALL(chinaMarket, ProcessCurrentTradeDayStock(lDay))
      .Times(1)
      .WillOnce(Return(4000));
    EXPECT_CALL(chinaMarket, RunningThreadCalculateThisDayRS(lDay))
      .Times(1)
      .WillOnce(Return(true));
    chinaMarket.SetSystemReady(true);
    EXPECT_EQ(ThreadProcessTodayStock(&chinaMarket), (UINT)14);
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 1);
    if (chinaMarket.GetTime() > 150400) {
      EXPECT_EQ(chinaMarket.GetRelativeStrongEndDay(), lDay);
      EXPECT_TRUE(chinaMarket.IsUpdateStockCodeDB());
      EXPECT_TRUE(chinaMarket.IsUpdateOptionDB());
      EXPECT_TRUE(chinaMarket.IsTodayStockProcessed());
    }
  }

  TEST_F(CChinaMarketMockTest, TestThreadCalculateDayLineRS) {
    chinaMarket.CalculateTime();
    time_t tStart = chinaMarket.GetMarketTime() - 3600 * 24 * 6; // 从一周前开始计算
    tm _tm;
    gmtime_s(&_tm, &tStart);
    long lStartDay = (_tm.tm_year + 1900) * 10000 + (_tm.tm_mon + 1) * 100 + _tm.tm_mday;
    gl_fExitingCalculatingRS = true; // 中间被打断
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    chinaMarket.SetRelativeStrongEndDay(0);
    chinaMarket.SetUpdateOptionDB(false);
    EXPECT_CALL(chinaMarket, RunningThreadCalculateThisDayRS(_))
      .Times(5)
      .WillRepeatedly(Return(true));
    EXPECT_EQ(ThreadCalculateDayLineRS(&chinaMarket, lStartDay), (UINT)11);
    EXPECT_FALSE(chinaMarket.IsUpdateOptionDB()) << _T("被打断后不设置此标识");
    EXPECT_EQ(chinaMarket.GetRelativeStrongEndDay(), 0);
    EXPECT_FALSE(gl_fExitingCalculatingRS);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());

    tStart = chinaMarket.GetMarketTime() - 3600 * 24 * 6; // 从一周前开始计算
    gmtime_s(&_tm, &tStart);
    lStartDay = (_tm.tm_year + 1900) * 10000 + (_tm.tm_mon + 1) * 100 + _tm.tm_mday;
    gl_fExitingCalculatingRS = false;
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_CALL(chinaMarket, RunningThreadCalculateThisDayRS(_))
      .Times(5);
    EXPECT_EQ(ThreadCalculateDayLineRS(&chinaMarket, lStartDay), (UINT)11);
    EXPECT_TRUE(chinaMarket.IsUpdateOptionDB());
    EXPECT_EQ(chinaMarket.GetRelativeStrongEndDay(), chinaMarket.GetDay());
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());
  }
}