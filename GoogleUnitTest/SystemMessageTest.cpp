//////////////////////////////////////////////////////////////////////////////////////////
//
// 系统信息中的各个函数皆采用的同步机制以防止线程之间出现数据不同步问题，但此下的测试函数没有测试这种同步机制，
// 只是简单的测试了内部逻辑。
//
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include <gtest/gtest.h>

#include"GeneralCheck.h"
#include "SystemConfiguration.h"
#include "SystemMessage.h"

namespace FireBirdTest {
	class SystemMessageTest : public ::testing::Test {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp

			while (gl_systemMessage.InformationSize() > 0) gl_systemMessage.PopInformationMessage();
			while (gl_systemMessage.DayLineInfoSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
			while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(SystemMessageTest, TestMapNetError) {
		string str;
		int counter = 0;
		for (int i = 12001; i < 12300; i++) {
			switch (i) {
			case NET_ERROR_INTERNET_OUT_OF_HANDLES:
			case NET_ERROR_INTERNET_TIMEOUT:
			case NET_ERROR_INTERNET_EXTENDED_ERROR:
			case NET_ERROR_INTERNET_INTERNAL_ERROR:
			case NET_ERROR_INTERNET_INVALID_URL:
			case NET_ERROR_INTERNET_UNRECOGNIZED_SCHEME:
			case NET_ERROR_INTERNET_NAME_NOT_RESOLVED:
			case NET_ERROR_INTERNET_PROTOCOL_NOT_FOUND:
			case NET_ERROR_INTERNET_INVALID_OPTION:
			case NET_ERROR_INTERNET_BAD_OPTION_LENGTH:
			case NET_ERROR_INTERNET_OPTION_NOT_SETTABLE:
			case NET_ERROR_INTERNET_SHUTDOWN:
			case NET_ERROR_INTERNET_INCORRECT_USER_NAME:
			case NET_ERROR_INTERNET_INCORRECT_PASSWORD:
			case NET_ERROR_INTERNET_LOGIN_FAILURE:
			case NET_ERROR_INTERNET_INVALID_OPERATION:
			case NET_ERROR_INTERNET_OPERATION_CANCELLED:
			case NET_ERROR_INTERNET_INCORRECT_HANDLE_TYPE:
			case NET_ERROR_INTERNET_INCORRECT_HANDLE_STATE:
			case NET_ERROR_INTERNET_NOT_PROXY_REQUEST:
			case NET_ERROR_INTERNET_REGISTRY_VALUE_NOT_FOUND:
			case NET_ERROR_INTERNET_BAD_REGISTRY_PARAMETER:
			case NET_ERROR_INTERNET_NO_DIRECT_ACCESS:
			case NET_ERROR_INTERNET_NO_CONTEXT:
			case NET_ERROR_INTERNET_NO_CALLBACK:
			case NET_ERROR_INTERNET_REQUEST_PENDING:
			case NET_ERROR_INTERNET_INCORRECT_FORMAT:
			case NET_ERROR_INTERNET_ITEM_NOT_FOUND:
			case NET_ERROR_INTERNET_CANNOT_CONNECT:
			case NET_ERROR_INTERNET_CONNECTION_ABORTED:
			case NET_ERROR_INTERNET_CONNECTION_RESET:
			case NET_ERROR_INTERNET_FORCE_RETRY:
			case NET_ERROR_INTERNET_INVALID_PROXY_REQUEST:

			case NET_ERROR_INTERNET_HANDLE_EXISTS:
			case NET_ERROR_INTERNET_SEC_CERT_DATE_INVALID:
			case NET_ERROR_INTERNET_SEC_CERT_CN_INVALID:
			case NET_ERROR_INTERNET_HTTP_TO_HTTPS_ON_REDIR:
			case NET_ERROR_INTERNET_HTTPS_TO_HTTP_ON_REDIR:
			case NET_ERROR_INTERNET_MIXED_SECURITY:
			case NET_ERROR_INTERNET_CHG_POST_IS_NON_SECURE:
			case NET_ERROR_INTERNET_POST_IS_NON_SECURE:

			case NET_ERROR_FTP_TRANSFER_IN_PROGRESS:
			case NET_ERROR_FTP_DROPPED:

			case NET_ERROR_GOPHER_PROTOCOL_ERROR:
			case NET_ERROR_GOPHER_NOT_FILE:
			case NET_ERROR_GOPHER_DATA_ERROR:
			case NET_ERROR_GOPHER_END_OF_DATA:
			case NET_ERROR_GOPHER_INVALID_LOCATOR:
			case NET_ERROR_GOPHER_INCORRECT_LOCATOR_TYPE:
			case NET_ERROR_GOPHER_NOT_GOPHER_PLUS:
			case NET_ERROR_GOPHER_ATTRIBUTE_NOT_FOUND:
			case NET_ERROR_GOPHER_UNKNOWN_LOCATOR:

			case NET_ERROR_HTTP_HEADER_NOT_FOUND:
			case NET_ERROR_HTTP_DOWNLEVEL_SERVER:
			case NET_ERROR_HTTP_INVALID_SERVER_RESPONSE:
			case NET_ERROR_HTTP_INVALID_HEADER:
			case NET_ERROR_HTTP_INVALID_QUERY_REQUEST:
			case NET_ERROR_HTTP_HEADER_ALREADY_EXISTS:
			case NET_ERROR_HTTP_REDIRECT_FAILED:
				counter++;
				str = gl_mapNetError.at(i);
				EXPECT_TRUE(!str.empty());
				break;
			default:
				;
			}
		}
		EXPECT_EQ(counter, 59) << "目前总项数";
	}

	TEST_F(SystemMessageTest, TestInitialize) {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		const size_t l = gl_systemMessage.InformationSize();
		CSystemMessage systemMessage; // 生成第二个实例（第一个为全局变量，系统启动时就生成了）
		EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), l + 1); // 系统报警队列
		EXPECT_EQ(gl_systemMessage.PopErrorMessage(), "错误：系统不允许生成多个CSystemMessage实例");
	}

