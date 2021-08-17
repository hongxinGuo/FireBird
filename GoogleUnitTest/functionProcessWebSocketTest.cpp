#include"pch.h"
#include"globedef.h"
#include"WebInquirer.h"

#include"function.h"

#include <ixwebsocket/IXWebSocket.h>

using namespace std;
#include<string>

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	struct WebSocketMessageData {
		WebSocketMessageData(long type, string strMessage, string strError) {
			m_lType = type;
			m_strMessage = strMessage;
			m_strError = strError;
		}

		~WebSocketMessageData() {
		}

	public:
		long m_lType;
		string m_strMessage;
		string m_strError;
	};

	const string s(_T("abcdefg")); // 此字符串用于初始化const pointer m_Msg中的const string，故而在测试期间需要一直保持存续状态。

	WebSocketMessageData finnhubWebSocketMessage1((long)ix::WebSocketMessageType::Message, _T("abcdefg"), _T(""));
	WebSocketMessageData finnhubWebSocketMessage2((long)ix::WebSocketMessageType::Open, _T("abcdefg"), _T(""));
	WebSocketMessageData finnhubWebSocketMessage3((long)ix::WebSocketMessageType::Pong, _T("abcdefg"), _T(""));
	WebSocketMessageData finnhubWebSocketMessage4((long)ix::WebSocketMessageType::Ping, _T("abcdefg"), _T(""));
	WebSocketMessageData finnhubWebSocketMessage5((long)ix::WebSocketMessageType::Error, _T("abcdefg"), _T("Error"));
	WebSocketMessageData finnhubWebSocketMessage6((long)ix::WebSocketMessageType::Close, _T("abcdefg"), _T(""));
	WebSocketMessageData finnhubWebSocketMessage7((long)ix::WebSocketMessageType::Fragment, _T("abcdefg"), _T(""));

	class FunctionProcessFinnhubWebSocketTest : public::testing::TestWithParam<WebSocketMessageData*>
	{
	protected:
		virtual void SetUp(void) override {
			ASSERT_FALSE(gl_fNormalMode);
			EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0);
			EXPECT_THAT(gl_WebInquirer.GetFinnhubWebSocketDataSize(), 0);
			WebSocketMessageData* pMsg = GetParam();
			ix::WebSocketErrorInfo e;
			ix::WebSocketCloseInfo c;
			ix::WebSocketOpenInfo o;
			m_pMsg = make_unique<ix::WebSocketMessage>((ix::WebSocketMessageType)0, s, s.size(), e, o, c);
			m_pMsg->type = (ix::WebSocketMessageType)pMsg->m_lType;
			m_pMsg->errorInfo.reason = pMsg->m_strError;
		}

		virtual void TearDown(void) override {
			// clearup
			EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0) << gl_systemMessage.PopInformationMessage();
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0) << gl_systemMessage.PopDayLineInfoMessage();
		}

	public:
		ix::WebSocketMessagePtr m_pMsg;
	};

	INSTANTIATE_TEST_SUITE_P(TestFunctionProcessFinnhubWebSocket1, FunctionProcessFinnhubWebSocketTest, testing::Values(&finnhubWebSocketMessage1,
		&finnhubWebSocketMessage2, &finnhubWebSocketMessage3, &finnhubWebSocketMessage4, &finnhubWebSocketMessage5, &finnhubWebSocketMessage6, &finnhubWebSocketMessage7));

	TEST_P(FunctionProcessFinnhubWebSocketTest, TestFunctionProcessWebSocket) {
		shared_ptr<string> pString;
		FunctionProcessFinnhubWebSocket(m_pMsg);
		switch (m_pMsg->type) {
		case ix::WebSocketMessageType::Message:
			EXPECT_THAT(gl_WebInquirer.GetFinnhubWebSocketDataSize(), 1) << "成功接收了一个数据";
			pString = gl_WebInquirer.PopFinnhubWebSocketData();
			EXPECT_STREQ(pString->c_str(), _T("abcdefg"));
			break;
		case ix::WebSocketMessageType::Open:
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Finnhub WebSocket已连接"));
			break;
		case ix::WebSocketMessageType::Close:
			EXPECT_THAT(gl_WebInquirer.GetFinnhubWebSocketDataSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			break;
		case ix::WebSocketMessageType::Error:
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Error"));
			break;
		case ix::WebSocketMessageType::Ping:
			EXPECT_THAT(gl_WebInquirer.GetFinnhubWebSocketDataSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			break;
		case ix::WebSocketMessageType::Pong:
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Finnhub WebSocket heart beat"));
			break;
		case ix::WebSocketMessageType::Fragment:
			EXPECT_THAT(gl_WebInquirer.GetFinnhubWebSocketDataSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			break;
		default:
			EXPECT_THAT(gl_WebInquirer.GetFinnhubWebSocketDataSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			break;
		}
	}

	WebSocketMessageData tiingoIEXWebSocketMessage1((long)ix::WebSocketMessageType::Message, _T("abcdefg"), _T(""));
	WebSocketMessageData tiingoIEXWebSocketMessage2((long)ix::WebSocketMessageType::Open, _T("abcdefg"), _T(""));
	WebSocketMessageData tiingoIEXWebSocketMessage3((long)ix::WebSocketMessageType::Pong, _T("abcdefg"), _T(""));
	WebSocketMessageData tiingoIEXWebSocketMessage4((long)ix::WebSocketMessageType::Ping, _T("abcdefg"), _T(""));
	WebSocketMessageData tiingoIEXWebSocketMessage5((long)ix::WebSocketMessageType::Error, _T("abcdefg"), _T("Error"));
	WebSocketMessageData tiingoIEXWebSocketMessage6((long)ix::WebSocketMessageType::Close, _T("abcdefg"), _T(""));
	WebSocketMessageData tiingoIEXWebSocketMessage7((long)ix::WebSocketMessageType::Fragment, _T("abcdefg"), _T(""));

	class FunctionProcessTiingoIEXWebSocketTest : public::testing::TestWithParam<WebSocketMessageData*>
	{
	protected:
		virtual void SetUp(void) override {
			ASSERT_FALSE(gl_fNormalMode);
			EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0);
			EXPECT_THAT(gl_WebInquirer.GetTiingoIEXWebSocketDataSize(), 0);
			WebSocketMessageData* pMsg = GetParam();
			ix::WebSocketErrorInfo e;
			ix::WebSocketCloseInfo c;
			ix::WebSocketOpenInfo o;
			m_pMsg = make_unique<ix::WebSocketMessage>((ix::WebSocketMessageType)0, s, s.size(), e, o, c);
			m_pMsg->type = (ix::WebSocketMessageType)pMsg->m_lType;
			m_pMsg->errorInfo.reason = pMsg->m_strError;
		}

		virtual void TearDown(void) override {
			// clearup
			EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0) << gl_systemMessage.PopInformationMessage();
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0) << gl_systemMessage.PopDayLineInfoMessage();
		}

	public:
		ix::WebSocketMessagePtr m_pMsg;
	};

	INSTANTIATE_TEST_SUITE_P(TestFunctionProcessTiingoIEXWebSocket1, FunctionProcessTiingoIEXWebSocketTest, testing::Values(&tiingoIEXWebSocketMessage1,
		&tiingoIEXWebSocketMessage2, &tiingoIEXWebSocketMessage3, &tiingoIEXWebSocketMessage4, &tiingoIEXWebSocketMessage5, &tiingoIEXWebSocketMessage6, &tiingoIEXWebSocketMessage7));

	TEST_P(FunctionProcessTiingoIEXWebSocketTest, TestFunctionProcessWebSocket) {
		shared_ptr<string> pString;
		FunctionProcessTiingoIEXWebSocket(m_pMsg);
		switch (m_pMsg->type) {
		case ix::WebSocketMessageType::Message:
			EXPECT_THAT(gl_WebInquirer.GetTiingoIEXWebSocketDataSize(), 1) << "成功接收了一个数据";
			pString = gl_WebInquirer.PopTiingoIEXWebSocketData();
			EXPECT_STREQ(pString->c_str(), _T("abcdefg"));
			break;
		case ix::WebSocketMessageType::Open:
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Tiingo IEX WebSocket已连接"));
			break;
		case ix::WebSocketMessageType::Close:
			EXPECT_THAT(gl_WebInquirer.GetTiingoIEXWebSocketDataSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			break;
		case ix::WebSocketMessageType::Error:
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Error"));
			break;
		case ix::WebSocketMessageType::Ping:
			EXPECT_THAT(gl_WebInquirer.GetTiingoIEXWebSocketDataSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			break;
		case ix::WebSocketMessageType::Pong:
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Tiingo IEX WebSocket heart beat"));
			break;
		case ix::WebSocketMessageType::Fragment:
			EXPECT_THAT(gl_WebInquirer.GetTiingoIEXWebSocketDataSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			break;
		default:
			EXPECT_THAT(gl_WebInquirer.GetTiingoIEXWebSocketDataSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			break;
		}
	}

	WebSocketMessageData tiingoCryptoWebSocketMessage1((long)ix::WebSocketMessageType::Message, _T("abcdefg"), _T(""));
	WebSocketMessageData tiingoCryptoWebSocketMessage2((long)ix::WebSocketMessageType::Open, _T("abcdefg"), _T(""));
	WebSocketMessageData tiingoCryptoWebSocketMessage3((long)ix::WebSocketMessageType::Pong, _T("abcdefg"), _T(""));
	WebSocketMessageData tiingoCryptoWebSocketMessage4((long)ix::WebSocketMessageType::Ping, _T("abcdefg"), _T(""));
	WebSocketMessageData tiingoCryptoWebSocketMessage5((long)ix::WebSocketMessageType::Error, _T("abcdefg"), _T("Error"));
	WebSocketMessageData tiingoCryptoWebSocketMessage6((long)ix::WebSocketMessageType::Close, _T("abcdefg"), _T(""));
	WebSocketMessageData tiingoCryptoWebSocketMessage7((long)ix::WebSocketMessageType::Fragment, _T("abcdefg"), _T(""));

	class FunctionProcessTiingoCryptoWebSocketTest : public::testing::TestWithParam<WebSocketMessageData*>
	{
	protected:
		virtual void SetUp(void) override {
			ASSERT_FALSE(gl_fNormalMode);
			EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0);
			EXPECT_THAT(gl_WebInquirer.GetTiingoCryptoWebSocketDataSize(), 0);
			WebSocketMessageData* pMsg = GetParam();
			ix::WebSocketErrorInfo e;
			ix::WebSocketCloseInfo c;
			ix::WebSocketOpenInfo o;
			m_pMsg = make_unique<ix::WebSocketMessage>((ix::WebSocketMessageType)0, s, s.size(), e, o, c);
			m_pMsg->type = (ix::WebSocketMessageType)pMsg->m_lType;
			m_pMsg->errorInfo.reason = pMsg->m_strError;
		}

		virtual void TearDown(void) override {
			// clearup
			EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0) << gl_systemMessage.PopInformationMessage();
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0) << gl_systemMessage.PopDayLineInfoMessage();
		}

	public:
		ix::WebSocketMessagePtr m_pMsg;
	};

	INSTANTIATE_TEST_SUITE_P(TestFunctionProcessTiingoCryptoWebSocket1, FunctionProcessTiingoCryptoWebSocketTest, testing::Values(&tiingoCryptoWebSocketMessage1,
		&tiingoCryptoWebSocketMessage2, &tiingoCryptoWebSocketMessage3, &tiingoCryptoWebSocketMessage4, &tiingoCryptoWebSocketMessage5, &tiingoCryptoWebSocketMessage6, &tiingoCryptoWebSocketMessage7));

	TEST_P(FunctionProcessTiingoCryptoWebSocketTest, TestFunctionProcessWebSocket) {
		shared_ptr<string> pString;
		FunctionProcessTiingoCryptoWebSocket(m_pMsg);
		switch (m_pMsg->type) {
		case ix::WebSocketMessageType::Message:
			EXPECT_THAT(gl_WebInquirer.GetTiingoCryptoWebSocketDataSize(), 1) << "成功接收了一个数据";
			pString = gl_WebInquirer.PopTiingoCryptoWebSocketData();
			EXPECT_STREQ(pString->c_str(), _T("abcdefg"));
			break;
		case ix::WebSocketMessageType::Open:
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Tiingo Crypto WebSocket已连接"));
			break;
		case ix::WebSocketMessageType::Close:
			EXPECT_THAT(gl_WebInquirer.GetTiingoCryptoWebSocketDataSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			break;
		case ix::WebSocketMessageType::Error:
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Error"));
			break;
		case ix::WebSocketMessageType::Ping:
			EXPECT_THAT(gl_WebInquirer.GetTiingoCryptoWebSocketDataSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			break;
		case ix::WebSocketMessageType::Pong:
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Tiingo Crypto WebSocket heart beat"));
			break;
		case ix::WebSocketMessageType::Fragment:
			EXPECT_THAT(gl_WebInquirer.GetTiingoCryptoWebSocketDataSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			break;
		default:
			EXPECT_THAT(gl_WebInquirer.GetTiingoCryptoWebSocketDataSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			break;
		}
	}

	WebSocketMessageData tiingoForexWebSocketMessage1((long)ix::WebSocketMessageType::Message, _T("abcdefg"), _T(""));
	WebSocketMessageData tiingoForexWebSocketMessage2((long)ix::WebSocketMessageType::Open, _T("abcdefg"), _T(""));
	WebSocketMessageData tiingoForexWebSocketMessage3((long)ix::WebSocketMessageType::Pong, _T("abcdefg"), _T(""));
	WebSocketMessageData tiingoForexWebSocketMessage4((long)ix::WebSocketMessageType::Ping, _T("abcdefg"), _T(""));
	WebSocketMessageData tiingoForexWebSocketMessage5((long)ix::WebSocketMessageType::Error, _T("abcdefg"), _T("Error"));
	WebSocketMessageData tiingoForexWebSocketMessage6((long)ix::WebSocketMessageType::Close, _T("abcdefg"), _T(""));
	WebSocketMessageData tiingoForexWebSocketMessage7((long)ix::WebSocketMessageType::Fragment, _T("abcdefg"), _T(""));

	class FunctionProcessTiingoForexWebSocketTest : public::testing::TestWithParam<WebSocketMessageData*>
	{
	protected:
		virtual void SetUp(void) override {
			ASSERT_FALSE(gl_fNormalMode);
			EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0);
			EXPECT_THAT(gl_WebInquirer.GetTiingoForexWebSocketDataSize(), 0);
			WebSocketMessageData* pMsg = GetParam();
			ix::WebSocketErrorInfo e;
			ix::WebSocketCloseInfo c;
			ix::WebSocketOpenInfo o;
			m_pMsg = make_unique<ix::WebSocketMessage>((ix::WebSocketMessageType)0, s, s.size(), e, o, c);
			m_pMsg->type = (ix::WebSocketMessageType)pMsg->m_lType;
			m_pMsg->errorInfo.reason = pMsg->m_strError;
		}

		virtual void TearDown(void) override {
			// clearup
			EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0) << gl_systemMessage.PopInformationMessage();
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0) << gl_systemMessage.PopDayLineInfoMessage();
		}

	public:
		ix::WebSocketMessagePtr m_pMsg;
	};

	INSTANTIATE_TEST_SUITE_P(TestFunctionProcessTiingoForexWebSocket1, FunctionProcessTiingoForexWebSocketTest, testing::Values(&tiingoForexWebSocketMessage1,
		&tiingoForexWebSocketMessage2, &tiingoForexWebSocketMessage3, &tiingoForexWebSocketMessage4, &tiingoForexWebSocketMessage5, &tiingoForexWebSocketMessage6, &tiingoForexWebSocketMessage7));

	TEST_P(FunctionProcessTiingoForexWebSocketTest, TestFunctionProcessWebSocket) {
		shared_ptr<string> pString;
		FunctionProcessTiingoForexWebSocket(m_pMsg);
		switch (m_pMsg->type) {
		case ix::WebSocketMessageType::Message:
			EXPECT_THAT(gl_WebInquirer.GetTiingoForexWebSocketDataSize(), 1) << "成功接收了一个数据";
			pString = gl_WebInquirer.PopTiingoForexWebSocketData();
			EXPECT_STREQ(pString->c_str(), _T("abcdefg"));
			break;
		case ix::WebSocketMessageType::Open:
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Tiingo Forex WebSocket已连接"));
			break;
		case ix::WebSocketMessageType::Close:
			EXPECT_THAT(gl_WebInquirer.GetTiingoForexWebSocketDataSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			break;
		case ix::WebSocketMessageType::Error:
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Error"));
			break;
		case ix::WebSocketMessageType::Ping:
			EXPECT_THAT(gl_WebInquirer.GetTiingoForexWebSocketDataSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			break;
		case ix::WebSocketMessageType::Pong:
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Tiingo Forex WebSocket heart beat"));
			break;
		case ix::WebSocketMessageType::Fragment:
			EXPECT_THAT(gl_WebInquirer.GetTiingoForexWebSocketDataSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			break;
		default:
			EXPECT_THAT(gl_WebInquirer.GetTiingoForexWebSocketDataSize(), 0);
			EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0);
			break;
		}
	}
}