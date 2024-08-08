#include"pch.h"

#include"VirtualWorldMarketWebProduct.h"
#include "WorldMarket.h"
#include "FinnhubInaccessibleExchange.h"
#include"GeneralCheck.h"

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
			gl_finnhubInaccessibleExchange.NeedUpdate(false);

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

	// GTest������������Ҫ�����Ĳ����࣬����DeathTestΪ��β���Ա�֤ϵͳ�ܹ�ʶ�����������������ִ���������ԣ���֤�̰߳�ȫ��
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
			gl_finnhubInaccessibleExchange.NeedUpdate(false);

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
