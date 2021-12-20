#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubCompanyInsiderTransaction.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubCompanyInsiderTransactionTest : public ::testing::Test
	{
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
		CFinnhubCompanyInsiderTransaction companyInsiderTransaction;
	};

	TEST_F(CFinnhubCompanyInsiderTransactionTest, TestInitialize) {
		EXPECT_EQ(companyInsiderTransaction.GetIndex(), -1);
		EXPECT_STREQ(companyInsiderTransaction.GetInquiringStr(), _T("https://finnhub.io/api/v1/stock/insider-transactions?symbol="));
	}

	TEST_F(CFinnhubCompanyInsiderTransactionTest, TestCreatMessage) {
		gl_pWorldMarket->GetStock(1)->SetInsiderTransactionNeedUpdate(true);
		companyInsiderTransaction.SetMarket(gl_pWorldMarket.get());
		companyInsiderTransaction.SetIndex(1);
		EXPECT_STREQ(companyInsiderTransaction.CreatMessage(), companyInsiderTransaction.GetInquiringStr() + gl_pWorldMarket->GetStock(1)->GetSymbol());
		EXPECT_FALSE(gl_pWorldMarket->GetStock(1)->IsInsiderTransactionNeedUpdate());

		gl_pWorldMarket->GetStock(1)->SetInsiderTransactionNeedUpdate(true);
	}

	TEST_F(CFinnhubCompanyInsiderTransactionTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	FinnhubWebData finnhubWebData132(2, _T("AAPL"), _T("{\"data\":[{\"name\":\"Long Brady K\",\"share\":269036,\"change\":-14236,\"filingDate\":\"2021-03-03\",\"transactionDate\":\"2021-03-02\",\"transactionCode\":\"F\",\"transactionPrice\":3.68},{\"name\":\"Adamson Keelan\",\"share\":221083,\"change\":-11347,\"filingDate\" : \"2021-03-03\",\"transactionDate\" : \"2021-03-02\",\"transactionCode\" : \"F\",\"transactionPrice\" : 3.68 }] , \"symbol\" : \"RIG\"}"));

	class ParseFinnhubInsiderTransactionTest : public::testing::TestWithParam<FinnhubWebData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pWebData = pData->m_pData;
			m_pvInsiderTransaction = nullptr;
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();

			GeneralCheck();
			m_pStock->SetUpdateProfileDB(false);
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CInsiderTransactionVectorPtr m_pvInsiderTransaction;
		CFinnhubCompanyInsiderTransaction m_finnhubCompanyInsiderTransaction;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubInsiderTransaction1, ParseFinnhubInsiderTransactionTest,
		testing::Values(&finnhubWebData132));

	TEST_P(ParseFinnhubInsiderTransactionTest, TestParseFinnhubInsiderTransaction0) {
		m_pvInsiderTransaction = m_finnhubCompanyInsiderTransaction.ParseFinnhubStockInsiderTransaction(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_EQ(m_pvInsiderTransaction->size(), 2);
			EXPECT_STREQ(m_pvInsiderTransaction->at(0)->m_strPersonName, _T("Long Brady K"));
			EXPECT_STREQ(m_pvInsiderTransaction->at(0)->m_strSymbol, _T("RIG"));
			EXPECT_EQ(m_pvInsiderTransaction->at(0)->m_lShare, 269036);
			EXPECT_EQ(m_pvInsiderTransaction->at(0)->m_lChange, -14236);
			EXPECT_EQ(m_pvInsiderTransaction->at(0)->m_lFilingDate, 20210303);
			EXPECT_EQ(m_pvInsiderTransaction->at(0)->m_lTransactionDate, 20210302);
			EXPECT_DOUBLE_EQ(m_pvInsiderTransaction->at(0)->m_dTransactionPrice, 3.68);
			EXPECT_TRUE(m_pvInsiderTransaction->at(0)->m_lTransactionDate <= m_pvInsiderTransaction->at(1)->m_lTransactionDate) << "此序列按交易日期顺序排列";
			break;
		default:
			break;
		}
	}
}