#include"pch.h"

#include"globedef.h"
#include"ChinaMarket.h"

#include"MockMainFrm.h"
using namespace testing;
using namespace ::testing;

namespace StockAnalysisTest {
  static CMockMainFrame* s_pMainFrame;
  class CMockMainFrameTest : public ::testing::Test {
  public:
    static void SetUpTestSuite(void) {
      EXPECT_FALSE(CMFCVisualManager::GetInstance() == NULL);//
      s_pMainFrame = new CMockMainFrame;
      EXPECT_TRUE(CMFCVisualManager::GetInstance() != NULL);//

      EXPECT_FALSE(gl_fNormalMode);
      EXPECT_TRUE(gl_fTestMode);
      EXPECT_EQ(gl_vMarketPtr.size(), 4);
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetSymbol();
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
    }
    static void TearDownTestSuite(void) {
      EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedProcessNumber(), 0);
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetSymbol();
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
      gl_pChinaStockMarket->ResetCurrentStock();
      gl_pChinaStockMarket->SetResetMarket(true);
      gl_pCrweberIndexMarket->SetResetMarket(true);
      gl_pPotenDailyBriefingMarket->SetResetMarket(true);
      gl_pChinaStockMarket->SetCurrentStockChanged(false);
      gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
      gl_pChinaStockMarket->SetUpdateOptionDB(false); // 这里使用了实际的数据库，故而不允许更新
      EXPECT_FALSE(gl_fNormalMode);
      EXPECT_TRUE(gl_fTestMode);
      EXPECT_EQ(gl_vMarketPtr.size(), 4);
      if (CMFCVisualManager::GetInstance() != NULL) {
        delete CMFCVisualManager::GetInstance(); // 在生成MainFrame时，会生成一个视觉管理器。故而在此删除之。
      }
      delete s_pMainFrame;
    }
    virtual void SetUp(void) override {
      gl_fExitingSystem = false;
      EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
    }
    virtual void TearDown(void) override {
      gl_pChinaStockMarket->ResetCurrentStock();
      gl_pChinaStockMarket->SetCurrentStockChanged(false);
      gl_pChinaStockMarket->ClearChoiceStockContainer();
      gl_ThreadStatus.SetCalculatingDayLineRS(false);
      gl_fExitingSystem = false;
    }
  };

  TEST_F(CMockMainFrameTest, TestCreateMarketContainer) {
    EXPECT_TRUE(s_pMainFrame->CreateMarketContainer());
    EXPECT_EQ(gl_vMarketPtr.size(), 8);
    CVirtualMarketPtr pMarket;
    pMarket = gl_vMarketPtr.at(7);
    gl_vMarketPtr.pop_back();
    EXPECT_STREQ(pMarket->GetMarketID(), gl_pCrweberIndexMarket->GetMarketID());
    pMarket = gl_vMarketPtr.at(6);
    gl_vMarketPtr.pop_back();
    EXPECT_STREQ(pMarket->GetMarketID(), gl_pPotenDailyBriefingMarket->GetMarketID());
    pMarket = gl_vMarketPtr.at(5);
    gl_vMarketPtr.pop_back();
    EXPECT_STREQ(pMarket->GetMarketID(), gl_pChinaStockMarket->GetMarketID());
    pMarket = gl_vMarketPtr.at(4);
    gl_vMarketPtr.pop_back();
    EXPECT_STREQ(pMarket->GetMarketID(), gl_pWorldMarket->GetMarketID());
    EXPECT_EQ(gl_vMarketPtr.size(), 4);
  }

  TEST_F(CMockMainFrameTest, TestIsNeedResetMarket) {
    gl_pWorldMarket->SetResetMarket(false);
    gl_pChinaStockMarket->SetResetMarket(false);
    gl_pPotenDailyBriefingMarket->SetResetMarket(false);
    gl_pCrweberIndexMarket->SetResetMarket(false);
    EXPECT_FALSE(s_pMainFrame->IsNeedResetMarket());
    gl_pChinaStockMarket->SetResetMarket(true);
    EXPECT_TRUE(s_pMainFrame->IsNeedResetMarket());
  }

  TEST_F(CMockMainFrameTest, TestUpdateStatus1) {
    InSequence seq;
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    gl_pChinaStockMarket->SetCurrentStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(4, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(5, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(6, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(7, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(8, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(10, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(11, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(12, _))
      .Times(1);
    s_pMainFrame->UpdateStatus();
  }

  TEST_F(CMockMainFrameTest, TestUpdateStatus2) {
    gl_pChinaStockMarket->SetCurrentEditStockChanged(true);
    gl_pChinaStockMarket->SetCurrentStockChanged(true);
    gl_pChinaStockMarket->SetCurrentStock(_T("600000.SS"));
    InSequence seq1;
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(1, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(2, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(3, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(4, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(5, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(6, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(7, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(8, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(10, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(11, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetPaneText(12, _))
      .Times(1);
    s_pMainFrame->UpdateStatus();
  }

  TEST_F(CMockMainFrameTest, TestUpdateInnerSystemStatus) {
    InSequence seq;
    EXPECT_CALL(*s_pMainFrame, SysCallSetInnerSystemPaneText(1, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetInnerSystemPaneText(2, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetInnerSystemPaneText(3, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetInnerSystemPaneText(4, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetInnerSystemPaneText(5, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetInnerSystemPaneText(6, _))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSetInnerSystemPaneText(7, _))
      .Times(1);
    s_pMainFrame->UpdateInnerSystemStatus();
  }

  TEST_F(CMockMainFrameTest, TestOnSysCommand) {
    gl_fExitingSystem = false;
    EXPECT_CALL(*s_pMainFrame, SysCallOnSysCommand)
      .Times(1);
    s_pMainFrame->OnSysCommand(SC_CLOSE, 0);
    EXPECT_TRUE(gl_fExitingSystem);
    gl_fExitingSystem = false;
    EXPECT_CALL(*s_pMainFrame, SysCallOnSysCommand)
      .Times(1);
    s_pMainFrame->OnSysCommand(0xFFFF ^ SC_CLOSE, 0);
    EXPECT_FALSE(gl_fExitingSystem);
    gl_fExitingSystem = false;
  }

  TEST_F(CMockMainFrameTest, TestOnCalculateTodayRS) {
    EXPECT_CALL(*s_pMainFrame, CalculateTodayRS)
      .Times(1);
    s_pMainFrame->OnCalculateTodayRS();
  }

  TEST_F(CMockMainFrameTest, TestOnProcessTodayStock) {
    gl_pChinaStockMarket->SetSystemReady(true);
    EXPECT_CALL(*s_pMainFrame, ProcessTodayStock)
      .Times(1);
    s_pMainFrame->OnProcessTodayStock();
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateProcessTodayStock) {
    CCmdUI cmdUI;
    gl_pChinaStockMarket->SetSystemReady(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateProcessTodayStock(&cmdUI);
    gl_pChinaStockMarket->SetSystemReady(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateProcessTodayStock(&cmdUI);
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateCalculateTodayRS) {
    CCmdUI cmdUI;
    gl_pChinaStockMarket->SetSystemReady(false);
    gl_ThreadStatus.SetCalculatingDayLineRS(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateCalculateTodayRS(&cmdUI);
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_ThreadStatus.SetCalculatingDayLineRS(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateCalculateTodayRS(&cmdUI);
    gl_pChinaStockMarket->SetSystemReady(false);
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateCalculateTodayRS(&cmdUI);
    gl_pChinaStockMarket->SetSystemReady(true);
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false));
    s_pMainFrame->OnUpdateCalculateTodayRS(&cmdUI);
  }

  TEST_F(CMockMainFrameTest, TestPreTranslateMessage) {
    MSG msg;
    msg.message = WM_KEYDOWN;
    EXPECT_CALL(*s_pMainFrame, SysCallPreTranslateMessage(&msg))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSendMessage(_, _, _))
      .Times(0);
    s_pMainFrame->PreTranslateMessage(&msg);

    msg.message = WM_KEYUP;
    msg.wParam = 201;
    msg.lParam = 102;
    EXPECT_CALL(*s_pMainFrame, SysCallPreTranslateMessage(&msg))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSendMessage(WM_KEYUP, 201, 102))
      .Times(1);
    s_pMainFrame->PreTranslateMessage(&msg);

    msg.message = WM_CHAR;
    msg.wParam = 200;
    msg.lParam = 100;
    EXPECT_CALL(*s_pMainFrame, SysCallPreTranslateMessage(&msg))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallSendMessage(WM_CHAR, 200, 100))
      .Times(1);
    s_pMainFrame->PreTranslateMessage(&msg);
  }

  TEST_F(CMockMainFrameTest, TestOnChar) {
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 0);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('6', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('6', 1, 1);
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('0', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('0', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 2);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('1', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('1', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 3);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('8', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('8', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 4);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('7', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('7', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 5);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('8', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('8', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 6);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar(0x008, 1, 1))
      .Times(1);
    s_pMainFrame->OnChar(0x008, 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 5);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('2', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('2', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 6);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('.', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('.', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 7);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('S', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('S', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 8);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('S', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('S', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 9);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar(0x00d, 1, 1))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallInvalidate())
      .Times(1);
    s_pMainFrame->OnChar(0x00d, 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 0);
    EXPECT_STREQ(gl_pChinaStockMarket->GetCurrentStock()->GetSymbol(), _T("601872.SS"));
  }

  TEST_F(CMockMainFrameTest, TestOnChar2) {
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 0);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar(0x008, 1, 1))
      .Times(1);
    s_pMainFrame->OnChar(0x008, 1, 1);
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 0);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('0', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('0', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 1);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('0', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('0', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 2);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('0', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('0', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 3);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('2', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('2', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 4);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('8', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('8', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 5);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('9', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('9', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 6);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('.', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('.', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 7);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('S', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('S', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 8);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar('Z', 1, 1))
      .Times(1);
    s_pMainFrame->OnChar('Z', 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 9);
    gl_pChinaStockMarket->SetCurrentEditStockChanged(false);
    gl_pChinaStockMarket->SetCurrentStock(_T("600604.SS"));
    EXPECT_CALL(*s_pMainFrame, SysCallOnChar(0x00d, 1, 1))
      .Times(1);
    EXPECT_CALL(*s_pMainFrame, SysCallInvalidate())
      .Times(0);
    s_pMainFrame->OnChar(0x00d, 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentEditStockChanged());
    EXPECT_EQ(s_pMainFrame->GetCurrentPos(), 0);
    EXPECT_STREQ(gl_pChinaStockMarket->GetCurrentStock()->GetSymbol(), _T("600604.SS"));
  }

  TEST_F(CMockMainFrameTest, TestOnKeyUp) {
    EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());
    gl_pChinaStockMarket->SetCurrentStock(_T("600000.SS"));
    EXPECT_CALL(*s_pMainFrame, SysCallOnKeyUp(34, 1, 1))
      .Times(1);
    s_pMainFrame->OnKeyUp(34, 1, 1);
    EXPECT_STREQ(gl_pChinaStockMarket->GetCurrentStock()->GetSymbol(), _T("600001.SS"));
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentStockChanged());

    gl_pChinaStockMarket->SetCurrentStockChanged(false);
    EXPECT_CALL(*s_pMainFrame, SysCallOnKeyUp(33, 1, 1))
      .Times(1);
    s_pMainFrame->OnKeyUp(33, 1, 1);
    EXPECT_STREQ(gl_pChinaStockMarket->GetCurrentStock()->GetSymbol(), _T("600000.SS"));
    EXPECT_TRUE(gl_pChinaStockMarket->IsCurrentStockChanged());

    gl_pChinaStockMarket->SetCurrentStockChanged(false);
    gl_pChinaStockMarket->GetCurrentStock()->SetChoiced(false);
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedStockSize(), 0);
    EXPECT_CALL(*s_pMainFrame, SysCallOnKeyUp(45, 1, 1))
      .Times(1);
    s_pMainFrame->OnKeyUp(45, 1, 1);
    EXPECT_TRUE(gl_pChinaStockMarket->GetCurrentStock()->IsChoiced());
    EXPECT_TRUE(gl_pChinaStockMarket->IsUpdateChoicedStockDB());
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedStockSize(), 1);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());

    gl_pChinaStockMarket->SetCurrentStockChanged(false);
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedStockSize(), 1);
    EXPECT_CALL(*s_pMainFrame, SysCallOnKeyUp(46, 1, 1))
      .Times(1);
    s_pMainFrame->OnKeyUp(46, 1, 1);
    EXPECT_FALSE(gl_pChinaStockMarket->GetCurrentStock()->IsChoiced());
    EXPECT_TRUE(gl_pChinaStockMarket->IsUpdateChoicedStockDB());
    EXPECT_EQ(gl_pChinaStockMarket->GetChoicedStockSize(), 0);
    EXPECT_FALSE(gl_pChinaStockMarket->IsCurrentStockChanged());

    // 恢复原状
    gl_pChinaStockMarket->ClearChoiceStockContainer();
    gl_pChinaStockMarket->SetUpdateChoicedStockDB(false);
  }

  TEST_F(CMockMainFrameTest, TestOnBuildResetMarket) {
    gl_pChinaStockMarket->SetResetMarket(false);
    gl_pPotenDailyBriefingMarket->SetResetMarket(false);
    gl_pCrweberIndexMarket->SetResetMarket(false);
    s_pMainFrame->OnBuildResetMarket();
    EXPECT_TRUE(gl_pChinaStockMarket->IsResetMarket());
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
    EXPECT_TRUE(gl_pCrweberIndexMarket->IsResetMarket());
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateRebuildDayLineRS) {
    CCmdUI cmdUI;
    gl_pChinaStockMarket->__TEST_SetFormatedMarketTime((long)83001);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateRebuildDayLineRS(&cmdUI);
    gl_pChinaStockMarket->__TEST_SetFormatedMarketTime((long)92959);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateRebuildDayLineRS(&cmdUI);
    gl_pChinaStockMarket->__TEST_SetFormatedMarketTime((long)83000);
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateRebuildDayLineRS(&cmdUI);
    gl_pChinaStockMarket->__TEST_SetFormatedMarketTime((long)83000);
    gl_ThreadStatus.SetCalculatingDayLineRS(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateRebuildDayLineRS(&cmdUI);
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateRecordRTData) {
    CCmdUI cmdUI;
    gl_pChinaStockMarket->SetRecordRTData(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUISetCheck(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateRecordRTData(&cmdUI);
    gl_pChinaStockMarket->SetRecordRTData(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUISetCheck(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateRecordRTData(&cmdUI);
  }

  TEST_F(CMockMainFrameTest, TestOnAbortBuildingRS) {
    gl_fExitingCalculatingRS = false;
    s_pMainFrame->OnAbortBuindingRS();
    EXPECT_TRUE(gl_fExitingCalculatingRS);

    gl_fExitingCalculatingRS = false;
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateAbortBuindingRS) {
    CCmdUI cmdUI;
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateAbortBuindingRS(&cmdUI);
    gl_ThreadStatus.SetCalculatingDayLineRS(false);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateAbortBuindingRS(&cmdUI);
  }

  TEST_F(CMockMainFrameTest, TestOnRecordRTData) {
    gl_pChinaStockMarket->SetRecordRTData(false);
    s_pMainFrame->OnRecordRTData();
    EXPECT_TRUE(gl_pChinaStockMarket->IsRecordingRTData());
    s_pMainFrame->OnRecordRTData();
    EXPECT_FALSE(gl_pChinaStockMarket->IsRecordingRTData());
  }

  TEST_F(CMockMainFrameTest, TestOnUsingRealtimeDataServer) {
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingSinaRTDataServer());
    s_pMainFrame->OnUsingNeteaseRealtimeDataServer();
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingNeteaseRTDataServer());
    EXPECT_FALSE(gl_pChinaStockMarket->IsUsingSinaRTDataServer());
    s_pMainFrame->OnUsingNeteaseRealtimeDataServer();
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingNeteaseRTDataServer());
    EXPECT_FALSE(gl_pChinaStockMarket->IsUsingSinaRTDataServer());
    s_pMainFrame->OnUsingSinaRealtimeDataServer();
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingSinaRTDataServer());
    EXPECT_FALSE(gl_pChinaStockMarket->IsUsingNeteaseRTDataServer());
    s_pMainFrame->OnUsingSinaRealtimeDataServer();
    EXPECT_TRUE(gl_pChinaStockMarket->IsUsingSinaRTDataServer());
    EXPECT_FALSE(gl_pChinaStockMarket->IsUsingNeteaseRTDataServer());
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateUsingNeteaseRealtimeDataServer) {
    CCmdUI cmdUI;
    gl_pChinaStockMarket->SetUsingNeteaseRTDataServer();
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUISetCheck(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateUsingNeteaseRealtimeDataServer(&cmdUI);
    gl_pChinaStockMarket->SetUsingSinaRTDataServer();
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUISetCheck(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateUsingNeteaseRealtimeDataServer(&cmdUI);
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateUsingSinaRealtimeDataServer) {
    CCmdUI cmdUI;
    gl_pChinaStockMarket->SetUsingSinaRTDataServer();
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUISetCheck(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateUsingSinaRealtimeDataServer(&cmdUI);
    gl_pChinaStockMarket->SetUsingNeteaseRTDataServer();
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUISetCheck(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateUsingSinaRealtimeDataServer(&cmdUI);

    //恢复初态
    gl_pChinaStockMarket->SetUsingSinaRTDataServer();
  }

  TEST_F(CMockMainFrameTest, TestOnUpdateBuildCurrentWeekLine) {
    CCmdUI cmdUI;
    long lTime = gl_pChinaStockMarket->GetFormatedMarketTime();
    gl_pChinaStockMarket->__TEST_SetFormatedMarketTime(150959);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateBuildCurrentWeekLine(&cmdUI);
    gl_pChinaStockMarket->__TEST_SetFormatedMarketTime(151000);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, false))
      .Times(1);
    s_pMainFrame->OnUpdateBuildCurrentWeekLine(&cmdUI);
    gl_pChinaStockMarket->__TEST_SetFormatedMarketTime(151001);
    EXPECT_CALL(*s_pMainFrame, SysCallCmdUIEnable(_, true))
      .Times(1);
    s_pMainFrame->OnUpdateBuildCurrentWeekLine(&cmdUI);
    gl_pChinaStockMarket->__TEST_SetFormatedMarketTime(lTime);
  }
}