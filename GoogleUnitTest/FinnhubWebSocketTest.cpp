#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubWebSocket.h"

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
		CFinnhubWebSocket m_finnhubWebSocket;
	};

	TEST_F(CDataFinnhubWebSocketTest, TestSetScriptionStatus) {
		EXPECT_FALSE(m_finnhubWebSocket.IsSubscriptable());
		m_finnhubWebSocket.SetSubscriptionStatus(true);
		EXPECT_TRUE(m_finnhubWebSocket.IsSubscriptable());
		m_finnhubWebSocket.SetSubscriptionStatus(false);
		EXPECT_FALSE(m_finnhubWebSocket.IsSubscriptable());
	}

	TEST_F(CDataFinnhubWebSocketTest, TestSetSubscriptionId) {
		EXPECT_FALSE(m_finnhubWebSocket.IsSubscriptable());// �������ô˱�ʶ
		m_finnhubWebSocket.SetSubscriptionStatus(true);
		EXPECT_EQ(m_finnhubWebSocket.GetSubscriptionId(), 0);
		m_finnhubWebSocket.SetSubscriptionId(101010);
		EXPECT_EQ(m_finnhubWebSocket.GetSubscriptionId(), 101010);

		m_finnhubWebSocket.SetSubscriptionStatus(false);
	}

	TEST_F(CDataFinnhubWebSocketTest, TestGetState) {
		EXPECT_EQ(m_finnhubWebSocket.GetState(), ix::ReadyState::Closed);
	}

	TEST_F(CDataFinnhubWebSocketTest, TestIsClosed) {
		EXPECT_EQ(m_finnhubWebSocket.GetState(), ix::ReadyState::Closed);
		EXPECT_TRUE(m_finnhubWebSocket.IsClosed()) << "�޷����ô˱�ʶ���˱�ʶĬ��Ϊ�棬ֻ���ж�����״̬�� �޷�����Ϊ�ٵ�״̬";
		EXPECT_FALSE(m_finnhubWebSocket.IsOpen()) << "�޷����ô˱�ʶ��ֻ���жϴ˱�ʶΪ��";
		EXPECT_FALSE(m_finnhubWebSocket.IsClosing()) << "�޷����ô˱�ʶ��ֻ���жϴ˱�ʶΪ��";
		EXPECT_FALSE(m_finnhubWebSocket.IsConnecting()) << "�޷����ô˱�ʶ��ֻ���жϴ˱�ʶΪ��";
	}

	TEST_F(CDataFinnhubWebSocketTest, TestCreateFinnhubWebSocketString) {
		EXPECT_STREQ(m_finnhubWebSocket.CreateFinnhubWebSocketString(_T("AAPL")).c_str(), _T("{\"type\":\"subscribe\",\"symbol\":\"AAPL\"}"));
		EXPECT_STREQ(m_finnhubWebSocket.CreateFinnhubWebSocketString(_T("BINANCE:BTCUSDT")).c_str(), _T("{\"type\":\"subscribe\",\"symbol\":\"BINANCE:BTCUSDT\"}"));
		EXPECT_STREQ(m_finnhubWebSocket.CreateFinnhubWebSocketString(_T("IC MARKETS:1")).c_str(), _T("{\"type\":\"subscribe\",\"symbol\":\"IC MARKETS:1\"}"));
	}
}