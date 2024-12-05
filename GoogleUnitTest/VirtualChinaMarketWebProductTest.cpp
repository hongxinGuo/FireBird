#include"pch.h"

//#include"gtest/gtest.h"

#include"GeneralCheck.h"

#include "ChinaMarket.h"
#include "FinnhubInaccessibleExchange.h"

#include"VirtualChinaMarketWebProduct.h"

using namespace testing;

namespace FireBirdTest {
	class CVirtualChinaMarketWebProductTest : public ::testing::Test {
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
			gl_finnhubInaccessibleExchange.SetUpdateDB(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CVirtualChinaMarketWebProduct webProduct;
	};

	TEST_F(CVirtualChinaMarketWebProductTest, Test_GetMarket) {
		webProduct.SetMarket(gl_pChinaMarket);
		EXPECT_EQ(webProduct.GetMarket(), gl_pChinaMarket);
	}

	// GTest的死亡测试需要单独的测试类，并以DeathTest为结尾，以保证系统能够识别出来。这样可以先执行死亡测试，保证线程安全。
	class CVirtualChinaMarketWebProductDeathTest : public ::testing::Test {
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
			gl_finnhubInaccessibleExchange.SetUpdateDB(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CVirtualChinaMarketWebProduct webProduct;
	};

	TEST_F(CVirtualChinaMarketWebProductDeathTest, Test_GetMarket2) {
		EXPECT_DEATH(auto p = webProduct.GetMarket(), "");
		// GetMarket函数使用exit(1)退出时，EXPECT_EXIT接收到的返回值是随机数，只有使用_exit(1)才能保证返回值。
		// 但_exit()函数导致测试函数执行得非常慢（20秒，exit只有3秒）。
		//EXPECT_EXIT(auto p = webProduct.GetMarket(), testing::ExitedWithCode(1), _);
	}

	TEST_F(CVirtualChinaMarketWebProductDeathTest, Test_Demo) {
		//	EXPECT_EXIT(_exit(10), testing::ExitedWithCode(10), _);
		//	EXPECT_EXIT(_exit(2), testing::ExitedWithCode(2), _);
	}
}
