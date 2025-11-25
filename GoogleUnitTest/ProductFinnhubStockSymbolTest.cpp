#include"pch.h"

#include"GeneralCheck.h"
#include"WorldMarket.h"

#include"ProductFinnhubStockSymbol.h"
#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubCompanySymbolProductTest : public Test {
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
		CProductFinnhubStockSymbol companySymbolProduct;
	};

	TEST_F(CFinnhubCompanySymbolProductTest, TestInitialize) {
		EXPECT_EQ(companySymbolProduct.GetIndex(), 0);
		EXPECT_EQ(companySymbolProduct.GetInquiryFunction(), "https://finnhub.io/api/v1/stock/symbol?exchange=");
	}

	TEST_F(CFinnhubCompanySymbolProductTest, TestCreatMessage) {
		companySymbolProduct.SetMarket(gl_pWorldMarket);
		companySymbolProduct.SetIndex(1);
		EXPECT_EQ(companySymbolProduct.CreateMessage(), (companySymbolProduct.GetInquiryFunction() + gl_dataContainerStockExchange.GetItemExchangeCode(1)));
	}

	TEST_F(CFinnhubCompanySymbolProductTest, TestIsNeedAddExchangeCode) {
		EXPECT_FALSE(companySymbolProduct.IsBadStockSymbol("abcdefg", "US"));
		EXPECT_FALSE(companySymbolProduct.IsBadStockSymbol("abcdefg.L", "US"));
		EXPECT_TRUE(companySymbolProduct.IsBadStockSymbol("abc", "LLLL"));
		EXPECT_FALSE(companySymbolProduct.IsBadStockSymbol("abcdefg.L", "L"));
		EXPECT_TRUE(companySymbolProduct.IsBadStockSymbol("abcdefg.ABC", "L"));
		EXPECT_TRUE(companySymbolProduct.IsBadStockSymbol("abcdefg", "L"));
		EXPECT_TRUE(companySymbolProduct.IsBadStockSymbol("abcdefg.", "L"));
	}

	// 格式不对(缺开始的‘{’），无法顺利Parser
	Test_FinnhubWebData finnhubWebData22(2, "AAPL", "[\"currency\":\"USD\",\"description\":\"CERECOR INC\",\"displaySymbol\":\"CERC\",\"figi\":\"BBG001QHW0Y8\",\"isin\":null,\"mic\":\"XNAS\",\"shareClassFIGI\":\"\",\"symbol\":\"CERC\",\"symbol2\":\"\",\"type\":\"Common Stock\"},{\"currency\":\"USD\",\"description\":\"ACTIVE ENERGY GROUP PLC\",\"displaySymbol\":\"ATGVF\",\"figi\":\"BBG009LQZG05\",\"isin\":null,\"mic\":\"OOTC\",\"shareClassFIGI\":\"\",\"symbol\":\"ATGVF\",\"symbol2\":\"\",\"type\":\"Common Stock\"}]");
	// 数据缺乏currency项
	Test_FinnhubWebData finnhubWebData23(3, "AAPL", "[{\"cuency\":\"USD\",\"description\":\"CERECOR INC\",\"displaySymbol\":\"CERC\",\"figi\":\"BBG001QHW0Y8\",\"isin\":null,\"mic\":\"XNAS\",\"shareClassFIGI\":\"\",\"symbol\":\"CERC\",\"symbol2\":\"\",\"type\":\"Common Stock\"},{\"currency\":\"USD\",\"description\":\"ACTIVE ENERGY GROUP PLC\",\"displaySymbol\":\"ATGVF\",\"figi\":\"BBG009LQZG05\",\"isin\":null,\"mic\":\"OOTC\",\"shareClassFIGI\":\"\",\"symbol\":\"ATGVF\",\"symbol2\":\"\",\"type\":\"Common Stock\"}]");
	// 正确的数据
	Test_FinnhubWebData finnhubWebData30(10, "AAPL", "[{\"currency\":\"USD\",\"description\":\"CERECOR INC\",\"displaySymbol\":\"A\",\"figi\":\"BBG001QHW0Y8\",\"isin\":\"not null\",\"mic\":\"XNAS\",\"shareClassFIGI\":\"\",\"symbol\":\"A\",\"symbol2\":\"\",\"type\":\"Common Stock\"},{\"currency\":\"USD\",\"description\":\"ACTIVE ENERGY GROUP PLC\",\"displaySymbol\":\"AA\",\"figi\":\"BBG009LQZG05\",\"isin\":null,\"mic\":\"OOTC\",\"shareClassFIGI\":\"\",\"symbol\":\"New Symbol\",\"symbol2\":\"\",\"type\":\"Common Stock\"}]");

	class ParseFinnhubStockSymbolTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_dataContainerFinnhubStock.GetItem(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pStock->SetCurrency("");
			m_pWebData = pData->m_pData;
			m_finnhubStockSymbolProduct.__Test_checkAccessRight(m_pWebData);
			m_finnhubStockSymbolProduct.SetInquiringExchange("US");

			m_pvStock = nullptr;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateCompanyProfile(true);
			m_pStock->SetUpdateProfileDB(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CFinnhubStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CFinnhubStocksPtr m_pvStock;
		CProductFinnhubStockSymbol m_finnhubStockSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockSymbol1, ParseFinnhubStockSymbolTest, testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData22, &finnhubWebData23,
		                         &finnhubWebData30));

	TEST_P(ParseFinnhubStockSymbolTest, TestParseFinnhubStockSymbol0) {
		m_pvStock = m_finnhubStockSymbolProduct.ParseFinnhubStockSymbol(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_EQ(m_pvStock->size(), 0);
			break;
		case 1: // 无权利访问的数据
			EXPECT_EQ(m_pvStock->size(), 0);
			break;
		case 2: // 格式不对
			EXPECT_EQ(m_pvStock->size(), 0);
			break;
		case 3: // 缺乏currency项
			EXPECT_EQ(m_pvStock->size(), 0);
			break;
		case 10:
			EXPECT_EQ(m_pvStock->at(0)->GetSymbol(), "A");
			EXPECT_EQ(m_pvStock->at(0)->GetIsin(), "not null") << "此时内容不为空，需要双引号";
			EXPECT_EQ(m_pvStock->at(1)->GetSymbol(), "New Symbol");
			EXPECT_EQ(m_pvStock->at(1)->GetIsin(), " ") << "当内容为空（null）时，使用默认值“ ”";
			EXPECT_EQ(m_pvStock->at(0)->GetExchangeCode(), "US");
			EXPECT_EQ(m_pvStock->size(), 2);
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubStockSymbolTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubStockSymbolProduct.__Test_checkAccessRight(m_pWebData);

			m_finnhubStockSymbolProduct.SetMarket(gl_pWorldMarket);
			m_finnhubStockSymbolProduct.SetIndex(0); // 第一个交易所（AS)
			m_finnhubStockSymbolProduct.SetInquiringExchange(gl_dataContainerStockExchange.GetItemExchangeCode(0));
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
		CProductFinnhubStockSymbol m_finnhubStockSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubStockSymbol1, ProcessFinnhubStockSymbolTest, testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData22, &finnhubWebData23,
		                         &finnhubWebData30));

	TEST_P(ProcessFinnhubStockSymbolTest, TestParseFinnhubStockSymbol0) {
		CFinnhubStockPtr pStock, pStock2;
		m_finnhubStockSymbolProduct.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			break;
		case 1: // 无权利访问的数据
			break;
		case 2: // 格式不对
			break;
		case 3: // 缺乏currency项
			break;
		case 10:
			EXPECT_TRUE(gl_dataContainerFinnhubStock.IsSymbol("New Symbol")) << "新增加的代码";
			pStock = gl_dataContainerFinnhubStock.GetItem("New Symbol");
			EXPECT_EQ(pStock->GetExchangeCode(), "AD") << "测试数据库的第一个交易所";
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();

		// 恢复原状
			gl_dataContainerFinnhubStock.Delete(pStock);
			break;
		default:
			break;
		}
	}
}
