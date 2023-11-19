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

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CMockWorldStockTest, TestUpdateEPSSurpriseDB) {
		CMockWorldStockPtr pStock = make_shared<CMockWorldStock>();
		EXPECT_CALL(*pStock, UpdateEPSSurpriseDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateEPSSurpriseDB(pStock), static_cast<UINT>(41));
	}
}
