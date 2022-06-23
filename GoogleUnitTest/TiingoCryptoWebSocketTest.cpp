#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoCryptoWebSocket.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CDataTiingoCryptoWebSocketTest : public ::testing::Test {
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
		CTiingoCryptoWebSocket m_tiingoCryptoWebSocket;
	};

	TEST_F(CDataTiingoCryptoWebSocketTest, TestGetURL) {
		EXPECT_STREQ(m_tiingoCryptoWebSocket.GetURL().c_str(), _T("wss://api.tiingo.com/crypto"));
	}

	TEST_F(CDataTiingoCryptoWebSocketTest, TestCreateTiingoCryptoWebSocketSymbolString) {
		vector<CString> vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AA"));
		vSymbol.push_back(_T("AAL"));
		vSymbol.push_back(_T("AAPL"));
		CString strSymbols = m_tiingoCryptoWebSocket.CreateTiingoWebSocketSymbolString(vSymbol);
		EXPECT_STREQ(strSymbols, _T("\"A\",\"AA\",\"AAL\",\"AAPL\""));
	}

	TEST_F(CDataTiingoCryptoWebSocketTest, TestCreateMessage) {
		vector<CString> vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AA"));
		vSymbol.push_back(_T("AAL"));
		vSymbol.push_back(_T("AAPL"));
		CString strSymbols = m_tiingoCryptoWebSocket.CreateMessage(vSymbol);
		EXPECT_STREQ(strSymbols, _T("{\"eventName\":\"subscribe\",\"authorization\":\"c897a00b7cfc2adffc630d23befd5316a4683156\",\"eventData\":{\"thresholdLevel\":2,\"tickers\":[\"A\",\"AA\",\"AAL\",\"AAPL\",\"dkaeth\",\"ksmust\"]}}")) << "最后两个代码是为了测试手工加上的";
	}
}