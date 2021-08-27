#include"pch.h"

#include"globedef.h"

#include"WebSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CWebSocketTest : public ::testing::Test {
		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
		}

	public:
		CWebSocket m_webSocket;
	};

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