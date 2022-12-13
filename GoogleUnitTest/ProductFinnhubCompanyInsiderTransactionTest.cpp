#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubCompanyInsiderTransaction.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubCompanyInsiderTransactionTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearu
			GeneralCheck();
		}

	protected:
		CProductFinnhubCompanyInsiderTransaction companyInsiderTransaction;
	};

	TEST_F(CFinnhubCompanyInsiderTransactionTest, TestInitialize) {
		EXPECT_EQ(companyInsiderTransaction.GetIndex(), -1);
		EXPECT_STREQ(companyInsiderTransaction.GetInquiry(), _T("https://finnhub.io/api/v1/stock/insider-transactions?symbol="));
	}

	TEST_F(CFinnhubCompanyInsiderTransactionTest, TestCreatMessage) {
		gl_pWorldMarket->GetStock(1)->SetInsiderTransactionNeedUpdate(true);
		companyInsiderTransaction.SetMarket(gl_pWorldMarket.get());
		companyInsiderTransaction.SetIndex(1);
		EXPECT_STREQ(companyInsiderTransaction.CreateMessage(), companyInsiderTransaction.GetInquiry() + gl_pWorldMarket->GetStock(1)->GetSymbol());
		EXPECT_TRUE(gl_pWorldMarket->GetStock(1)->IsInsiderTransactionNeedUpdate()) << "接收到的数据处理后方设置此标识";

		gl_pWorldMarket->GetStock(1)->SetInsiderTransactionNeedUpdate(true);
	}

	// 正确数据
	FinnhubWebData finnhubWebData131(1, _T("AAPL"), _T(
		                                 "{\"data\":[{\"name\":\"Long Brady K\",\"share\":269036,\"change\":-14236,\"filingDate\":\"2021-03-03\",\"transactionDate\":\"2021-03-03\",\"transactionCode\":\"F\",\"transactionPrice\":3.68},{\"name\":\"Adamson Keelan\",\"share\":221083,\"change\":-11347,\"filingDate\" : \"2021-03-03\",\"transactionDate\" : \"2021-03-02\",\"transactionCode\" : \"F\",\"transactionPrice\" : 3.68 }] , \"symbol\" : \"RIG\"}"));
	// 缺乏 data项
	FinnhubWebData finnhubWebData132(2, _T("AAPL"), _T(
		                                 "{\"no data\":[{\"name\":\"Long Brady K\",\"share\":269036,\"change\":-14236,\"filingDate\":\"2021-03-03\",\"transactionDate\":\"2021-03-02\",\"transactionCode\":\"F\",\"transactionPrice\":3.68},{\"name\":\"Adamson Keelan\",\"share\":221083,\"change\":-11347,\"filingDate\" : \"2021-03-03\",\"transactionDate\" : \"2021-03-02\",\"transactionCode\" : \"F\",\"transactionPrice\" : 3.68 }] , \"symbol\" : \"RIG\"}"));
	// 缺乏 Symbol项
	FinnhubWebData finnhubWebData133(3, _T("AAPL"), _T(
		                                 "{\"data\":[{\"name\":\"Long Brady K\",\"share\":269036,\"change\":-14236,\"filingDate\":\"2021-03-03\",\"transactionDate\":\"2021-03-02\",\"transactionCode\":\"F\",\"transactionPrice\":3.68},{\"name\":\"Adamson Keelan\",\"share\":221083,\"change\":-11347,\"filingDate\" : \"2021-03-03\",\"transactionDate\" : \"2021-03-02\",\"transactionCode\" : \"F\",\"transactionPrice\" : 3.68 }] , \"no symbol\" : \"RIG\"}"));

	class ProcessFinnhubInsiderTransactionTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			EXPECT_EQ(m_pStock->GetInsiderTransactionUpdateDate(), 19800101);
			m_pStock->SetInsiderTransactionNeedSave(false);
			EXPECT_FALSE(m_pStock->IsUpdateProfileDB());
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePropertyTree();
			m_pWebData->SetJSonContentType(true);
			m_finnhubCompanyInsiderTransaction.SetMarket(gl_pWorldMarket.get());
			long lIndex = gl_pWorldMarket->GetStockIndex(pData->m_strSymbol);
			m_finnhubCompanyInsiderTransaction.SetIndex(lIndex);
		}

		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateProfileDB(false);
			m_pStock->SetInsiderTransactionNeedSave(false);
			m_pStock->SetInsiderTransactionUpdateDate(19800101);

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubCompanyInsiderTransaction m_finnhubCompanyInsiderTransaction;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubInsiderTransaction1, ProcessFinnhubInsiderTransactionTest,
	                         testing::Values(&finnhubWebData131, &finnhubWebData132, &finnhubWebData133));

	TEST_P(ProcessFinnhubInsiderTransactionTest, TestProsessFinnhubInsiderTransaction0) {
		m_finnhubCompanyInsiderTransaction.ParseAndStoreWebData(m_pWebData);
		EXPECT_FALSE(m_pStock->IsInsiderTransactionNeedUpdate());
		switch (m_lIndex) {
		case 1: // 正确
			EXPECT_TRUE(m_pStock->IsInsiderTransactionNeedSave());
			EXPECT_NE(m_pStock->GetInsiderTransactionUpdateDate(), 19700101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			break;
		case 2:
			EXPECT_NE(m_pStock->GetInsiderTransactionUpdateDate(), 19700101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_FALSE(m_pStock->IsInsiderTransactionNeedSave());
			break;
		case 3:
			EXPECT_NE(m_pStock->GetInsiderTransactionUpdateDate(), 19700101) << "已更改为当前市场日期";
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());
			EXPECT_FALSE(m_pStock->IsInsiderTransactionNeedSave());
			break;
		default:
			break;
		}
	}

	class ParseFinnhubInsiderTransactionTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePropertyTree();
			m_pWebData->SetJSonContentType(true);
			m_pvInsiderTransaction = nullptr;
		}

		virtual void TearDown(void) override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();

			GeneralCheck();
			m_pStock->SetUpdateProfileDB(false);
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CInsiderTransactionVectorPtr m_pvInsiderTransaction;
		CProductFinnhubCompanyInsiderTransaction m_finnhubCompanyInsiderTransaction;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubInsiderTransaction1, ParseFinnhubInsiderTransactionTest,
	                         testing::Values(&finnhubWebData131, &finnhubWebData132, &finnhubWebData133));

	TEST_P(ParseFinnhubInsiderTransactionTest, TestParseFinnhubInsiderTransaction0) {
		m_pvInsiderTransaction = m_finnhubCompanyInsiderTransaction.ParseFinnhubStockInsiderTransaction(m_pWebData);
		switch (m_lIndex) {
		case 1: // 正确
			EXPECT_EQ(m_pvInsiderTransaction->size(), 2);
			EXPECT_STREQ(m_pvInsiderTransaction->at(1)->m_strPersonName, _T("Long Brady K")) << "数据按日期排列，此第一条排到了第二位";
			EXPECT_STREQ(m_pvInsiderTransaction->at(1)->m_strSymbol, _T("RIG"));
			EXPECT_EQ(m_pvInsiderTransaction->at(1)->m_lShare, 269036);
			EXPECT_EQ(m_pvInsiderTransaction->at(1)->m_lChange, -14236);
			EXPECT_EQ(m_pvInsiderTransaction->at(1)->m_lFilingDate, 20210303);
			EXPECT_EQ(m_pvInsiderTransaction->at(1)->m_lTransactionDate, 20210303) << "数据按日期排列，此第一条排到了第二位";
			EXPECT_DOUBLE_EQ(m_pvInsiderTransaction->at(1)->m_dTransactionPrice, 3.68);
			EXPECT_TRUE(m_pvInsiderTransaction->at(1)->m_lTransactionDate <= m_pvInsiderTransaction->at(1)->m_lTransactionDate) << "此序列按交易日期顺序排列";
			break;
		case 2: // 缺乏data项
			EXPECT_EQ(m_pvInsiderTransaction->size(), 0);
			break;
		case 3: // 缺乏Symbol
			EXPECT_EQ(m_pvInsiderTransaction->size(), 0);
			break;
		default:
			break;
		}
	}
}
