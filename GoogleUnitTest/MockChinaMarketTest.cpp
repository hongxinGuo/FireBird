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
  extern CMockChinaMarketPtr gl_pMockChinaMarket;
  class CMockChinaMarketTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      ASSERT_FALSE(gl_fNormalMode);
      EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

      EXPECT_TRUE(gl_pMockChinaMarket != nullptr) << "此Mock变量在EnvironmentSetUp.h中生成";
      gl_pMockChinaMarket->SetTodayStockProcessed(false);
      EXPECT_FALSE(gl_pMockChinaMarket->IsDayLineNeedSaving());
      EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
      EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateStockCodeDB());

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }
    static void TearDownTestSuite(void) {
      EXPECT_FALSE(gl_fExitingSystem);
      EXPECT_FALSE(gl_pMockChinaMarket->IsDayLineNeedSaving());
      EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);

      EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
      EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
    }
    virtual void SetUp(void) override {
      //EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
      EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
      EXPECT_FALSE(gl_fExitingSystem);

      gl_pMockChinaMarket->SetTodayStockProcessed(false);
      gl_pMockChinaMarket->SetRSEndDate(19900101);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateStockCodeDB());
      EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
      CChinaStockPtr pStock;
      if (gl_pMockChinaMarket->GetDayLineNeedSaveNumber() > 0) {
        for (int i = 0; i < gl_pMockChinaMarket->GetTotalStock(); i++) {
          pStock = gl_pMockChinaMarket->GetStock(i);
          if (pStock->IsDayLineNeedSaving()) {
            EXPECT_STREQ(pStock->GetSymbol(), _T(""));
          }
        }
      }
      gl_pMockChinaMarket->SetTodayStockProcessed(false);
      gl_pMockChinaMarket->SetRSEndDate(19900101);
      gl_pMockChinaMarket->SetUpdateOptionDB(false);
      gl_ThreadStatus.SetSavingTempData(false);

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }
  };

  TEST_F(CMockChinaMarketTest, TestTaskSaveDayLineData1) {
    CChinaStockPtr pStock = gl_pMockChinaMarket->GetStock(_T("600666.SS"));
    EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
    EXPECT_FALSE(pStock->IsDayLineNeedSaving());
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadSaveDayLineBasicInfoOfStock(_))
      .Times(0);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskSaveDayLineData());
  }

  TEST_F(CMockChinaMarketTest, TestTaskSaveDayLineData2) {
    CChinaStockPtr pStock = gl_pMockChinaMarket->GetStock(_T("600668.SS"));

    EXPECT_FALSE(pStock->IsDayLineNeedSaving());
    EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
    pStock->SetDayLineNeedSaving(true);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadSaveDayLineBasicInfoOfStock(_))
      .Times(0);
    EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 1);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskSaveDayLineData());
    EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
    pStock = gl_pMockChinaMarket->GetStock(_T("600668.SS"));
    EXPECT_FALSE(pStock->IsDayLineNeedSaving()) << "无论执行与否皆清除此标识";
  }

  TEST_F(CMockChinaMarketTest, TestTaskSaveDayLineData3) {
    CChinaStockPtr pStock = gl_pMockChinaMarket->GetStock(_T("600282.SS"));
    long lDate = pStock->GetDayLineEndDate();

    EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
    EXPECT_FALSE(pStock->IsDayLineNeedSaving());
    pStock->SetDayLineNeedSaving(true);
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(19900101);
    pStock->SetDayLineEndDate(20000101);
    pStock->StoreDayLine(pDayLine);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadSaveDayLineBasicInfoOfStock(_))
      .Times(0);
    EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 1);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskSaveDayLineData());
    EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);

    pStock = gl_pMockChinaMarket->GetStock(_T("600282.SS"));
    EXPECT_FALSE(pStock->IsDayLineNeedSaving()) << "无论执行与否皆清除此标识";

    pStock->SetDayLineEndDate(lDate);
    pStock->UnloadDayLine();
  }

  TEST_F(CMockChinaMarketTest, TestTaskSaveDayLineData4) {
    CChinaStockPtr pStock = gl_pMockChinaMarket->GetStock(_T("600608.SS"));
    long lDate = pStock->GetDayLineEndDate();

    EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);
    EXPECT_FALSE(gl_pMockChinaMarket->IsDayLineNeedSaving());
    EXPECT_FALSE(pStock->IsDayLineNeedSaving());
    pStock->SetDayLineNeedSaving(true);
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(19900101);
    pStock->SetDayLineEndDate(20000101);
    pStock->StoreDayLine(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDate(20210101);
    pStock->StoreDayLine(pDayLine);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadSaveDayLineBasicInfoOfStock(_))
      .Times(1);
    EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 1);
    EXPECT_TRUE(gl_pMockChinaMarket->TaskSaveDayLineData());
    EXPECT_EQ(gl_pMockChinaMarket->GetDayLineNeedSaveNumber(), 0);

    pStock = gl_pMockChinaMarket->GetStock(_T("600608.SS"));
    EXPECT_FALSE(pStock->IsDayLineNeedSaving()) << "无论执行与否皆清除此标识";

    pStock->SetDayLineEndDate(lDate);
    pStock->UnloadDayLine();
  }

  TEST_F(CMockChinaMarketTest, TestUpdateTempRTData) {
    gl_ThreadStatus.SetSavingTempData(true);
    EXPECT_FALSE(gl_pMockChinaMarket->UpdateTempRTData());
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    EXPECT_TRUE(gl_pMockChinaMarket->UpdateTempRTData());
    EXPECT_TRUE(gl_ThreadStatus.IsSavingTempData());
    gl_ThreadStatus.SetSavingTempData(true);
  }

  TEST_F(CMockChinaMarketTest, TestTaskLoadCurrentStockDayLine1) {
    gl_pMockChinaMarket->ResetCurrentStock();
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadLoadDayLine(_))
      .Times(0);
    EXPECT_TRUE(gl_pMockChinaMarket->TaskLoadCurrentStockHistoryData());
  }

  TEST_F(CMockChinaMarketTest, TestTaskLoadCurrentStockDayLine2) {
    gl_pMockChinaMarket->ResetCurrentStock();
    gl_pMockChinaMarket->SetCurrentStock(_T("600000.SS"));
    gl_pMockChinaMarket->GetCurrentStock()->SetDayLineLoaded(true);
    gl_pMockChinaMarket->GetCurrentStock()->SetWeekLineLoaded(true);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadLoadDayLine(_))
      .Times(0);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadLoadWeekLine(_))
      .Times(0);
    EXPECT_TRUE(gl_pMockChinaMarket->TaskLoadCurrentStockHistoryData());
    EXPECT_TRUE(gl_pMockChinaMarket->GetCurrentStock()->IsDayLineLoaded());
    EXPECT_TRUE(gl_pMockChinaMarket->GetCurrentStock()->IsWeekLineLoaded());
  }

  TEST_F(CMockChinaMarketTest, TestTaskLoadCurrentStockDayLine3) {
    gl_pMockChinaMarket->ResetCurrentStock();
    gl_pMockChinaMarket->SetCurrentStock(_T("600000.SS"));
    gl_pMockChinaMarket->GetCurrentStock()->SetDayLineLoaded(false);
    gl_pMockChinaMarket->GetCurrentStock()->SetWeekLineLoaded(false);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadLoadDayLine(_))
      .Times(1);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadLoadWeekLine(_))
      .Times(1);
    EXPECT_TRUE(gl_pMockChinaMarket->TaskLoadCurrentStockHistoryData());
    EXPECT_TRUE(gl_pMockChinaMarket->GetCurrentStock()->IsDayLineLoaded());
    EXPECT_TRUE(gl_pMockChinaMarket->GetCurrentStock()->IsWeekLineLoaded());
  }

  TEST_F(CMockChinaMarketTest, TestTaskUpdateStockCodeDB) {
  }

  TEST_F(CMockChinaMarketTest, TestTaskUpdateOptionDB) {
    gl_pMockChinaMarket->SetUpdateOptionDB(false);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadUpdateOptionDB())
      .Times(0);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskUpdateOptionDB());
    gl_pMockChinaMarket->SetUpdateOptionDB(true);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadUpdateOptionDB())
      .Times(1);
    EXPECT_TRUE(gl_pMockChinaMarket->TaskUpdateOptionDB());
    EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateOptionDB());
  }

  TEST_F(CMockChinaMarketTest, TestTaskUpdateChoicedStockDB) {
    gl_pMockChinaMarket->SetUpdateChoicedStockDB(false);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadAppendChoicedStockDB())
      .Times(0);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskUpdateChoicedStockDB());
    gl_pMockChinaMarket->SetUpdateChoicedStockDB(true);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadAppendChoicedStockDB())
      .Times(1);
    EXPECT_TRUE(gl_pMockChinaMarket->TaskUpdateChoicedStockDB());
    EXPECT_TRUE(gl_pMockChinaMarket->IsUpdateChoicedStockDB()) << _T("自选股票更改标识设置后，就再也不重置，这样能够保证系统退出时再次更新数据库");
  }

  TEST_F(CMockChinaMarketTest, TestTaskSaveChoicedRTData) {
    gl_pMockChinaMarket->SetSystemReady(false);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadSaveChoicedRTData())
      .Times(0);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskSaveChoicedRTData());
    gl_pMockChinaMarket->SetSystemReady(true);
    gl_pMockChinaMarket->SetRecordRTData(false);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadSaveChoicedRTData())
      .Times(0);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskSaveChoicedRTData());
    gl_pMockChinaMarket->SetRecordRTData(true);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadSaveChoicedRTData())
      .Times(1);
    EXPECT_TRUE(gl_pMockChinaMarket->TaskSaveChoicedRTData());
    gl_pMockChinaMarket->SetRecordRTData(false);
    gl_pMockChinaMarket->SetSystemReady(false);
  }

  TEST_F(CMockChinaMarketTest, TestTaskSaveTempDataIntoDB) {
    gl_pMockChinaMarket->SetSystemReady(true);
    gl_pMockChinaMarket->SetMarketOpened(true);
    gl_ThreadStatus.SetCalculatingRTData(false);
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    gl_pMockChinaMarket->TaskSaveTempDataIntoDB(93001);
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    gl_pMockChinaMarket->TaskSaveTempDataIntoDB(113559);
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    gl_pMockChinaMarket->TaskSaveTempDataIntoDB(130001);
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    gl_pMockChinaMarket->TaskSaveTempDataIntoDB(150559);
  }

  TEST_F(CMockChinaMarketTest, TestTaskChoice10RSStrong2StockSet) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    gl_pMockChinaMarket->__TEST_SetMarketTM(tm_);
    gl_pMockChinaMarket->SetSystemReady(false);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151200));
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151201));
    gl_pMockChinaMarket->SetSystemReady(true);
    gl_pMockChinaMarket->SetChoiced10RSStrong2StockSet(true);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151200));
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151201));
    gl_pMockChinaMarket->SetSystemReady(true);
    gl_pMockChinaMarket->SetChoiced10RSStrong2StockSet(false);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadChoice10RSStrong2StockSet)
      .Times(1);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151200));
    EXPECT_TRUE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151201));
    EXPECT_TRUE(gl_pMockChinaMarket->IsChoiced10RSStrong2StockSet());

    tm_.tm_wday = 0; // 星期日
    gl_pMockChinaMarket->__TEST_SetMarketTM(tm_);
    gl_pMockChinaMarket->SetTodayStockProcessed(false);
    gl_pMockChinaMarket->SetChoiced10RSStrong2StockSet(false);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong2StockSet(151201));
    EXPECT_FALSE(gl_pMockChinaMarket->IsChoiced10RSStrong2StockSet()) << _T("休息日不处理");
  }

  TEST_F(CMockChinaMarketTest, TestTaskChoice10RSStrong1StockSet) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    gl_pMockChinaMarket->__TEST_SetMarketTM(tm_);
    gl_pMockChinaMarket->SetSystemReady(false);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151100));
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151101));
    gl_pMockChinaMarket->SetSystemReady(true);
    gl_pMockChinaMarket->SetChoiced10RSStrong1StockSet(true);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151100));
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151101));
    gl_pMockChinaMarket->SetSystemReady(true);
    gl_pMockChinaMarket->SetChoiced10RSStrong1StockSet(false);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadChoice10RSStrong1StockSet)
      .Times(1);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151100));
    EXPECT_TRUE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151101));
    EXPECT_TRUE(gl_pMockChinaMarket->IsChoiced10RSStrong1StockSet());

    tm_.tm_wday = 0; // 星期日
    gl_pMockChinaMarket->__TEST_SetMarketTM(tm_);
    gl_pMockChinaMarket->SetTodayStockProcessed(false);
    gl_pMockChinaMarket->SetChoiced10RSStrong1StockSet(false);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrong1StockSet(151101));
    EXPECT_FALSE(gl_pMockChinaMarket->IsChoiced10RSStrong1StockSet()) << _T("休息日不处理");
  }

  TEST_F(CMockChinaMarketTest, TestTaskChoice10RSStrongStockSet) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    gl_pMockChinaMarket->__TEST_SetMarketTM(tm_);
    gl_pMockChinaMarket->SetSystemReady(false);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151000));
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
    gl_pMockChinaMarket->SetSystemReady(true);
    gl_pMockChinaMarket->SetChoiced10RSStrongStockSet(true);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151000));
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
    gl_pMockChinaMarket->SetSystemReady(true);
    gl_pMockChinaMarket->SetChoiced10RSStrongStockSet(false);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadChoice10RSStrongStockSet)
      .Times(1);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151000));
    EXPECT_TRUE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
    EXPECT_TRUE(gl_pMockChinaMarket->IsChoiced10RSStrongStockSet());

    tm_.tm_wday = 0; // 星期日
    gl_pMockChinaMarket->__TEST_SetMarketTM(tm_);
    gl_pMockChinaMarket->SetTodayStockProcessed(false);
    gl_pMockChinaMarket->SetChoiced10RSStrongStockSet(false);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskChoice10RSStrongStockSet(151001));
    EXPECT_FALSE(gl_pMockChinaMarket->IsChoiced10RSStrongStockSet()) << _T("休息日不处理");
  }

  TEST_F(CMockChinaMarketTest, TestProcessTodayStock) {
    tm tm_;
    tm_.tm_wday = 1; // 星期一
    gl_pMockChinaMarket->__TEST_SetMarketTM(tm_);
    gl_pMockChinaMarket->SetSystemReady(false);
    gl_pMockChinaMarket->SetTodayStockProcessed(false);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150359));
    EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150400));
    EXPECT_FALSE(gl_pMockChinaMarket->IsTodayStockProcessed());
    gl_pMockChinaMarket->SetSystemReady(true);
    gl_pMockChinaMarket->SetTodayStockProcessed(true);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150359));
    EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150400));
    EXPECT_TRUE(gl_pMockChinaMarket->IsTodayStockProcessed());
    gl_pMockChinaMarket->SetSystemReady(true);
    gl_pMockChinaMarket->SetTodayStockProcessed(false);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadProcessTodayStock)
      .Times(1);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150359));
    EXPECT_TRUE(gl_pMockChinaMarket->TaskProcessTodayStock(150400));
    EXPECT_FALSE(gl_pMockChinaMarket->IsTodayStockProcessed()) << _T("此标志由工作线程负责设置，故而此时尚未设置");

    tm_.tm_wday = 0; // 星期日
    gl_pMockChinaMarket->__TEST_SetMarketTM(tm_);
    gl_pMockChinaMarket->SetTodayStockProcessed(false);
    EXPECT_FALSE(gl_pMockChinaMarket->TaskProcessTodayStock(150400));
    EXPECT_FALSE(gl_pMockChinaMarket->IsTodayStockProcessed()) << _T("休息日不处理");
  }

  TEST_F(CMockChinaMarketTest, TestThreadProcessTodayStock) {
    gl_pMockChinaMarket->SetTodayStockProcessed(false);
    gl_pMockChinaMarket->CalculateTime();
    gl_pMockChinaMarket->SetNewestTransactionTime(gl_pMockChinaMarket->GetLocalTime());
    long lDate = FormatToDate(gl_pMockChinaMarket->GetNewestTransactionTime());
    gl_pMockChinaMarket->__TEST_SetFormatedMarketTime(130000); // 设置市场时间为小于150400，
    EXPECT_CALL(*gl_pMockChinaMarket, BuildDayLineOfDate(lDate))
      .Times(1)
      .WillOnce(Return(4000));
    EXPECT_CALL(*gl_pMockChinaMarket, BuildDayLineRSOfDate(lDate))
      .Times(1)
      .WillOnce(Return(true));
    EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineOfCurrentWeek)
      .Times(1);
    EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineRSOfDate(GetCurrentMonday(lDate)))
      .Times(1)
      .WillOnce(Return(true));
    gl_pMockChinaMarket->SetSystemReady(true);
    EXPECT_EQ(ThreadProcessTodayStock(gl_pMockChinaMarket.get()), (UINT)14);
    // 市场时间小于150400时
    EXPECT_EQ(gl_pMockChinaMarket->GetRSEndDate(), 19900101) << "没有执行修改最新相对强度日的动作";
    EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateStockCodeDB());
    EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateOptionDB());
    EXPECT_FALSE(gl_pMockChinaMarket->IsTodayStockProcessed());

    gl_pMockChinaMarket->__TEST_SetFormatedMarketTime(150500); // 设置市场时间为大于150400，
    EXPECT_CALL(*gl_pMockChinaMarket, BuildDayLineOfDate(lDate))
      .Times(1)
      .WillOnce(Return(4000));
    EXPECT_CALL(*gl_pMockChinaMarket, BuildDayLineRSOfDate(lDate))
      .Times(1)
      .WillOnce(Return(true));
    EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineOfCurrentWeek)
      .Times(1);
    EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineRSOfDate(GetCurrentMonday(lDate)))
      .Times(1)
      .WillOnce(Return(true));
    gl_pMockChinaMarket->SetSystemReady(true);
    EXPECT_EQ(ThreadProcessTodayStock(gl_pMockChinaMarket.get()), (UINT)14);
    // 市场时间大于150400时
    EXPECT_EQ(gl_pMockChinaMarket->GetRSEndDate(), lDate);
    EXPECT_TRUE(gl_pMockChinaMarket->IsUpdateOptionDB());
    EXPECT_TRUE(gl_pMockChinaMarket->IsTodayStockProcessed());
  }

  TEST_F(CMockChinaMarketTest, TestThreadCalculateDayLineRS) {
    gl_pMockChinaMarket->CalculateTime();
    time_t tStart = gl_pMockChinaMarket->GetMarketTime() - 3600 * 24 * 6; // 从一周前开始计算
    tm _tm;
    gmtime_s(&_tm, &tStart);
    long lStartDate = (_tm.tm_year + 1900) * 10000 + (_tm.tm_mon + 1) * 100 + _tm.tm_mday;
    gl_fExitingCalculatingRS = true; // 中间被打断
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    gl_pMockChinaMarket->SetRSEndDate(0);
    gl_pMockChinaMarket->SetUpdateOptionDB(false);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadBuildDayLineRSOfDate(_))
      .Times(5)
      .WillRepeatedly(Return(true));
    EXPECT_EQ(ThreadBuildDayLineRS(gl_pMockChinaMarket.get(), lStartDate), (UINT)11);
    EXPECT_FALSE(gl_pMockChinaMarket->IsUpdateOptionDB()) << _T("被打断后不设置此标识");
    EXPECT_EQ(gl_pMockChinaMarket->GetRSEndDate(), 0);
    EXPECT_FALSE(gl_fExitingCalculatingRS);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());

    tStart = gl_pMockChinaMarket->GetMarketTime() - 3600 * 24 * 6; // 从一周前开始计算
    gmtime_s(&_tm, &tStart);
    lStartDate = (_tm.tm_year + 1900) * 10000 + (_tm.tm_mon + 1) * 100 + _tm.tm_mday;
    gl_fExitingCalculatingRS = false;
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_CALL(*gl_pMockChinaMarket, RunningThreadBuildDayLineRSOfDate(_))
      .Times(5);
    EXPECT_EQ(ThreadBuildDayLineRS(gl_pMockChinaMarket.get(), lStartDate), (UINT)11);
    EXPECT_TRUE(gl_pMockChinaMarket->IsUpdateOptionDB());
    EXPECT_EQ(gl_pMockChinaMarket->GetRSEndDate(), gl_pMockChinaMarket->GetFormatedMarketDate());
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());
  }

  TEST_F(CMockChinaMarketTest, TestThreadUpdateOptionDB) {
    EXPECT_CALL(*gl_pMockChinaMarket, UpdateOptionDB)
      .Times(1);
    EXPECT_EQ(ThreadUpdateOptionDB(gl_pMockChinaMarket.get()), (UINT)20);
  }

  TEST_F(CMockChinaMarketTest, TestThreadUpdateStockCodeDB) {
    EXPECT_CALL(*gl_pMockChinaMarket, UpdateStockCodeDB)
      .Times(1);
    gl_pMockChinaMarket->SetSystemReady(true);
    gl_ThreadStatus.SetCalculatingRTData(false);
    EXPECT_EQ(ThreadUpdateStockCodeDB(gl_pMockChinaMarket.get()), (UINT)18);
  }

  TEST_F(CMockChinaMarketTest, TestThreadUpdateChoicedStockDB) {
    EXPECT_CALL(*gl_pMockChinaMarket, AppendChoicedStockDB)
      .Times(1);
    gl_pMockChinaMarket->SetSystemReady(true);
    EXPECT_EQ(ThreadAppendChoicedStockDB(gl_pMockChinaMarket.get()), (UINT)22);
  }

  TEST_F(CMockChinaMarketTest, TestThreadSaveTempRTData) {
    EXPECT_CALL(*gl_pMockChinaMarket, UpdateTodayTempDB)
      .Times(1);
    gl_pMockChinaMarket->SetSystemReady(true);
    EXPECT_EQ(ThreadSaveTempRTData(gl_pMockChinaMarket.get()), (UINT)13);
  }

  TEST_F(CMockChinaMarketTest, TestThreadSaveRTData) {
    EXPECT_CALL(*gl_pMockChinaMarket, SaveRTData)
      .Times(1);
    gl_pMockChinaMarket->SetSystemReady(true);
    EXPECT_EQ(ThreadSaveRTData(gl_pMockChinaMarket.get()), (UINT)19);
  }

  TEST_F(CMockChinaMarketTest, TestThreadMaintainDayLineDatabase) {
    EXPECT_EQ(ThreadMaintainDayLineDataBase(), (UINT)17) << "此工作线程目前没有实际动作";
  }

  TEST_F(CMockChinaMarketTest, TestThreadBuildWeekLineOfCurrentWeek) {
    EXPECT_CALL(*gl_pMockChinaMarket, BuildWeekLineOfCurrentWeek)
      .Times(1);
    EXPECT_EQ(ThreadBuildWeekLineOfCurrentWeek(gl_pMockChinaMarket.get()), (UINT)32);
  }

  TEST_F(CMockChinaMarketTest, TestThreadBuildCurrentWeekWeekLineTable) {
    EXPECT_CALL(*gl_pMockChinaMarket, BuildCurrentWeekWeekLineTable)
      .Times(1);
    EXPECT_EQ(ThreadBuildCurrentWeekWeekLineTable(gl_pMockChinaMarket.get()), (UINT)33);
  }

  TEST_F(CMockChinaMarketTest, TestThreadSaveStockSection) {
    EXPECT_CALL(*gl_pMockChinaMarket, SaveStockSection)
      .Times(1);
    EXPECT_EQ(ThreadSaveStockSection(gl_pMockChinaMarket.get()), (UINT)35);
  }
}