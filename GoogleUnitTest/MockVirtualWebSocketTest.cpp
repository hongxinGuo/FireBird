#include"pch.h"

#include"GeneralCheck.h"

#include"MockVirtualWebSocket.h"
#include "SystemMessage.h"
#include "WorldMarket.h"

using namespace testing;
using namespace std;

namespace {
	CMockVirtualWebSocketPtr s_pMockVirtualWebSocket;
}

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
			s_pMockVirtualWebSocket = make_shared<CMockVirtualWebSocket>();

			vSymbol.emplace_back("A");
			vSymbol.emplace_back("AAPL");
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();

			s_pMockVirtualWebSocket = nullptr;
		}

	protected:
		vector<string> vSymbol;
	};

	TEST_F(CMockVirtualWebSocketTest, TestMonitorWebSocket1) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		EXPECT_CALL(*s_pMockVirtualWebSocket, GetState()).Times(1)
		.WillOnce(Return(ix::ReadyState::Open));
		EXPECT_CALL(*s_pMockVirtualWebSocket, TaskDisconnect()).Times(1);

		s_pMockVirtualWebSocket->MonitorWebSocket(true, true, vSymbol);

		// 恢复原状
	}

	TEST_F(CMockVirtualWebSocketTest, TestMonitorWebSocket2) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		EXPECT_CALL(*s_pMockVirtualWebSocket, GetState()).Times(1)
		.WillOnce(Return(ix::ReadyState::Closed));
		EXPECT_CALL(*s_pMockVirtualWebSocket, TaskDisconnect()).Times(0); // "web Socket已关闭，无需再次关闭";

		s_pMockVirtualWebSocket->MonitorWebSocket(true, false, vSymbol);

		// 恢复原状
	}

	TEST_F(CMockVirtualWebSocketTest, TestMonitorWebSocket3) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		EXPECT_CALL(*s_pMockVirtualWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Closed));

		EXPECT_CALL(*s_pMockVirtualWebSocket, TaskDisconnect()).Times(1);
		EXPECT_CALL(*s_pMockVirtualWebSocket, TaskConnectAndSendMessage).Times(1);

		s_pMockVirtualWebSocket->MonitorWebSocket(false, true, vSymbol);
		EXPECT_EQ(s_pMockVirtualWebSocket->GetHeartbeatTime(), GetUTCTime());

		// 恢复原状
	}

	TEST_F(CMockVirtualWebSocketTest, TestMonitorWebSocket4) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		s_pMockVirtualWebSocket->SetError(true);

		EXPECT_CALL(*s_pMockVirtualWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed));

		EXPECT_CALL(*s_pMockVirtualWebSocket, TaskDisconnect()).Times(0);
		EXPECT_CALL(*s_pMockVirtualWebSocket, TaskConnectAndSendMessage).Times(1);

		s_pMockVirtualWebSocket->MonitorWebSocket(false, true, vSymbol);
		EXPECT_EQ(s_pMockVirtualWebSocket->GetHeartbeatTime(), GetUTCTime());

		// 恢复原状
	}

	TEST_F(CMockVirtualWebSocketTest, TestMonitorWebSocket5) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		s_pMockVirtualWebSocket->SetError(true);

		EXPECT_CALL(*s_pMockVirtualWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(*s_pMockVirtualWebSocket, TaskDisconnect()).Times(1);
		EXPECT_CALL(*s_pMockVirtualWebSocket, TaskConnectAndSendMessage).Times(0);

		s_pMockVirtualWebSocket->MonitorWebSocket(false, true, vSymbol);

		// 恢复原状
	}

	TEST_F(CMockVirtualWebSocketTest, TestMonitorWebSocket6) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		s_pMockVirtualWebSocket->SetError(true);

		EXPECT_CALL(*s_pMockVirtualWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(*s_pMockVirtualWebSocket, TaskDisconnect()).Times(0);
		EXPECT_CALL(*s_pMockVirtualWebSocket, TaskConnectAndSendMessage).Times(0);

		s_pMockVirtualWebSocket->MonitorWebSocket(false, true, vSymbol);

		// 恢复原状
	}

	TEST_F(CMockVirtualWebSocketTest, TestMonitorWebSocket7) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		s_pMockVirtualWebSocket->SetError(true);

		EXPECT_CALL(*s_pMockVirtualWebSocket, GetState()).Times(1)
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(*s_pMockVirtualWebSocket, TaskDisconnect()).Times(1);

		s_pMockVirtualWebSocket->MonitorWebSocket(false, false, vSymbol);

		// 恢复原状
	}
}
