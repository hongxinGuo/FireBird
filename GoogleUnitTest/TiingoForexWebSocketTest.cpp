#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"TiingoForexWebSocket.h"

using namespace testing;

namespace FireBirdTest {
	class CDataTiingoForexWebSocketTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CTiingoForexWebSocket gl_tiingoForexWebSocket;
	};

	TEST_F(CDataTiingoForexWebSocketTest, TestGetURL) {
		EXPECT_STREQ(gl_tiingoForexWebSocket.GetURL().c_str(), _T("wss://api.tiingo.com/fx"));
	}

	TEST_F(CDataTiingoForexWebSocketTest, TestCreateMessage) {
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AA"));
		vSymbol.push_back(_T("AAL"));
		vSymbol.push_back(_T("AAPL"));
		string sSymbols = gl_tiingoForexWebSocket.CreateMessage(vSymbol);
		json jsonMessage;
		try {
			jsonMessage = json::parse(sSymbols);
		}
		catch (json::exception&) { EXPECT_TRUE(false) << "此str应该是json制式的"; }
		EXPECT_TRUE(jsonMessage["eventName"] == _T("subscribe"));
		EXPECT_EQ(jsonMessage["eventData"]["thresholdLevel"], 5);
		EXPECT_STREQ(sSymbols.c_str(), _T("{\"eventName\":\"subscribe\",\"authorization\":\"c897a00b7cfc2adffc630d23befd5316a4683156\",\"eventData\":{\"thresholdLevel\":5,\"tickers\":[\"a\",\"aa\",\"aal\",\"aapl\"]}}"));
	}
}
