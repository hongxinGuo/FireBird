#include"pch.h"

#include"GeneralCheck.h"

#include"MockVirtualWebSocket.h"
#include "WorldMarket.h"

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

			vSymbol.push_back(_T("A"));
			vSymbol.push_back(_T("AAPL"));
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();

			gl_pMockVirtualWebSocket = nullptr;
		}

	protected:
		vectorString vSymbol;
	};

	TEST_F(CMockVirtualWebSocketTest, TestConnectAndSendMessage1) {
		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(1)
		.WillOnce(Return(ix::ReadyState::Open)); // 调用Connect()后要等待ix链接上，其状态变为Open。链接需要时间。
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(vSymbol)).Times(1);

		EXPECT_TRUE(gl_pMockVirtualWebSocket->ConnectAndSendMessage(vSymbol));
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 0);
	}

	TEST_F(CMockVirtualWebSocketTest, TestConnectAndSendMessage2) {
		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(1)
		.WillOnce(Return(ix::ReadyState::Open)); // 调用Connect()后要等待ix链接上，其状态变为Open。链接需要时间。
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(vSymbol)).Times(1);

		EXPECT_TRUE(gl_pMockVirtualWebSocket->ConnectAndSendMessage(vSymbol));
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 0);
	}

	TEST_F(CMockVirtualWebSocketTest, TestConnectAndSendMessage3) {
		exception e(_T("Test Message"));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(1)
		.WillOnce(Throw(e));
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(_)).Times(0);

		EXPECT_FALSE(gl_pMockVirtualWebSocket->ConnectAndSendMessage(vSymbol));
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Test Message"));
	}

	TEST_F(CMockVirtualWebSocketTest, TestConnectAndSendMessage5) {
		exception e(_T("Test Message"));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(1)
		.WillOnce(Return(ix::ReadyState::Open)); // 调用Connect()后要等待ix链接上，其状态变为Open。链接需要时间。
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(vSymbol)).Times(1)
		.WillOnce(Throw(e));

		EXPECT_FALSE(gl_pMockVirtualWebSocket->ConnectAndSendMessage(vSymbol));
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage(), _T("Test Message"));
	}

	TEST_F(CMockVirtualWebSocketTest, TestMonitorWebSocket1) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState()).Times(1)
		.WillOnce(Return(ix::ReadyState::Open));
		EXPECT_CALL(*gl_pMockVirtualWebSocket, TaskDisconnect()).Times(1);

		gl_pMockVirtualWebSocket->MonitorWebSocket(true, true, vSymbol);

		// 恢复原状
	}

	TEST_F(CMockVirtualWebSocketTest, TestMonitorWebSocket2) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState()).Times(1)
		.WillOnce(Return(ix::ReadyState::Closed));
		EXPECT_CALL(*gl_pMockVirtualWebSocket, TaskDisconnect()).Times(0); // "web Socket已关闭，无需再次关闭";

		gl_pMockVirtualWebSocket->MonitorWebSocket(true, false, vSymbol);

		// 恢复原状
	}

	TEST_F(CMockVirtualWebSocketTest, TestMonitorWebSocket3) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Closed));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, TaskDisconnect()).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, TaskConnectAndSendMessage).Times(1);

		gl_pMockVirtualWebSocket->MonitorWebSocket(false, true, vSymbol);
		EXPECT_EQ(gl_pMockVirtualWebSocket->GetHeartbeatTime(), GetUTCTime());

		// 恢复原状
	}

	TEST_F(CMockVirtualWebSocketTest, TestMonitorWebSocket4) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		gl_pMockVirtualWebSocket->SetError(true);

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, TaskDisconnect()).Times(0);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, TaskConnectAndSendMessage).Times(1);

		gl_pMockVirtualWebSocket->MonitorWebSocket(false, true, vSymbol);
		EXPECT_EQ(gl_pMockVirtualWebSocket->GetHeartbeatTime(), GetUTCTime());

		// 恢复原状
	}

	TEST_F(CMockVirtualWebSocketTest, TestMonitorWebSocket5) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		gl_pMockVirtualWebSocket->SetError(true);

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, TaskDisconnect()).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, TaskConnectAndSendMessage).Times(0);

		gl_pMockVirtualWebSocket->MonitorWebSocket(false, true, vSymbol);

		// 恢复原状
	}

	TEST_F(CMockVirtualWebSocketTest, TestMonitorWebSocket6) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		gl_pMockVirtualWebSocket->SetError(true);

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, TaskDisconnect()).Times(0);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, TaskConnectAndSendMessage).Times(0);

		gl_pMockVirtualWebSocket->MonitorWebSocket(false, true, vSymbol);

		// 恢复原状
	}

	TEST_F(CMockVirtualWebSocketTest, TestMonitorWebSocket7) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		gl_pMockVirtualWebSocket->SetError(true);

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState()).Times(1)
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, TaskDisconnect()).Times(1);

		gl_pMockVirtualWebSocket->MonitorWebSocket(false, false, vSymbol);

		// 恢复原状
	}
}
