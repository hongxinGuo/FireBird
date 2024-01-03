#include"pch.h"

#include"GeneralCheck.h"

#include"MockVirtualWebSocket.h"

#include<exception>
using std::exception;

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

	TEST_F(CMockVirtualWebSocketTest, TestConnectWebSocketAndSendMessage1) {
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AAPL"));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(3)
		.WillOnce(Return(ix::ReadyState::Open)) // 调用Disconnect()后要等到ix关闭链接，其状态变为Closed。关闭需要时间
		.WillOnce(Return(ix::ReadyState::Closed)) // 调用Disconnect()后要等到ix关闭链接，其状态变为Closed。关闭需要时间
		.WillOnce(Return(ix::ReadyState::Open)); // 调用Connect()后要等待ix链接上，其状态变为Open。链接需要时间。
		EXPECT_CALL(*gl_pMockVirtualWebSocket, StopWebSocket).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(vSymbol)).Times(1);

		EXPECT_TRUE(gl_pMockVirtualWebSocket->ConnectWebSocketAndSendMessage(vSymbol));
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 0);
	}

	TEST_F(CMockVirtualWebSocketTest, TestConnectWebSocketAndSendMessage2) {
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AAPL"));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(3)
		.WillOnce(Return(ix::ReadyState::Closed)) // 调用Disconnect()后要等到ix关闭链接，其状态变为Closed。关闭需要时间
		.WillOnce(Return(ix::ReadyState::Closed)) // 调用Disconnect()后要等到ix关闭链接，其状态变为Closed。关闭需要时间
		.WillOnce(Return(ix::ReadyState::Open)); // 调用Connect()后要等待ix链接上，其状态变为Open。链接需要时间。
		EXPECT_CALL(*gl_pMockVirtualWebSocket, StopWebSocket).Times(0); // 初始状态为closed时，无需执行关闭任务。
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(vSymbol)).Times(1);

		EXPECT_TRUE(gl_pMockVirtualWebSocket->ConnectWebSocketAndSendMessage(vSymbol));
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 0);
	}

	TEST_F(CMockVirtualWebSocketTest, TestConnectWebSocketAndSendMessage3) {
		exception e(_T("Test Message"));
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AAPL"));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(2)
		.WillOnce(Return(ix::ReadyState::Open)) // 当状态不为Closed时调用StopWebSocket()。
		.WillOnce(Return(ix::ReadyState::Closed)); // 调用Disconnect()后要等到ix关闭链接，其状态变为Closed。关闭需要时间
		EXPECT_CALL(*gl_pMockVirtualWebSocket, StopWebSocket).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(1)
		.WillOnce(Throw(e));
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(_)).Times(0);

		EXPECT_FALSE(gl_pMockVirtualWebSocket->ConnectWebSocketAndSendMessage(vSymbol));
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Test Message"));
	}

	TEST_F(CMockVirtualWebSocketTest, TestConnectWebSocketAndSendMessage4) {
		exception e(_T("Test Message"));
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AAPL"));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(1)
		.WillOnce(Return(ix::ReadyState::Open)); // 调用Disconnect()后要等到ix关闭链接，其状态变为Closed。关闭需要时间
		EXPECT_CALL(*gl_pMockVirtualWebSocket, StopWebSocket).Times(1)
		.WillOnce(Throw(e));
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(0);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(_)).Times(0);

		EXPECT_FALSE(gl_pMockVirtualWebSocket->ConnectWebSocketAndSendMessage(vSymbol));
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Test Message"));
	}

	TEST_F(CMockVirtualWebSocketTest, TestConnectWebSocketAndSendMessage5) {
		exception e(_T("Test Message"));
		vectorString vSymbol, vSymbol2;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AAPL"));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(3)
		.WillOnce(Return(ix::ReadyState::Open)) // 当状态不为Closed时调用StopWebSocket()。
		.WillOnce(Return(ix::ReadyState::Closed)) // 调用Disconnect()后要等到ix关闭链接，其状态变为Closed。关闭需要时间
		.WillOnce(Return(ix::ReadyState::Open)); // 调用Connect()后要等待ix链接上，其状态变为Open。链接需要时间。
		EXPECT_CALL(*gl_pMockVirtualWebSocket, StopWebSocket).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(vSymbol)).Times(1)
		.WillOnce(Throw(e));

		EXPECT_FALSE(gl_pMockVirtualWebSocket->ConnectWebSocketAndSendMessage(vSymbol));
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Test Message"));
	}

	TEST_F(CMockVirtualWebSocketTest, TestDisconnect1) {
		gl_pMockVirtualWebSocket->SetSubscriptionId(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(3)
		.WillOnce(Return(ix::ReadyState::Open))	// 状态不是Closed时，调用StopWebSocket函数。
		.WillOnce(Return(ix::ReadyState::Open)) // 当状态不是Closed时，函数会等待，直到状态变成Closed。
		.WillOnce(Return(ix::ReadyState::Closed)); // 调用Disconnect()后要等到ix关闭链接，其状态变为Closed。关闭需要时间
		EXPECT_CALL(*gl_pMockVirtualWebSocket, StopWebSocket).Times(1); // 当webSocket状态不是Closed时，需要执行关闭任务

		gl_pMockVirtualWebSocket->Disconnect();

		EXPECT_EQ(gl_pMockVirtualWebSocket->GetSubscriptionId(), 0);
	}

	TEST_F(CMockVirtualWebSocketTest, TestDisconnect2) {
		gl_pMockVirtualWebSocket->SetSubscriptionId(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(2)
		.WillOnce(Return(ix::ReadyState::Closed)) // 
		.WillOnce(Return(ix::ReadyState::Closed)); // 调用Disconnect()后要等到ix关闭链接，其状态变为Closed。关闭需要时间
		EXPECT_CALL(*gl_pMockVirtualWebSocket, StopWebSocket).Times(0); // 当webSocket状态为Closed时，无需执行关闭任务

		gl_pMockVirtualWebSocket->Disconnect();

		EXPECT_EQ(gl_pMockVirtualWebSocket->GetSubscriptionId(), 0);
	}

	TEST_F(CMockVirtualWebSocketTest, TestDisconnectWithoutWaitingSucceed1) {
		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(1)
		.WillOnce(Return(ix::ReadyState::Open));
		EXPECT_CALL(*gl_pMockVirtualWebSocket, StopWebSocket).Times(1);

		EXPECT_TRUE(gl_pMockVirtualWebSocket->DisconnectWithoutWaitingSucceed());
	}

	TEST_F(CMockVirtualWebSocketTest, TestDisconnectWithoutWaitingSucceed2) {
		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(1)
		.WillOnce(Return(ix::ReadyState::Closed));
		EXPECT_CALL(*gl_pMockVirtualWebSocket, StopWebSocket).Times(0);

		EXPECT_TRUE(gl_pMockVirtualWebSocket->DisconnectWithoutWaitingSucceed());
	}
}
