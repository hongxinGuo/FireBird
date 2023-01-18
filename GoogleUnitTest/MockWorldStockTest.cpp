///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试ChinaMarket的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"GeneralCheck.h"

#include"Thread.h"

#include"WorldStock.h"
#include"MockWorldStock.h"

using namespace testing;

namespace FireBirdTest {
	class CMockWorldStockTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			// clearUp

			GeneralCheck();
		}
	};

	TEST_F(CMockWorldStockTest, TestUpdateEPSSurpriseDB) {
		CMockWorldStock stock;
		EXPECT_CALL(stock, UpdateEPSSurpriseDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateEPSSurpriseDB(&stock), static_cast<UINT>(41));
	}
}
