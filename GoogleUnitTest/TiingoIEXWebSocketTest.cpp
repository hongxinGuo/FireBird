#include"pch.h"

////#include"gtest/gtest.h"

import System.Configuration;

#include"GeneralCheck.h"

#include"TiingoIEXWebSocket.h"

#include"nlohmann/json.hpp"

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
		EXPECT_STREQ(gl_pTiingoIEXWebSocket->GetURL().c_str(), _T("wss://api.tiingo.com/iex"));
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
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AA"));
		vSymbol.push_back(_T("AAL"));
		vSymbol.push_back(_T("AAPL"));
		string str = gl_pTiingoIEXWebSocket->CreateMessage(vSymbol);
		nlohmann::ordered_json jsonMessage;
		try {
			jsonMessage = nlohmann::ordered_json::parse(str);
		} catch (nlohmann::ordered_json::exception&) { EXPECT_TRUE(false) << "此str应该是json制式的"; }
		EXPECT_TRUE(jsonMessage["eventName"] == _T("subscribe"));
		EXPECT_EQ(jsonMessage["eventData"]["thresholdLevel"], 5);
		EXPECT_EQ(jsonMessage["authorization"], _T("c897a00b7cfc2630d235316a4683156"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(0), _T("a"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(1), _T("aa"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(2), _T("aal"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(3), _T("aapl"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(4), _T("rig"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(5), _T("eurusd"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(6), _T("spy"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(7), _T("uso"));
	}
}
