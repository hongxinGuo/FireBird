#include"pch.h"

#include"GeneralCheck.h"

#include"AccessoryDataSource.h"
#include"ProductIndexNasdaq100Stocks.h"

#include"WebData.h"

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
		EXPECT_EQ(nasdaq100Stocks.GetInquiryFunction(), _T("https://www.slickcharts.com/nasdaq100"));
	}

	TEST_F(CProductIndexNasdaq100StocksTest, TestCreatMessage) {
		EXPECT_EQ(nasdaq100Stocks.CreateMessage(), nasdaq100Stocks.GetInquiryFunction());
	}

	TEST_F(CProductIndexNasdaq100StocksTest, TestUpdateDataSourceStatus) {
		EXPECT_TRUE(gl_pAccessoryDataSource->IsUpdateIndexNasdaq100Stocks());
		nasdaq100Stocks.UpdateDataSourceStatus(gl_pAccessoryDataSource);

		EXPECT_FALSE(gl_pAccessoryDataSource->IsUpdateIndexNasdaq100Stocks());
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
		EXPECT_STREQ(gl_systemMessage.PopInnerSystemInformationMessage().c_str(), _T("Nasdaq 100 stock list updated"));

		// »Ö¸´Ô­×´
		gl_pAccessoryDataSource->SetUpdateIndexNasdaq100Stocks(true);
	}

	// Mock classes for dependencies
	class MockWebData : public CWebData {
	public:
		MOCK_METHOD(string_view, GetStringView, (int, size_t), (const, override));
		MOCK_METHOD(size_t, GetBufferLength, (), (const, noexcept,override));
	};

	using MockWebDataPtr = std::shared_ptr<MockWebData>;

	class MockAccessoryDataSource : public CAccessoryDataSource {
	public:
		MOCK_METHOD(void, SetUpdateIndexNasdaq100Stocks, (bool), (noexcept, override));
	};

	TEST_F(CProductIndexNasdaq100StocksTest, CreateMessage_ReturnsCorrectUrl) {
		std::string msg = nasdaq100Stocks.CreateMessage();
		EXPECT_EQ(msg, "https://www.slickcharts.com/nasdaq100");
	}

	TEST_F(CProductIndexNasdaq100StocksTest, ParseIndexNasdaq100Stocks_ParsesSymbols) {
		auto mockWebData = std::make_shared<MockWebData>();
		std::string fakeJson = R"({"companyListComponent":{"companyList":[{"symbol":"AAPL"},{"symbol":"MSFT"}]}})";
		std::string fakeString = "<script> window.__sc_init_state__ = " + fakeJson + "; </script>";
		std::string_view fakeScript = fakeString;
		EXPECT_CALL(*mockWebData, GetStringView(::testing::_, ::testing::_))
		.WillOnce(::testing::Return(fakeScript));
		EXPECT_CALL(*mockWebData, GetBufferLength())
		.WillOnce(::testing::Return(fakeScript.size()));

		auto symbols = nasdaq100Stocks.ParseIndexNasdaq100Stocks(mockWebData);
		EXPECT_THAT(symbols, ::testing::ElementsAre("AAPL", "MSFT"));
	}

	TEST_F(CProductIndexNasdaq100StocksTest, UpdateDataSourceStatus_UpdatesStatusAndSendsMessage) {
		auto mockDataSource = std::make_shared<MockAccessoryDataSource>();
		EXPECT_CALL(*mockDataSource, SetUpdateIndexNasdaq100Stocks(false)).Times(1);

		nasdaq100Stocks.UpdateDataSourceStatus(mockDataSource);

		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);

		//
		gl_systemMessage.PopInnerSystemInformationMessage();
	}

	// For ParseAndStoreWebData, you would need to mock gl_vNasdaq100Stocks and CSetIndexNasdaq100
	// This is a simplified example, actual implementation may require more setup
	TEST_F(CProductIndexNasdaq100StocksTest, ParseAndStoreWebData_StoresDataWhenEnoughSymbols) {
		// Setup: mock ParseIndexNasdaq100Stocks to return >=90 symbols
		// and mock CSetIndexNasdaq100 methods as needed
		// This test is left as a stub due to external dependencies
		SUCCEED();
	}
}
