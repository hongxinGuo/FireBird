#include"pch.h"

 

#include"globedef.h"

import FireBird.DataSource.Finnhub;
#include"GeneralCheck.h"

#include"MockFinnhubWebSocket.h"
#include "WorldMarket.h"

using namespace testing;

namespace FireBirdTest {
	class CMockFinnhubWebSocketTest : public ::testing::Test {
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
			vSymbol.push_back("a");
			vSymbol.push_back("b");
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();

			gl_pFinnhubDataSource->SetWebError(false);
		}

	protected:
		CMockFinnhubWebSocket m_finnhubWebSocket;
		vectorString vSymbol;
	};

	TEST_F(CMockFinnhubWebSocketTest, TestMonitorWebSocket1) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebError());
		gl_pFinnhubDataSource->SetWebError(true);

		EXPECT_CALL(m_finnhubWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open));
		EXPECT_CALL(m_finnhubWebSocket, TaskDisconnect()).Times(1);

		m_finnhubWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
		gl_pFinnhubDataSource->SetWebError(false);
	}

	TEST_F(CMockFinnhubWebSocketTest, TestMonitorWebSocket2) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebError());
		gl_pFinnhubDataSource->SetWebError(true);

		EXPECT_CALL(m_finnhubWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed));

		EXPECT_CALL(m_finnhubWebSocket, TaskDisconnect()).Times(0); // "web SocketÒÑ¹Ø±Õ£¬ÎÞÐèÔÙ´Î¹Ø±Õ";

		m_finnhubWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
		gl_pFinnhubDataSource->SetWebError(false);
	}

	TEST_F(CMockFinnhubWebSocketTest, TestMonitorWebSocket3) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingFinnhubWebSocket());
		m_finnhubWebSocket.SetError(true);

		EXPECT_CALL(m_finnhubWebSocket, GetState()).Times(3)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Closed));

		EXPECT_CALL(m_finnhubWebSocket, TaskDisconnect()).Times(1);
		EXPECT_CALL(m_finnhubWebSocket, TaskConnectAndSendMessage).Times(1);

		m_finnhubWebSocket.MonitorWebSocket(vSymbol);
		EXPECT_EQ(m_finnhubWebSocket.GetHeartbeatTime(), GetUTCTime());

		// »Ö¸´Ô­×´
	}

	TEST_F(CMockFinnhubWebSocketTest, TestMonitorWebSocket4) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingFinnhubWebSocket());
		m_finnhubWebSocket.SetError(true);

		EXPECT_CALL(m_finnhubWebSocket, GetState()).Times(3)
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed));

		EXPECT_CALL(m_finnhubWebSocket, TaskDisconnect()).Times(0);
		EXPECT_CALL(m_finnhubWebSocket, TaskConnectAndSendMessage).Times(1);

		m_finnhubWebSocket.MonitorWebSocket(vSymbol);
		EXPECT_EQ(m_finnhubWebSocket.GetHeartbeatTime(), GetUTCTime());

		// »Ö¸´Ô­×´
	}

	TEST_F(CMockFinnhubWebSocketTest, TestMonitorWebSocket5) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingFinnhubWebSocket());
		m_finnhubWebSocket.SetError(true);

		EXPECT_CALL(m_finnhubWebSocket, GetState()).Times(3)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(m_finnhubWebSocket, TaskDisconnect()).Times(1);
		EXPECT_CALL(m_finnhubWebSocket, TaskConnectAndSendMessage).Times(0);

		m_finnhubWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
	}

	TEST_F(CMockFinnhubWebSocketTest, TestMonitorWebSocket6) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingFinnhubWebSocket());
		m_finnhubWebSocket.SetError(true);

		EXPECT_CALL(m_finnhubWebSocket, GetState()).Times(3)
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(m_finnhubWebSocket, TaskDisconnect()).Times(0);
		EXPECT_CALL(m_finnhubWebSocket, TaskConnectAndSendMessage).Times(0);

		m_finnhubWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
	}

	TEST_F(CMockFinnhubWebSocketTest, TestMonitorWebSocket7) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingFinnhubWebSocket());
		gl_systemConfiguration.SetUsingFinnhubWebSocket(false);
		m_finnhubWebSocket.SetError(true);

		EXPECT_CALL(m_finnhubWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(m_finnhubWebSocket, TaskDisconnect()).Times(1);

		m_finnhubWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
		gl_systemConfiguration.SetUsingFinnhubWebSocket(true);
	}

	TEST_F(CMockFinnhubWebSocketTest, TestMonitorWebSocket8) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingFinnhubWebSocket());

		EXPECT_CALL(m_finnhubWebSocket, GetState()).Times(1)
		.WillOnce(Return(ix::ReadyState::Connecting));

		EXPECT_CALL(m_finnhubWebSocket, TaskDisconnect()).Times(0);
		EXPECT_CALL(m_finnhubWebSocket, TaskConnectAndSendMessage(_)).Times(0);

		m_finnhubWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
	}
}
