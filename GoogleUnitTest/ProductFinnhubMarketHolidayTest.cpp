#include"pch.h"

#include"GeneralCheck.h"

#include"ProductFinnhubMarketHoliday.h"
#include "WorldMarket.h"
#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubMarketHolidayProductTest : public Test {
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
		CProductFinnhubMarketHoliday MarketHolidayProduct;
	};

	TEST_F(CFinnhubMarketHolidayProductTest, TestInitialize) {
		EXPECT_EQ(MarketHolidayProduct.GetIndex(), 0);
		EXPECT_EQ(MarketHolidayProduct.GetInquiryFunction(), "https://finnhub.io/api/v1/stock/market-holiday?exchange=");
	}

	TEST_F(CFinnhubMarketHolidayProductTest, TestCreatMessage) {
		MarketHolidayProduct.SetMarket(gl_pWorldMarket);
		MarketHolidayProduct.SetIndex(1);
		EXPECT_EQ(MarketHolidayProduct.CreateMessage(), (MarketHolidayProduct.GetInquiryFunction() + gl_dataContainerStockExchange.GetItemExchangeCode(1)));
	}

	// 正确的数据
	Test_FinnhubWebData finnhubMarketHolidayWebData22(2, "AAPL", "{\"data\": [{\"eventName\": \"Christmas\",\"atDate\": \"2023-12-25\",\"tradingHour\" : \"\"},{\"eventName\": \"Independence Day\",\"atDate\": \"2023-07-04\",\"tradingHour\" : \"09:30-13:00\"}],\"exchange\": \"US\",\"timezone\": \"America/New_York\"}");

	class ParseFinnhubMarketHolidayTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubMarketHolidayProduct.__Test_checkAccessRight(m_pWebData);

			m_pvMarketHoliday = nullptr;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CMarketHolidayPtr m_pMarketHoliday;
		CWebDataPtr m_pWebData;
		CMarketHolidaysPtr m_pvMarketHoliday;
		CProductFinnhubMarketHoliday m_finnhubMarketHolidayProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubMarketHoliday1, ParseFinnhubMarketHolidayTest, testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubMarketHolidayWebData22));

	TEST_P(ParseFinnhubMarketHolidayTest, TestParseFinnhubMarketHoliday0) {
		m_pvMarketHoliday = m_finnhubMarketHolidayProduct.ParseFinnhubMarketHoliday(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_EQ(m_pvMarketHoliday->size(), 0);
			break;
		case 1: // 无权利访问的数据
			EXPECT_EQ(m_pvMarketHoliday->size(), 0);
			break;
		case 2: // 正确的数据
			EXPECT_EQ(m_pvMarketHoliday->size(), 2);
			EXPECT_EQ(m_pvMarketHoliday->at(0)->m_strExchange, "US");
			EXPECT_EQ(m_pvMarketHoliday->at(0)->m_strEventName, "Christmas");
			EXPECT_EQ(m_pvMarketHoliday->at(1)->m_strExchange, "US");
			EXPECT_EQ(m_pvMarketHoliday->at(1)->m_strEventName, "Independence Day");
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubMarketHolidayTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubMarketHolidayProduct.__Test_checkAccessRight(m_pWebData);

			m_finnhubMarketHolidayProduct.SetMarket(gl_pWorldMarket);
			m_finnhubMarketHolidayProduct.SetIndex(0); // 第一个交易所（AS)
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
		CProductFinnhubMarketHoliday m_finnhubMarketHolidayProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubMarketHoliday1, ProcessFinnhubMarketHolidayTest, testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubMarketHolidayWebData22));

	TEST_P(ProcessFinnhubMarketHolidayTest, TestParseFinnhubMarketHoliday0) {
		CMarketHolidayPtr pMarketHoliday;
		m_finnhubMarketHolidayProduct.ParseAndStoreWebData(m_pWebData);
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
