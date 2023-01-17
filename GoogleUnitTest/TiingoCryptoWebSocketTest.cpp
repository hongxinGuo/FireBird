#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoCryptoWebSocket.h"

using namespace testing;

namespace FireBirdTest {
	class CTiingoCryptoWebSocketTest : public Test {
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
		CTiingoCryptoWebSocket gl_tiingoCryptoWebSocket;
	};

	TEST_F(CTiingoCryptoWebSocketTest, TestGetURL) {
		EXPECT_STREQ(gl_tiingoCryptoWebSocket.GetURL().c_str(), _T("wss://api.tiingo.com/crypto"));
	}

	TEST_F(CTiingoCryptoWebSocketTest, TestCreateMessage) {
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AA"));
		vSymbol.push_back(_T("AAL"));
		vSymbol.push_back(_T("AAPL"));
		string strSymbols = gl_tiingoCryptoWebSocket.CreateMessage(vSymbol);
		json jsonMessage;
		try {
			jsonMessage = json::parse(strSymbols);
		}
		catch (json::exception&) { EXPECT_TRUE(false) << "此str应该是json制式的"; }
		EXPECT_TRUE(jsonMessage["eventName"] == _T("subscribe"));
		EXPECT_EQ(jsonMessage["eventData"]["thresholdLevel"], 2);
		EXPECT_EQ(jsonMessage["authorization"], _T("c897a00b7cfc2adffc630d23befd5316a4683156"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(0), _T("a"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(1), _T("aa"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(2), _T("aal"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(3), _T("aapl"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(4), _T("dkaeth"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(5), _T("eurusd"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(6), _T("ksmust"));
	}
}
