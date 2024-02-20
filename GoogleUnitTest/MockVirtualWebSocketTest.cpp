#include"pch.h"

#include"GeneralCheck.h"

#include"MockVirtualWebSocket.h"

using std::exception;
using std::make_shared;

using namespace testing;
CMockVirtualWebSocketPtr gl_pMockVirtualWebSocket;

namespace FireBirdTest {
	class CMockVirtualWebSocketTest : public ::testing::Test {
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
			gl_pMockVirtualWebSocket = make_shared<CMockVirtualWebSocket>();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();

			gl_pMockVirtualWebSocket = nullptr;
		}

	protected:
	};

	TEST_F(CMockVirtualWebSocketTest, TestConnectAndSendMessage1) {
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AAPL"));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(1)
		.WillOnce(Return(ix::ReadyState::Open)); // 调用Connect()后要等待ix链接上，其状态变为Open。链接需要时间。
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(vSymbol)).Times(1);

		EXPECT_TRUE(gl_pMockVirtualWebSocket->ConnectAndSendMessage(vSymbol));
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 0);
	}

	TEST_F(CMockVirtualWebSocketTest, TestConnectAndSendMessage2) {
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AAPL"));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(1)
		.WillOnce(Return(ix::ReadyState::Open)); // 调用Connect()后要等待ix链接上，其状态变为Open。链接需要时间。
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(vSymbol)).Times(1);

		EXPECT_TRUE(gl_pMockVirtualWebSocket->ConnectAndSendMessage(vSymbol));
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 0);
	}

	TEST_F(CMockVirtualWebSocketTest, TestConnectAndSendMessage3) {
		exception e(_T("Test Message"));
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AAPL"));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(1)
		.WillOnce(Throw(e));
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(_)).Times(0);

		EXPECT_FALSE(gl_pMockVirtualWebSocket->ConnectAndSendMessage(vSymbol));
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Test Message"));
	}

	TEST_F(CMockVirtualWebSocketTest, TestConnectAndSendMessage5) {
		exception e(_T("Test Message"));
		vectorString vSymbol, vSymbol2;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AAPL"));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(1)
		.WillOnce(Return(ix::ReadyState::Open)); // 调用Connect()后要等待ix链接上，其状态变为Open。链接需要时间。
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(vSymbol)).Times(1)
		.WillOnce(Throw(e));

		EXPECT_FALSE(gl_pMockVirtualWebSocket->ConnectAndSendMessage(vSymbol));
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Test Message"));
	}
}
