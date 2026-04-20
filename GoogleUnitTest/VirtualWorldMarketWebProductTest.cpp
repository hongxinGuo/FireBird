#include"pch.h"

#include"VirtualWorldMarketWebProduct.h"
#include "WorldMarket.h"
#include "InaccessibleSymbol.h"
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
			gl_finnhubInaccessibleExchange.SetUpdateDB(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CVirtualWorldMarketWebProduct webProduct;
	};

	//using CVirtualWorldMarketWebProductDeathTest = CVirtualWorldMarketWebProductTest;
}
