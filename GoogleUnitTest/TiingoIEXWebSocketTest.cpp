#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoIEXWebSocket.h"

using namespace testing;

namespace FireBirdTest {
	class CDataTiingoIEXWebSocketTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CTiingoIEXWebSocket gl_tiingoIEXWebSocket;
	};

	TEST_F(CDataTiingoIEXWebSocketTest, TestGetURL) {
		EXPECT_STREQ(gl_tiingoIEXWebSocket.GetURL().c_str(), _T("wss://api.tiingo.com/iex"));
	}

	TEST_F(CDataTiingoIEXWebSocketTest, TestSetScriptionStatus) {
		EXPECT_TRUE(gl_tiingoIEXWebSocket.IsSubscriptable());
		gl_tiingoIEXWebSocket.SetSubscriptionStatus(false);
		EXPECT_FALSE(gl_tiingoIEXWebSocket.IsSubscriptable());
		gl_tiingoIEXWebSocket.SetSubscriptionStatus(true);
		EXPECT_TRUE(gl_tiingoIEXWebSocket.IsSubscriptable());
	}

	TEST_F(CDataTiingoIEXWebSocketTest, TestSetSubscriptionId) {
		EXPECT_TRUE(gl_tiingoIEXWebSocket.IsSubscriptable());// 必须设置此标识
		EXPECT_EQ(gl_tiingoIEXWebSocket.GetSubscriptionId(), 0);
		gl_tiingoIEXWebSocket.SetSubscriptionId(101010);
		EXPECT_EQ(gl_tiingoIEXWebSocket.GetSubscriptionId(), 101010);
	}

	TEST_F(CDataTiingoIEXWebSocketTest, TestCreateTiingoIEXWebSocketSymbolString) {
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AA"));
		vSymbol.push_back(_T("AAL"));
		vSymbol.push_back(_T("AAPL"));
		string sSymbols = gl_tiingoIEXWebSocket.CreateTiingoWebSocketSymbolString(vSymbol);
		EXPECT_TRUE(sSymbols == _T("\"A\",\"AA\",\"AAL\",\"AAPL\""));
	}

	TEST_F(CDataTiingoIEXWebSocketTest, TestCreateMessage) {
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AA"));
		vSymbol.push_back(_T("AAL"));
		vSymbol.push_back(_T("AAPL"));
		string str = gl_tiingoIEXWebSocket.CreateMessage(vSymbol);
		EXPECT_TRUE(str == _T("{\"eventName\":\"subscribe\",\"authorization\":\"c897a00b7cfc2adffc630d23befd5316a4683156\",\"eventData\":{\"thresholdLevel\":5,\"tickers\":[\"A\",\"AA\",\"AAL\",\"AAPL\",\"rig\",\"aapl\"]}}")) << "最后两个代码是为了测试手工加上的";
	}
}
