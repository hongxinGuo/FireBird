///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试ChinaMarket的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"globedef.h"

#include"Thread.h"

#include"WorldStock.h"
#include"WebInquirer.h"
#include"MockWorldStock.h"

#include"GeneralCheck.h"

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
			ASSERT_FALSE(gl_fNormalMode);
		}

		static void TearDownTestSuite(void) {
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
		EXPECT_EQ(ThreadUpdateEPSSurpriseDB(&stock), 41);
	}
}