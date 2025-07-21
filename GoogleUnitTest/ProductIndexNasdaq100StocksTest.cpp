#include"pch.h"

#include"GeneralCheck.h"

#include"AccessoryDataSource.h"
#include"ProductIndexNasdaq100Stocks.h"

using namespace testing;

namespace FireBirdTest {
	class CProductIndexNasdaq100StocksTest : public Test {
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

	protected:
		CProductIndexNasdaq100Stocks nasdaq100Stocks;
	};

	TEST_F(CProductIndexNasdaq100StocksTest, TestInitialize) {
		EXPECT_EQ(nasdaq100Stocks.GetIndex(), 0);
		EXPECT_STREQ(nasdaq100Stocks.GetInquiryFunction().c_str(), _T("https://www.slickcharts.com/nasdaq100"));
	}

	TEST_F(CProductIndexNasdaq100StocksTest, TestCreatMessage) {
		EXPECT_STREQ(nasdaq100Stocks.CreateMessage().c_str(), nasdaq100Stocks.GetInquiryFunction().c_str());
	}

	TEST_F(CProductIndexNasdaq100StocksTest, TestUpdateDataSourceStatus) {
		EXPECT_TRUE(gl_pAccessoryDataSource->IsUpdateIndexNasdaq100Stocks());
		nasdaq100Stocks.UpdateDataSourceStatus(gl_pAccessoryDataSource);

		EXPECT_FALSE(gl_pAccessoryDataSource->IsUpdateIndexNasdaq100Stocks());

		// »Ö¸´Ô­×´
		gl_pAccessoryDataSource->SetUpdateIndexNasdaq100Stocks(true);
	}
}
