#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubCompanyInsiderSentiment.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubCompanyInsiderSentimentTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
		}

		void TearDown(void) override {
			// clearu
			GeneralCheck();
		}

	protected:
		CProductFinnhubCompanyInsiderSentiment companyInsiderSentiment;
	};

	TEST_F(CFinnhubCompanyInsiderSentimentTest, TestInitialize) {
		EXPECT_EQ(companyInsiderSentiment.GetIndex(), -1);
		EXPECT_STREQ(companyInsiderSentiment.GetInquiry(), _T("https://finnhub.io/api/v1/stock/insider-sentiment?symbol="));
	}

	TEST_F(CFinnhubCompanyInsiderSentimentTest, TestCreatMessage) {
		long lCurrentDate = gl_pWorldMarket->GetMarketDate();
		char buffer[100];
		CString strCurrentDate;
		sprintf_s(buffer, _T("%4d-%02d-%02d"), lCurrentDate / 10000, (lCurrentDate % 10000) / 100, lCurrentDate % 100);
		strCurrentDate = buffer;

		gl_pWorldMarket->GetStock(1)->SetInsiderSentimentNeedUpdate(true);
		companyInsiderSentiment.SetMarket(gl_pWorldMarket.get());
		companyInsiderSentiment.SetIndex(1);
		EXPECT_STREQ(companyInsiderSentiment.CreateMessage(),
		             companyInsiderSentiment.GetInquiry() + gl_pWorldMarket->GetStock(1)->GetSymbol() + _T("&from=1980-01-01&to=") + strCurrentDate);
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsInsiderSentimentNeedUpdate()) << "接收到的数处理后方设置此标识";

		gl_pWorldMarket->GetStock(1)->SetInsiderSentimentNeedUpdate(true);
	}

	// 正确数据
	FinnhubWebData finnhubWebData141(1, _T("AAPL"), _T("{\"data\":[{\"symbol\":\"TSLA\",\"year\":2022,\"month\":3,\"change\":5540,\"mspr\":12.209097},{\"symbol\":\"TSLA\",\"year\":2021,\"month\":1,\"change\":-1250,\"mspr\":-5.6179776}], \"symbol\":\"TSLA\"}"));
	// 缺乏 data项
	FinnhubWebData finnhubWebData142(2, _T("AAPL"), _T(
		                                 "{\"no data\":[{\"symbol\":\"TSLA\",\"year\":2021,\"month\":3,\"change\":5540,\"mspr\":12.209097},{\"symbol\":\"TSLA\",\"year\":2022,\"month\":1,\"change\":-1250,\"mspr\":-5.6179776}], \"symbol\":\"TSLA\"}"));
	// 缺乏 Symbol项
	FinnhubWebData finnhubWebData143(3, _T("AAPL"), _T(
		                                 "{\"data\":[{\"no symbol\":\"TSLA\",\"year\":2021,\"month\":3,\"change\":5540,\"mspr\":12.209097},{\"symbol\":\"TSLA\",\"year\":2022,\"month\":1,\"change\":-1250,\"mspr\":-5.6179776}], \"symbol\":\"TSLA\"}"));
	// 空数据
	FinnhubWebData finnhubWebData144(4, _T("AAPL"), _T("{\"data\":[], \"symbol\":\"QNICF\"}"));

	class ProcessFinnhubInsiderSentimentTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();
			const FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			EXPECT_EQ(m_pStock->GetInsiderSentimentUpdateDate(), 19800101);
			m_pStock->SetInsiderSentimentNeedSave(false);
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			m_pWebData = pData->m_pData;
			m_pWebData->CreateNlohmannJson();
			m_pWebData->SetJSonContentType(true);
			m_finnhubCompanyInsiderSentiment.SetMarket(gl_pWorldMarket.get());
			const long lIndex = gl_pWorldMarket->GetStockIndex(pData->m_strSymbol);
			m_finnhubCompanyInsiderSentiment.SetIndex(lIndex);
		}

		void TearDown(void) override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateProfileDB(false);
			m_pStock->SetInsiderSentimentNeedSave(false);
			m_pStock->SetInsiderSentimentUpdateDate(19800101);

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyInsiderSentiment m_finnhubCompanyInsiderSentiment;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubInsiderSentiment1, ProcessFinnhubInsiderSentimentTest,
	                         testing::Values(&finnhubWebData141, &finnhubWebData142, &finnhubWebData143, &finnhubWebData144));

	TEST_P(ProcessFinnhubInsiderSentimentTest, TestProsessFinnhubInsiderSentiment0) {
		m_finnhubCompanyInsiderSentiment.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确
			EXPECT_FALSE(m_pStock->IsInsiderSentimentNeedUpdate());
			EXPECT_TRUE(m_pStock->IsInsiderSentimentNeedSave());
			EXPECT_NE(m_pStock->GetInsiderSentimentUpdateDate(), 19800101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 2:
			EXPECT_FALSE(m_pStock->IsInsiderSentimentNeedUpdate());
			EXPECT_NE(m_pStock->GetInsiderSentimentUpdateDate(), 19800101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_FALSE(m_pStock->IsInsiderSentimentNeedSave());
			break;
		case 3:
			EXPECT_FALSE(m_pStock->IsInsiderSentimentNeedUpdate());
			EXPECT_NE(m_pStock->GetInsiderSentimentUpdateDate(), 19800101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_FALSE(m_pStock->IsInsiderSentimentNeedSave());
			break;
		case 4: // 空数据
			EXPECT_FALSE(m_pStock->IsInsiderSentimentNeedUpdate());
			EXPECT_NE(m_pStock->GetInsiderSentimentUpdateDate(), 19800101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_FALSE(m_pStock->IsInsiderSentimentNeedSave());
			break;
		default:
			break;
		}
	}

	class ParseFinnhubInsiderSentimentTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pWebData = pData->m_pData;
			m_pWebData->CreateNlohmannJson();
			m_pWebData->SetJSonContentType(true);
			m_pvInsiderSentiment = nullptr;
		}

		void TearDown(void) override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();

			GeneralCheck();
			m_pStock->SetUpdateProfileDB(false);
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CInsiderSentimentVectorPtr m_pvInsiderSentiment;
		CProductFinnhubCompanyInsiderSentiment m_finnhubCompanyInsiderSentiment;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubInsiderSentiment1, ParseFinnhubInsiderSentimentTest,
	                         testing::Values(&finnhubWebData141, &finnhubWebData142, &finnhubWebData143, &finnhubWebData144));

	TEST_P(ParseFinnhubInsiderSentimentTest, TestParseFinnhubInsiderSentiment0) {
		m_pvInsiderSentiment = m_finnhubCompanyInsiderSentiment.ParseFinnhubStockInsiderSentiment(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确
			EXPECT_EQ(m_pvInsiderSentiment->size(), 2);
			EXPECT_STREQ(m_pvInsiderSentiment->at(1)->m_strSymbol, _T("TSLA")) << "数据按日期排列，此第一条排到了第二位";
			EXPECT_EQ(m_pvInsiderSentiment->at(1)->m_lDate, 20220301) << "使用有效日期：每月的第一天，故而要加一";
			EXPECT_EQ(m_pvInsiderSentiment->at(1)->m_lChange, 5540);
			EXPECT_DOUBLE_EQ(m_pvInsiderSentiment->at(1)->m_mspr, 12.209097);
			EXPECT_TRUE(m_pvInsiderSentiment->at(1)->m_lDate <= m_pvInsiderSentiment->at(1)->m_lDate) << "此序列按交易日期顺序排列";
			break;
		case 2: // 缺乏data项
			EXPECT_EQ(m_pvInsiderSentiment->size(), 0);
			break;
		case 3: // 缺乏Symbol
			EXPECT_EQ(m_pvInsiderSentiment->size(), 0);
			break;
		case 4: //空数据
			EXPECT_EQ(m_pvInsiderSentiment->size(), 0);
			break;
		default:
			break;
		}
	}

	class ParseFinnhubInsiderSentimentTest2 : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pWebData = pData->m_pData;
			m_pWebData->CreateNlohmannJson();
			m_pWebData->SetJSonContentType(true);
			m_pvInsiderSentiment = nullptr;
		}

		void TearDown(void) override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();

			GeneralCheck();
			m_pStock->SetUpdateProfileDB(false);
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CInsiderSentimentVectorPtr m_pvInsiderSentiment;
		CProductFinnhubCompanyInsiderSentiment m_finnhubCompanyInsiderSentiment;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubInsiderSentiment1, ParseFinnhubInsiderSentimentTest2,
	                         testing::Values(&finnhubWebData141, &finnhubWebData142, &finnhubWebData143, &finnhubWebData144));

	TEST_P(ParseFinnhubInsiderSentimentTest2, TestParseFinnhubInsiderSentiment0) {
		m_pvInsiderSentiment = m_finnhubCompanyInsiderSentiment.ParseFinnhubStockInsiderSentiment(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确
			EXPECT_EQ(m_pvInsiderSentiment->size(), 2);
			EXPECT_STREQ(m_pvInsiderSentiment->at(1)->m_strSymbol, _T("TSLA")) << "数据按日期排列，此第一条排到了第二位";
			EXPECT_EQ(m_pvInsiderSentiment->at(1)->m_lDate, 20220301) << "使用有效日期：每月的第一天，故而要加一";
			EXPECT_EQ(m_pvInsiderSentiment->at(1)->m_lChange, 5540);
			EXPECT_DOUBLE_EQ(m_pvInsiderSentiment->at(1)->m_mspr, 12.209097);
			EXPECT_TRUE(m_pvInsiderSentiment->at(1)->m_lDate <= m_pvInsiderSentiment->at(1)->m_lDate) << "此序列按交易日期顺序排列";
			break;
		case 2: // 缺乏data项
			EXPECT_EQ(m_pvInsiderSentiment->size(), 0);
			break;
		case 3: // 缺乏Symbol
			EXPECT_EQ(m_pvInsiderSentiment->size(), 0);
			break;
		case 4: //空数据
			EXPECT_EQ(m_pvInsiderSentiment->size(), 0);
			break;
		default:
			break;
		}
	}
}
