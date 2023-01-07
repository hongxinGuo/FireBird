#include"pch.h"

#include"GeneralCheck.h"

#include"EPSSurprise.h"
#include"DayLine.h"
#include"Country.h"
#include"InsiderTransaction.h"
#include"ChinaStock.h"
#include"WorldStock.h"

#include"Callablefunction.h"

#include"FinnhubWebSocket.h"
#include"TiingoIEXWebSocket.h"
#include"TiingoCryptoWebSocket.h"
#include"TiingoForexWebSocket.h"

#include <ixwebsocket/IXWebSocket.h>

#include<memory>
using std::make_shared;

using namespace testing;

namespace FireBirdTest {
	struct WebSocketMessageData {
		WebSocketMessageData(long type, string strMessage, string strError) {
			m_lType = type;
			m_strMessage = strMessage;
			m_strError = strError;
		}

		~WebSocketMessageData() { }

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

	class ProcessFinnhubWebSocketTest : public::testing::TestWithParam<WebSocketMessageData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();

			EXPECT_THAT(gl_finnhubWebSocket.DataSize(), 0);
			WebSocketMessageData* pMsg = GetParam();
			ix::WebSocketErrorInfo e;
			ix::WebSocketCloseInfo c;
			ix::WebSocketOpenInfo o;
			m_pMsg = make_unique<ix::WebSocketMessage>((ix::WebSocketMessageType)0, s, s.size(), e, o, c);
			m_pMsg->type = (ix::WebSocketMessageType)pMsg->m_lType;
			m_pMsg->errorInfo.reason = pMsg->m_strError;
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	public:
		ix::WebSocketMessagePtr m_pMsg;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubWebSocket1, ProcessFinnhubWebSocketTest,
	                         testing::Values(&finnhubWebSocketMessage1,
		                         &finnhubWebSocketMessage2, &finnhubWebSocketMessage3, &finnhubWebSocketMessage4, &
		                         finnhubWebSocketMessage5, &finnhubWebSocketMessage6, &finnhubWebSocketMessage7));

	TEST_P(ProcessFinnhubWebSocketTest, TestProcessWebSocket) {
		shared_ptr<string> pString;
		ProcessFinnhubWebSocket(m_pMsg);
		switch (m_pMsg->type) {
		case ix::WebSocketMessageType::Message:
			EXPECT_THAT(gl_finnhubWebSocket.DataSize(), 1) << "成功接收了一个数据";
			pString = gl_finnhubWebSocket.PopData();
			EXPECT_STREQ(pString->c_str(), _T("abcdefg"));
			break;
		case ix::WebSocketMessageType::Open:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Finnhub WebSocket Open"));
			break;
		case ix::WebSocketMessageType::Close:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Finnhub WebSocket Close"));
			break;
		case ix::WebSocketMessageType::Error:
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Finnhub WebSocket Error: Error"));
			break;
		case ix::WebSocketMessageType::Ping:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Finnhub WebSocket Ping"));
			break;
		case ix::WebSocketMessageType::Pong:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Finnhub WebSocket Pong"));
			break;
		case ix::WebSocketMessageType::Fragment:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Finnhub WebSocket Fragment"));
			break;
		default:
			EXPECT_THAT(gl_finnhubWebSocket.DataSize(), 0);
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 0);
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

	class ProcessTiingoIEXWebSocketTest : public::testing::TestWithParam<WebSocketMessageData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();

			EXPECT_THAT(gl_tiingoIEXWebSocket.DataSize(), 0);
			WebSocketMessageData* pMsg = GetParam();
			ix::WebSocketErrorInfo e;
			ix::WebSocketCloseInfo c;
			ix::WebSocketOpenInfo o;
			m_pMsg = make_unique<ix::WebSocketMessage>((ix::WebSocketMessageType)0, s, s.size(), e, o, c);
			m_pMsg->type = (ix::WebSocketMessageType)pMsg->m_lType;
			m_pMsg->errorInfo.reason = pMsg->m_strError;
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	public:
		ix::WebSocketMessagePtr m_pMsg;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessTiingoIEXWebSocket1, ProcessTiingoIEXWebSocketTest,
	                         testing::Values(&tiingoIEXWebSocketMessage1,
		                         &tiingoIEXWebSocketMessage2, &tiingoIEXWebSocketMessage3, &tiingoIEXWebSocketMessage4, &
		                         tiingoIEXWebSocketMessage5, &tiingoIEXWebSocketMessage6, &tiingoIEXWebSocketMessage7));

