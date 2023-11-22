#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoIEXWebSocket.h"

using namespace testing;

namespace FireBirdTest {
	class CTiingoIEXWebSocketTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE(""); GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE(""); GeneralCheck();
		}

	protected:
		CTiingoIEXWebSocket gl_tiingoIEXWebSocket;
	};

	TEST_F(CTiingoIEXWebSocketTest, TestGetURL) {
		EXPECT_STREQ(gl_pTiingoIEXWebSocket->GetURL().c_str(), _T("wss://api.tiingo.com/iex"));
	}

	TEST_F(CTiingoIEXWebSocketTest, TestSetScriptionStatus) {
		EXPECT_TRUE(gl_pTiingoIEXWebSocket->IsSubscriptable());
		gl_pTiingoIEXWebSocket->SetSubscriptionStatus(false);
		EXPECT_FALSE(gl_pTiingoIEXWebSocket->IsSubscriptable());
		gl_pTiingoIEXWebSocket->SetSubscriptionStatus(true);
		EXPECT_TRUE(gl_pTiingoIEXWebSocket->IsSubscriptable());
	}

	TEST_F(CTiingoIEXWebSocketTest, TestSetSubscriptionId) {
		EXPECT_TRUE(gl_pTiingoIEXWebSocket->IsSubscriptable());// 必须设置此标识
		EXPECT_EQ(gl_pTiingoIEXWebSocket->GetSubscriptionId(), 0);
		gl_pTiingoIEXWebSocket->SetSubscriptionId(101010);
		EXPECT_EQ(gl_pTiingoIEXWebSocket->GetSubscriptionId(), 101010);
	}

	TEST_F(CTiingoIEXWebSocketTest, TestCreateMessage) {
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AA"));
		vSymbol.push_back(_T("AAL"));
		vSymbol.push_back(_T("AAPL"));
		string str = gl_pTiingoIEXWebSocket->CreateMessage(vSymbol);
		json jsonMessage;
		try {
			jsonMessage = json::parse(str);
		}
		catch (json::exception&) { EXPECT_TRUE(false) << "此str应该是json制式的"; }
		EXPECT_TRUE(jsonMessage["eventName"] == _T("subscribe"));
		EXPECT_EQ(jsonMessage["eventData"]["thresholdLevel"], 5);
		EXPECT_EQ(jsonMessage["authorization"], _T("c897a00b7cfc2adffc630d23befd5316a4683156"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(0), _T("a"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(1), _T("aa"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(2), _T("aal"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(3), _T("aapl"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(4), _T("rig"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(5), _T("eurusd"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(6), _T("spy"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(7), _T("uso"));
	}
}
