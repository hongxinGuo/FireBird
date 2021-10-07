#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WebSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CWebSocketTest : public ::testing::Test {
		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			GeneralCheck();
		}

	public:
		CWebSocket m_webSocket;
	};

	TEST_F(CWebSocketTest, TestSetScriptionStatus) {
		EXPECT_TRUE(m_webSocket.IsSubscriptable());
		m_webSocket.SetSubscriptionStatus(false);
		EXPECT_FALSE(m_webSocket.IsSubscriptable());
		m_webSocket.SetSubscriptionStatus(true);
		EXPECT_TRUE(m_webSocket.IsSubscriptable());
	}

	TEST_F(CWebSocketTest, TestSetSubscriptionId) {
		EXPECT_TRUE(m_webSocket.IsSubscriptable());// 必须设置此标识
		EXPECT_EQ(m_webSocket.GetSubscriptionId(), 0);
		m_webSocket.SetSubscriptionId(101010);
		EXPECT_EQ(m_webSocket.GetSubscriptionId(), 101010);
	}

	TEST_F(CWebSocketTest, TestSetMessagePreffix) {
		EXPECT_STREQ(m_webSocket.GetMessagePreffix().c_str(), _T(""));
		m_webSocket.SetMessagePreffix(_T("abcde"));
		EXPECT_STREQ(m_webSocket.GetMessagePreffix().c_str(), _T("abcde"));
	}

	TEST_F(CWebSocketTest, TestSetMessageSuffix) {
		EXPECT_STREQ(m_webSocket.GetMessageSuffix().c_str(), _T(""));
		m_webSocket.SetMessageSuffix(_T("abcde"));
		EXPECT_STREQ(m_webSocket.GetMessageSuffix().c_str(), _T("abcde"));
	}

	TEST_F(CWebSocketTest, TestGetState) {
		EXPECT_EQ(m_webSocket.GetState(), ix::ReadyState::Closed);
	}
}