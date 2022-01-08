#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"TiingoIEXWebSocket.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CDataTiingoIEXWebSocketTest : public ::testing::Test {
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
		CTiingoIEXWebSocket m_tiingoIEXWebSocket;
	};

	TEST_F(CDataTiingoIEXWebSocketTest, TestSetScriptionStatus) {
		EXPECT_TRUE(m_tiingoIEXWebSocket.IsSubscriptable());
		m_tiingoIEXWebSocket.SetSubscriptionStatus(false);
		EXPECT_FALSE(m_tiingoIEXWebSocket.IsSubscriptable());
		m_tiingoIEXWebSocket.SetSubscriptionStatus(true);
		EXPECT_TRUE(m_tiingoIEXWebSocket.IsSubscriptable());
	}

	TEST_F(CDataTiingoIEXWebSocketTest, TestSetSubscriptionId) {
		EXPECT_TRUE(m_tiingoIEXWebSocket.IsSubscriptable());// 必须设置此标识
		EXPECT_EQ(m_tiingoIEXWebSocket.GetSubscriptionId(), 0);
		m_tiingoIEXWebSocket.SetSubscriptionId(101010);
		EXPECT_EQ(m_tiingoIEXWebSocket.GetSubscriptionId(), 101010);
	}

	TEST_F(CDataTiingoIEXWebSocketTest, TestCreateTiingoIEXWebSocketSymbolString) {
		vector<CString> vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AA"));
		vSymbol.push_back(_T("AAL"));
		vSymbol.push_back(_T("AAPL"));
		CString strSymbols = m_tiingoIEXWebSocket.CreateTiingoWebSocketSymbolString(vSymbol);
		EXPECT_STREQ(strSymbols, _T("\"A\",\"AA\",\"AAL\",\"AAPL\""));
	}
}