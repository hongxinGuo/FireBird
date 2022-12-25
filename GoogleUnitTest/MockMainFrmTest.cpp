#include"pch.h"

#include"ThreadStatus.h"
#include"GeneralCheck.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"MockMainFrm.h"

using namespace testing;
using namespace ::testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	extern CMockMainFrame *gl_pMockMainFrame;

	class CMockMainFrameTest : public ::testing::Test {
	public:
		static void SetUpTestSuite(void) {
			GeneralCheck();

			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_THAT(gl_pMockMainFrame, NotNull());
			EXPECT_EQ(gl_vMarketPtr.size(), 2) << "ChinaMarket and WorldMarket";
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
		}

		static void TearDownTestSuite(void) {
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
			gl_pChinaMarket->ResetCurrentStock();
			gl_pChinaMarket->SetResetMarket(true);
			gl_pChinaMarket->SetCurrentStockChanged(false);
			gl_pChinaMarket->SetCurrentEditStockChanged(false);
			gl_pChinaMarket->SetUpdateOptionDB(false); // 这里使用了实际的数据库，故而不允许更新
			EXPECT_EQ(gl_vMarketPtr.size(), 2);

			GeneralCheck();
		}

		void SetUp(void) override {
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			gl_systemStatus.SetExitingSystem(false);
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
		}

		void TearDown(void) override {
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			gl_pChinaMarket->ResetCurrentStock();
			gl_pChinaMarket->SetCurrentStockChanged(false);
			gl_pChinaMarket->ClearChoiceStockContainer();
			gl_pChinaMarket->SetCalculatingDayLineRS(false);
			gl_systemStatus.SetExitingSystem(false);

			GeneralCheck();
		}
	};

	TEST_F(CMockMainFrameTest, TestFormatToMK) {
		CString str;
		str = gl_pMockMainFrame->FormatToMK(123);
		EXPECT_STREQ(str, _T(" 123"));
		str = gl_pMockMainFrame->FormatToMK(12 * 1024);
		EXPECT_STREQ(str, _T("  12K"));
		str = gl_pMockMainFrame->FormatToMK(12 * 1024 * 1024);
		EXPECT_STREQ(str, _T("  12M"));
		str = gl_pMockMainFrame->FormatToMK((long long)12345 * 1024 * 1024);
		EXPECT_STREQ(str, _T("12345M"));
	}

	TEST_F(CMockMainFrameTest, TestCreateMarketContainer) {
		EXPECT_TRUE(gl_pMockMainFrame->CreateMarketContainer());
		EXPECT_EQ(gl_vMarketPtr.size(), 4);
		CVirtualMarketPtr pMarket;
		pMarket = gl_vMarketPtr.at(3);
		gl_vMarketPtr.pop_back();
		EXPECT_STREQ(pMarket->GetMarketID(), gl_pChinaMarket->GetMarketID());
		pMarket = gl_vMarketPtr.at(2);
		gl_vMarketPtr.pop_back();
		EXPECT_STREQ(pMarket->GetMarketID(), gl_pWorldMarket->GetMarketID());
		EXPECT_EQ(gl_vMarketPtr.size(), 2);
	}

	TEST_F(CMockMainFrameTest, TestUpdateStatus1) {
		InSequence seq;
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		gl_pChinaMarket->SetCurrentStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(4, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(5, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(6, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(7, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(8, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(9, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(11, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(12, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(13, _))
			.Times(1);
		gl_pMockMainFrame->UpdateStatus();
	}

	TEST_F(CMockMainFrameTest, TestUpdateStatus2) {
		gl_pChinaMarket->SetCurrentEditStockChanged(true);
		gl_pChinaMarket->SetCurrentStockChanged(true);
		gl_pChinaMarket->SetCurrentStock(_T("600000.SS"));
		InSequence seq1;
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(1, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(2, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(3, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(4, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(5, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(6, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(7, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(8, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(9, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(11, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(12, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(13, _))
			.Times(1);
		gl_pMockMainFrame->UpdateStatus();
	}

	TEST_F(CMockMainFrameTest, TestUpdateInnerSystemStatus) {
		gl_systemMessage.SetProcessedFinnhubWebSocket(1);
		gl_systemMessage.SetProcessedTiingoIEXWebSocket(1);
		gl_systemMessage.SetProcessedTiingoCryptoWebSocket(1);
		gl_systemMessage.SetProcessedTiingoForexWebSocket(1);
		InSequence seq;
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(1, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(2, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(3, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(4, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(5, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(6, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(7, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(8, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(9, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(10, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(11, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(12, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(13, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(14, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(15, _))
			.Times(1);
		gl_pMockMainFrame->UpdateInnerSystemStatus();

		// 恢复原状
		gl_systemMessage.SetProcessedFinnhubWebSocket(0);
		gl_systemMessage.SetProcessedTiingoIEXWebSocket(0);
		gl_systemMessage.SetProcessedTiingoCryptoWebSocket(0);
		gl_systemMessage.SetProcessedTiingoForexWebSocket(0);
	}

	TEST_F(CMockMainFrameTest, TestOnSysCommand) {
		gl_systemStatus.SetExitingSystem(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnSysCommand)
			.Times(1);
		gl_pMockMainFrame->OnSysCommand(SC_CLOSE, 0);
		EXPECT_TRUE(gl_systemStatus.IsExitingSystem()) << "关闭系统时设置此标识";
		gl_systemStatus.SetExitingSystem(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnSysCommand)
			.Times(1);
		gl_pMockMainFrame->OnSysCommand(0xFFFF ^ SC_CLOSE, 0);
		EXPECT_FALSE(gl_systemStatus.IsExitingSystem()) << "其他系统命令不影响此标识";
		gl_systemStatus.SetExitingSystem(false);
	}

	TEST_F(CMockMainFrameTest, TestOnCalculateTodayRS) {
		EXPECT_CALL(*gl_pMockMainFrame, CalculateTodayRS)
			.Times(1);
		gl_pMockMainFrame->OnCalculateTodayRS();
	}

	TEST_F(CMockMainFrameTest, TestOnProcessTodayStock) {
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(*gl_pMockMainFrame, ProcessChinaMarketStock)
			.Times(1);
		gl_pMockMainFrame->OnProcessTodayStock();
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateProcessTodayStock) {
		CCmdUI cmdUI;
		gl_pChinaMarket->SetSystemReady(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false))
			.Times(1);
		gl_pMockMainFrame->OnUpdateProcessTodayStock(&cmdUI);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true))
			.Times(1);
		gl_pMockMainFrame->OnUpdateProcessTodayStock(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateCalculateTodayRS) {
		CCmdUI cmdUI;
		gl_pChinaMarket->SetSystemReady(false);
		gl_pChinaMarket->SetCalculatingDayLineRS(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateCalculateTodayRS(&cmdUI);
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->SetCalculatingDayLineRS(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateCalculateTodayRS(&cmdUI);
		gl_pChinaMarket->SetSystemReady(false);
		gl_pChinaMarket->SetCalculatingDayLineRS(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateCalculateTodayRS(&cmdUI);
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->SetCalculatingDayLineRS(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false));
		gl_pMockMainFrame->OnUpdateCalculateTodayRS(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestPreTranslateMessage) {
		MSG msg;
		msg.message = WM_KEYDOWN;
		EXPECT_CALL(*gl_pMockMainFrame, SysCallPreTranslateMessage(&msg))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSendMessage(_, _, _))
			.Times(0);
		gl_pMockMainFrame->PreTranslateMessage(&msg);

		msg.message = WM_KEYUP;
		msg.wParam = 201;
		msg.lParam = 102;
		EXPECT_CALL(*gl_pMockMainFrame, SysCallPreTranslateMessage(&msg))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSendMessage(WM_KEYUP, 201, 102))
			.Times(1);
		gl_pMockMainFrame->PreTranslateMessage(&msg);

		msg.message = WM_CHAR;
		msg.wParam = 200;
		msg.lParam = 100;
		EXPECT_CALL(*gl_pMockMainFrame, SysCallPreTranslateMessage(&msg))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSendMessage(WM_CHAR, 200, 100))
			.Times(1);
		gl_pMockMainFrame->PreTranslateMessage(&msg);
	}

	TEST_F(CMockMainFrameTest, TestOnChar) {
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 0);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('6', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('6', 1, 1);
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 2);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('1', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('1', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 3);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('8', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('8', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 4);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('7', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('7', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 5);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('8', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('8', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 6);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar(0x008, 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar(0x008, 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 5);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('2', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('2', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 6);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('.', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('.', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 7);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('S', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('S', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 8);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('s', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('s', 1, 1); // 小写s
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 9);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar(0x00d, 1, 1))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallInvalidate())
			.Times(1);
		gl_pMockMainFrame->OnChar(0x00d, 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 0);
		EXPECT_STREQ(gl_pChinaMarket->GetCurrentStock()->GetSymbol(), _T("601872.SS")) << "小写的s转换为大写";
	}

	TEST_F(CMockMainFrameTest, TestOnChar2) {
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 0);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar(0x008, 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar(0x008, 1, 1);
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 0);
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentEditStockChanged());
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 2);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 3);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 4);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 5);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('1', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('1', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 6);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('.', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('.', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 7);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('S', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('S', 1, 1); // 小写s
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 8);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('z', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('z', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 9);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		gl_pChinaMarket->SetCurrentStock(_T("600604.SS"));
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar(0x00d, 1, 1))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallInvalidate())
			.Times(1);
		gl_pMockMainFrame->OnChar(0x00d, 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 0);
		EXPECT_STREQ(gl_pChinaMarket->GetCurrentStock()->GetSymbol(), _T("000001.SZ")) << "小写的z转换为大写";
	}

	TEST_F(CMockMainFrameTest, TestOnChar3) {
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 0);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar(0x008, 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar(0x008, 1, 1);
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 0);
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentEditStockChanged());
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 2);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 3);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('2', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('2', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 4);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('8', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('8', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 5);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('9', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('9', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 6);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('.', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('.', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 7);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('S', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('S', 1, 1); // 小写s
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 8);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('z', 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnChar('z', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 9);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		gl_pChinaMarket->SetCurrentStock(_T("600604.SS"));
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar(0x00d, 1, 1))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallInvalidate())
			.Times(0); // 没有更新
		gl_pMockMainFrame->OnChar(0x00d, 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 0);
		EXPECT_STREQ(gl_pChinaMarket->GetCurrentStock()->GetSymbol(), _T("600604.SS")) << "000289.SZ为无效股票代码，故而没有设置新代码";
	}

	TEST_F(CMockMainFrameTest, TestOnKeyUp) {
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
		gl_pChinaMarket->SetCurrentStock(_T("600000.SS"));
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnKeyUp(34, 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnKeyUp(34, 1, 1);
		EXPECT_STREQ(gl_pChinaMarket->GetCurrentStock()->GetSymbol(), _T("600001.SS"));
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentStockChanged());

		gl_pChinaMarket->SetCurrentStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnKeyUp(33, 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnKeyUp(33, 1, 1);
		EXPECT_STREQ(gl_pChinaMarket->GetCurrentStock()->GetSymbol(), _T("600000.SS"));
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentStockChanged());

		gl_pChinaMarket->SetCurrentStockChanged(false);
		gl_pChinaMarket->GetCurrentStock()->SetChosen(false);
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 0);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnKeyUp(45, 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnKeyUp(45, 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->GetCurrentStock()->IsChosen());
		EXPECT_TRUE(gl_pChinaMarket->IsUpdateChosenStockDB());
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 1);
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

		gl_pChinaMarket->SetCurrentStockChanged(false);
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnKeyUp(46, 1, 1))
			.Times(1);
		gl_pMockMainFrame->OnKeyUp(46, 1, 1);
		EXPECT_FALSE(gl_pChinaMarket->GetCurrentStock()->IsChosen());
		EXPECT_TRUE(gl_pChinaMarket->IsUpdateChosenStockDB());
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 0);
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

		// 恢复原状
		gl_pChinaMarket->ClearChoiceStockContainer();
		gl_pChinaMarket->SetUpdateChosenStockDB(false);
	}

	TEST_F(CMockMainFrameTest, TestOnBuildResetMarket) {
		gl_pChinaMarket->SetResetMarket(false);
		gl_pMockMainFrame->OnBuildResetMarket();
		EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateRebuildDayLineRS) {
		CCmdUI cmdUI;
		gl_pChinaMarket->TEST_SetFormattedMarketTime((long)83001);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false))
			.Times(1);
		gl_pMockMainFrame->OnUpdateRebuildDayLineRS(&cmdUI);
		gl_pChinaMarket->TEST_SetFormattedMarketTime((long)92959);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false))
			.Times(1);
		gl_pMockMainFrame->OnUpdateRebuildDayLineRS(&cmdUI);
		gl_pChinaMarket->TEST_SetFormattedMarketTime((long)83000);
		gl_pChinaMarket->SetCalculatingDayLineRS(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false))
			.Times(1);
		gl_pMockMainFrame->OnUpdateRebuildDayLineRS(&cmdUI);
		gl_pChinaMarket->TEST_SetFormattedMarketTime((long)83000);
		gl_pChinaMarket->SetCalculatingDayLineRS(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true))
			.Times(1);
		gl_pMockMainFrame->OnUpdateRebuildDayLineRS(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateRecordRTData) {
		CCmdUI cmdUI;
		gl_pChinaMarket->SetRecordRTData(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, true))
			.Times(1);
		gl_pMockMainFrame->OnUpdateRecordRTData(&cmdUI);
		gl_pChinaMarket->SetRecordRTData(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, false))
			.Times(1);
		gl_pMockMainFrame->OnUpdateRecordRTData(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnAbortBuildingRS) {
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_pMockMainFrame->OnAbortBuildingRS();
		EXPECT_TRUE(gl_systemStatus.IsExitingCalculatingRS());

		gl_systemStatus.SetExitingCalculatingRS(false);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateAbortBuindingRS) {
		CCmdUI cmdUI;
		gl_pChinaMarket->SetCalculatingDayLineRS(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateAbortBuildingRS(&cmdUI);
		gl_pChinaMarket->SetCalculatingDayLineRS(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateAbortBuildingRS(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnRecordRTData) {
		gl_pChinaMarket->SetRecordRTData(false);
		gl_pMockMainFrame->OnRecordRTData();
		EXPECT_TRUE(gl_pChinaMarket->IsRecordingRTData());
		gl_pMockMainFrame->OnRecordRTData();
		EXPECT_FALSE(gl_pChinaMarket->IsRecordingRTData());
	}

	TEST_F(CMockMainFrameTest, TestOnUsingRealtimeDataServer) {
		EXPECT_TRUE(gl_systemConfiguration.IsUsingSinaRTServer());
		gl_pMockMainFrame->OnUsingNeteaseRealtimeDataServer();
		EXPECT_TRUE(gl_systemConfiguration.IsUsingNeteaseRTServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingSinaRTServer());
		gl_pMockMainFrame->OnUsingNeteaseRealtimeDataServer();
		EXPECT_TRUE(gl_systemConfiguration.IsUsingNeteaseRTServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingSinaRTServer());
		gl_pMockMainFrame->OnUsingSinaRealtimeDataServer();
		EXPECT_TRUE(gl_systemConfiguration.IsUsingSinaRTServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingNeteaseRTServer());
		gl_pMockMainFrame->OnUsingSinaRealtimeDataServer();
		EXPECT_TRUE(gl_systemConfiguration.IsUsingSinaRTServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingNeteaseRTServer());
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateUsingNeteaseRealtimeDataServer) {
		CCmdUI cmdUI;
		gl_systemConfiguration.SetChinaMarketRealtimeServer(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, true))
			.Times(1);
		gl_pMockMainFrame->OnUpdateUsingNeteaseRealtimeDataServer(&cmdUI);
		gl_systemConfiguration.SetChinaMarketRealtimeServer(0);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, false))
			.Times(1);
		gl_pMockMainFrame->OnUpdateUsingNeteaseRealtimeDataServer(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateUsingSinaRealtimeDataServer) {
		CCmdUI cmdUI;
		gl_systemConfiguration.SetChinaMarketRealtimeServer(0);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, true))
			.Times(1);
		gl_pMockMainFrame->OnUpdateUsingSinaRealtimeDataServer(&cmdUI);
		gl_systemConfiguration.SetChinaMarketRealtimeServer(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, false))
			.Times(1);
		gl_pMockMainFrame->OnUpdateUsingSinaRealtimeDataServer(&cmdUI);

		//恢复初态
		gl_systemConfiguration.SetChinaMarketRealtimeServer(0);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateBuildCurrentWeekLine) {
		CCmdUI cmdUI;
		long lTime = gl_pChinaMarket->GetMarketTime();
		gl_pChinaMarket->TEST_SetFormattedMarketTime(150959);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateBuildCurrentWeekLine(&cmdUI);
		gl_pChinaMarket->TEST_SetFormattedMarketTime(151000);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateBuildCurrentWeekLine(&cmdUI);
		gl_pChinaMarket->TEST_SetFormattedMarketTime(151001);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateBuildCurrentWeekLine(&cmdUI);
		gl_pChinaMarket->TEST_SetFormattedMarketTime(lTime);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateCalculate10dayRS) {
		CCmdUI cmdUI;
		EXPECT_THAT(gl_pChinaMarket->IsDayLineDBUpdated(), IsFalse());
		EXPECT_THAT(gl_pChinaMarket->IsDayLineNeedSaving(), IsFalse());
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateCalculate10dayRS(&cmdUI);

		auto pStock = gl_pChinaMarket->GetStock(1);
		pStock->SetDayLineDBUpdated(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateCalculate10dayRS(&cmdUI);
		pStock->SetDayLineNeedSaving(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateCalculate10dayRS(&cmdUI);

		pStock->SetDayLineDBUpdated(false);
		pStock->SetDayLineNeedSaving(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateCalculate10dayRS(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateCalculate10dayRS1) {
		CCmdUI cmdUI;
		EXPECT_THAT(gl_pChinaMarket->IsDayLineDBUpdated(), IsFalse());
		EXPECT_THAT(gl_pChinaMarket->IsDayLineNeedSaving(), IsFalse());
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateCalculate10dayRS1(&cmdUI);

		auto pStock = gl_pChinaMarket->GetStock(1);
		pStock->SetDayLineDBUpdated(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateCalculate10dayRS1(&cmdUI);
		pStock->SetDayLineNeedSaving(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateCalculate10dayRS1(&cmdUI);

		pStock->SetDayLineDBUpdated(false);
		pStock->SetDayLineNeedSaving(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateCalculate10dayRS1(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateCalculate10dayRS2) {
		CCmdUI cmdUI;
		EXPECT_THAT(gl_pChinaMarket->IsDayLineDBUpdated(), IsFalse());
		EXPECT_THAT(gl_pChinaMarket->IsDayLineNeedSaving(), IsFalse());
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateCalculate10dayRS2(&cmdUI);

		auto pStock = gl_pChinaMarket->GetStock(1);
		pStock->SetDayLineDBUpdated(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateCalculate10dayRS2(&cmdUI);
		pStock->SetDayLineNeedSaving(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateCalculate10dayRS2(&cmdUI);

		pStock->SetDayLineDBUpdated(false);
		pStock->SetDayLineNeedSaving(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true))
			.Times(1)
			.RetiresOnSaturation();
		gl_pMockMainFrame->OnUpdateCalculate10dayRS2(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnTimer) {
		gl_pChinaMarket->SetCurrentStockChanged(false);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		gl_systemMessage.SetProcessedFinnhubWebSocket(1);
		gl_systemMessage.SetProcessedTiingoIEXWebSocket(1);
		gl_systemMessage.SetProcessedTiingoCryptoWebSocket(1);
		gl_systemMessage.SetProcessedTiingoForexWebSocket(1);

		EXPECT_CALL(*gl_pMockMainFrame, ResetMarket())
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SchedulingTask())
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnTimer(_STOCK_ANALYSIS_TIMER_))
			.Times(1);

		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(4, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(5, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(6, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(7, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(8, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(9, _))
			.Times(1);

		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(11, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(12, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(13, _))
			.Times(1);

		InSequence seq;
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(1, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(2, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(3, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(4, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(5, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(6, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(7, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(8, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(9, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(10, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(11, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(12, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(13, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(14, _))
			.Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(15, _))
			.Times(1);

		gl_pMockMainFrame->OnTimer(_STOCK_ANALYSIS_TIMER_);

		EXPECT_THAT(gl_systemMessage.InformationSize(), 1);
		gl_systemMessage.PopInformationMessage();

		// 恢复原状
		gl_systemMessage.SetProcessedFinnhubWebSocket(0);
		gl_systemMessage.SetProcessedTiingoIEXWebSocket(0);
		gl_systemMessage.SetProcessedTiingoCryptoWebSocket(0);
		gl_systemMessage.SetProcessedTiingoForexWebSocket(0);
	}
}
