#include"pch.h"

#include"GeneralCheck.h"

#include"ProductFinnhubStockSymbol.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubCompanySymbolProductTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CProductFinnhubStockSymbol companySymbolProduct;
	};

	TEST_F(CFinnhubCompanySymbolProductTest, TestInitialize) {
		EXPECT_EQ(companySymbolProduct.GetIndex(), -1);
		EXPECT_STREQ(companySymbolProduct.GetInquiryFunction(), _T("https://finnhub.io/api/v1/stock/symbol?exchange="));
	}

	TEST_F(CFinnhubCompanySymbolProductTest, TestCreatMessage) {
		companySymbolProduct.SetMarket(gl_pWorldMarket.get());
		companySymbolProduct.SetIndex(1);
		EXPECT_STREQ(companySymbolProduct.CreateMessage(), companySymbolProduct.GetInquiryFunction() + gl_pWorldMarket->GetStockExchangeCode(1));
	}

	TEST_F(CFinnhubCompanySymbolProductTest, TestIsNeedAddExchangeCode) {
		EXPECT_TRUE(companySymbolProduct.IsNeedAddExchangeCode(_T("abcdefg.L"), _T("L")));
		EXPECT_TRUE(companySymbolProduct.IsNeedAddExchangeCode(_T("abcdefg.L"), _T("l")));
		EXPECT_TRUE(companySymbolProduct.IsNeedAddExchangeCode(_T("abcdefg.l"), _T("L")));
		EXPECT_TRUE(companySymbolProduct.IsNeedAddExchangeCode(_T("abcdefg.abc"), _T("Abc")));
		EXPECT_FALSE(companySymbolProduct.IsNeedAddExchangeCode(_T("abcdefg.ABC"), _T("L")));
		EXPECT_FALSE(companySymbolProduct.IsNeedAddExchangeCode(_T("abcdefg"), _T("L")));
		EXPECT_FALSE(companySymbolProduct.IsNeedAddExchangeCode(_T("abcdefg."), _T("L")));
	}

	// 格式不对(缺开始的‘{’），无法顺利Parser
	FinnhubWebData finnhubWebData22(2, _T("AAPL"), _T("[\"currency\":\"USD\",\"description\":\"CERECOR INC\",\"displaySymbol\":\"CERC\",\"figi\":\"BBG001QHW0Y8\",\"isin\":null,\"mic\":\"XNAS\",\"shareClassFIGI\":\"\",\"symbol\":\"CERC\",\"symbol2\":\"\",\"type\":\"Common Stock\"},{\"currency\":\"USD\",\"description\":\"ACTIVE ENERGY GROUP PLC\",\"displaySymbol\":\"ATGVF\",\"figi\":\"BBG009LQZG05\",\"isin\":null,\"mic\":\"OOTC\",\"shareClassFIGI\":\"\",\"symbol\":\"ATGVF\",\"symbol2\":\"\",\"type\":\"Common Stock\"}]"));
	// 数据缺乏currency项
	FinnhubWebData finnhubWebData23(3, _T("AAPL"), _T("[{\"cuency\":\"USD\",\"description\":\"CERECOR INC\",\"displaySymbol\":\"CERC\",\"figi\":\"BBG001QHW0Y8\",\"isin\":null,\"mic\":\"XNAS\",\"shareClassFIGI\":\"\",\"symbol\":\"CERC\",\"symbol2\":\"\",\"type\":\"Common Stock\"},{\"currency\":\"USD\",\"description\":\"ACTIVE ENERGY GROUP PLC\",\"displaySymbol\":\"ATGVF\",\"figi\":\"BBG009LQZG05\",\"isin\":null,\"mic\":\"OOTC\",\"shareClassFIGI\":\"\",\"symbol\":\"ATGVF\",\"symbol2\":\"\",\"type\":\"Common Stock\"}]"));
	// 正确的数据
	FinnhubWebData finnhubWebData30(10, _T("AAPL"), _T("[{\"currency\":\"USD\",\"description\":\"CERECOR INC\",\"displaySymbol\":\"A\",\"figi\":\"BBG001QHW0Y8\",\"isin\":\"not null\",\"mic\":\"XNAS\",\"shareClassFIGI\":\"\",\"symbol\":\"A\",\"symbol2\":\"\",\"type\":\"Common Stock\"},{\"currency\":\"USD\",\"description\":\"ACTIVE ENERGY GROUP PLC\",\"displaySymbol\":\"AA\",\"figi\":\"BBG009LQZG05\",\"isin\":null,\"mic\":\"OOTC\",\"shareClassFIGI\":\"\",\"symbol\":\"New Symbol\",\"symbol2\":\"\",\"type\":\"Common Stock\"}]"));

	class ParseFinnhubStockSymbolTest : public TestWithParam<FinnhubWebData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pStock->SetCurrency(_T(""));
			m_pWebData = pData->m_pData;
			m_pWebData->CreateJson();
			m_pWebData->SetJSonContentType(true);
			m_pvStock = nullptr;
		}

		void TearDown(void) override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateCompanyProfile(true);
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

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockSymbol1, ParseFinnhubStockSymbolTest, testing::Values(&finnhubWebData22, &finnhubWebData23,
		                         &finnhubWebData30));

	TEST_P(ParseFinnhubStockSymbolTest, TestParseFinnhubStockSymbol0) {
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
			EXPECT_STREQ(m_pvStock->at(0)->GetIsin(), _T("not null")) << "此时内容不为空，需要双引号";
			EXPECT_STREQ(m_pvStock->at(1)->GetSymbol(), _T("New Symbol"));
			EXPECT_STREQ(m_pvStock->at(1)->GetIsin(), _T(" ")) << "当内容为空（null）时，使用默认值“ ”";
			EXPECT_EQ(m_pvStock->size(), 2);
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubStockSymbolTest : public TestWithParam<FinnhubWebData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreateJson();
			m_pWebData->SetJSonContentType(true);

			m_finnhubStockSymbolProduct.SetMarket(gl_pWorldMarket.get());
			m_finnhubStockSymbolProduct.SetIndex(0); // 第一个交易所（AS)
		}

		void TearDown(void) override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductFinnhubStockSymbol m_finnhubStockSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockSymbol1, ProcessFinnhubStockSymbolTest, testing::Values(&finnhubWebData22, &finnhubWebData23,
		                         &finnhubWebData30));

	TEST_P(ProcessFinnhubStockSymbolTest, TestParseFinnhubStockSymbol0) {
		CWorldStockPtr pStock;
		const bool fSucceed = m_finnhubStockSymbolProduct.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_TRUE(fSucceed);
			break;
		case 3: // 缺乏currency项
			EXPECT_TRUE(fSucceed);
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(gl_pWorldMarket->IsStock(_T("New Symbol"))) << "新增加的代码";
			pStock = gl_pWorldMarket->GetStock(_T("New Symbol"));
			EXPECT_STREQ(pStock->GetExchangeCode(), _T("AS")) << "第一个交易所";
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();

		// 恢复原状
			EXPECT_TRUE(gl_pWorldMarket->DeleteStock(pStock));
			break;
		default:
			break;
		}
	}
}