	TEST_P(ProcessTiingoIEXWebSocketTest, TestProcessWebSocket) {
		shared_ptr<string> pString;
		ProcessTiingoIEXWebSocket(m_pMsg);
		switch (m_pMsg->type) {
		case ix::WebSocketMessageType::Message:
			EXPECT_THAT(gl_tiingoIEXWebSocket.DataSize(), 1) << "成功接收了一个数据";
			pString = gl_tiingoIEXWebSocket.PopData();
			EXPECT_STREQ(pString->c_str(), _T("abcdefg"));
			break;
		case ix::WebSocketMessageType::Open:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Tiingo IEX WebSocket Open"));
			break;
		case ix::WebSocketMessageType::Close:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Tiingo IEX WebSocket Close"));
			break;
		case ix::WebSocketMessageType::Error:
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Error"));
			break;
		case ix::WebSocketMessageType::Ping:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Tiingo IEX WebSocket Ping"));
			break;
		case ix::WebSocketMessageType::Pong:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Tiingo IEX WebSocket Pong"));
			break;
		case ix::WebSocketMessageType::Fragment:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Tiingo IEX WebSocket Fragment"));
			break;
		default:
			EXPECT_THAT(gl_tiingoIEXWebSocket.DataSize(), 0);
			EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0);
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

	class ProcessTiingoCryptoWebSocketTest : public::testing::TestWithParam<WebSocketMessageData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();

			EXPECT_THAT(gl_tiingoCryptoWebSocket.DataSize(), 0);
			WebSocketMessageData* pMsg = GetParam();
			ix::WebSocketErrorInfo e;
			ix::WebSocketCloseInfo c;
			ix::WebSocketOpenInfo o;
			m_pMsg = make_unique<ix::WebSocketMessage>((ix::WebSocketMessageType)0, s, s.size(), e, o, c);
			m_pMsg->type = (ix::WebSocketMessageType)pMsg->m_lType;
			m_pMsg->errorInfo.reason = pMsg->m_strError;
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	public:
		ix::WebSocketMessagePtr m_pMsg;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessTiingoCryptoWebSocket1, ProcessTiingoCryptoWebSocketTest,
	                         testing::Values(&tiingoCryptoWebSocketMessage1,
		                         &tiingoCryptoWebSocketMessage2, &tiingoCryptoWebSocketMessage3, &
		                         tiingoCryptoWebSocketMessage4, &tiingoCryptoWebSocketMessage5, &
		                         tiingoCryptoWebSocketMessage6, &tiingoCryptoWebSocketMessage7));

	TEST_P(ProcessTiingoCryptoWebSocketTest, TestProcessWebSocket) {
		shared_ptr<string> pString;
		ProcessTiingoCryptoWebSocket(m_pMsg);
		switch (m_pMsg->type) {
		case ix::WebSocketMessageType::Message:
			EXPECT_THAT(gl_tiingoCryptoWebSocket.DataSize(), 1) << "成功接收了一个数据";
			pString = gl_tiingoCryptoWebSocket.PopData();
			EXPECT_STREQ(pString->c_str(), _T("abcdefg"));
			break;
		case ix::WebSocketMessageType::Open:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Tiingo Crypto WebSocket Open"));
			break;
		case ix::WebSocketMessageType::Close:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Tiingo Crypto WebSocket Close"));
			break;
		case ix::WebSocketMessageType::Error:
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Error"));
			break;
		case ix::WebSocketMessageType::Ping:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Tiingo Crypto WebSocket Ping"));
			break;
		case ix::WebSocketMessageType::Pong:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Tiingo Crypto WebSocket Pong"));
			break;
		case ix::WebSocketMessageType::Fragment:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Tiingo Crypto WebSocket Fragment"));
			break;
		default:
			EXPECT_THAT(gl_tiingoCryptoWebSocket.DataSize(), 0);
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 0);
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

	class ProcessTiingoForexWebSocketTest : public::testing::TestWithParam<WebSocketMessageData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();

			EXPECT_THAT(gl_tiingoForexWebSocket.DataSize(), 0);
			WebSocketMessageData* pMsg = GetParam();
			ix::WebSocketErrorInfo e;
			ix::WebSocketCloseInfo c;
			ix::WebSocketOpenInfo o;
			m_pMsg = make_unique<ix::WebSocketMessage>((ix::WebSocketMessageType)0, s, s.size(), e, o, c);
			m_pMsg->type = (ix::WebSocketMessageType)pMsg->m_lType;
			m_pMsg->errorInfo.reason = pMsg->m_strError;
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	public:
		ix::WebSocketMessagePtr m_pMsg;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessTiingoForexWebSocket1, ProcessTiingoForexWebSocketTest,
	                         testing::Values(&tiingoForexWebSocketMessage1,
		                         &tiingoForexWebSocketMessage2, &tiingoForexWebSocketMessage3, &tiingoForexWebSocketMessage4
		                         , &tiingoForexWebSocketMessage5, &tiingoForexWebSocketMessage6, &
		                         tiingoForexWebSocketMessage7));

