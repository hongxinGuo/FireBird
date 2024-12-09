#include"pch.h"

 

#include"GeneralCheck.h"
#include "WorldMarket.h"
import FireBird.WebProduct.VirtualWorldMarket;

import FireBird.System.FinnhubInaccessibleExchange;

using namespace testing;

namespace FireBirdTest {
	class CVirtualWorldMarketWebProductTest : public ::testing::Test {
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
		CVirtualWorldMarketWebProduct webProduct;
	};

	TEST_F(CVirtualWorldMarketWebProductTest, Test_GetMarket) {
		webProduct.SetMarket(gl_pWorldMarket);
		EXPECT_EQ(webProduct.GetMarket(), gl_pWorldMarket);
	}

	//using CVirtualWorldMarketWebProductDeathTest = CVirtualWorldMarketWebProductTest;

	// GTest的死亡测试需要单独的测试类，并以DeathTest为结尾，以保证系统能够识别出来。这样可以先执行死亡测试，保证线程安全。
	class CVirtualWorldMarketWebProductDeathTest : public ::testing::Test {
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
		CVirtualWorldMarketWebProduct webProduct;
	};

	TEST_F(CVirtualWorldMarketWebProductDeathTest, Test_GetMarket2) {
		EXPECT_DEATH(auto p = webProduct.GetMarket(), "");
	}
}
