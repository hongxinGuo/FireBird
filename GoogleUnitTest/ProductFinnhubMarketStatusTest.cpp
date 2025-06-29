#include"pch.h"

#include"GeneralCheck.h"

#include"ProductFinnhubMarketStatus.h"
#include "WorldMarket.h"
#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubMarketStatusProductTest : public Test {
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
		CProductFinnhubMarketStatus MarketStatusProduct;
	};

	TEST_F(CFinnhubMarketStatusProductTest, TestInitialize) {
		EXPECT_EQ(MarketStatusProduct.GetIndex(), 0);
		EXPECT_STREQ(MarketStatusProduct.GetInquiryFunction().c_str(), _T("https://finnhub.io/api/v1/stock/market-status?exchange="));
	}

	TEST_F(CFinnhubMarketStatusProductTest, TestCreatMessage) {
		MarketStatusProduct.SetMarket(gl_pWorldMarket);
		MarketStatusProduct.SetIndex(1);
		EXPECT_STREQ(MarketStatusProduct.CreateMessage().c_str(), (MarketStatusProduct.GetInquiryFunction() + gl_dataContainerStockExchange.GetExchangeCode(1)).c_str());
	}

	// 正确的数据
	Test_FinnhubWebData finnhubMarketStatusWebData22(2, _T("AAPL"), _T("{\"exchange\": \"US\",\"holiday\": null,\"isOpen\" : false,\"session\" : \"pre-market\",\"timezone\" : \"America/New York\",\"t\" : 1697018041}"));

	class ParseFinnhubMarketStatusTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubMarketStatusProduct.__Test_checkAccessRight(m_pWebData);

			m_pvMarketStatus = nullptr;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CMarketStatusPtr m_pMarketStatus;
		CWebDataPtr m_pWebData;
		CMarketStatusesPtr m_pvMarketStatus;
		CProductFinnhubMarketStatus m_finnhubMarketStatusProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubMarketStatus1, ParseFinnhubMarketStatusTest, testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubMarketStatusWebData22));

	TEST_P(ParseFinnhubMarketStatusTest, TestParseFinnhubMarketStatus0) {
		m_pvMarketStatus = m_finnhubMarketStatusProduct.ParseFinnhubMarketStatus(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_EQ(m_pvMarketStatus->size(), 0);
			break;
		case 1: // 无权利访问的数据
			EXPECT_EQ(m_pvMarketStatus->size(), 0);
			break;
		case 2: // 正确的数据
			EXPECT_EQ(m_pvMarketStatus->size(), 1);
			EXPECT_STREQ(m_pvMarketStatus->at(0)->m_strExchange.c_str(), _T("US"));
			EXPECT_STREQ(m_pvMarketStatus->at(0)->m_strHoliday.c_str(), _T(""));
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubMarketStatusTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubMarketStatusProduct.__Test_checkAccessRight(m_pWebData);

			m_finnhubMarketStatusProduct.SetMarket(gl_pWorldMarket);
			m_finnhubMarketStatusProduct.SetIndex(0); // 第一个交易所（AS)
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductFinnhubMarketStatus m_finnhubMarketStatusProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubMarketStatus1, ProcessFinnhubMarketStatusTest, testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubMarketStatusWebData22));

	TEST_P(ProcessFinnhubMarketStatusTest, TestParseFinnhubMarketStatus0) {
		CMarketStatusPtr pMarketStatus;
		m_finnhubMarketStatusProduct.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			break;
		case 1: // 无权利访问的数据
			break;
		case 2: // 正确的数据

			break;
		default:
			break;
		}
	}
}
