#include"pch.h"

#include"GeneralCheck.h"

#include"virtualWebSocket.h"

#include<exception>

#include "TimeConvert.h"
using std::exception;

using namespace testing;

namespace FireBirdTest {
	class CVirtualWebSocketTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		void SetUp() override {
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
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

		webSocket.SetHeartbeatTime(GetUTCTime() - 301); // 默认值为五分钟
		EXPECT_TRUE(webSocket.IsIdle());
	}
}