	TEST_P(ProcessTiingoForexWebSocketTest, TestProcessWebSocket) {
		shared_ptr<string> pString;
		ProcessTiingoForexWebSocket(m_pMsg);
		switch (m_pMsg->type) {
		case ix::WebSocketMessageType::Message:
			EXPECT_THAT(gl_tiingoForexWebSocket.DataSize(), 1) << "成功接收了一个数据";
			pString = gl_tiingoForexWebSocket.PopData();
			EXPECT_STREQ(pString->c_str(), _T("abcdefg"));
			break;
		case ix::WebSocketMessageType::Open:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Tiingo Forex WebSocket Open"));
			break;
		case ix::WebSocketMessageType::Close:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Tiingo Forex WebSocket Close"));
			break;
		case ix::WebSocketMessageType::Error:
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), _T("Error"));
			break;
		case ix::WebSocketMessageType::Ping:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Tiingo Forex WebSocket Ping"));
			break;
		case ix::WebSocketMessageType::Pong:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Tiingo Forex WebSocket Pong"));
			break;
		case ix::WebSocketMessageType::Fragment:
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 1);
			EXPECT_STREQ(gl_systemMessage.PopWebSocketInfoMessage(), _T("Tiingo Forex WebSocket Fragment"));
			break;
		default:
			EXPECT_THAT(gl_tiingoForexWebSocket.DataSize(), 0);
			EXPECT_THAT(gl_systemMessage.WebSocketInfoSize(), 0);
			break;
		}
	}

	TEST(CallableFunctionTest, TestCompareEPSSurprise) {
		auto p1 = make_shared<CEPSSurprise>();
		auto p2 = make_shared<CEPSSurprise>();
		p1->m_lDate = 0;
		p2->m_lDate = 1;
		EXPECT_TRUE(CompareEPSSurprise(p1, p2));
		p1->m_lDate = 1;
		p2->m_lDate = 1;
		EXPECT_FALSE(CompareEPSSurprise(p1, p2));
		p1->m_lDate = 2;
		p2->m_lDate = 1;
		EXPECT_FALSE(CompareEPSSurprise(p1, p2));
	}

	TEST(CallableFunctionTest, TestCompareDayLine) {
		auto p1 = make_shared<CDayLine>();
		auto p2 = make_shared<CDayLine>();
		p1->SetDate(0);
		p2->SetDate(1);
		EXPECT_TRUE(CompareDayLineDate(p1, p2));
		p1->SetDate(1);
		p2->SetDate(1);
		EXPECT_FALSE(CompareDayLineDate(p1, p2));
		p1->SetDate(2);
		p2->SetDate(1);
		EXPECT_FALSE(CompareDayLineDate(p1, p2));
	}

	TEST(CallableFunctionTest, TestCompareCountryList) {
		auto p1 = make_shared<CCountry>();
		auto p2 = make_shared<CCountry>();
		p1->m_strCountry = _T("abc");
		p2->m_strCountry = _T("abd");
		EXPECT_TRUE(CompareCountryList(p1, p2));
		p1->m_strCountry = _T("abd");
		p2->m_strCountry = _T("abd");
		EXPECT_FALSE(CompareCountryList(p1, p2));
		p1->m_strCountry = _T("abe");
		p2->m_strCountry = _T("abd");
		EXPECT_FALSE(CompareCountryList(p1, p2));
	}

	TEST(CallableFunctionTest, TestCompareInsiderTransaction) {
		auto p1 = make_shared<CInsiderTransaction>();
		auto p2 = make_shared<CInsiderTransaction>();
		p1->m_lTransactionDate = 0;
		p2->m_lTransactionDate = 1;
		EXPECT_TRUE(CompareInsiderTransaction(p1, p2));
		p1->m_lTransactionDate = 1;
		p2->m_lTransactionDate = 1;
		EXPECT_FALSE(CompareInsiderTransaction(p1, p2));
		p1->m_lTransactionDate = 2;
		p2->m_lTransactionDate = 1;
		EXPECT_FALSE(CompareInsiderTransaction(p1, p2));
	}

	TEST(CallableFunctionTest, TestCompareChinaStock) {
		auto p1 = make_shared<CChinaStock>();
		auto p2 = make_shared<CChinaStock>();
		p1->SetSymbol(_T("abc"));
		p2->SetSymbol(_T("abd"));
		EXPECT_TRUE(CompareChinaStock(p1, p2));
		p1->SetSymbol(_T("abd"));
		p2->SetSymbol(_T("abd"));
		EXPECT_FALSE(CompareChinaStock(p1, p2));
		p1->SetSymbol(_T("abe"));
		p2->SetSymbol(_T("abd"));
		EXPECT_FALSE(CompareChinaStock(p1, p2));
	}

	TEST(CallableFunctionTest, TestCompareWorldStock) {
		auto p1 = make_shared<CWorldStock>();
		auto p2 = make_shared<CWorldStock>();
		p1->SetSymbol(_T("abc"));
		p2->SetSymbol(_T("abd"));
		EXPECT_TRUE(CompareWorldStock(p1, p2));
		p1->SetSymbol(_T("abd"));
		p2->SetSymbol(_T("abd"));
		EXPECT_FALSE(CompareWorldStock(p1, p2));
		p1->SetSymbol(_T("abe"));
		p2->SetSymbol(_T("abd"));
		EXPECT_FALSE(CompareWorldStock(p1, p2));
	}
}
