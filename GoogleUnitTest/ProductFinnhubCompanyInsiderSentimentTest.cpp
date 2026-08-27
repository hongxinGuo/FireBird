#include"pch.h"

#include <gtest/gtest.h>

#include"GeneralCheck.h"
#include"WorldMarket.h"
#include "ContainerFinnhubStock.h"
#include"FinnhubStock.h"
#include"InsiderSentiment.h"
#include"TimeConvert.h"

#include"ProductFinnhubCompanyInsiderSentiment.h"
#include "SystemMessage.h"
#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubCompanyInsiderSentimentTest : public Test {
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
		CProductFinnhubCompanyInsiderSentiment companyInsiderSentiment;
	};

	TEST_F(CFinnhubCompanyInsiderSentimentTest, TestInitialize) {
		EXPECT_EQ(companyInsiderSentiment.GetIndex(), 0);
		EXPECT_EQ(companyInsiderSentiment.GetInquiryFunction(), "https://finnhub.io/api/v1/stock/insider-sentiment?symbol=");
	}

	TEST_F(CFinnhubCompanyInsiderSentimentTest, TestCreatMessage) {
		const local_days lCurrentDate = gl_pWorldMarket->GetMarketDate();
		string sCurrentDate = std::format("{:%F}", lCurrentDate);

		gl_dataContainerFinnhubStock.GetItem(1)->SetUpdateInsiderSentiment(true);
		companyInsiderSentiment.SetIndex(1);
		EXPECT_EQ(companyInsiderSentiment.CreateMessage()->front(),
		          (companyInsiderSentiment.GetInquiryFunction() + gl_dataContainerFinnhubStock.GetItem(1)->GetSymbol() + "&from=1980-01-01&to=" + sCurrentDate));
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetItem(1)->IsUpdateInsiderSentiment()) << "接收到的数处理后方设置此标识";

		gl_dataContainerFinnhubStock.GetItem(1)->SetUpdateInsiderSentiment(true);
	}

	namespace {
		// 正确数据
		Test_FinnhubWebData finnhubWebData142(2, "AAPL", R"({"data":[{"symbol":"TSLA","year":2022,"month":3,"change":5540,"mspr":12.209097},{"symbol":"TSLA","year":2021,"month":1,"change":-1250,"mspr":-5.6179776}], "symbol":"TSLA"})");
		// 缺乏 data项
		Test_FinnhubWebData finnhubWebData143(3, "AAPL", R"({"no data":[{"symbol":"TSLA","year":2021,"month":3,"change":5540,"mspr":12.209097},{"symbol":"TSLA","year":2022,"month":1,"change":-1250,"mspr":-5.6179776}], "symbol":"TSLA"})");
		// 缺乏 Symbol项
		Test_FinnhubWebData finnhubWebData144(4, "AAPL", R"({"data":[{"no symbol":"TSLA","year":2021,"month":3,"change":5540,"mspr":12.209097},{"symbol":"TSLA","year":2022,"month":1,"change":-1250,"mspr":-5.6179776}], "symbol":"TSLA"})");
		// 空数据
		Test_FinnhubWebData finnhubWebData145(5, "AAPL", R"({"data":[], "symbol":"QNICF"})");
	}

	class ProcessFinnhubInsiderSentimentTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_index = pData->m_index;
			m_pStock = gl_dataContainerFinnhubStock.GetItem(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			EXPECT_EQ(m_pStock->GetInsiderSentimentUpdateDate(), toLocalDays(19800101));
			m_pStock->SetUpdateInsiderSentimentDB(false);
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			m_pWebData = pData->m_pData;
			m_finnhubCompanyInsiderSentiment.Test_checkAccessRight_(m_pWebData);

			const auto lIndex = gl_dataContainerFinnhubStock.GetOffset(pData->m_strSymbol);
			m_finnhubCompanyInsiderSentiment.SetIndex(lIndex);
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateProfileDB(false);
			m_pStock->SetUpdateInsiderSentimentDB(false);
			m_pStock->SetInsiderSentimentUpdateDate(toLocalDays(19800101));

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		int m_index;
		CFinnhubStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyInsiderSentiment m_finnhubCompanyInsiderSentiment;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubInsiderSentiment1, ProcessFinnhubInsiderSentimentTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData145, &finnhubWebData142, &finnhubWebData143, &finnhubWebData144));

	TEST_P(ProcessFinnhubInsiderSentimentTest, TestProsessFinnhubInsiderSentiment0) {
		auto pv = m_finnhubCompanyInsiderSentiment.Parse(m_pWebData->GetDataBuffer());
		switch (m_index) {
		case 0: // 空数据
			EXPECT_EQ(pv->size(), 0);
			break;
		case 1: // 无权利访问的数据
			EXPECT_EQ(pv->size(), 0);
			break;
		case 2: // 正确
			EXPECT_EQ(pv->size(), 2);
			break;
		case 3:
			EXPECT_EQ(pv->size(), 0);
			break;
		case 4:
			EXPECT_EQ(pv->size(), 0);
			break;
		case 5: // 空数据
			EXPECT_EQ(pv->size(), 0);
			break;
		default:
			break;
		}
	}

	class ParseFinnhubInsiderSentimentTest2 : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_index = pData->m_index;
			m_pStock = gl_dataContainerFinnhubStock.GetItem(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pWebData = pData->m_pData;
			m_finnhubCompanyInsiderSentiment.Test_checkAccessRight_(m_pWebData);

			m_pvInsiderSentiment = nullptr;
			const auto lIndex = gl_dataContainerFinnhubStock.GetOffset(pData->m_strSymbol);
			m_finnhubCompanyInsiderSentiment.SetIndex(lIndex);
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();

			SCOPED_TRACE("");
			GeneralCheck();
			m_pStock->SetUpdateProfileDB(false);
		}

	public:
		int m_index;
		CFinnhubStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CInsiderSentimentsPtr m_pvInsiderSentiment;
		CProductFinnhubCompanyInsiderSentiment m_finnhubCompanyInsiderSentiment;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubInsiderSentiment1, ParseFinnhubInsiderSentimentTest2,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData145, &finnhubWebData142, &finnhubWebData143, &finnhubWebData144));

	TEST_P(ParseFinnhubInsiderSentimentTest2, TestParseFinnhubInsiderSentiment0) {
		m_pvInsiderSentiment = m_finnhubCompanyInsiderSentiment.Parse(m_pWebData->GetDataBuffer());
		switch (m_index) {
		case 0: // 空数据
			EXPECT_EQ(m_pvInsiderSentiment->size(), 0);
			break;
		case 1: // 无权利访问的数据
			EXPECT_EQ(m_pvInsiderSentiment->size(), 0);
			break;
		case 2: // 正确
			EXPECT_EQ(m_pvInsiderSentiment->size(), 2);
			EXPECT_EQ(m_pvInsiderSentiment->at(1).m_strSymbol, "TSLA") << "数据按日期排列，此第一条排到了第二位";
			EXPECT_EQ(m_pvInsiderSentiment->at(1).m_lDate, 20220301) << "使用有效日期：每月的第一天，故而要加一";
			EXPECT_EQ(m_pvInsiderSentiment->at(1).m_lChange, 5540);
			EXPECT_DOUBLE_EQ(m_pvInsiderSentiment->at(1).m_mspr, 12.209097);
			EXPECT_TRUE(m_pvInsiderSentiment->at(1).m_lDate <= m_pvInsiderSentiment->at(1).m_lDate) << "此序列按交易日期顺序排列";
			break;
		case 3: // 缺乏data项
			EXPECT_EQ(m_pvInsiderSentiment->size(), 0);
			break;
		case 4: // 缺乏Symbol
			EXPECT_EQ(m_pvInsiderSentiment->size(), 0);
			break;
		case 5: //空数据
			EXPECT_EQ(m_pvInsiderSentiment->size(), 0);
			break;
		default:
			break;
		}
	}
}
