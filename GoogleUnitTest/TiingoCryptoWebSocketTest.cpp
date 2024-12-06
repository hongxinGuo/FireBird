#include"pch.h"

////#include"gtest/gtest.h"

import System.Configuration;

#include"GeneralCheck.h"

#include"TiingoCryptoWebSocket.h"

#include"nlohmannJsonDeclaration.h" // 按照顺序输出json，必须使用此ordered_json,以保证解析后的数据与解析前的顺序一致。
#include"nlohmann/json.hpp"

using namespace testing;

namespace FireBirdTest {
	class CTiingoCryptoWebSocketTest : public Test {
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
		CTiingoCryptoWebSocket gl_tiingoCryptoWebSocket;
	};

	TEST_F(CTiingoCryptoWebSocketTest, TestGetURL) {
		EXPECT_STREQ(gl_pTiingoCryptoWebSocket->GetURL().c_str(), _T("wss://api.tiingo.com/crypto"));
	}

	TEST_F(CTiingoCryptoWebSocketTest, TestCreateMessage) {
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AA"));
		vSymbol.push_back(_T("AAL"));
		vSymbol.push_back(_T("AAPL"));
		string strSymbol = gl_pTiingoCryptoWebSocket->CreateMessage(vSymbol);
		json jsonMessage;
		try {
			jsonMessage = json::parse(strSymbol);
		} catch (json::exception&) { EXPECT_TRUE(false) << "此str应该是json制式的"; }
		EXPECT_TRUE(jsonMessage["eventName"] == _T("subscribe"));
		EXPECT_EQ(jsonMessage["eventData"]["thresholdLevel"], 2);
		EXPECT_EQ(jsonMessage["authorization"], _T("c897a00b7cfc2630d235316a4683156"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(0), _T("a"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(1), _T("aa"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(2), _T("aal"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(3), _T("aapl"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(4), _T("dkaeth"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(5), _T("eurusd"));
		EXPECT_EQ(jsonMessage["eventData"]["tickers"].at(6), _T("ksmust"));
	}
}
