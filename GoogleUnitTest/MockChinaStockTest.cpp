#include"pch.h"

#include"Thread.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

#include"MockChinaStock.h"

#include"GeneralCheck.h"

using namespace testing;

namespace FireBirdTest {
	class CMockChinaStockTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();

			pStock = make_shared<CMockChinaStock>();
			gl_pChinaMarket->CalculateTime();
		}

		void TearDown() override {
			// clearUp
			gl_systemConfiguration.SetExitingSystem(false);
			gl_pChinaMarket->CalculateTime();
			gl_pChinaMarket->SetUpdateOptionDB(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CMockChinaStockPtr pStock;
		CMockChinaStock stock;
	};

	TEST_F(CMockChinaStockTest, TestShowCurrentTransaction) {
		EXPECT_CALL(*pStock, ReportGuadanTransaction())
		.Times(0);
		pStock->ShowCurrentTransaction();
		EXPECT_EQ(pStock->GetCurrentTransactionVolume(), 0);
		pStock->SetCurrentTransactionVolume(1);
		EXPECT_CALL(*pStock, ReportGuadanTransaction())
		.Times(1);
		pStock->ShowCurrentTransaction();
	}

	TEST_F(CMockChinaStockTest, TestShowCurrentInformationOfCancelingGuadan) {
		EXPECT_CALL(*pStock, ReportGuadan())
		.Times(1);
		pStock->ShowCurrentInformationOfCancelingGuadan();
	}
}
