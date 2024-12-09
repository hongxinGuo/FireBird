#include"pch.h"

 

#include"globedef.h"

#include "TiingoDataSource.h"
#include"GeneralCheck.h"

#include"MockTiingoForexWebSocket.h"
#include "WorldMarket.h"

using namespace testing;

namespace FireBirdTest {
	class CMockTiingoForexWebSocketTest : public ::testing::Test {
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
		CMockTiingoForexWebSocket m_tiingoForexWebSocket;
		vectorString vSymbol;
	};

	TEST_F(CMockTiingoForexWebSocketTest, TestMonitorWebSocket1) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		gl_pTiingoDataSource->SetWebError(true);

		EXPECT_CALL(m_tiingoForexWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open));
		EXPECT_CALL(m_tiingoForexWebSocket, TaskDisconnect()).Times(1);

		m_tiingoForexWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
		gl_pTiingoDataSource->SetWebError(false);
	}

	TEST_F(CMockTiingoForexWebSocketTest, TestMonitorWebSocket2) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		gl_pTiingoDataSource->SetWebError(true);

		EXPECT_CALL(m_tiingoForexWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed));

		EXPECT_CALL(m_tiingoForexWebSocket, TaskDisconnect()).Times(0); // "web SocketÒÑ¹Ø±Õ£¬ÎÞÐèÔÙ´Î¹Ø±Õ";

		m_tiingoForexWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
		gl_pTiingoDataSource->SetWebError(false);
	}

	TEST_F(CMockTiingoForexWebSocketTest, TestMonitorWebSocket3) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoForexWebSocket());
		m_tiingoForexWebSocket.SetError(true);

		EXPECT_CALL(m_tiingoForexWebSocket, GetState()).Times(3)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Closed));

		EXPECT_CALL(m_tiingoForexWebSocket, TaskDisconnect()).Times(1);
		EXPECT_CALL(m_tiingoForexWebSocket, TaskConnectAndSendMessage).Times(1);

		m_tiingoForexWebSocket.MonitorWebSocket(vSymbol);
		EXPECT_EQ(m_tiingoForexWebSocket.GetHeartbeatTime(), GetUTCTime());

		// »Ö¸´Ô­×´
	}

	TEST_F(CMockTiingoForexWebSocketTest, TestMonitorWebSocket4) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoForexWebSocket());
		m_tiingoForexWebSocket.SetError(true);

		EXPECT_CALL(m_tiingoForexWebSocket, GetState()).Times(3)
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed));

		EXPECT_CALL(m_tiingoForexWebSocket, TaskDisconnect()).Times(0);
		EXPECT_CALL(m_tiingoForexWebSocket, TaskConnectAndSendMessage).Times(1);

		m_tiingoForexWebSocket.MonitorWebSocket(vSymbol);
		EXPECT_EQ(m_tiingoForexWebSocket.GetHeartbeatTime(), GetUTCTime());

		// »Ö¸´Ô­×´
	}

	TEST_F(CMockTiingoForexWebSocketTest, TestMonitorWebSocket5) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoForexWebSocket());
		m_tiingoForexWebSocket.SetError(true);

		EXPECT_CALL(m_tiingoForexWebSocket, GetState()).Times(3)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(m_tiingoForexWebSocket, TaskDisconnect()).Times(1);
		EXPECT_CALL(m_tiingoForexWebSocket, TaskConnectAndSendMessage).Times(0);

		m_tiingoForexWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
	}

	TEST_F(CMockTiingoForexWebSocketTest, TestMonitorWebSocket6) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoForexWebSocket());
		m_tiingoForexWebSocket.SetError(true);

		EXPECT_CALL(m_tiingoForexWebSocket, GetState()).Times(3)
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(m_tiingoForexWebSocket, TaskDisconnect()).Times(0);
		EXPECT_CALL(m_tiingoForexWebSocket, TaskConnectAndSendMessage).Times(0);

		m_tiingoForexWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
	}

	TEST_F(CMockTiingoForexWebSocketTest, TestMonitorWebSocket7) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoForexWebSocket());
		gl_systemConfiguration.SetUsingTiingoForexWebSocket(false);
		m_tiingoForexWebSocket.SetError(true);

		EXPECT_CALL(m_tiingoForexWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(m_tiingoForexWebSocket, TaskDisconnect()).Times(1);

		m_tiingoForexWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
		gl_systemConfiguration.SetUsingTiingoForexWebSocket(true);
		gl_systemConfiguration.SetUpdateDB(false);
	}

	TEST_F(CMockTiingoForexWebSocketTest, TestMonitorWebSocket8) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoForexWebSocket());

		EXPECT_CALL(m_tiingoForexWebSocket, GetState()).Times(1)
		.WillOnce(Return(ix::ReadyState::Connecting));

		EXPECT_CALL(m_tiingoForexWebSocket, TaskDisconnect()).Times(0);
		EXPECT_CALL(m_tiingoForexWebSocket, TaskConnectAndSendMessage(_)).Times(0);

		m_tiingoForexWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
	}
}
