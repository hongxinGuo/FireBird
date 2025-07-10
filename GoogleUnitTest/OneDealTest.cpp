#include"pch.h"

#include"GeneralCheck.h"

#include"OneDeal.h"

namespace FireBirdTest {
	class COneDealTest : public ::testing::Test {
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

	TEST_F(COneDealTest, TestInitialize) {
		const COneDeal oneDeal;
		EXPECT_EQ(oneDeal.GetMarketTime().time_since_epoch().count(), 0);
		EXPECT_EQ(oneDeal.GetType(), 0);
		EXPECT_EQ(oneDeal.GetPrice(), 0);
		EXPECT_EQ(oneDeal.GetVolume(), 0);
	}

	TEST_F(COneDealTest, TestGetTime) {
		COneDeal oneDeal;
		oneDeal.SetTime(20202020);
		EXPECT_EQ(oneDeal.GetMarketTime().time_since_epoch().count(), 20202020);
	}

	TEST_F(COneDealTest, TestGetType) {
		COneDeal oneDeal;
		oneDeal.SetType(30303030);
		EXPECT_EQ(oneDeal.GetType(), 30303030);
	}

	TEST_F(COneDealTest, TestGetPrice) {
		COneDeal oneDeal;
		oneDeal.SetPrice(40404040);
		EXPECT_EQ(oneDeal.GetPrice(), 40404040);
	}

	TEST_F(COneDealTest, TestGetVolume) {
		COneDeal oneDeal;
		oneDeal.SetVolume(50505050);
		EXPECT_EQ(oneDeal.GetVolume(), 50505050);
	}
}
