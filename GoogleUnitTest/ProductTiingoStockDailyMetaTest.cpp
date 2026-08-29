#include"pch.h"

#include <gtest/gtest.h>
#include"GeneralCheck.h"

#include "ContainerTiingoStock.h"
#include "TiingoStock.h"

#include"WorldMarket.h"
#include "TiingoStockDailyMeta.h"

#include"ProductTiingoStockDailyMeta.h"
#include "SystemConfiguration.h"

#include"TestWebData.h"
#include "TimeConvert.h"

using namespace testing;

namespace FireBirdTest {
	class CProductTiingoStockDailyMetaTest : public Test {
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
		CProductTiingoStockDailyMeta stockDailyMeta;
	};

	TEST_F(CProductTiingoStockDailyMetaTest, TestInitialize) {
		EXPECT_EQ(stockDailyMeta.GetIndex(), 0);
		EXPECT_EQ(stockDailyMeta.GetInquiryFunction(), "https://api.tiingo.com/tiingo/daily/");
	}

	TEST_F(CProductTiingoStockDailyMetaTest, TestCreatMessage) {
		stockDailyMeta.SetIndex(1); // stock code is AA
		EXPECT_EQ(stockDailyMeta.CreateMessage()->front(), "https://api.tiingo.com/tiingo/daily/AA?");
	}

	namespace {
		// 正确的数据
		Test_TiingoWebData tiingoStockDailyMeta1(1, "", R"({"ticker":"AAPL","name":"Apple Inc","exchangeCode":"NASDAQ","startDate":"1980-12-12","endDate":"2019-01-25","description":"Apple Inc.services, peripherals"})");
		// 日期皆为null
		Test_TiingoWebData tiingoStockDailyMeta2(2, "", R"({"ticker": "AAPL","name" : "Apple Inc","exchangeCode" : "NASDAQ","startDate" : null,"endDate" : null,"description" : "Apple Inc. (Apple) designs, manufactures and markets mobile"})");
		// 第二个数据缺项
		Test_TiingoWebData tiingoStockDailyMeta3(3, "", R"({"ticker": "AAPL","name" : "Apple Inc","exchangeCode" : "NASDAQ","startDate" : "1980-12-12","endDate" : "2019-01-25","description" : "Apple Inc. (Apple) desi"})");
		// 正确的数据
		Test_TiingoWebData tiingoStockDailyMeta4(4, "", R"({"ticker": "AAPL","name" : "Apple Inc","exchangeCode" : "NASDAQ","startDate" : "1980-12-12","endDate" : "2019-01-25","description" : "Apple Inc. (Apple) des"})");
		// 正确的数据
		Test_TiingoWebData tiingoStockDailyMeta10(10, "", R"({"ticker": "AAPL","name" : "Apple Inc","exchangeCode" : "NASDAQ","startDate" : "1980-12-12","endDate" : "2019-01-25","description" : "Apple Inc. (Apple) designs, manufactures"})");
	}

	class ProcessTiingoStockDailyMetaTest2 : public TestWithParam<Test_TiingoWebData*> {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			auto pStock = gl_dataContainerTiingoStock.GetStock("AAPL");
			pStock->SetHistoryDayLineStartDate(toLocalDays(19000101));
			pStock->SetHistoryDayLineEndDate(toLocalDays(19000101));
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_TiingoWebData* pData = GetParam();
			m_index = pData->m_index;
			m_text = pData->m_data;
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		int m_index;
		string m_text;
		CProductTiingoStockDailyMeta m_tiingoStockProduct;
		long long m_llTiingoBandWidthLeft;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessTiingoStockDailyMeta,
	                         ProcessTiingoStockDailyMetaTest2,
	                         testing::Values(&tiingoStockDailyMeta1, &tiingoStockDailyMeta2,
		                         &tiingoStockDailyMeta3, &tiingoStockDailyMeta4, &tiingoStockDailyMeta10));

	TEST_P(ProcessTiingoStockDailyMetaTest2, TestProcessStockDailyMeta) {
		auto pDailyMeta = m_tiingoStockProduct.Parse(m_text);
		switch (m_index) {
		case 1: // 格式不对
			EXPECT_EQ(pDailyMeta->m_strCode, "AAPL");
			EXPECT_EQ(pDailyMeta->m_strName, "Apple Inc");
			EXPECT_EQ(pDailyMeta->m_strExchange, "NASDAQ");
			EXPECT_EQ(pDailyMeta->m_lHistoryDayLineStartDate, toLocalDays(19801212));
			EXPECT_EQ(pDailyMeta->m_lHistoryDayLineEndDate, toLocalDays(20190125));
			break;
		case 2: // 日期皆为空null
			EXPECT_EQ(pDailyMeta->m_strCode, "AAPL");
			EXPECT_EQ(pDailyMeta->m_strName, "Apple Inc");
			EXPECT_EQ(pDailyMeta->m_strExchange, "NASDAQ");
			EXPECT_EQ(pDailyMeta->m_lHistoryDayLineStartDate, toLocalDays(19000101));
			EXPECT_EQ(pDailyMeta->m_lHistoryDayLineEndDate, toLocalDays(19000101 ));
			break;
		case 3: // 第二个数据缺乏address项,返回一个成功
			break;
		case 4:
			break;
		case 10:
			EXPECT_EQ(pDailyMeta->m_strCode, "AAPL");
			EXPECT_EQ(pDailyMeta->m_strName, "Apple Inc");
			EXPECT_EQ(pDailyMeta->m_strExchange, "NASDAQ");
			EXPECT_EQ(pDailyMeta->m_lHistoryDayLineStartDate, toLocalDays(19801212));
			EXPECT_EQ(pDailyMeta->m_lHistoryDayLineEndDate, toLocalDays(20190125));
			// 恢复原状
			break;
		default:
			break;
		}
	}
}
