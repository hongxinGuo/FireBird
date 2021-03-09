///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试ChinaMarket的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"

#include"Thread.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"

#include"WebInquirer.h"

#include"MockNeteaseDayLineWebInquiry.h"
#include"MockChinaMarket.h"
using namespace std;
using namespace testing;
#include<memory>

namespace StockAnalysisTest {
  CMockChinaMarket* s_pchinaMarket;
  class CMockChinaMarketTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      ASSERT_FALSE(gl_fNormalMode);
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
      s_pchinaMarket = new CMockChinaMarket;
      s_pchinaMarket->LoadStockCodeDB();
      s_pchinaMarket->SetTodayStockProcessed(false);
      EXPECT_FALSE(s_pchinaMarket->IsDayLineNeedSaving());
      EXPECT_EQ(s_pchinaMarket->GetDayLineNeedSaveNumber(), 0);

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }
    static void TearDownTestSuite(void) {
      EXPECT_FALSE(s_pchinaMarket->IsDayLineNeedSaving());
      EXPECT_EQ(s_pchinaMarket->GetDayLineNeedSaveNumber(), 0);
      delete s_pchinaMarket;
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetSymbol();
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
    }
    virtual void SetUp(void) override {
      //EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedUpdateNumber(), gl_pChinaStockMarket->GetTotalStock());
      EXPECT_EQ(s_pchinaMarket->GetDayLineNeedSaveNumber(), 0);
      EXPECT_FALSE(gl_fExitingSystem);

      s_pchinaMarket->SetTodayStockProcessed(false);
      s_pchinaMarket->SetRSEndDate(19900101);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      EXPECT_EQ(s_pchinaMarket->GetDayLineNeedSaveNumber(), 0);
      CChinaStockPtr pStock;
      if (s_pchinaMarket->GetDayLineNeedSaveNumber() > 0) {
        for (int i = 0; i < s_pchinaMarket->GetTotalStock(); i++) {
          pStock = s_pchinaMarket->GetStock(i);
          if (pStock->IsDayLineNeedSaving()) {
            EXPECT_STREQ(pStock->GetSymbol(), _T(""));
          }
        }
      }
      s_pchinaMarket->SetTodayStockProcessed(false);
      s_pchinaMarket->SetRSEndDate(19900101);
      s_pchinaMarket->SetUpdateOptionDB(false);
      gl_ThreadStatus.SetSavingTempData(false);

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }
  };

  TEST_F(CMockChinaMarketTest, TestTaskSaveDayLineData1) {
    CChinaStockPtr pStock = s_pchinaMarket->GetStock(_T("600666.SS"));
    EXPECT_EQ(s_pchinaMarket->GetDayLineNeedSaveNumber(), 0);
    EXPECT_FALSE(pStock->IsDayLineNeedSaving());
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveDayLineBasicInfoOfStock(_))
      .Times(0);
    EXPECT_FALSE(s_pchinaMarket->TaskSaveDayLineData());
  }

  TEST_F(CMockChinaMarketTest, TestTaskSaveDayLineData2) {
    CChinaStockPtr pStock = s_pchinaMarket->GetStock(_T("600668.SS"));

    EXPECT_FALSE(pStock->IsDayLineNeedSaving());
    EXPECT_EQ(s_pchinaMarket->GetDayLineNeedSaveNumber(), 0);
    pStock->SetDayLineNeedSaving(true);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveDayLineBasicInfoOfStock(_))
      .Times(0);
    EXPECT_EQ(s_pchinaMarket->GetDayLineNeedSaveNumber(), 1);
    EXPECT_FALSE(s_pchinaMarket->TaskSaveDayLineData());
    EXPECT_EQ(s_pchinaMarket->GetDayLineNeedSaveNumber(), 0);
    pStock = s_pchinaMarket->GetStock(_T("600668.SS"));
    EXPECT_FALSE(pStock->IsDayLineNeedSaving()) << "无论执行与否皆清除此标识";
  }

  TEST_F(CMockChinaMarketTest, TestTaskSaveDayLineData3) {
    CChinaStockPtr pStock = s_pchinaMarket->GetStock(_T("600282.SS"));
    long lDate = pStock->GetDayLineEndDate();

    EXPECT_EQ(s_pchinaMarket->GetDayLineNeedSaveNumber(), 0);
    EXPECT_FALSE(pStock->IsDayLineNeedSaving());
    pStock->SetDayLineNeedSaving(true);
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(19900101);
    pStock->SetDayLineEndDate(20000101);
    pStock->StoreDayLine(pDayLine);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveDayLineBasicInfoOfStock(_))
      .Times(0);
    EXPECT_EQ(s_pchinaMarket->GetDayLineNeedSaveNumber(), 1);
    EXPECT_FALSE(s_pchinaMarket->TaskSaveDayLineData());
    EXPECT_EQ(s_pchinaMarket->GetDayLineNeedSaveNumber(), 0);

    pStock = s_pchinaMarket->GetStock(_T("600282.SS"));
    EXPECT_FALSE(pStock->IsDayLineNeedSaving()) << "无论执行与否皆清除此标识";

    pStock->SetDayLineEndDate(lDate);
    pStock->UnloadDayLine();
  }

  TEST_F(CMockChinaMarketTest, TestTaskSaveDayLineData4) {
    CChinaStockPtr pStock = s_pchinaMarket->GetStock(_T("600608.SS"));
    long lDate = pStock->GetDayLineEndDate();

    EXPECT_EQ(s_pchinaMarket->GetDayLineNeedSaveNumber(), 0);
    EXPECT_FALSE(s_pchinaMarket->IsDayLineNeedSaving());
    EXPECT_FALSE(pStock->IsDayLineNeedSaving());
    pStock->SetDayLineNeedSaving(true);
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(19900101);
    pStock->SetDayLineEndDate(20000101);
    pStock->StoreDayLine(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20210101);
    pStock->StoreDayLine(pDayLine);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveDayLineBasicInfoOfStock(_))
      .Times(1);
    EXPECT_EQ(s_pchinaMarket->GetDayLineNeedSaveNumber(), 1);
    EXPECT_TRUE(s_pchinaMarket->TaskSaveDayLineData());
    EXPECT_EQ(s_pchinaMarket->GetDayLineNeedSaveNumber(), 0);

    pStock = s_pchinaMarket->GetStock(_T("600608.SS"));
    EXPECT_FALSE(pStock->IsDayLineNeedSaving()) << "无论执行与否皆清除此标识";

    pStock->SetDayLineEndDate(lDate);
    pStock->UnloadDayLine();
  }

  TEST_F(CMockChinaMarketTest, TestUpdateTempRTData) {
    gl_ThreadStatus.SetSavingTempData(true);
    EXPECT_FALSE(s_pchinaMarket->UpdateTempRTData());
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    EXPECT_TRUE(s_pchinaMarket->UpdateTempRTData());
    EXPECT_TRUE(gl_ThreadStatus.IsSavingTempData());
    gl_ThreadStatus.SetSavingTempData(true);
  }

  TEST_F(CMockChinaMarketTest, TestTaskLoadCurrentStockDayLine1) {
    s_pchinaMarket->ResetCurrentStock();
    EXPECT_CALL(*s_pchinaMarket, RunningThreadLoadDayLine(_))
      .Times(0);
    EXPECT_TRUE(s_pchinaMarket->TaskLoadCurrentStockHistoryData());
  }

  TEST_F(CMockChinaMarketTest, TestTaskLoadCurrentStockDayLine2) {
    s_pchinaMarket->ResetCurrentStock();
    s_pchinaMarket->SetCurrentStock(_T("600000.SS"));
    s_pchinaMarket->GetCurrentStock()->SetDayLineLoaded(true);
    s_pchinaMarket->GetCurrentStock()->SetWeekLineLoaded(true);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadLoadDayLine(_))
      .Times(0);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadLoadWeekLine(_))
      .Times(0);
    EXPECT_TRUE(s_pchinaMarket->TaskLoadCurrentStockHistoryData());
    EXPECT_TRUE(s_pchinaMarket->GetCurrentStock()->IsDayLineLoaded());
    EXPECT_TRUE(s_pchinaMarket->GetCurrentStock()->IsWeekLineLoaded());
  }

  TEST_F(CMockChinaMarketTest, TestTaskLoadCurrentStockDayLine3) {
    s_pchinaMarket->ResetCurrentStock();
    s_pchinaMarket->SetCurrentStock(_T("600000.SS"));
    s_pchinaMarket->GetCurrentStock()->SetDayLineLoaded(false);
    s_pchinaMarket->GetCurrentStock()->SetWeekLineLoaded(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadLoadDayLine(_))
      .Times(1);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadLoadWeekLine(_))
      .Times(1);
    EXPECT_TRUE(s_pchinaMarket->TaskLoadCurrentStockHistoryData());
    EXPECT_TRUE(s_pchinaMarket->GetCurrentStock()->IsDayLineLoaded());
    EXPECT_TRUE(s_pchinaMarket->GetCurrentStock()->IsWeekLineLoaded());
  }

  TEST_F(CMockChinaMarketTest, TestTaskUpdateStockCodeDB) {
  }

  TEST_F(CMockChinaMarketTest, TestTaskUpdateOptionDB) {
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

  TEST_F(CMockChinaMarketTest, TestTaskUpdateChoicedStockDB) {
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

  TEST_F(CMockChinaMarketTest, TestTaskSaveChoicedRTData) {
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

  TEST_F(CMockChinaMarketTest, TestTaskSaveTempDataIntoDB) {
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

  TEST_F(CMockChinaMarketTest, TestTaskChoice10RSStrong2StockSet) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    s_pchinaMarket->__TEST_SetMarketTM(tm_);
    s_pchinaMarket->SetSystemReady(false);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong2StockSet(151200));
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong2StockSet(151201));
    s_pchinaMarket->SetSystemReady(true);
    s_pchinaMarket->SetChoiced10RSStrong2StockSet(true);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong2StockSet(151200));
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong2StockSet(151201));
    s_pchinaMarket->SetSystemReady(true);
    s_pchinaMarket->SetChoiced10RSStrong2StockSet(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadChoice10RSStrong2StockSet)
      .Times(1);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong2StockSet(151200));
    EXPECT_TRUE(s_pchinaMarket->TaskChoice10RSStrong2StockSet(151201));
    EXPECT_TRUE(s_pchinaMarket->IsChoiced10RSStrong2StockSet());

    tm_.tm_wday = 0; // 星期日
    s_pchinaMarket->__TEST_SetMarketTM(tm_);
    s_pchinaMarket->SetTodayStockProcessed(false);
    s_pchinaMarket->SetChoiced10RSStrong2StockSet(false);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong2StockSet(151201));
    EXPECT_FALSE(s_pchinaMarket->IsChoiced10RSStrong2StockSet()) << _T("休息日不处理");
  }

  TEST_F(CMockChinaMarketTest, TestTaskChoice10RSStrong1StockSet) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    s_pchinaMarket->__TEST_SetMarketTM(tm_);
    s_pchinaMarket->SetSystemReady(false);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong1StockSet(151100));
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong1StockSet(151101));
    s_pchinaMarket->SetSystemReady(true);
    s_pchinaMarket->SetChoiced10RSStrong1StockSet(true);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong1StockSet(151100));
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong1StockSet(151101));
    s_pchinaMarket->SetSystemReady(true);
    s_pchinaMarket->SetChoiced10RSStrong1StockSet(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadChoice10RSStrong1StockSet)
      .Times(1);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong1StockSet(151100));
    EXPECT_TRUE(s_pchinaMarket->TaskChoice10RSStrong1StockSet(151101));
    EXPECT_TRUE(s_pchinaMarket->IsChoiced10RSStrong1StockSet());

    tm_.tm_wday = 0; // 星期日
    s_pchinaMarket->__TEST_SetMarketTM(tm_);
    s_pchinaMarket->SetTodayStockProcessed(false);
    s_pchinaMarket->SetChoiced10RSStrong1StockSet(false);
    EXPECT_FALSE(s_pchinaMarket->TaskChoice10RSStrong1StockSet(151101));
    EXPECT_FALSE(s_pchinaMarket->IsChoiced10RSStrong1StockSet()) << _T("休息日不处理");
  }

  TEST_F(CMockChinaMarketTest, TestTaskChoice10RSStrongStockSet) {
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

  TEST_F(CMockChinaMarketTest, TestProcessTodayStock) {
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
    EXPECT_FALSE(s_pchinaMarket->IsTodayStockProcessed()) << _T("此标志由工作线程负责设置，故而此时尚未设置");

    tm_.tm_wday = 0; // 星期日
    s_pchinaMarket->__TEST_SetMarketTM(tm_);
    s_pchinaMarket->SetTodayStockProcessed(false);
    EXPECT_FALSE(s_pchinaMarket->TaskProcessTodayStock(150400));
    EXPECT_FALSE(s_pchinaMarket->IsTodayStockProcessed()) << _T("休息日不处理");
  }

  TEST_F(CMockChinaMarketTest, TestThreadProcessTodayStock) {
    s_pchinaMarket->SetTodayStockProcessed(false);
    s_pchinaMarket->CalculateTime();
    s_pchinaMarket->SetNewestTransactionTime(s_pchinaMarket->GetLocalTime());
    long lDate = FormatToDate(s_pchinaMarket->GetNewestTransactionTime());
    s_pchinaMarket->__TEST_SetFormatedMarketTime(130000); // 设置市场时间为小于150400，
    EXPECT_CALL(*s_pchinaMarket, BuildDayLineOfDate(lDate))
      .Times(1)
      .WillOnce(Return(4000));
    EXPECT_CALL(*s_pchinaMarket, BuildDayLineRSOfDate(lDate))
      .Times(1)
      .WillOnce(Return(true));
    EXPECT_CALL(*s_pchinaMarket, BuildWeekLineOfCurrentWeek)
      .Times(1);
    EXPECT_CALL(*s_pchinaMarket, BuildWeekLineRSOfDate(GetCurrentMonday(lDate)))
      .Times(1)
      .WillOnce(Return(true));
    s_pchinaMarket->SetSystemReady(true);
    EXPECT_EQ(ThreadProcessTodayStock(s_pchinaMarket), (UINT)14);
    // 市场时间小于150400时
    EXPECT_EQ(s_pchinaMarket->GetRSEndDate(), 19900101) << "没有执行修改最新相对强度日的动作";
    EXPECT_FALSE(s_pchinaMarket->IsUpdateStockCodeDB());
    EXPECT_FALSE(s_pchinaMarket->IsUpdateOptionDB());
    EXPECT_FALSE(s_pchinaMarket->IsTodayStockProcessed());

    s_pchinaMarket->__TEST_SetFormatedMarketTime(150500); // 设置市场时间为大于150400，
    EXPECT_CALL(*s_pchinaMarket, BuildDayLineOfDate(lDate))
      .Times(1)
      .WillOnce(Return(4000));
    EXPECT_CALL(*s_pchinaMarket, BuildDayLineRSOfDate(lDate))
      .Times(1)
      .WillOnce(Return(true));
    EXPECT_CALL(*s_pchinaMarket, BuildWeekLineOfCurrentWeek)
      .Times(1);
    EXPECT_CALL(*s_pchinaMarket, BuildWeekLineRSOfDate(GetCurrentMonday(lDate)))
      .Times(1)
      .WillOnce(Return(true));
    s_pchinaMarket->SetSystemReady(true);
    EXPECT_EQ(ThreadProcessTodayStock(s_pchinaMarket), (UINT)14);
    // 市场时间大于150400时
    EXPECT_EQ(s_pchinaMarket->GetRSEndDate(), lDate);
    EXPECT_TRUE(s_pchinaMarket->IsUpdateOptionDB());
    EXPECT_TRUE(s_pchinaMarket->IsTodayStockProcessed());
  }

  TEST_F(CMockChinaMarketTest, TestThreadCalculateDayLineRS) {
    s_pchinaMarket->CalculateTime();
    time_t tStart = s_pchinaMarket->GetMarketTime() - 3600 * 24 * 6; // 从一周前开始计算
    tm _tm;
    gmtime_s(&_tm, &tStart);
    long lStartDate = (_tm.tm_year + 1900) * 10000 + (_tm.tm_mon + 1) * 100 + _tm.tm_mday;
    gl_fExitingCalculatingRS = true; // 中间被打断
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    s_pchinaMarket->SetRSEndDate(0);
    s_pchinaMarket->SetUpdateOptionDB(false);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadBuildDayLineRSOfDate(_))
      .Times(5)
      .WillRepeatedly(Return(true));
    EXPECT_EQ(ThreadBuildDayLineRS(s_pchinaMarket, lStartDate), (UINT)11);
    EXPECT_FALSE(s_pchinaMarket->IsUpdateOptionDB()) << _T("被打断后不设置此标识");
    EXPECT_EQ(s_pchinaMarket->GetRSEndDate(), 0);
    EXPECT_FALSE(gl_fExitingCalculatingRS);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());

    tStart = s_pchinaMarket->GetMarketTime() - 3600 * 24 * 6; // 从一周前开始计算
    gmtime_s(&_tm, &tStart);
    lStartDate = (_tm.tm_year + 1900) * 10000 + (_tm.tm_mon + 1) * 100 + _tm.tm_mday;
    gl_fExitingCalculatingRS = false;
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_CALL(*s_pchinaMarket, RunningThreadBuildDayLineRSOfDate(_))
      .Times(5);
    EXPECT_EQ(ThreadBuildDayLineRS(s_pchinaMarket, lStartDate), (UINT)11);
    EXPECT_TRUE(s_pchinaMarket->IsUpdateOptionDB());
    EXPECT_EQ(s_pchinaMarket->GetRSEndDate(), s_pchinaMarket->GetFormatedMarketDate());
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());
  }

  TEST_F(CMockChinaMarketTest, TestThreadUpdateOptionDB) {
    EXPECT_CALL(*s_pchinaMarket, UpdateOptionDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateOptionDB(s_pchinaMarket), (UINT)20);
  }

  TEST_F(CMockChinaMarketTest, TestThreadUpdateStockCodeDB) {
    EXPECT_CALL(*s_pchinaMarket, UpdateStockCodeDB)
      .Times(1);
    s_pchinaMarket->SetSystemReady(true);
    gl_ThreadStatus.SetCalculatingRTData(false);
    EXPECT_EQ(ThreadUpdateStockCodeDB(s_pchinaMarket), (UINT)18);
  }

  TEST_F(CMockChinaMarketTest, TestThreadUpdateChoicedStockDB) {
    EXPECT_CALL(*s_pchinaMarket, AppendChoicedStockDB)
      .Times(1);
    s_pchinaMarket->SetSystemReady(true);
    EXPECT_EQ(ThreadAppendChoicedStockDB(s_pchinaMarket), (UINT)22);
  }

  TEST_F(CMockChinaMarketTest, TestThreadSaveTempRTData) {
    EXPECT_CALL(*s_pchinaMarket, UpdateTodayTempDB)
      .Times(1);
    s_pchinaMarket->SetSystemReady(true);
    EXPECT_EQ(ThreadSaveTempRTData(s_pchinaMarket), (UINT)13);
  }

  TEST_F(CMockChinaMarketTest, TestThreadSaveRTData) {
    EXPECT_CALL(*s_pchinaMarket, SaveRTData)
      .Times(1);
    s_pchinaMarket->SetSystemReady(true);
    EXPECT_EQ(ThreadSaveRTData(s_pchinaMarket), (UINT)19);
  }

  TEST_F(CMockChinaMarketTest, TestThreadMaintainDayLineDatabase) {
    EXPECT_EQ(ThreadMaintainDayLineDataBase(), (UINT)17) << "此工作线程目前没有实际动作";
  }

  TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLineOfCurrentWeek) {
    EXPECT_CALL(*s_pchinaMarket, BuildWeekLineOfCurrentWeek)
      .Times(1);
    EXPECT_EQ(ThreadBuildWeekLineOfCurrentWeek(s_pchinaMarket), (UINT)32);
  }

  TEST_F(CMockChinaMarketTest, TestThreadBuildCurrentWeekWeekLineTable) {
    EXPECT_CALL(*s_pchinaMarket, BuildCurrentWeekWeekLineTable)
      .Times(1);
    EXPECT_EQ(ThreadBuildCurrentWeekWeekLineTable(s_pchinaMarket), (UINT)33);
  }

  TEST_F(CMockChinaMarketTest, TestThreadSaveStockSection) {
    EXPECT_CALL(*s_pchinaMarket, SaveStockSection)
      .Times(1);
    EXPECT_EQ(ThreadSaveStockSection(s_pchinaMarket), (UINT)35);
  }
}