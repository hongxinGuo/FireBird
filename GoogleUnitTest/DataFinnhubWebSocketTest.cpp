#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"DataFinnhubWebSocket.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CDataFinnhubWebSocketTest : public ::testing::Test {
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
		CDataFinnhubWebSocket m_dataFinnhubWebSocket;
	};

	TEST_F(CDataFinnhubWebSocketTest, TestCreateFinnhubWebSocketString) {
		EXPECT_STREQ(m_dataFinnhubWebSocket.CreateFinnhubWebSocketString(_T("AAPL")).c_str(), _T("{\"type\":\"subscribe\",\"symbol\":\"AAPL\"}"));
		EXPECT_STREQ(m_dataFinnhubWebSocket.CreateFinnhubWebSocketString(_T("BINANCE:BTCUSDT")).c_str(), _T("{\"type\":\"subscribe\",\"symbol\":\"BINANCE:BTCUSDT\"}"));
		EXPECT_STREQ(m_dataFinnhubWebSocket.CreateFinnhubWebSocketString(_T("IC MARKETS:1")).c_str(), _T("{\"type\":\"subscribe\",\"symbol\":\"IC MARKETS:1\"}"));
	}
}