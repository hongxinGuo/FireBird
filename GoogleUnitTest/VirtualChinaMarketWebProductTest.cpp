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
			gl_finnhubInaccessibleExchange.SetUpdateDB(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CVirtualChinaMarketWebProduct webProduct;
	};

	TEST_F(CVirtualChinaMarketWebProductDeathTest, Test_GetMarket2) {
		EXPECT_DEATH(auto p = webProduct.GetMarket(), "");
		// GetMarket����ʹ��exit(1)�˳�ʱ��EXPECT_EXIT���յ��ķ���ֵ���������ֻ��ʹ��_exit(1)���ܱ�֤����ֵ��
		// ��_exit()�������²��Ժ���ִ�е÷ǳ�����20�룬exitֻ��3�룩��
		//EXPECT_EXIT(auto p = webProduct.GetMarket(), testing::ExitedWithCode(1), _);
	}

	TEST_F(CVirtualChinaMarketWebProductDeathTest, Test_Demo) {
		//	EXPECT_EXIT(_exit(10), testing::ExitedWithCode(10), _);
		//	EXPECT_EXIT(_exit(2), testing::ExitedWithCode(2), _);
	}
}
