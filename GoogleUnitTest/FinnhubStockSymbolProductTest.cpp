#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubStockSymbol.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubCompanySymbolProductTest : public ::testing::Test {
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
		CProductFinnhubStockSymbol companySymbolProduct;
	};

	TEST_F(CFinnhubCompanySymbolProductTest, TestInitialize) {
		EXPECT_EQ(companySymbolProduct.GetIndex(), -1);
		EXPECT_STREQ(companySymbolProduct.GetInquiringStr(), _T("https://finnhub.io/api/v1/stock/symbol?exchange="));
	}

	TEST_F(CFinnhubCompanySymbolProductTest, TestCreatMessage) {
		companySymbolProduct.SetMarket(gl_pWorldMarket.get());
		companySymbolProduct.SetIndex(1);
		EXPECT_STREQ(companySymbolProduct.CreatMessage(), companySymbolProduct.GetInquiringStr() + gl_pWorldMarket->GetExchangeCode(1));
	}

	// 格式不对(缺开始的‘{’），无法顺利Parser
	FinnhubWebData finnhubWebData22(2, _T("AAPL"), _T("[\"currency\":\"USD\",\"description\":\"CERECOR INC\",\"displaySymbol\":\"CERC\",\"figi\":\"BBG001QHW0Y8\",\"mic\":\"XNAS\",\"symbol\":\"CERC\",\"type\":\"Common Stock\"},{\"currency\":\"USD\",\"description\":\"ACTIVE ENERGY GROUP PLC\",\"displaySymbol\":\"ATGVF\",\"figi\":\"BBG009LQZG05\",\"mic\":\"OOTC\",\"symbol\":\"ATGVF\",\"type\":\"Common Stock\"}]"));
	// 数据缺乏currency项
	FinnhubWebData finnhubWebData23(3, _T("AAPL"), _T("[{\"cuency\":\"USD\",\"description\":\"CERECOR INC\",\"displaySymbol\":\"CERC\",\"figi\":\"BBG001QHW0Y8\",\"mic\":\"XNAS\",\"symbol\":\"CERC\",\"type\":\"Common Stock\"},{\"currency\":\"USD\",\"description\":\"ACTIVE ENERGY GROUP PLC\",\"displaySymbol\":\"ATGVF\",\"figi\":\"BBG009LQZG05\",\"mic\":\"OOTC\",\"symbol\":\"ATGVF\",\"type\":\"Common Stock\"}]"));
	// 正确的数据
	FinnhubWebData finnhubWebData30(10, _T("AAPL"), _T("[{\"currency\":\"USD\",\"description\":\"CERECOR INC\",\"displaySymbol\":\"A\",\"figi\":\"BBG001QHW0Y8\",\"mic\":\"XNAS\",\"symbol\":\"A\",\"type\":\"Common Stock\"},{\"currency\":\"USD\",\"description\":\"ACTIVE ENERGY GROUP PLC\",\"displaySymbol\":\"AA\",\"figi\":\"BBG009LQZG05\",\"mic\":\"OOTC\",\"symbol\":\"AA\",\"type\":\"Common Stock\"}]"));

	class ProcessFinnhubStockSymbolTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pStock->SetCurrency(_T(""));
			m_pWebData = pData->m_pData;
			m_pvStock = nullptr;
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetProfileUpdated(false);
			m_pStock->SetUpdateProfileDB(false);

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CWorldStockVectorPtr m_pvStock;
		CProductFinnhubStockSymbol m_finnhubStockSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockSymbol1, ProcessFinnhubStockSymbolTest, testing::Values(&finnhubWebData22, &finnhubWebData23,
																																																				&finnhubWebData30));

	TEST_P(ProcessFinnhubStockSymbolTest, TestParseFinnhubStockSymbol0) {
		m_pvStock = m_finnhubStockSymbolProduct.ParseFinnhubStockSymbol(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_EQ(m_pvStock->size(), 0);
			break;
		case 3: // 缺乏currency项
			EXPECT_EQ(m_pvStock->size(), 0);
			break;
		case 10:
			EXPECT_STREQ(m_pvStock->at(0)->GetSymbol(), _T("A"));
			EXPECT_STREQ(m_pvStock->at(1)->GetSymbol(), _T("AA"));
			EXPECT_EQ(m_pvStock->size(), 2);
			break;
		default:
			break;
		}
	}
}