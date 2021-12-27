#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"DataTiingoCryptoWebSocket.h"

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
		CDataTiingoCryptoWebSocket m_dataTiingoCryptoWebSocket;
	};

	TEST_F(CDataTiingoCryptoWebSocketTest, TestCreateTiingoCryptoWebSocketSymbolString) {
		vector<CString> vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AA"));
		vSymbol.push_back(_T("AAL"));
		vSymbol.push_back(_T("AAPL"));
		CString strSymbols = m_dataTiingoCryptoWebSocket.CreateTiingoWebSocketSymbolString(vSymbol);
		EXPECT_STREQ(strSymbols, _T("\"A\",\"AA\",\"AAL\",\"AAPL\""));
	}
}