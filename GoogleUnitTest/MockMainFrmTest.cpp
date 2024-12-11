#include"pch.h"

#include"GeneralCheck.h"
import FireBird.Market.China;
#include "MockMainFrm.h"

import FireBird.System.Message;
import FireBird.WindowInterface.MainFrm;
import FireBird.DataSource.NeteaseRT;
import FireBird.DataSource.TengxunRT;

using namespace testing;

namespace FireBirdTest {
	extern CMockMainFramePtr gl_pMockMainFrame;

	class CMockMainFrameTest : public Test {
	public:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();

			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_THAT(gl_pMockMainFrame, NotNull());
			EXPECT_EQ(gl_vMarket.size(), 2) << "ChinaMarket and WorldMarket";
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
		}

		static void TearDownTestSuite() {
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
			gl_pChinaMarket->ClearCurrentStock();
			gl_pChinaMarket->SetCurrentStockChanged(false);
			gl_pChinaMarket->SetCurrentEditStockChanged(false);
			gl_pChinaMarket->SetUpdateOptionDB(false); // 这里使用了实际的数据库，故而不允许更新
			EXPECT_EQ(gl_vMarket.size(), 2);

			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			gl_systemConfiguration.SetExitingSystem(false);
			EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
		}

		void TearDown() override {
			EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			gl_pChinaMarket->ClearCurrentStock();
			gl_pChinaMarket->SetCurrentStockChanged(false);
			gl_pChinaMarket->ClearChoiceStockContainer();
			gl_pChinaMarket->SetCalculatingDayLineRS(false);
			gl_systemConfiguration.SetExitingSystem(false);
			gl_systemConfiguration.SetUpdateDB(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CMockMainFrameTest, TestUpdateStatus1) {
		EXPECT_FALSE(gl_pChinaMarket->IsResetting()) << "默认状态";
		InSequence seq;
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		gl_pChinaMarket->SetCurrentStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(4, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(5, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(6, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(7, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(8, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(9, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(10, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(11, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(12, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(13, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(14, _)).Times(1);
		gl_pMockMainFrame->UpdateStatus();
	}

	TEST_F(CMockMainFrameTest, TestUpdateStatus2) {
		EXPECT_FALSE(gl_pChinaMarket->IsResetting()) << "默认状态";
		gl_pChinaMarket->SetCurrentEditStockChanged(true);
		gl_pChinaMarket->SetCurrentStockChanged(true);
		gl_pChinaMarket->SetCurrentStock(_T("600000.SS"));
		InSequence seq1;
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(1, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(2, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(3, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(4, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(5, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(6, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(7, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(8, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(9, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(10, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(11, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(12, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(13, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetPaneText(14, _)).Times(1);
		gl_pMockMainFrame->UpdateStatus();

		//恢复原状
		while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardCurrentMarketTask();
	}

	TEST_F(CMockMainFrameTest, TestUpdateInnerSystemStatus) {
		EXPECT_FALSE(gl_pChinaMarket->IsResetting()) << "默认状态";
		gl_systemMessage.SetProcessedFinnhubWebSocket(1);
		gl_systemMessage.SetProcessedTiingoIEXWebSocket(1);
		gl_systemMessage.SetProcessedTiingoCryptoWebSocket(1);
		gl_systemMessage.SetProcessedTiingoForexWebSocket(1);
		InSequence seq;
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(1, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(2, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(3, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(4, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(5, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(6, _)).Times(1);
		//EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(7, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(8, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(9, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(10, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(11, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(12, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(13, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(14, _)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSetInnerSystemPaneText(15, _)).Times(1);
		gl_pMockMainFrame->UpdateInnerSystemStatus();

		// 恢复原状
		gl_systemMessage.SetProcessedFinnhubWebSocket(0);
		gl_systemMessage.SetProcessedTiingoIEXWebSocket(0);
		gl_systemMessage.SetProcessedTiingoCryptoWebSocket(0);
		gl_systemMessage.SetProcessedTiingoForexWebSocket(0);
	}

	TEST_F(CMockMainFrameTest, TestOnSysCommand) {
		gl_systemConfiguration.SetExitingSystem(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnSysCommand).Times(1);
		gl_pMockMainFrame->OnSysCommand(SC_CLOSE, 0);
		EXPECT_TRUE(gl_systemConfiguration.IsExitingSystem()) << "关闭系统时设置此标识";
		gl_systemConfiguration.SetExitingSystem(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnSysCommand).Times(1);
		gl_pMockMainFrame->OnSysCommand(0xFFFF ^ SC_CLOSE, 0);
		EXPECT_FALSE(gl_systemConfiguration.IsExitingSystem()) << "其他系统命令不影响此标识";
		gl_systemConfiguration.SetExitingSystem(false);
	}

	TEST_F(CMockMainFrameTest, TestOnCalculateTodayRS) {
		EXPECT_CALL(*gl_pMockMainFrame, CalculateTodayRS).Times(1);
		gl_pMockMainFrame->OnCalculateTodayRS();
	}

	TEST_F(CMockMainFrameTest, TestOnProcessTodayStock) {
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(*gl_pMockMainFrame, ProcessChinaMarketStock).Times(1);
		gl_pMockMainFrame->OnProcessTodayStock();
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateProcessTodayStock) {
		CCmdUI cmdUI;
		gl_pChinaMarket->SetSystemReady(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateProcessTodayStock(&cmdUI);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateProcessTodayStock(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateRecordFinnhubWebSocket) {
		EXPECT_TRUE(gl_systemConfiguration.IsUsingFinnhubWebSocket());
		CCmdUI cmdUI;
		gl_systemConfiguration.SetUsingFinnhubWebSocket(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateRecordFinnhubWebSocket(&cmdUI);
		gl_systemConfiguration.SetUsingFinnhubWebSocket(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, true)).Times(1);

		gl_pMockMainFrame->OnUpdateRecordFinnhubWebSocket(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateRecordTiingoIEXWebSocket) {
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoIEXWebSocket());
		CCmdUI cmdUI;
		gl_systemConfiguration.SetUsingTiingoIEXWebSocket(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateRecordTiingoIEXWebSocket(&cmdUI);
		gl_systemConfiguration.SetUsingTiingoIEXWebSocket(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, true)).Times(1);

		gl_pMockMainFrame->OnUpdateRecordTiingoIEXWebSocket(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateRecordTiingoForexWebSocket) {
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoForexWebSocket());
		CCmdUI cmdUI;
		gl_systemConfiguration.SetUsingTiingoForexWebSocket(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateRecordTiingoForexWebSocket(&cmdUI);
		gl_systemConfiguration.SetUsingTiingoForexWebSocket(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, true)).Times(1);

		gl_pMockMainFrame->OnUpdateRecordTiingoForexWebSocket(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateRecordTiingoCryptoWebSocket) {
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoCryptoWebSocket());
		CCmdUI cmdUI;
		gl_systemConfiguration.SetUsingTiingoCryptoWebSocket(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateRecordTiingoCryptoWebSocket(&cmdUI);
		gl_systemConfiguration.SetUsingTiingoCryptoWebSocket(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, true)).Times(1);

		gl_pMockMainFrame->OnUpdateRecordTiingoCryptoWebSocket(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateCalculateTodayRS) {
		CCmdUI cmdUI;
		gl_pChinaMarket->SetSystemReady(false);
		gl_pChinaMarket->SetCalculatingDayLineRS(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateCalculateTodayRS(&cmdUI);
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->SetCalculatingDayLineRS(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateCalculateTodayRS(&cmdUI);
		gl_pChinaMarket->SetSystemReady(false);
		gl_pChinaMarket->SetCalculatingDayLineRS(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateCalculateTodayRS(&cmdUI);
		gl_pChinaMarket->SetSystemReady(true);
		gl_pChinaMarket->SetCalculatingDayLineRS(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false));
		gl_pMockMainFrame->OnUpdateCalculateTodayRS(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestPreTranslateMessage) {
		MSG msg;
		msg.message = WM_KEYDOWN;
		EXPECT_CALL(*gl_pMockMainFrame, SysCallPreTranslateMessage(&msg)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSendMessage(_, _, _)).Times(0);
		gl_pMockMainFrame->PreTranslateMessage(&msg);

		msg.message = WM_KEYUP;
		msg.wParam = 201;
		msg.lParam = 102;
		EXPECT_CALL(*gl_pMockMainFrame, SysCallPreTranslateMessage(&msg)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSendMessage(WM_KEYUP, 201, 102)).Times(1);
		gl_pMockMainFrame->PreTranslateMessage(&msg);

		msg.message = WM_CHAR;
		msg.wParam = 200;
		msg.lParam = 100;
		EXPECT_CALL(*gl_pMockMainFrame, SysCallPreTranslateMessage(&msg)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallSendMessage(WM_CHAR, 200, 100)).Times(1);
		gl_pMockMainFrame->PreTranslateMessage(&msg);
	}

	TEST_F(CMockMainFrameTest, TestOnChar) {
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 0);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('6', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('6', 1, 1);
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 2);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('1', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('1', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 3);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('8', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('8', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 4);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('7', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('7', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 5);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('8', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('8', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 6);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar(0x008, 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar(0x008, 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 5);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('2', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('2', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 6);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('.', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('.', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 7);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('S', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('S', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 8);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('s', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('s', 1, 1); // 小写s
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 9);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar(0x00d, 1, 1)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallInvalidate()).Times(1);
		gl_pMockMainFrame->OnChar(0x00d, 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 0);
		EXPECT_STREQ(gl_pChinaMarket->GetCurrentStock()->GetSymbol(), _T("601872.SS")) << "小写的s转换为大写";

		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		//恢复原状
		while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardCurrentMarketTask();
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
	}

	TEST_F(CMockMainFrameTest, TestOnChar2) {
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 0);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar(0x008, 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar(0x008, 1, 1);
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 0);
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentEditStockChanged());
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 2);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 3);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 4);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 5);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('1', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('1', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 6);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('.', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('.', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 7);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('S', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('S', 1, 1); // 小写s
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 8);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('z', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('z', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 9);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		gl_pChinaMarket->SetCurrentStock(_T("600604.SS"));
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar(0x00d, 1, 1)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallInvalidate()).Times(1);
		gl_pMockMainFrame->OnChar(0x00d, 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 0);
		EXPECT_STREQ(gl_pChinaMarket->GetCurrentStock()->GetSymbol(), _T("000001.SZ")) << "小写的z转换为大写";

		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		//恢复原状
		while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardCurrentMarketTask();
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
	}

	TEST_F(CMockMainFrameTest, TestOnChar3) {
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 0);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar(0x008, 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar(0x008, 1, 1);
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 0);
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentEditStockChanged());
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 2);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('0', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('0', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 3);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('2', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('2', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 4);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('8', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('8', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 5);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('9', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('9', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 6);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('.', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('.', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 7);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('S', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('S', 1, 1); // 小写s
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 8);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar('z', 1, 1)).Times(1);
		gl_pMockMainFrame->OnChar('z', 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 9);
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
		gl_pChinaMarket->SetCurrentStock(_T("600604.SS"));
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnChar(0x00d, 1, 1)).Times(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallInvalidate()).Times(0); // 没有更新
		gl_pMockMainFrame->OnChar(0x00d, 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		EXPECT_EQ(gl_pMockMainFrame->GetCurrentPos(), 0);
		EXPECT_STREQ(gl_pChinaMarket->GetCurrentStock()->GetSymbol(), _T("600604.SS")) << "000289.SZ为无效股票代码，故而没有设置新代码";

		EXPECT_TRUE(gl_pChinaMarket->IsCurrentEditStockChanged());
		//恢复原状
		while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardCurrentMarketTask();
		gl_pChinaMarket->SetCurrentEditStockChanged(false);
	}

	TEST_F(CMockMainFrameTest, TestOnKeyUp) {
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());
		gl_pChinaMarket->SetCurrentStock(_T("600000.SS"));
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnKeyUp(34, 1, 1)).Times(1);
		gl_pMockMainFrame->OnKeyUp(34, 1, 1);
		EXPECT_STREQ(gl_pChinaMarket->GetCurrentStock()->GetSymbol(), _T("600001.SS"));
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentStockChanged());

		gl_pChinaMarket->SetCurrentStockChanged(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnKeyUp(33, 1, 1)).Times(1);
		gl_pMockMainFrame->OnKeyUp(33, 1, 1);
		EXPECT_STREQ(gl_pChinaMarket->GetCurrentStock()->GetSymbol(), _T("600000.SS"));
		EXPECT_TRUE(gl_pChinaMarket->IsCurrentStockChanged());

		gl_pChinaMarket->SetCurrentStockChanged(false);
		gl_pChinaMarket->GetCurrentStock()->SetChosen(false);
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 0);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnKeyUp(45, 1, 1)).Times(1);
		gl_pMockMainFrame->OnKeyUp(45, 1, 1);
		EXPECT_TRUE(gl_pChinaMarket->GetCurrentStock()->IsChosen());
		EXPECT_TRUE(gl_pChinaMarket->IsUpdateChosenStockDB());
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 1);
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

		gl_pChinaMarket->SetCurrentStockChanged(false);
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallOnKeyUp(46, 1, 1)).Times(1);
		gl_pMockMainFrame->OnKeyUp(46, 1, 1);
		EXPECT_FALSE(gl_pChinaMarket->GetCurrentStock()->IsChosen());
		EXPECT_TRUE(gl_pChinaMarket->IsUpdateChosenStockDB());
		EXPECT_EQ(gl_pChinaMarket->GetChosenStockSize(), 0);
		EXPECT_FALSE(gl_pChinaMarket->IsCurrentStockChanged());

		//恢复原状
		while (!gl_pChinaMarket->IsMarketTaskEmpty()) gl_pChinaMarket->DiscardCurrentMarketTask();
		gl_pChinaMarket->ClearChoiceStockContainer();
		gl_pChinaMarket->SetUpdateChosenStockDB(false);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateRebuildDayLineRS) {
		CCmdUI cmdUI;
		gl_pChinaMarket->TEST_SetFormattedMarketTime(83001);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateRebuildChinaMarketStockDayLineRS(&cmdUI);
		gl_pChinaMarket->TEST_SetFormattedMarketTime(92959);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateRebuildChinaMarketStockDayLineRS(&cmdUI);
		gl_pChinaMarket->TEST_SetFormattedMarketTime(83000);
		gl_pChinaMarket->SetCalculatingDayLineRS(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateRebuildChinaMarketStockDayLineRS(&cmdUI);
		gl_pChinaMarket->TEST_SetFormattedMarketTime(83000);
		gl_pChinaMarket->SetCalculatingDayLineRS(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true)).Times(1);

		gl_pMockMainFrame->OnUpdateRebuildChinaMarketStockDayLineRS(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnAbortBuildingRS) {
		gl_systemConfiguration.SetExitingCalculatingRS(false);
		gl_pMockMainFrame->OnAbortBuildingRS();
		EXPECT_TRUE(gl_systemConfiguration.IsExitingCalculatingRS());

		gl_systemConfiguration.SetExitingCalculatingRS(false);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateAbortBuindingRS) {
		CCmdUI cmdUI;
		gl_pChinaMarket->SetCalculatingDayLineRS(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateAbortBuildingRS(&cmdUI);
		gl_pChinaMarket->SetCalculatingDayLineRS(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateAbortBuildingRS(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateMaintainDayLine) {
		CCmdUI cmdUI;
		constexpr tm tm_{ 0, 0, 0, 0, 0, 0, 4 }; // 星期四
		gl_pChinaMarket->TEST_SetMarketTM(tm_);
		gl_pChinaMarket->TEST_SetFormattedMarketTime(180000); // DummyTime
		EXPECT_TRUE(gl_pChinaMarket->IsDummyTime());
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateMaintainChinaMarketStockDayLine(&cmdUI);

		gl_pChinaMarket->TEST_SetFormattedMarketTime(110000); // WorkingTime
		EXPECT_FALSE(gl_pChinaMarket->IsDummyTime());
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateMaintainChinaMarketStockDayLine(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnUsingRealtimeDataServer) {
		EXPECT_TRUE(gl_systemConfiguration.IsUsingSinaRTServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingNeteaseRTServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingTengxunRTServer());

		gl_pMockMainFrame->OnUsingNeteaseRealtimeDataServer();
		EXPECT_TRUE(gl_systemConfiguration.IsUsingNeteaseRTServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingSinaRTServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingTengxunRTServer());

		gl_pMockMainFrame->OnUsingTengxunRealtimeDataServer();
		EXPECT_FALSE(gl_systemConfiguration.IsUsingSinaRTServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingNeteaseRTServer());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTengxunRTServer());

		gl_pMockMainFrame->OnUsingSinaRealtimeDataServer();
		EXPECT_TRUE(gl_systemConfiguration.IsUsingSinaRTServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingNeteaseRTServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingTengxunRTServer());

		gl_pNeteaseRTDataSource->Enable(true);
		gl_pTengxunRTDataSource->Enable(true);
	}

	TEST_F(CMockMainFrameTest, TestOnUsingDayLineDataServer) {
		EXPECT_FALSE(gl_systemConfiguration.IsUsingTengxunDayLineServer());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingNeteaseDayLineServer());
		gl_pMockMainFrame->OnUsingTengxunDayLineDataServer();
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTengxunDayLineServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingNeteaseRTServer());
		gl_pMockMainFrame->OnUsingNeteaseDayLineDataServer();
		EXPECT_TRUE(gl_systemConfiguration.IsUsingNeteaseDayLineServer());
		EXPECT_FALSE(gl_systemConfiguration.IsUsingTengxunDayLineServer());
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateUsingTengxunRealtimeDataServer) {
		CCmdUI cmdUI;
		gl_systemConfiguration.SetChinaMarketRealtimeServer(2);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateUsingTengxunRealtimeDataServer(&cmdUI);
		gl_systemConfiguration.SetChinaMarketRealtimeServer(0);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateUsingTengxunRealtimeDataServer(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateUsingNeteaseRealtimeDataServer) {
		CCmdUI cmdUI;
		gl_systemConfiguration.SetChinaMarketRealtimeServer(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateUsingNeteaseRealtimeDataServer(&cmdUI);
		gl_systemConfiguration.SetChinaMarketRealtimeServer(0);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateUsingNeteaseRealtimeDataServer(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateUsingSinaRealtimeDataServer) {
		CCmdUI cmdUI;
		gl_systemConfiguration.SetChinaMarketRealtimeServer(0);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateUsingSinaRealtimeDataServer(&cmdUI);
		gl_systemConfiguration.SetChinaMarketRealtimeServer(1);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateUsingSinaRealtimeDataServer(&cmdUI);

		//恢复初态
		gl_systemConfiguration.SetChinaMarketRealtimeServer(0);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateBuildCurrentWeekLine) {
		CCmdUI cmdUI;
		const long lTime = gl_pChinaMarket->GetMarketTime();
		gl_pChinaMarket->TEST_SetFormattedMarketTime(150959);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateBuildCurrentWeekLine(&cmdUI);
		gl_pChinaMarket->TEST_SetFormattedMarketTime(151000);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateBuildCurrentWeekLine(&cmdUI);
		gl_pChinaMarket->TEST_SetFormattedMarketTime(151001);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateBuildCurrentWeekLine(&cmdUI);
		gl_pChinaMarket->TEST_SetFormattedMarketTime(lTime);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateCalculate10dayRS) {
		CCmdUI cmdUI;
		EXPECT_THAT(gl_dataContainerChinaStock.IsDayLineDBUpdated(), IsFalse());
		EXPECT_THAT(gl_dataContainerChinaStock.IsUpdateDayLineDB(), IsFalse());
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateCalculate10dayRS(&cmdUI);

		const auto pStock = gl_dataContainerChinaStock.GetStock(1);
		pStock->SetDayLineDBUpdated(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateCalculate10dayRS(&cmdUI);
		pStock->SetUpdateDayLineDB(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateCalculate10dayRS(&cmdUI);

		pStock->SetDayLineDBUpdated(false);
		pStock->SetUpdateDayLineDB(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateCalculate10dayRS(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateCalculate10dayRS1) {
		CCmdUI cmdUI;
		EXPECT_THAT(gl_dataContainerChinaStock.IsDayLineDBUpdated(), IsFalse());
		EXPECT_THAT(gl_dataContainerChinaStock.IsUpdateDayLineDB(), IsFalse());
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateCalculate10dayRS1(&cmdUI);

		const auto pStock = gl_dataContainerChinaStock.GetStock(1);
		pStock->SetDayLineDBUpdated(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateCalculate10dayRS1(&cmdUI);
		pStock->SetUpdateDayLineDB(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateCalculate10dayRS1(&cmdUI);

		pStock->SetDayLineDBUpdated(false);
		pStock->SetUpdateDayLineDB(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateCalculate10dayRS1(&cmdUI);
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateUsingNeteaseDayLineDataServer) {
		CCmdUI cmdUI;
		EXPECT_TRUE(gl_systemConfiguration.IsUsingNeteaseDayLineServer());

		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(&cmdUI, true)).Times(1);

		gl_pMockMainFrame->OnUpdateUsingNeteaseDayLineDataServer(&cmdUI);

		gl_systemConfiguration.SetChinaMarketDayLineServer(1); // 腾讯日线服务器
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(&cmdUI, false)).Times(1);

		gl_pMockMainFrame->OnUpdateUsingNeteaseDayLineDataServer(&cmdUI);

		// 恢复原状
		gl_systemConfiguration.SetChinaMarketDayLineServer(0); // 腾讯日线服务器
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateUsingTengxunDayLineDataServer) {
		CCmdUI cmdUI;
		EXPECT_FALSE(gl_systemConfiguration.IsUsingTengxunDayLineServer());

		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(&cmdUI, false)).Times(1);

		gl_pMockMainFrame->OnUpdateUsingTengxunDayLineDataServer(&cmdUI);

		gl_systemConfiguration.SetChinaMarketDayLineServer(1); // 腾讯日线服务器
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUISetCheck(&cmdUI, true)).Times(1);

		gl_pMockMainFrame->OnUpdateUsingTengxunDayLineDataServer(&cmdUI);

		// 恢复原状
		gl_systemConfiguration.SetChinaMarketDayLineServer(0); // 腾讯日线服务器
	}

	TEST_F(CMockMainFrameTest, TestOnUpdateCalculate10dayRS2) {
		CCmdUI cmdUI;
		EXPECT_THAT(gl_dataContainerChinaStock.IsDayLineDBUpdated(), IsFalse());
		EXPECT_THAT(gl_dataContainerChinaStock.IsUpdateDayLineDB(), IsFalse());
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateCalculate10dayRS2(&cmdUI);

		const auto pStock = gl_dataContainerChinaStock.GetStock(1);
		pStock->SetDayLineDBUpdated(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateCalculate10dayRS2(&cmdUI);
		pStock->SetUpdateDayLineDB(true);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, false)).Times(1);
		gl_pMockMainFrame->OnUpdateCalculate10dayRS2(&cmdUI);

		pStock->SetDayLineDBUpdated(false);
		pStock->SetUpdateDayLineDB(false);
		EXPECT_CALL(*gl_pMockMainFrame, SysCallCmdUIEnable(_, true)).Times(1);
		gl_pMockMainFrame->OnUpdateCalculate10dayRS2(&cmdUI);
	}
}
