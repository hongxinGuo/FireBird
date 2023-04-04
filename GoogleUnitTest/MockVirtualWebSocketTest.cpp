#include"pch.h"

#include"GeneralCheck.h"

#include"MockvirtualWebSocket.h"

#include<exception>
using std::exception;

using namespace testing;
CMockVirtualWebSocketPtr gl_pMockVirtualWebSocket;

namespace FireBirdTest {
	class CMockVirtualWebSocketTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		void SetUp() override {
			GeneralCheck();
			gl_pMockVirtualWebSocket = make_shared<CMockVirtualWebSocket>();
		}

		void TearDown() override {
			// clearUp
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
		.WillOnce(Return(ix::ReadyState::Open)) // ����Disconnect()��Ҫ�ȵ�ix�ر����ӣ���״̬��ΪClosed���ر���Ҫʱ��
		.WillOnce(Return(ix::ReadyState::Closed)) // ����Disconnect()��Ҫ�ȵ�ix�ر����ӣ���״̬��ΪClosed���ر���Ҫʱ��
		.WillOnce(Return(ix::ReadyState::Open)); // ����Connect()��Ҫ�ȴ�ix�����ϣ���״̬��ΪOpen��������Ҫʱ�䡣
		EXPECT_CALL(*gl_pMockVirtualWebSocket, StopWebSocket).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(vSymbol)).Times(1);

		EXPECT_TRUE(gl_pMockVirtualWebSocket->ConnectWebSocketAndSendMessage(vSymbol));
	}

	TEST_F(CMockVirtualWebSocketTest, TestConnectWebSocketAndSendMessage2) {
		exception e;
		vectorString vSymbol, vSymbol2;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AAPL"));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(3)
		.WillOnce(Return(ix::ReadyState::Open)) // ����Disconnect()��Ҫ�ȵ�ix�ر����ӣ���״̬��ΪClosed���ر���Ҫʱ��
		.WillOnce(Return(ix::ReadyState::Closed)) // ����Disconnect()��Ҫ�ȵ�ix�ر����ӣ���״̬��ΪClosed���ر���Ҫʱ��
		.WillOnce(Return(ix::ReadyState::Open)); // ����Connect()��Ҫ�ȴ�ix�����ϣ���״̬��ΪOpen��������Ҫʱ�䡣
		EXPECT_CALL(*gl_pMockVirtualWebSocket, StopWebSocket).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(vSymbol)).Times(1)
		.WillOnce(DoAll(Throw(e)));

		EXPECT_FALSE(gl_pMockVirtualWebSocket->ConnectWebSocketAndSendMessage(vSymbol));
	}

	TEST_F(CMockVirtualWebSocketTest, TestConnectWebSocketAndSendMessage3) {
		exception e;
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AAPL"));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(2)
		.WillOnce(Return(ix::ReadyState::Open)) // ����Disconnect()��Ҫ�ȵ�ix�ر����ӣ���״̬��ΪClosed���ر���Ҫʱ��
		.WillOnce(Return(ix::ReadyState::Closed)); // ����Disconnect()��Ҫ�ȵ�ix�ر����ӣ���״̬��ΪClosed���ر���Ҫʱ��
		EXPECT_CALL(*gl_pMockVirtualWebSocket, StopWebSocket).Times(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(1)
		.WillOnce(Throw(e));
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(_)).Times(0);

		EXPECT_FALSE(gl_pMockVirtualWebSocket->ConnectWebSocketAndSendMessage(vSymbol));
	}

	TEST_F(CMockVirtualWebSocketTest, TestConnectWebSocketAndSendMessage4) {
		exception e;
		vectorString vSymbol;
		vSymbol.push_back(_T("A"));
		vSymbol.push_back(_T("AAPL"));

		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(1)
		.WillOnce(Return(ix::ReadyState::Open)); // ����Disconnect()��Ҫ�ȵ�ix�ر����ӣ���״̬��ΪClosed���ر���Ҫʱ��
		EXPECT_CALL(*gl_pMockVirtualWebSocket, StopWebSocket).Times(1)
		.WillOnce(Throw(e));
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Connect).Times(0);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, Send(_)).Times(0);

		EXPECT_FALSE(gl_pMockVirtualWebSocket->ConnectWebSocketAndSendMessage(vSymbol));
	}

	TEST_F(CMockVirtualWebSocketTest, TestDisconnect1) {
		gl_pMockVirtualWebSocket->SetSubscriptionId(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(3)
		.WillOnce(Return(ix::ReadyState::Open))	// ״̬����Closedʱ������StopWebSocket������
		.WillOnce(Return(ix::ReadyState::Open)) // ��״̬����Closedʱ��������ȴ���ֱ��״̬���Closed��
		.WillOnce(Return(ix::ReadyState::Closed)); // ����Disconnect()��Ҫ�ȵ�ix�ر����ӣ���״̬��ΪClosed���ر���Ҫʱ��
		EXPECT_CALL(*gl_pMockVirtualWebSocket, StopWebSocket).Times(1); // ��webSocket״̬����Closedʱ����Ҫִ�йر�����

		gl_pMockVirtualWebSocket->Disconnect();

		EXPECT_EQ(gl_pMockVirtualWebSocket->GetSubscriptionId(), 0);
	}

	TEST_F(CMockVirtualWebSocketTest, TestDisconnect2) {
		gl_pMockVirtualWebSocket->SetSubscriptionId(1);
		EXPECT_CALL(*gl_pMockVirtualWebSocket, GetState).Times(2)
		.WillOnce(Return(ix::ReadyState::Closed)) // 
		.WillOnce(Return(ix::ReadyState::Closed)); // ����Disconnect()��Ҫ�ȵ�ix�ر����ӣ���״̬��ΪClosed���ر���Ҫʱ��
		EXPECT_CALL(*gl_pMockVirtualWebSocket, StopWebSocket).Times(0); // ��webSocket״̬ΪClosedʱ������ִ�йر�����

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
