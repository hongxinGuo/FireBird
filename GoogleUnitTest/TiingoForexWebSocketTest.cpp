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

namespace StockAnalysisTest {
	class CDataTiingoForexWebSocketTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearu
			GeneralCheck();
		}

	protected:
		CTiingoForexWebSocket m_tiingoForexWebSocket;
	};

	TEST_F(CDataTiingoForexWebSocketTest, TestGetURL) {
		EXPECT_STREQ(m_tiingoForexWebSocket.GetURL().c_str(), _T("wss://api.tiingo.com/fx"));
	}

	TEST_F(CDataTiingoForexWebSocketTest, TestCreateTiingoForexWebSocketSymbolString) {
		vector<CString> vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AA"));
		vSymbol.push_back(_T("AAL"));
		vSymbol.push_back(_T("AAPL"));
		CString strSymbols = m_tiingoForexWebSocket.CreateTiingoWebSocketSymbolString(vSymbol);
		EXPECT_STREQ(strSymbols, _T("\"A\",\"AA\",\"AAL\",\"AAPL\""));
	}

	TEST_F(CDataTiingoForexWebSocketTest, TestCreateMessage) {
		vector<CString> vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AA"));
		vSymbol.push_back(_T("AAL"));
		vSymbol.push_back(_T("AAPL"));
		CString strSymbols = m_tiingoForexWebSocket.CreateMessage(vSymbol);
		EXPECT_STREQ(strSymbols, _T("{\"eventName\":\"subscribe\",\"authorization\":\"c897a00b7cfc2adffc630d23befd5316a4683156\",\"eventData\":{\"thresholdLevel\":5,\"tickers\":[\"A\",\"AA\",\"AAL\",\"AAPL\",\"gbpaud\",\"eurusd\"]}}")) << "最后两个代码是为了测试手工加上的";
	}
}