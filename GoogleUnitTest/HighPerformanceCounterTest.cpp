#include"pch.h"

#include"GeneralCheck.h"

#include"HighPerformanceCounter.h"

namespace FireBirdTest {
	class CHighPerformanceCounterTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE(""); GeneralCheck();
		}

		static void TearDownTestSuite() { }

		void SetUp() override {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE(""); GeneralCheck();
		}
	};

	TEST_F(CHighPerformanceCounterTest, TestInitialize) {
		const CHighPerformanceCounter counter;
		EXPECT_TRUE(counter.IsInitialized());
		EXPECT_EQ(counter.GetElapseTick(), 0);
	}

	TEST_F(CHighPerformanceCounterTest, TestGetElapsedSecond) {
		CHighPerformanceCounter counter;

		counter.start();
		Sleep(10);
		counter.stop();
		EXPECT_GT(counter.GetElapseTick(), 0);
		EXPECT_LE(counter.GetElapsedMillisecond() * 1000, counter.GetElapsedMicroSecond()) << "变成毫秒时去掉了小数部分";
		EXPECT_GE(counter.GetElapsedNanoSecond(), counter.GetElapsedMicroSecond() * 1000) << "变成微秒时去掉了小数部分";
	}
}
