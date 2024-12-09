//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CVirtualWebSocket是虚类，无法测试。其函数的测试放在这里
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

 

#include"GeneralCheck.h"

import FireBird.WebSocket.Finnhub;

using namespace testing;

namespace FireBirdTest {
	class CFinnhubWebSocketTest : public ::testing::Test {
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
		CFinnhubWebSocket m_finnhubWebSocket;
	};

	TEST_F(CFinnhubWebSocketTest, TestGetURL) {
		EXPECT_STREQ(m_finnhubWebSocket.GetURL().c_str(), _T("wss://ws.finnhub.io"));
	}

	TEST_F(CFinnhubWebSocketTest, TestSetSubscriptionId) {
		EXPECT_EQ(m_finnhubWebSocket.GetSubscriptionId(), 0);
		m_finnhubWebSocket.SetSubscriptionId(101010);
		EXPECT_EQ(m_finnhubWebSocket.GetSubscriptionId(), 101010);
	}

	TEST_F(CFinnhubWebSocketTest, TestGetState) {
		EXPECT_EQ(m_finnhubWebSocket.GetState(), ix::ReadyState::Closed);
	}

	TEST_F(CFinnhubWebSocketTest, TestIsClosed) {
		EXPECT_EQ(m_finnhubWebSocket.GetState(), ix::ReadyState::Closed);
		EXPECT_TRUE(m_finnhubWebSocket.IsClosed()) << "无法设置此标识。此标识默认为真，只能判断这种状态， 无法测试为假的状态";
		EXPECT_FALSE(m_finnhubWebSocket.IsOpen()) << "无法设置此标识，只能判断此标识为假";
		EXPECT_FALSE(m_finnhubWebSocket.IsClosing()) << "无法设置此标识，只能判断此标识为假";
		EXPECT_FALSE(m_finnhubWebSocket.IsConnecting()) << "无法设置此标识，只能判断此标识为假";
	}

	TEST_F(CFinnhubWebSocketTest, TestCreateFinnhubWebSocketString) {
		EXPECT_STREQ(m_finnhubWebSocket.CreateFinnhubWebSocketString(_T("AAPL")).c_str(), _T("{\"type\":\"subscribe\",\"symbol\":\"AAPL\"}"));
		EXPECT_STREQ(m_finnhubWebSocket.CreateFinnhubWebSocketString(_T("BINANCE:BTCUSDT")).c_str(), _T("{\"type\":\"subscribe\",\"symbol\":\"BINANCE:BTCUSDT\"}"));
		EXPECT_STREQ(m_finnhubWebSocket.CreateFinnhubWebSocketString(_T("IC MARKETS:1")).c_str(), _T("{\"type\":\"subscribe\",\"symbol\":\"IC MARKETS:1\"}"));
	}

	TEST_F(CFinnhubWebSocketTest, TestAddSymbol) {
		const vectorString vSymbol{ _T("a"), _T("b"), _T("c"), _T("d"), _T("e") };

		EXPECT_EQ(m_finnhubWebSocket.GetSymbolSize(), 0);
		m_finnhubWebSocket.AppendSymbol(vSymbol);
		EXPECT_EQ(m_finnhubWebSocket.GetSymbolSize(), 5);
		EXPECT_TRUE(m_finnhubWebSocket.IsSymbol(_T("a")));
		EXPECT_TRUE(m_finnhubWebSocket.IsSymbol(_T("c")));
		EXPECT_TRUE(m_finnhubWebSocket.IsSymbol(_T("e")));

		EXPECT_FALSE(m_finnhubWebSocket.AddSymbol(_T("a"))) << "a已在符号集中";
		EXPECT_TRUE(m_finnhubWebSocket.AddSymbol(_T("f"))) << "f不在符号集中";
		EXPECT_EQ(m_finnhubWebSocket.GetSymbolSize(), 6);

		EXPECT_FALSE(m_finnhubWebSocket.DeleteSymbol(_T("g"))) << "g不在符号集中";
		EXPECT_TRUE(m_finnhubWebSocket.DeleteSymbol(_T("a"))) << "a在符号集中";
		EXPECT_FALSE(m_finnhubWebSocket.IsSymbol(_T("a"))) << "a已被删除";
		EXPECT_TRUE(m_finnhubWebSocket.DeleteSymbol(_T("c"))) << "c在符号集中";
		EXPECT_FALSE(m_finnhubWebSocket.IsSymbol(_T("c"))) << "c已被删除";
		EXPECT_EQ(m_finnhubWebSocket.GetSymbolSize(), 4);

		m_finnhubWebSocket.ClearSymbol();
		EXPECT_EQ(m_finnhubWebSocket.GetSymbolSize(), 0);
	}

	TEST_F(CFinnhubWebSocketTest, Test3) {}

	TEST_F(CFinnhubWebSocketTest, Test4) {}

	TEST_F(CFinnhubWebSocketTest, Test5) {}
}
