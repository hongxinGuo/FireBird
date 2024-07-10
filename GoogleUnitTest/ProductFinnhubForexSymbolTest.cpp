#include"pch.h"

#include"GeneralCheck.h"

#include"ProductFinnhubForexSymbol.h"
#include "WorldMarket.h"

using namespace testing;

namespace FireBirdTest {
	class CProductFinnhubForexSymbolTest : public Test {
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
		CProductFinnhubForexSymbol productForexSymbol;
	};

	TEST_F(CProductFinnhubForexSymbolTest, TestInitialize) {
		EXPECT_EQ(productForexSymbol.GetIndex(), -1);
		EXPECT_STREQ(productForexSymbol.GetInquiryFunction(), _T("https://finnhub.io/api/v1/forex/symbol?exchange="));
	}

	TEST_F(CProductFinnhubForexSymbolTest, TestCreatMessage) {
		productForexSymbol.SetMarket(gl_pWorldMarket);
		productForexSymbol.SetIndex(1);
		EXPECT_STREQ(productForexSymbol.CreateMessage(), productForexSymbol.GetInquiryFunction() + gl_dataContainerFinnhubForexExchange.GetExchange(1));
	}

	TEST_F(CProductFinnhubForexSymbolTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	// 格式不对(缺开始的‘[’），无法顺利Parser
	Test_FinnhubWebData finnhubWebData82(2, _T(""), _T("[\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// 数据缺乏description
	Test_FinnhubWebData finnhubWebData83(3, _T(""), _T("[{\"a\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// 数据缺乏displaySymbol
	Test_FinnhubWebData finnhubWebData84(4, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"a\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// 数据缺乏symbol
	Test_FinnhubWebData finnhubWebData85(5, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"a\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// 正确的数据
	Test_FinnhubWebData finnhubWebData90(10, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"New Symbol\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));

	class ParseFinnhubForexSymbolTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_productFinnhubForexSymbol.CheckAccessRight(m_pWebData);

			m_pvForexSymbol = nullptr;
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
		CForexSymbolsPtr m_pvForexSymbol;
		CProductFinnhubForexSymbol m_productFinnhubForexSymbol;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubForexSymbol1, ParseFinnhubForexSymbolTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData82, &finnhubWebData83, &finnhubWebData84,
		                         &finnhubWebData85, &finnhubWebData90));

	TEST_P(ParseFinnhubForexSymbolTest, TestParseFinnhubForexSymbol0) {
		m_pvForexSymbol = m_productFinnhubForexSymbol.ParseFinnhubForexSymbol(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_EQ(m_pvForexSymbol->size(), 0);
			break;
		case 1: // 无权利访问的数据
			EXPECT_EQ(m_pvForexSymbol->size(), 0);
			break;
		case 2: // 格式不对
			EXPECT_EQ(m_pvForexSymbol->size(), 0);
			break;
		case 3: // 缺乏字符串
			EXPECT_EQ(m_pvForexSymbol->size(), 0);
			break;
		case 4: // 缺乏字符串
			EXPECT_EQ(m_pvForexSymbol->size(), 0);
			break;
		case 5: // 缺乏字符串
			EXPECT_EQ(m_pvForexSymbol->size(), 0);
			break;
		case 10:
			EXPECT_STREQ(m_pvForexSymbol->at(0)->GetSymbol(), _T("New Symbol"));
			EXPECT_STREQ(m_pvForexSymbol->at(1)->GetSymbol(), _T("OANDA:DE10YB_EUR"));
			EXPECT_EQ(m_pvForexSymbol->size(), 2);
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubForexSymbolTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_productFinnhubForexSymbol.CheckAccessRight(m_pWebData);

			m_productFinnhubForexSymbol.SetMarket(gl_pWorldMarket);
			m_productFinnhubForexSymbol.SetIndex(0);
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
		CProductFinnhubForexSymbol m_productFinnhubForexSymbol;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubForexSymbol1, ProcessFinnhubForexSymbolTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData82, &finnhubWebData83, &finnhubWebData84,
		                         &finnhubWebData85, &finnhubWebData90));

	TEST_P(ProcessFinnhubForexSymbolTest, TestParseFinnhubForexSymbol0) {
		CForexSymbolPtr pForexSymbol;
		m_productFinnhubForexSymbol.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			break;
		case 1: // 无权利访问的数据
			break;
		case 2: // 格式不对
			break;
		case 3: // 缺乏字符串
			break;
		case 4: // 缺乏字符串
			break;
		case 5: // 缺乏字符串
			break;
		case 10:
			EXPECT_TRUE(gl_dataFinnhubForexSymbol.IsSymbol(_T("New Symbol"))) << "新添加的Forex代码";
			pForexSymbol = gl_dataFinnhubForexSymbol.GetSymbol(_T("New Symbol"));
			EXPECT_STREQ(pForexSymbol->GetExchangeCode(), _T("oanda")) << "Index为零时的交易所";

		// 恢复原状
			gl_dataFinnhubForexSymbol.Delete(pForexSymbol);
			break;
		default:
			break;
		}
	}
}
