#include"pch.h"

 

#include"GeneralCheck.h"

#include"WorldMarket.h"

import FireBird.WebSocket.TiingoForex;

using namespace testing;

namespace FireBirdTest {
	class CTiingoForexWebSocketTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CTiingoForexWebSocket gl_tiingoForexWebSocket;
	};

	TEST_F(CTiingoForexWebSocketTest, TestGetURL) {
		EXPECT_STREQ(gl_pTiingoForexWebSocket->GetURL().c_str(), _T("wss://api.tiingo.com/fx"));
	}

	TEST_F(CTiingoForexWebSocketTest, TestCreateMessage) {
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AA"));
		vSymbol.push_back(_T("AAL"));
		vSymbol.push_back(_T("AAPL"));
		string sSymbol = gl_pTiingoForexWebSocket->CreateMessage(vSymbol);
		nlohmann::ordered_json jsonMessage;
		try {
			jsonMessage = nlohmann::ordered_json::parse(sSymbol);
		} catch (nlohmann::ordered_json::exception&) { EXPECT_TRUE(false) << "此str应该是json制式的"; }
		EXPECT_TRUE(jsonMessage["eventName"] == _T("subscribe"));
		EXPECT_EQ(jsonMessage["eventData"]["thresholdLevel"], 5);
		EXPECT_EQ(jsonMessage["authorization"], _T("c897a00b7cfc2630d235316a4683156"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(0), _T("a"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(1), _T("aa"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(2), _T("aal"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(3), _T("aapl"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(4), _T("gbpaud"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(5), _T("eurusd"));
	}
}
