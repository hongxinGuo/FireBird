//////////////////////////////////////////////////////////////////////////////////////////
//
// 系统信息中的各个函数皆采用的同步机制以防止线程之间出现数据不同步问题，但此下的测试函数没有测试这种同步机制，
// 只是简单的测试了内部逻辑。
//
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"GeneralCheck.h"

namespace FireBirdTest {
	class SystemMessageTest : public ::testing::Test {
	protected:
		void SetUp() override {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void TearDown() override {
			// clearUp

			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
			while (gl_systemMessage.DayLineInfoSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
			while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();

			SCOPED_TRACE(""); GeneralCheck();
		}
	};

	TEST_F(SystemMessageTest, TestInitialize) {
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
		const size_t l = gl_systemMessage.InformationSize();
		CSystemMessage systemMessage; // 生成第二个实例（第一个为全局变量，系统启动时就生成了）
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), l + 1); // 系统报警队列
		EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("错误：系统不允许生成多个CSystemMessage实例"));
	}

	TEST_F(SystemMessageTest, TestGetInformationDequeSize) {
		EXPECT_EQ(gl_systemMessage.InformationSize(), 0);
		gl_systemMessage.PushInformationMessage(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		const CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_EQ(gl_systemMessage.InformationSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetDayLineInfoDequeSize) {
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 0);
		gl_systemMessage.PushDayLineInfoMessage(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 1);
		const CString str = gl_systemMessage.PopDayLineInfoMessage();
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetFindDequeSize) {
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
		EXPECT_EQ(gl_systemMessage.TransactionInfoSize(), 0);
		gl_systemMessage.PushTransactionMessage(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.TransactionInfoSize(), 1);
		const CString str = gl_systemMessage.PopTransactionMessage();
		EXPECT_EQ(gl_systemMessage.TransactionInfoSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetCancelSellDequeSize) {
		EXPECT_EQ(gl_systemMessage.CancelSellInfoSize(), 0);
		gl_systemMessage.PushCancelSellMessage(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.CancelSellInfoSize(), 1);
		const CString str = gl_systemMessage.PopCancelSellMessage();
		EXPECT_EQ(gl_systemMessage.CancelSellInfoSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetCancelBuyDequeSize) {
		EXPECT_EQ(gl_systemMessage.CancelBuyInfoSize(), 0);
		gl_systemMessage.PushCancelBuyMessage(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.CancelBuyInfoSize(), 1);
		const CString str = gl_systemMessage.PopCancelBuyMessage();
		EXPECT_EQ(gl_systemMessage.CancelBuyInfoSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetWebSocketInfoDequeSize) {
		EXPECT_EQ(gl_systemMessage.WebSocketInfoSize(), 0);
		gl_systemMessage.PushWebSocketInfoMessage(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.WebSocketInfoSize(), 1);
		const CString str = gl_systemMessage.PopWebSocketInfoMessage();
		EXPECT_EQ(gl_systemMessage.WebSocketInfoSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetTrace2DequeSize) {
		EXPECT_EQ(gl_systemMessage.Trace2Size(), 0);
		gl_systemMessage.PushTrace2Message(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.Trace2Size(), 1);
		const CString str = gl_systemMessage.PopTrace2Message();
		EXPECT_EQ(gl_systemMessage.Trace2Size(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetProcessedFinnhubWebSocket) {
		EXPECT_EQ(gl_systemMessage.GetProcessedFinnhubWebSocket(), 0);
		gl_systemMessage.SetProcessedFinnhubWebSocket(100);
		EXPECT_EQ(gl_systemMessage.GetProcessedFinnhubWebSocket(), 100);
		gl_systemMessage.ClearProcessedFinnhubWebSocket();
	}

	TEST_F(SystemMessageTest, TestGetProcessedTiingoIEXWebSocket) {
		EXPECT_EQ(gl_systemMessage.GetProcessedTiingoIEXWebSocket(), 0);
		gl_systemMessage.SetProcessedTiingoIEXWebSocket(100);
		EXPECT_EQ(gl_systemMessage.GetProcessedTiingoIEXWebSocket(), 100);
		gl_systemMessage.ClearProcessedTiingoIEXWebSocket();
	}

	TEST_F(SystemMessageTest, TestGetProcessedTiingoCryptoWebSocket) {
		EXPECT_EQ(gl_systemMessage.GetProcessedTiingoCryptoWebSocket(), 0);
		gl_systemMessage.SetProcessedTiingoCryptoWebSocket(100);
		EXPECT_EQ(gl_systemMessage.GetProcessedTiingoCryptoWebSocket(), 100);
		gl_systemMessage.ClearProcessedTiingoCryptoWebSocket();
	}

	TEST_F(SystemMessageTest, TestGetProcessedTiingoForexWebSocket) {
		EXPECT_EQ(gl_systemMessage.GetProcessedTiingoForexWebSocket(), 0);
		gl_systemMessage.SetProcessedTiingoForexWebSocket(100);
		EXPECT_EQ(gl_systemMessage.GetProcessedTiingoForexWebSocket(), 100);
		gl_systemMessage.ClearProcessedTiingoForexWebSocket();
	}

	TEST_F(SystemMessageTest, TestGetStockCodeForInquiringSinaRTData) {
		gl_systemMessage.SetStockCodeForInquiringRTData(_T("601919.SS"));
		EXPECT_EQ(gl_systemMessage.GetStockCodeForInquiringRTData(), _T("601919.SS"));
	}

	TEST_F(SystemMessageTest, TestGetStockCodeForInquiringNeteaseDayLine) {
		gl_systemMessage.SetStockCodeForInquiringNeteaseDayLine(_T("600000.SS"));
		EXPECT_EQ(gl_systemMessage.GetStockCodeForInquiringNeteaseDayLine(), _T("600000.SS"));
	}

	TEST_F(SystemMessageTest, TestGetCurrentFinnhubWebSocketStake) {
		gl_systemMessage.SetCurrentFinnhubWebSocketStake(_T("600000.SS"));
		EXPECT_EQ(gl_systemMessage.GetCurrentFinnhubWebSocketStake(), _T("600000.SS"));
	}

	TEST_F(SystemMessageTest, TestGetCurrentTiingoWebSocketIEX) {
		gl_systemMessage.SetCurrentTiingoWebSocketIEX(_T("600000.SS"));
		EXPECT_EQ(gl_systemMessage.GetCurrentTiingoWebSocketIEX(), _T("600000.SS"));
	}

	TEST_F(SystemMessageTest, TestGetCurrentTiingoWebSocketForex) {
		gl_systemMessage.SetCurrentTiingoWebSocketForex(_T("600000.SS"));
		EXPECT_EQ(gl_systemMessage.GetCurrentTiingoWebSocketForex(), _T("600000.SS"));
	}

	TEST_F(SystemMessageTest, TestGetCurrentTiingoWebSocketCrypto) {
		gl_systemMessage.SetCurrentTiingoWebSocketCrypto(_T("600000.SS"));
		EXPECT_EQ(gl_systemMessage.GetCurrentTiingoWebSocketCrypto(), _T("600000.SS"));
	}
}
