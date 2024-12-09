#include"pch.h"

 

#include"globedef.h"

#include "TiingoDataSource.h"
#include"GeneralCheck.h"

#include"MockTiingoIEXWebSocket.h"
#include "WorldMarket.h"

using namespace testing;

namespace FireBirdTest {
	class CMockTiingoIEXWebSocketTest : public ::testing::Test {
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
		CMockTiingoIEXWebSocket m_tiingoIEXWebSocket;
		vectorString vSymbol;
	};

	TEST_F(CMockTiingoIEXWebSocketTest, TestMonitorWebSocket1) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		gl_pTiingoDataSource->SetWebError(true);

		EXPECT_CALL(m_tiingoIEXWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open));
		EXPECT_CALL(m_tiingoIEXWebSocket, TaskDisconnect()).Times(1);

		m_tiingoIEXWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
		gl_pTiingoDataSource->SetWebError(false);
	}

	TEST_F(CMockTiingoIEXWebSocketTest, TestMonitorWebSocket2) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		gl_pTiingoDataSource->SetWebError(true);

		EXPECT_CALL(m_tiingoIEXWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed));

		EXPECT_CALL(m_tiingoIEXWebSocket, TaskDisconnect()).Times(0); // "web SocketÒÑ¹Ø±Õ£¬ÎÞÐèÔÙ´Î¹Ø±Õ";

		m_tiingoIEXWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
		gl_pTiingoDataSource->SetWebError(false);
	}

	TEST_F(CMockTiingoIEXWebSocketTest, TestMonitorWebSocket3) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoIEXWebSocket());
		m_tiingoIEXWebSocket.SetError(true);

		EXPECT_CALL(m_tiingoIEXWebSocket, GetState()).Times(3)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Closed));

		EXPECT_CALL(m_tiingoIEXWebSocket, TaskDisconnect()).Times(1);
		EXPECT_CALL(m_tiingoIEXWebSocket, TaskConnectAndSendMessage).Times(1);

		m_tiingoIEXWebSocket.MonitorWebSocket(vSymbol);
		EXPECT_EQ(m_tiingoIEXWebSocket.GetHeartbeatTime(), GetUTCTime());

		// »Ö¸´Ô­×´
	}

	TEST_F(CMockTiingoIEXWebSocketTest, TestMonitorWebSocket4) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoIEXWebSocket());
		m_tiingoIEXWebSocket.SetError(true);

		EXPECT_CALL(m_tiingoIEXWebSocket, GetState()).Times(3)
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed));

		EXPECT_CALL(m_tiingoIEXWebSocket, TaskDisconnect()).Times(0);
		EXPECT_CALL(m_tiingoIEXWebSocket, TaskConnectAndSendMessage).Times(1);

		m_tiingoIEXWebSocket.MonitorWebSocket(vSymbol);
		EXPECT_EQ(m_tiingoIEXWebSocket.GetHeartbeatTime(), GetUTCTime());

		// »Ö¸´Ô­×´
	}

	TEST_F(CMockTiingoIEXWebSocketTest, TestMonitorWebSocket5) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoIEXWebSocket());
		m_tiingoIEXWebSocket.SetError(true);

		EXPECT_CALL(m_tiingoIEXWebSocket, GetState()).Times(3)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(m_tiingoIEXWebSocket, TaskDisconnect()).Times(1);
		EXPECT_CALL(m_tiingoIEXWebSocket, TaskConnectAndSendMessage).Times(0);

		m_tiingoIEXWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
	}

	TEST_F(CMockTiingoIEXWebSocketTest, TestMonitorWebSocket6) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoIEXWebSocket());
		m_tiingoIEXWebSocket.SetError(true);

		EXPECT_CALL(m_tiingoIEXWebSocket, GetState()).Times(3)
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Closed))
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(m_tiingoIEXWebSocket, TaskDisconnect()).Times(0);
		EXPECT_CALL(m_tiingoIEXWebSocket, TaskConnectAndSendMessage).Times(0);

		m_tiingoIEXWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
	}

	TEST_F(CMockTiingoIEXWebSocketTest, TestMonitorWebSocket7) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoIEXWebSocket());
		gl_systemConfiguration.SetUsingTiingoIEXWebSocket(false);
		m_tiingoIEXWebSocket.SetError(true);

		EXPECT_CALL(m_tiingoIEXWebSocket, GetState()).Times(2)
		.WillOnce(Return(ix::ReadyState::Open))
		.WillOnce(Return(ix::ReadyState::Open));

		EXPECT_CALL(m_tiingoIEXWebSocket, TaskDisconnect()).Times(1);

		m_tiingoIEXWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
		gl_systemConfiguration.SetUsingTiingoIEXWebSocket(true);
		gl_systemConfiguration.SetUpdateDB(false);
	}

	TEST_F(CMockTiingoIEXWebSocketTest, TestMonitorWebSocket8) {
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingTiingoIEXWebSocket());

		EXPECT_CALL(m_tiingoIEXWebSocket, GetState()).Times(1)
		.WillOnce(Return(ix::ReadyState::Connecting));

		EXPECT_CALL(m_tiingoIEXWebSocket, TaskDisconnect()).Times(0);
		EXPECT_CALL(m_tiingoIEXWebSocket, TaskConnectAndSendMessage(_)).Times(0);

		m_tiingoIEXWebSocket.MonitorWebSocket(vSymbol);

		// »Ö¸´Ô­×´
	}
}
