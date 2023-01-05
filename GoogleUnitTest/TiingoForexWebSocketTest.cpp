#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"TiingoForexWebSocket.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace FireBirdTest {
	class CDataTiingoForexWebSocketTest : public ::testing::Test {
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
		CTiingoForexWebSocket gl_tiingoForexWebSocket;
	};

	TEST_F(CDataTiingoForexWebSocketTest, TestGetURL) {
		EXPECT_STREQ(gl_tiingoForexWebSocket.GetURL().c_str(), _T("wss://api.tiingo.com/fx"));
	}

	TEST_F(CDataTiingoForexWebSocketTest, TestCreateTiingoForexWebSocketSymbolString) {
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AA"));
		vSymbol.push_back(_T("AAL"));
		vSymbol.push_back(_T("AAPL"));
		string sSymbols = gl_tiingoForexWebSocket.CreateTiingoWebSocketSymbolString(vSymbol);
		EXPECT_TRUE(sSymbols == _T("\"A\",\"AA\",\"AAL\",\"AAPL\""));
	}

	TEST_F(CDataTiingoForexWebSocketTest, TestCreateMessage) {
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AA"));
		vSymbol.push_back(_T("AAL"));
		vSymbol.push_back(_T("AAPL"));
		string sSymbols = gl_tiingoForexWebSocket.CreateMessage(vSymbol);
		EXPECT_TRUE(sSymbols == _T("{\"eventName\":\"subscribe\",\"authorization\":\"c897a00b7cfc2adffc630d23befd5316a4683156\",\"eventData\":{\"thresholdLevel\":5,\"tickers\":[\"A\",\"AA\",\"AAL\",\"AAPL\",\"gbpaud\",\"eurusd\"]}}")) << "最后两个代码是为了测试手工加上的";
	}
}
