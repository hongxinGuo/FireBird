#include"pch.h"

//#include"gtest/gtest.h"

#include"globedef.h"

#include "TiingoDataSource.h"
#include"GeneralCheck.h"

#include"MockTiingoCryptoWebSocket.h"
#include "WorldMarket.h"

using namespace testing;

namespace FireBirdTest {
	class CMockTiingoCryptoWebSocketTest : public ::testing::Test {
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

			gl_pTiingoDataSource->SetWebError(false);
		}

	protected:
		CMockTiingoCryptoWebSocket m_tiingoCryptoWebSocket;
		vectorString vSymbol;
	};

	TEST_F(CMockTiingoCryptoWebSocketTest, TestMonitorWebSocket1) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		gl_pTiingoDataSource->SetWebError(true);

		EXPECT_CALL(m_tiingoCryptoWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open));
		EXPECT_CALL(m_tiingoCryptoWebSocket, TaskDisconnect()).Times(1);

		m_tiingoCryptoWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
		gl_pTiingoDataSource->SetWebError(false);
	}

	TEST_F(CMockTiingoCryptoWebSocketTest, TestMonitorWebSocket2) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		gl_pTiingoDataSource->SetWebError(true);

		EXPECT_CALL(m_tiingoCryptoWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed));

		EXPECT_CALL(m_tiingoCryptoWebSocket, TaskDisconnect()).Times(0); // "web SocketÒÑ¹Ø±Õ£¬ÎÞÐèÔÙ´Î¹Ø±Õ";

		m_tiingoCryptoWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
		gl_pTiingoDataSource->SetWebError(false);
	}

	TEST_F(CMockTiingoCryptoWebSocketTest, TestMonitorWebSocket3) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoCryptoWebSocket());
		m_tiingoCryptoWebSocket.SetError(true);

		EXPECT_CALL(m_tiingoCryptoWebSocket, GetState()).Times(3)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Closed));

		EXPECT_CALL(m_tiingoCryptoWebSocket, TaskDisconnect()).Times(1);
		EXPECT_CALL(m_tiingoCryptoWebSocket, TaskConnectAndSendMessage).Times(1);

		m_tiingoCryptoWebSocket.MonitorWebSocket(vSymbol);
		EXPECT_EQ(m_tiingoCryptoWebSocket.GetHeartbeatTime(), GetUTCTime());

		// »Ö¸´Ô­×´
	}

	TEST_F(CMockTiingoCryptoWebSocketTest, TestMonitorWebSocket4) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoCryptoWebSocket());
		m_tiingoCryptoWebSocket.SetError(true);

		EXPECT_CALL(m_tiingoCryptoWebSocket, GetState()).Times(3)
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed));

		EXPECT_CALL(m_tiingoCryptoWebSocket, TaskDisconnect()).Times(0);
		EXPECT_CALL(m_tiingoCryptoWebSocket, TaskConnectAndSendMessage).Times(1);

		m_tiingoCryptoWebSocket.MonitorWebSocket(vSymbol);
		EXPECT_EQ(m_tiingoCryptoWebSocket.GetHeartbeatTime(), GetUTCTime());

		// »Ö¸´Ô­×´
	}

	TEST_F(CMockTiingoCryptoWebSocketTest, TestMonitorWebSocket5) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoCryptoWebSocket());
		m_tiingoCryptoWebSocket.SetError(true);

		EXPECT_CALL(m_tiingoCryptoWebSocket, GetState()).Times(3)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(m_tiingoCryptoWebSocket, TaskDisconnect()).Times(1);
		EXPECT_CALL(m_tiingoCryptoWebSocket, TaskConnectAndSendMessage).Times(0);

		m_tiingoCryptoWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
	}

	TEST_F(CMockTiingoCryptoWebSocketTest, TestMonitorWebSocket6) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoCryptoWebSocket());
		m_tiingoCryptoWebSocket.SetError(true);

		EXPECT_CALL(m_tiingoCryptoWebSocket, GetState()).Times(3)
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(m_tiingoCryptoWebSocket, TaskDisconnect()).Times(0);
		EXPECT_CALL(m_tiingoCryptoWebSocket, TaskConnectAndSendMessage).Times(0);

		m_tiingoCryptoWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
	}

	TEST_F(CMockTiingoCryptoWebSocketTest, TestMonitorWebSocket7) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoCryptoWebSocket());
		gl_systemConfiguration.SetUsingTiingoCryptoWebSocket(false);
		m_tiingoCryptoWebSocket.SetError(true);

		EXPECT_CALL(m_tiingoCryptoWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(m_tiingoCryptoWebSocket, TaskDisconnect()).Times(1);

		m_tiingoCryptoWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
		gl_systemConfiguration.SetUsingTiingoCryptoWebSocket(true);
		gl_systemConfiguration.SetUpdateDB(false);
	}

	TEST_F(CMockTiingoCryptoWebSocketTest, TestMonitorWebSocket8) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoCryptoWebSocket());

		EXPECT_CALL(m_tiingoCryptoWebSocket, GetState()).Times(1)
		.WillOnce(Return(ix::ReadyState::Connecting));

		EXPECT_CALL(m_tiingoCryptoWebSocket, TaskDisconnect()).Times(0);
		EXPECT_CALL(m_tiingoCryptoWebSocket, TaskConnectAndSendMessage(_)).Times(0);

		m_tiingoCryptoWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
	}
}
