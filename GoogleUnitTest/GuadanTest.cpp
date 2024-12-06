#include"pch.h"

////#include"gtest/gtest.h"

#include"SystemConfiguration.h"

#include"GeneralCheck.h"

#include"Guadan.h"

namespace FireBirdTest {
	class COneGuadanTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
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
	};

	TEST_F(COneGuadanTest, TestSetTime) {
		COneGuadan OneGuadan;
		EXPECT_EQ(OneGuadan.GetMarketTime().time_since_epoch().count(), 0);
		OneGuadan.SetTime(10101010);
		EXPECT_EQ(OneGuadan.GetMarketTime().time_since_epoch().count(), 10101010);
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

namespace FireBirdTest {
	class CGuadanTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		}

		static void TearDownTestSuite() {}

		void SetUp() override {
			ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		}

		void TearDown() override {
			// clearUp
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
		const COneGuadan OneGuadan1;
		const COneGuadan OneGuadan2;
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
		EXPECT_EQ(Guadan.GetGuadan().GetMarketTime().time_since_epoch().count(), 20202020);
		EXPECT_EQ(Guadan.GetGuadan(0).GetMarketTime().time_since_epoch().count(), 10101010);
		EXPECT_EQ(Guadan.GetGuadan(1).GetMarketTime().time_since_epoch().count(), 20202020);
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
