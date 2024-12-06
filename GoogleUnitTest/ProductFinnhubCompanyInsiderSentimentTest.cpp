#include"pch.h"

////#include"gtest/gtest.h"

#include"SystemMessage.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubCompanyInsiderSentiment.h"

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
		EXPECT_STREQ(companyInsiderSentiment.GetInquiryFunction(), _T("https://finnhub.io/api/v1/stock/insider-sentiment?symbol="));
	}

	TEST_F(CFinnhubCompanyInsiderSentimentTest, TestCreatMessage) {
		const long lCurrentDate = gl_pWorldMarket->GetMarketDate();
		char buffer[100];
		sprintf_s(buffer, _T("%4d-%02d-%02d"), lCurrentDate / 10000, (lCurrentDate % 10000) / 100, lCurrentDate % 100);
		const CString strCurrentDate = buffer;

		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateInsiderSentiment(true);
		companyInsiderSentiment.SetMarket(gl_pWorldMarket);
		companyInsiderSentiment.SetIndex(1);
		EXPECT_STREQ(companyInsiderSentiment.CreateMessage(),
		             companyInsiderSentiment.GetInquiryFunction() + gl_dataContainerFinnhubStock.GetStock(1)->GetSymbol() + _T("&from=1980-01-01&to=") + strCurrentDate);
		EXPECT_TRUE(gl_dataContainerFinnhubStock.GetStock(1)->IsUpdateInsiderSentiment()) << "接收到的数处理后方设置此标识";

		gl_dataContainerFinnhubStock.GetStock(1)->SetUpdateInsiderSentiment(true);
	}

	// 正确数据
	Test_FinnhubWebData finnhubWebData142(2, _T("AAPL"), _T("{\"data\":[{\"symbol\":\"TSLA\",\"year\":2022,\"month\":3,\"change\":5540,\"mspr\":12.209097},{\"symbol\":\"TSLA\",\"year\":2021,\"month\":1,\"change\":-1250,\"mspr\":-5.6179776}], \"symbol\":\"TSLA\"}"));
	// 缺乏 data项
	Test_FinnhubWebData finnhubWebData143(3, _T("AAPL"), _T(
		                                      "{\"no data\":[{\"symbol\":\"TSLA\",\"year\":2021,\"month\":3,\"change\":5540,\"mspr\":12.209097},{\"symbol\":\"TSLA\",\"year\":2022,\"month\":1,\"change\":-1250,\"mspr\":-5.6179776}], \"symbol\":\"TSLA\"}"));
	// 缺乏 Symbol项
	Test_FinnhubWebData finnhubWebData144(4, _T("AAPL"), _T(
		                                      "{\"data\":[{\"no symbol\":\"TSLA\",\"year\":2021,\"month\":3,\"change\":5540,\"mspr\":12.209097},{\"symbol\":\"TSLA\",\"year\":2022,\"month\":1,\"change\":-1250,\"mspr\":-5.6179776}], \"symbol\":\"TSLA\"}"));
	// 空数据
	Test_FinnhubWebData finnhubWebData145(5, _T("AAPL"), _T("{\"data\":[], \"symbol\":\"QNICF\"}"));

	class ProcessFinnhubInsiderSentimentTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_dataContainerFinnhubStock.GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			EXPECT_EQ(m_pStock->GetInsiderSentimentUpdateDate(), 19800101);
			m_pStock->SetUpdateInsiderSentimentDB(false);
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			m_pWebData = pData->m_pData;
			m_finnhubCompanyInsiderSentiment.__Test_checkAccessRight(m_pWebData);

			m_finnhubCompanyInsiderSentiment.SetMarket(gl_pWorldMarket);
			const auto lIndex = gl_dataContainerFinnhubStock.GetOffset(pData->m_strSymbol);
			m_finnhubCompanyInsiderSentiment.SetIndex(lIndex);
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateProfileDB(false);
			m_pStock->SetUpdateInsiderSentimentDB(false);
			m_pStock->SetInsiderSentimentUpdateDate(19800101);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CFinnhubStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyInsiderSentiment m_finnhubCompanyInsiderSentiment;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubInsiderSentiment1, ProcessFinnhubInsiderSentimentTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData145, &finnhubWebData142, &finnhubWebData143, &finnhubWebData144));

	TEST_P(ProcessFinnhubInsiderSentimentTest, TestProsessFinnhubInsiderSentiment0) {
		m_finnhubCompanyInsiderSentiment.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentiment());
			EXPECT_NE(m_pStock->GetInsiderSentimentUpdateDate(), 19800101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentimentDB());
			break;
		case 1: // 无权利访问的数据
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentiment());
			EXPECT_NE(m_pStock->GetInsiderSentimentUpdateDate(), 19800101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentimentDB());
			break;
		case 2: // 正确
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentiment());
			EXPECT_TRUE(m_pStock->IsUpdateInsiderSentimentDB());
			EXPECT_NE(m_pStock->GetInsiderSentimentUpdateDate(), 19800101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 3:
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentiment());
			EXPECT_NE(m_pStock->GetInsiderSentimentUpdateDate(), 19800101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentimentDB());
			break;
		case 4:
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentiment());
			EXPECT_NE(m_pStock->GetInsiderSentimentUpdateDate(), 19800101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentimentDB());
			break;
		case 5: // 空数据
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentiment());
			EXPECT_NE(m_pStock->GetInsiderSentimentUpdateDate(), 19800101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentimentDB());
			break;
		default:
			break;
		}
	}

	class ParseFinnhubInsiderSentimentTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_dataContainerFinnhubStock.GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentimentDB());
			m_pWebData = pData->m_pData;
			m_finnhubCompanyInsiderSentiment.__Test_checkAccessRight(m_pWebData);

			m_pvInsiderSentiment = nullptr;
			m_finnhubCompanyInsiderSentiment.SetMarket(gl_pWorldMarket);
			const auto lIndex = gl_dataContainerFinnhubStock.GetOffset(pData->m_strSymbol);
			m_finnhubCompanyInsiderSentiment.SetIndex(lIndex);
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateProfileDB(false);
			m_pStock->SetUpdateInsiderSentimentDB(false);
			m_pStock->SetInsiderSentimentUpdateDate(19800101);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CFinnhubStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CInsiderSentimentsPtr m_pvInsiderSentiment;
		CProductFinnhubCompanyInsiderSentiment m_finnhubCompanyInsiderSentiment;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubInsiderSentiment1, ParseFinnhubInsiderSentimentTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData145, &finnhubWebData142, &finnhubWebData143, &finnhubWebData144));

	TEST_P(ParseFinnhubInsiderSentimentTest, TestParseFinnhubInsiderSentiment0) {
		m_finnhubCompanyInsiderSentiment.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentiment());
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentimentDB());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetInsiderSentimentUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 1: // 无权利访问的数据
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentiment());
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentimentDB());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetInsiderSentimentUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 2: // 正确
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentiment());
			EXPECT_TRUE(m_pStock->IsUpdateInsiderSentimentDB());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetInsiderSentimentUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 3: // 缺乏data项
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentiment());
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentimentDB());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetInsiderSentimentUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 4: // 缺乏Symbol
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentiment());
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentimentDB());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetInsiderSentimentUpdateDate(), gl_pWorldMarket->GetMarketDate());
			break;
		case 5: //空数据
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentiment());
			EXPECT_FALSE(m_pStock->IsUpdateInsiderSentimentDB());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_EQ(m_pStock->GetInsiderSentimentUpdateDate(), gl_pWorldMarket->GetMarketDate());
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
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_dataContainerFinnhubStock.GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pWebData = pData->m_pData;
			m_finnhubCompanyInsiderSentiment.__Test_checkAccessRight(m_pWebData);

			m_pvInsiderSentiment = nullptr;
			m_finnhubCompanyInsiderSentiment.SetMarket(gl_pWorldMarket);
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
		long m_lIndex;
		CFinnhubStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CInsiderSentimentsPtr m_pvInsiderSentiment;
		CProductFinnhubCompanyInsiderSentiment m_finnhubCompanyInsiderSentiment;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubInsiderSentiment1, ParseFinnhubInsiderSentimentTest2,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData145, &finnhubWebData142, &finnhubWebData143, &finnhubWebData144));

	TEST_P(ParseFinnhubInsiderSentimentTest2, TestParseFinnhubInsiderSentiment0) {
		m_pvInsiderSentiment = m_finnhubCompanyInsiderSentiment.ParseFinnhubStockInsiderSentiment(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_EQ(m_pvInsiderSentiment->size(), 0);
			break;
		case 1: // 无权利访问的数据
			EXPECT_EQ(m_pvInsiderSentiment->size(), 0);
			break;
		case 2: // 正确
			EXPECT_EQ(m_pvInsiderSentiment->size(), 2);
			EXPECT_STREQ(m_pvInsiderSentiment->at(1)->m_strSymbol, _T("TSLA")) << "数据按日期排列，此第一条排到了第二位";
			EXPECT_EQ(m_pvInsiderSentiment->at(1)->m_lDate, 20220301) << "使用有效日期：每月的第一天，故而要加一";
			EXPECT_EQ(m_pvInsiderSentiment->at(1)->m_lChange, 5540);
			EXPECT_DOUBLE_EQ(m_pvInsiderSentiment->at(1)->m_mspr, 12.209097);
			EXPECT_TRUE(m_pvInsiderSentiment->at(1)->m_lDate <= m_pvInsiderSentiment->at(1)->m_lDate) << "此序列按交易日期顺序排列";
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
