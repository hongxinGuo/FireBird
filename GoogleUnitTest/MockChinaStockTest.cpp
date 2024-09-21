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
}