	TEST_F(SystemMessageTest, TestGetInformationDequeSize) {
		EXPECT_EQ(gl_systemMessage.InformationSize(), 0);
		gl_systemMessage.PushInformationMessage("TEST");
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		const string str = gl_systemMessage.PopInformationMessage();
		EXPECT_EQ(gl_systemMessage.InformationSize(), 0);
		EXPECT_EQ(str, "TEST");
	}

	TEST_F(SystemMessageTest, TestGetDayLineInfoDequeSize) {
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 0);
		gl_systemMessage.PushDayLineInfoMessage("TEST");
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 1);
		const string str = gl_systemMessage.PopDayLineInfoMessage();
		EXPECT_EQ(gl_systemMessage.DayLineInfoSize(), 0);
		EXPECT_EQ(str, "TEST");
	}

	TEST_F(SystemMessageTest, TestGetWebSocketInfoDequeSize) {
		EXPECT_EQ(gl_systemMessage.WebSocketInfoSize(), 0);
		gl_systemMessage.PushWebSocketInfoMessage("TEST");
		EXPECT_EQ(gl_systemMessage.WebSocketInfoSize(), 1);
		const string str = gl_systemMessage.PopWebSocketInfoMessage();
		EXPECT_EQ(gl_systemMessage.WebSocketInfoSize(), 0);
		EXPECT_EQ(str, "TEST");
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
		gl_systemMessage.SetStockCodeForInquiringRTData("601919.SS");
		EXPECT_EQ(gl_systemMessage.GetStockCodeForInquiringRTData(), "601919.SS");
	}

	TEST_F(SystemMessageTest, TestGetStockCodeForInquiryDayLine) {
		gl_systemMessage.SetStockCodeForInquiryDayLine("600000.SS");
		EXPECT_EQ(gl_systemMessage.GetStockCodeForInquiryDayLine(), "600000.SS");
	}

	TEST_F(SystemMessageTest, TestGetCurrentFinnhubWebSocketStake) {
		gl_systemMessage.SetCurrentFinnhubWebSocketStake("600000.SS");
		EXPECT_EQ(gl_systemMessage.GetCurrentFinnhubWebSocketStake(), "600000.SS");
	}

	TEST_F(SystemMessageTest, TestGetCurrentTiingoWebSocketIEX) {
		gl_systemMessage.SetCurrentTiingoWebSocketIEX("600000.SS");
		EXPECT_EQ(gl_systemMessage.GetCurrentTiingoWebSocketIEX(), "600000.SS");
	}

	TEST_F(SystemMessageTest, TestGetCurrentTiingoWebSocketForex) {
		gl_systemMessage.SetCurrentTiingoWebSocketForex("600000.SS");
		EXPECT_EQ(gl_systemMessage.GetCurrentTiingoWebSocketForex(), "600000.SS");
	}

	TEST_F(SystemMessageTest, TestGetCurrentTiingoWebSocketCrypto) {
		gl_systemMessage.SetCurrentTiingoWebSocketCrypto("600000.SS");
		EXPECT_EQ(gl_systemMessage.GetCurrentTiingoWebSocketCrypto(), "600000.SS");
	}

	TEST_F(SystemMessageTest, TestIncreaseScheduleTaskTime) {
		EXPECT_EQ(gl_systemMessage.GetScheduleTaskTimePerSecond(), 0);

		gl_systemMessage.IncreaseScheduleTaskTime(100);
		gl_systemMessage.CalcScheduleTaskTimePerSecond();

		EXPECT_EQ(gl_systemMessage.GetScheduleTaskTimePerSecond(), 100);

		gl_systemMessage.IncreaseScheduleTaskTime(100);
		gl_systemMessage.CalcScheduleTaskTimePerSecond();
		EXPECT_EQ(gl_systemMessage.GetScheduleTaskTimePerSecond(), 100) << "m_lScheduleTaskTime be reset";

		gl_systemMessage.CalcScheduleTaskTimePerSecond();
		EXPECT_EQ(gl_systemMessage.GetScheduleTaskTimePerSecond(), 0) << "m_lScheduleTaskTime be reset";
	}
}
