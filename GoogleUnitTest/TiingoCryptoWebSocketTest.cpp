#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoCryptoWebSocket.h"

using namespace testing;

namespace FireBirdTest {
	class CDataTiingoCryptoWebSocketTest : public ::testing::Test {
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

	TEST_F(CDataTiingoCryptoWebSocketTest, TestGetURL) {
		EXPECT_STREQ(gl_tiingoCryptoWebSocket.GetURL().c_str(), _T("wss://api.tiingo.com/crypto"));
	}

	TEST_F(CDataTiingoCryptoWebSocketTest, TestCreateMessage) {
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
		EXPECT_STREQ(strSymbols.c_str(), _T("{\"eventName\":\"subscribe\",\"authorization\":\"c897a00b7cfc2adffc630d23befd5316a4683156\",\"eventData\":{\"thresholdLevel\":2,\"tickers\":[\"a\",\"aa\",\"aal\",\"aapl\",\"bcteth\",\"ksmust\"]}}"));
	}
}
