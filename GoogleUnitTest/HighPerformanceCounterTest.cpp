#include"pch.h"

#include"GeneralCheck.h"

#include"HighPerformanceCounter.h"

namespace FireBirdTest {
	class CHighPerformanceCounterTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			GeneralCheck();
		}

		static void TearDownTestSuite() { }

		void SetUp() override {
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			GeneralCheck();
		}
	};

	TEST_F(CHighPerformanceCounterTest, TestInitialize) {
		CHighPerformanceCounter counter;
		EXPECT_TRUE(counter.IsInitialized());
		EXPECT_EQ(counter.GetElapseTick(), 0);
	}

	TEST_F(CHighPerformanceCounterTest, TestGetElapsedSecond) {
		CHighPerformanceCounter counter;

		counter.start();
		Sleep(10);
		counter.stop();
		EXPECT_GT(counter.GetElapseTick(), 0);
		EXPECT_LE(counter.GetElapsedMilliSecond() * 1000, counter.GetElapsedMicroSecond()) << "��ɺ���ʱȥ����С������";
		EXPECT_GE(counter.GetElapsedNanoSecond(), counter.GetElapsedMicroSecond() * 1000) << "���΢��ʱȥ����С������";
	}
}
