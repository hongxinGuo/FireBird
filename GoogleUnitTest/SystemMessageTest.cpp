//////////////////////////////////////////////////////////////////////////////////////////
//
// 系统信息中的各个函数皆采用的同步机制以防止线程之间出现数据不同步问题，但此下的测试函数没有测试这种同步机制，
// 只是简单的测试了内部逻辑。
//
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"SystemMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class SystemMessageTest : public ::testing::Test
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
			GeneralCheck();

			while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
			while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
			while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
		}
	};

	TEST_F(SystemMessageTest, TestInitialize) {
		ASSERT_FALSE(gl_fNormalMode);
		size_t l = gl_systemMessage.GetInformationDequeSize();
		CSystemMessage systemMessage; // 生成第二个实例（第一个为全局变量，系统启动时就生成了）
		EXPECT_EQ(gl_systemMessage.GetErrorMessageDequeSize(), l + 1); // 系统报警队列
		EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("错误：系统不允许生成多个CSystemMessage实例"));
	}

	TEST_F(SystemMessageTest, TestGetInformationDequeSize) {
		EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
		gl_systemMessage.PushInformationMessage(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 1);
		CString str = gl_systemMessage.PopInformationMessage();
		EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetDayLineInfoDequeSize) {
		EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), 0);
		gl_systemMessage.PushDayLineInfoMessage(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), 1);
		CString str = gl_systemMessage.PopDayLineInfoMessage();
		EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetFindDequeSize) {
		ASSERT_FALSE(gl_fNormalMode);
		EXPECT_EQ(gl_systemMessage.GetTransactionDequeSize(), 0);
		gl_systemMessage.PushTransactionMessage(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.GetTransactionDequeSize(), 1);
		CString str = gl_systemMessage.PopTransactionMessage();
		EXPECT_EQ(gl_systemMessage.GetTransactionDequeSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetCancelSellDequeSize) {
		EXPECT_EQ(gl_systemMessage.GetCancelSellDequeSize(), 0);
		gl_systemMessage.PushCancelSellMessage(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.GetCancelSellDequeSize(), 1);
		CString str = gl_systemMessage.PopCancelSellMessage();
		EXPECT_EQ(gl_systemMessage.GetCancelSellDequeSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetCancelBuyDequeSize) {
		EXPECT_EQ(gl_systemMessage.GetCancelBuyDequeSize(), 0);
		gl_systemMessage.PushCancelBuyMessage(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.GetCancelBuyDequeSize(), 1);
		CString str = gl_systemMessage.PopCancelBuyMessage();
		EXPECT_EQ(gl_systemMessage.GetCancelBuyDequeSize(), 0);
		EXPECT_STREQ(str, _T("TEST"));
	}

	TEST_F(SystemMessageTest, TestGetTrace2DequeSize) {
		EXPECT_EQ(gl_systemMessage.GetTrace2DequeSize(), 0);
		gl_systemMessage.PushTrace2Message(_T("TEST"));
		EXPECT_EQ(gl_systemMessage.GetTrace2DequeSize(), 1);
		CString str = gl_systemMessage.PopTrace2Message();
		EXPECT_EQ(gl_systemMessage.GetTrace2DequeSize(), 0);
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

	TEST_F(SystemMessageTest, TestGetReadingNeteaseDayLineDataTime) {
		gl_systemMessage.SetStockCodeForInquiringNeteaseDayLine(_T("600000.SS"));
		EXPECT_EQ(gl_systemMessage.GetStockCodeForInquiringNeteaseDayLine(), _T("600000.SS"));
	}
}