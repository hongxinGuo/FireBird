#include"pch.h"

#include "ChinaMarket.h"
#include "FinnhubInaccessibleExchange.h"
#include"GeneralCheck.h"

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
			gl_finnhubInaccessibleExchange.SetUpdate(false);

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

	//using CVirtualChinaMarketWebProductDeathTest = CVirtualChinaMarketWebProductTest; 

	// GTest������������Ҫ�����Ĳ����࣬����DeathTestΪ��β���Ա�֤ϵͳ�ܹ�ʶ�����������������ִ���������ԣ���֤�̰߳�ȫ��
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
			gl_finnhubInaccessibleExchange.SetUpdate(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CVirtualChinaMarketWebProduct webProduct;
	};

	TEST_F(CVirtualChinaMarketWebProductDeathTest, Test_GetMarket2) {
		EXPECT_DEATH(auto p = webProduct.GetMarket(), "");
	}
}
