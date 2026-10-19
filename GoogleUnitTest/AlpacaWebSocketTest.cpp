#include"pch.h"

#include "gtest/gtest.h"
#include "GeneralCheck.h"

#include "AlpacaIEXWebSocket.h"

using namespace std;

namespace FireBirdTest {
	class AlpacaIEXWebSocketTest : public testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			FireBirdTest::GeneralCheck();
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
		CAlpacaIEXWebSocket m_alpacaWebSocket;
	};

	TEST_F(AlpacaIEXWebSocketTest, ParseValidQuoteReturnsTrue) {
		auto payload = make_shared<string>(
			R"([{"T":"q","S":"AAPL","bp":133.85,"bs":4,"ap":135.77,"as":5,"t":"2024-07-24T07:56:53.639713735Z"}])"
		);

		EXPECT_TRUE(m_alpacaWebSocket.ParseAlpacaIEXWebSocketData(payload));
		// If there are accessors to inspect parsed result, assert them here.
	}

	TEST_F(AlpacaIEXWebSocketTest, ParseSubscriptionReturnsTrue) {
		auto payload = make_shared<string>(
			R"([{"T":"subscription","trades":["FSK","RIG"],"quotes":["FSK"],"bars":["FSK"]}])"
		);

		EXPECT_TRUE(m_alpacaWebSocket.ParseAlpacaIEXWebSocketData(payload));
	}

	TEST_F(AlpacaIEXWebSocketTest, ParseInvalidJsonReturnsFalse) {
		auto payload = make_shared<string>(R"({"invalid": )"); // malformed JSON

		EXPECT_FALSE(m_alpacaWebSocket.ParseAlpacaIEXWebSocketData(payload));
	}
}
