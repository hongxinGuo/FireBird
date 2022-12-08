#include"pch.h"

#include"GeneralCheck.h"

#include"HighPerformanceCounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CHighPerformanceCounterTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
		}

		static void TearDownTestSuite(void) {
		}

		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
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