#include"pch.h"

////#include"gtest/gtest.h"

#include"globedef.h"

#include"GeneralCheck.h"

#include"virtualWebSocket.h"

using namespace testing;

namespace FireBirdTest {
	class CVirtualWebSocketTest : public ::testing::Test {
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
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CVirtualWebSocket webSocket;
	};

	TEST_F(CVirtualWebSocketTest, TestSetHeartbeatTime) {
		EXPECT_EQ(webSocket.GetHeartbeatTime(), 0);
		webSocket.SetHeartbeatTime(10101010);

		EXPECT_EQ(webSocket.GetHeartbeatTime(), 10101010);
	}

	TEST_F(CVirtualWebSocketTest, TestIsIdle) {
		EXPECT_TRUE(webSocket.IsIdle());
		webSocket.SetHeartbeatTime(GetUTCTime() - 300);

		EXPECT_FALSE(webSocket.IsIdle());

		webSocket.SetHeartbeatTime(GetUTCTime() - 301); // Ĭ��ֵΪ�����
		EXPECT_TRUE(webSocket.IsIdle());
	}
}
