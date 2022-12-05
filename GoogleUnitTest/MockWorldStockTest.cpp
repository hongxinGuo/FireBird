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

using namespace std;
using namespace testing;
#include<memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CMockWorldStockTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup

			GeneralCheck();
		}
	};

	TEST_F(CMockWorldStockTest, TestUpdateEPSSurpriseDB) {
		CMockWorldStock stock;
		EXPECT_CALL(stock, UpdateEPSSurpriseDB)
			.Times(1);
		EXPECT_EQ(ThreadUpdateEPSSurpriseDB(&stock), (UINT)41);
	}
}