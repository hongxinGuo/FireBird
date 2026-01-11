#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"TiingoForexWebSocket.h"

using namespace testing;

namespace FireBirdTest {
	class CTiingoForexWebSocketTest : public Test {
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
		CTiingoForexWebSocket gl_tiingoForexWebSocket;
	};

	TEST_F(CTiingoForexWebSocketTest, TestGetURL) {
		EXPECT_EQ(gl_pTiingoForexWebSocket->GetURL(), "wss://api.tiingo.com/fx");
	}

	TEST_F(CTiingoForexWebSocketTest, TestCreateMessage) {
		vectorString vSymbol;
		vSymbol.push_back("A");
		vSymbol.push_back("AA");
		vSymbol.push_back("AAL");
		vSymbol.push_back("AAPL");
		string sSymbol = gl_pTiingoForexWebSocket->CreateMessage(vSymbol);
		nlohmannJson jsonMessage;
		try {
			jsonMessage = nlohmannJson::parse(sSymbol);
		} catch (nlohmannJson::exception&) { EXPECT_TRUE(false) << "此str应该是json制式的"; }
		EXPECT_TRUE(jsonMessage["eventName"] == "subscribe");
		EXPECT_EQ(jsonMessage["eventData"]["thresholdLevel"], 5);
		EXPECT_EQ(jsonMessage["authorization"], "c897a00b7cfc2630d235316a4683156");
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(0), "a");
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(1), "aa");
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(2), "aal");
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(3), "aapl");
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(4), "gbpaud");
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(5), "eurusd");
	}
}
