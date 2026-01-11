#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoIEXWebSocket.h"

using namespace testing;

namespace FireBirdTest {
	class CTiingoIEXWebSocketTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CTiingoIEXWebSocket gl_tiingoIEXWebSocket;
	};

	TEST_F(CTiingoIEXWebSocketTest, TestGetURL) {
		EXPECT_EQ(gl_pTiingoIEXWebSocket->GetURL(), "wss://api.tiingo.com/iex");
	}

	TEST_F(CTiingoIEXWebSocketTest, TestSetSubscriptionId) {
		EXPECT_EQ(gl_pTiingoIEXWebSocket->GetSubscriptionId(), 0);
		gl_pTiingoIEXWebSocket->SetSubscriptionId(101010);
		EXPECT_EQ(gl_pTiingoIEXWebSocket->GetSubscriptionId(), 101010);

		// 恢复原状
		gl_pTiingoIEXWebSocket->SetSubscriptionId(0);
	}

	TEST_F(CTiingoIEXWebSocketTest, TestCreateMessage) {
		vectorString vSymbol;
		vSymbol.emplace_back("A");
		vSymbol.emplace_back("AA");
		vSymbol.emplace_back("AAL");
		vSymbol.emplace_back("AAPL");
		string str = gl_pTiingoIEXWebSocket->CreateMessage(vSymbol);
		nlohmannJson jsonMessage;
		try {
			jsonMessage = nlohmannJson::parse(str);
		} catch (nlohmannJson::exception&) { EXPECT_TRUE(false) << "此str应该是json制式的"; }
		EXPECT_TRUE(jsonMessage["eventName"] == "subscribe");
		EXPECT_EQ(jsonMessage["eventData"]["thresholdLevel"], 6);
		EXPECT_EQ(jsonMessage["authorization"], "c897a00b7cfc2630d235316a4683156");
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(0), "a");
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(1), "aa");
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(2), "aal");
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(3), "aapl");
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(4), "rig");
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(5), "eurusd");
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(6), "spy");
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(7), "nmm");
	}
}
