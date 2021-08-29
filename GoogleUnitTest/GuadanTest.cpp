#include"pch.h"
#include"globedef.h"
#include"GeneralCheck.h"

#include"Guadan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class COneGuadanTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) { // 本测试类的初始化函数
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
		}
	};

	TEST_F(COneGuadanTest, TestSetTime) {
		COneGuadan OneGuadan;
		EXPECT_EQ(OneGuadan.GetFormatedMarketTime(), 0);
		OneGuadan.SetTime(10101010);
		EXPECT_EQ(OneGuadan.GetFormatedMarketTime(), 10101010);
	}

	TEST_F(COneGuadanTest, TestSetVolume) {
		COneGuadan OneGuadan;
		EXPECT_EQ(OneGuadan.GetVolume(), 0);
		OneGuadan.SetVolume(10101010);
		EXPECT_EQ(OneGuadan.GetVolume(), 10101010);
	}

	TEST_F(COneGuadanTest, TestSetStatus) {
		COneGuadan OneGuadan;
		EXPECT_EQ(OneGuadan.GetStatus(), 0);
		OneGuadan.SetStatus(10101010);
		EXPECT_EQ(OneGuadan.GetStatus(), 10101010);
	}
}

namespace StockAnalysisTest {
	class CGuadanTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) { // 本测试类的初始化函数
			ASSERT_FALSE(gl_fNormalMode);
		}

		static void TearDownTestSuite() {
		}

		virtual void SetUp(void) override {
			ASSERT_FALSE(gl_fNormalMode);
			ASSERT_TRUE(gl_fTestMode);
		}

		virtual void TearDown(void) override {
			// clearup
		}
	};

	TEST_F(CGuadanTest, TestSetCurrentVolume) {
		CGuadan Guadan;
		EXPECT_EQ(Guadan.GetCurrentPos(), 0);
		EXPECT_EQ(Guadan.GetCompletedPos(), 0);
		EXPECT_EQ(Guadan.GetCurrentVolume(), 0);
		EXPECT_EQ(Guadan.GetFormerVolume(), 0);
		Guadan.SetCurrentVolume(10101010);
		EXPECT_EQ(Guadan.GetCurrentVolume(), 10101010);
		EXPECT_EQ(Guadan.GetFormerVolume(), 0);
		Guadan.SetCurrentVolume(20202020);
		EXPECT_EQ(Guadan.GetCurrentVolume(), 20202020);
		EXPECT_EQ(Guadan.GetFormerVolume(), 10101010);
	}

	TEST_F(CGuadanTest, TestReset) {
		CGuadan Guadan;
		COneGuadan OneGuadan1, OneGuadan2;
		OneGuadan1.SetTime(10101010);
		OneGuadan2.SetTime(20202020);
		EXPECT_EQ(Guadan.GetCurrentPos(), 0);
		EXPECT_EQ(Guadan.GetCompletedPos(), 0);
		EXPECT_EQ(Guadan.GetCurrentVolume(), 0);
		EXPECT_EQ(Guadan.GetFormerVolume(), 0);
		Guadan.SetCurrentVolume(10101010);
		Guadan.SetCurrentVolume(20202020);
		Guadan.AddGuadan(OneGuadan1);
		EXPECT_EQ(Guadan.GetCurrentPos(), 1);
		Guadan.AddGuadan(OneGuadan2);
		EXPECT_EQ(Guadan.GetCurrentPos(), 2);
		Guadan.ReSet();
		EXPECT_EQ(Guadan.GetCurrentPos(), 0);
		EXPECT_EQ(Guadan.GetCompletedPos(), 0);
		EXPECT_EQ(Guadan.GetCurrentVolume(), 0);
		EXPECT_EQ(Guadan.GetFormerVolume(), 0);
	}

	TEST_F(CGuadanTest, TestGetCurrentPos) {
		CGuadan Guadan;
		COneGuadan OneGuadan1, OneGuadan2;
		EXPECT_EQ(Guadan.GetCurrentPos(), 0);
		EXPECT_EQ(Guadan.GetCompletedPos(), 0);
		EXPECT_EQ(Guadan.GetCurrentVolume(), 0);
		EXPECT_EQ(Guadan.GetFormerVolume(), 0);
		Guadan.AddGuadan(OneGuadan1);
		EXPECT_EQ(Guadan.GetCurrentPos(), 1);
		Guadan.AddGuadan(OneGuadan2);
		EXPECT_EQ(Guadan.GetCurrentPos(), 2);
	}

	TEST_F(CGuadanTest, TestGetGuadan) {
		CGuadan Guadan;
		COneGuadan OneGuadan1, OneGuadan2;
		OneGuadan1.SetTime(10101010);
		OneGuadan2.SetTime(20202020);
		EXPECT_EQ(Guadan.GetCurrentPos(), 0);
		EXPECT_EQ(Guadan.GetCompletedPos(), 0);
		EXPECT_EQ(Guadan.GetCurrentVolume(), 0);
		EXPECT_EQ(Guadan.GetFormerVolume(), 0);
		Guadan.AddGuadan(OneGuadan1);
		EXPECT_EQ(Guadan.GetCurrentPos(), 1);
		Guadan.AddGuadan(OneGuadan2);
		EXPECT_EQ(Guadan.GetCurrentPos(), 2);
		EXPECT_EQ(Guadan.GetGuadan().GetFormatedMarketTime(), 20202020);
		EXPECT_EQ(Guadan.GetGuadan(0).GetFormatedMarketTime(), 10101010);
		EXPECT_EQ(Guadan.GetGuadan(1).GetFormatedMarketTime(), 20202020);
	}

	TEST_F(CGuadanTest, TestSetStatus) {
		CGuadan Guadan;
		COneGuadan OneGuadan1, OneGuadan2;
		OneGuadan1.SetStatus(10101010);
		OneGuadan2.SetStatus(20202020);
		EXPECT_EQ(Guadan.GetCurrentPos(), 0);
		EXPECT_EQ(Guadan.GetCompletedPos(), 0);
		EXPECT_EQ(Guadan.GetCurrentVolume(), 0);
		EXPECT_EQ(Guadan.GetFormerVolume(), 0);
		Guadan.AddGuadan(OneGuadan1);
		EXPECT_EQ(Guadan.GetCurrentPos(), 1);
		Guadan.AddGuadan(OneGuadan2);
		EXPECT_EQ(Guadan.GetCurrentPos(), 2);
		EXPECT_EQ(Guadan.GetGuadan().GetStatus(), 20202020);
		EXPECT_EQ(Guadan.GetGuadan(0).GetStatus(), 10101010);
		EXPECT_EQ(Guadan.GetGuadan(1).GetStatus(), 20202020);
		Guadan.SetStatus(0, 30303030);
		Guadan.SetStatus(1, 40404040);
		EXPECT_EQ(Guadan.GetGuadan(0).GetStatus(), 30303030);
		EXPECT_EQ(Guadan.GetGuadan(1).GetStatus(), 40404040);
	}
}