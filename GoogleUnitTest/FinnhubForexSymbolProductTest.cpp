#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubForexSymbolProduct.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubForexSymbolProductTest : public ::testing::Test
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
		CFinnhubForexSymbolProduct forexSymbolProduct;
	};

	TEST_F(CFinnhubForexSymbolProductTest, TestInitialize) {
		EXPECT_EQ(forexSymbolProduct.GetIndex(), -1);
		EXPECT_STREQ(forexSymbolProduct.GetInquiringStr(), _T("https://finnhub.io/api/v1/forex/symbol?exchange="));
	}

	TEST_F(CFinnhubForexSymbolProductTest, TestCreatMessage) {
		forexSymbolProduct.SetMarket(gl_pWorldMarket.get());
		forexSymbolProduct.SetIndex(1);
		EXPECT_STREQ(forexSymbolProduct.CreatMessage(), forexSymbolProduct.GetInquiringStr() + gl_pWorldMarket->GetForexExchange(1));
	}

	TEST_F(CFinnhubForexSymbolProductTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	// 格式不对(缺开始的‘[’），无法顺利Parser
	FinnhubWebData finnhubWebData82(2, _T(""), _T("[\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// 数据缺乏description
	FinnhubWebData finnhubWebData83(3, _T(""), _T("[{\"a\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// 数据缺乏displaySymbol
	FinnhubWebData finnhubWebData84(4, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"a\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// 数据缺乏symbol
	FinnhubWebData finnhubWebData85(5, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"a\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// 正确的数据
	FinnhubWebData finnhubWebData90(10, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));

	class ParseFinnhubForexSymbolTest : public::testing::TestWithParam<FinnhubWebData*>
	{
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pvForexSymbol = nullptr;
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CForexSymbolVectorPtr m_pvForexSymbol;
		CFinnhubForexSymbolProduct m_finnhubForexSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubForexSymbol1, ParseFinnhubForexSymbolTest,
		testing::Values(&finnhubWebData82, &finnhubWebData83, &finnhubWebData84,
			&finnhubWebData85, &finnhubWebData90));

	TEST_P(ParseFinnhubForexSymbolTest, TestParseFinnhubForexSymbol0) {
		m_pvForexSymbol = m_finnhubForexSymbolProduct.ParseFinnhubForexSymbol(m_pWebData);
		switch (m_lIndex) {
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
			EXPECT_STREQ(m_pvForexSymbol->at(0)->GetSymbol(), _T("OANDA:SG30_SGD"));
			EXPECT_STREQ(m_pvForexSymbol->at(1)->GetSymbol(), _T("OANDA:DE10YB_EUR"));
			EXPECT_EQ(m_pvForexSymbol->size(), 2);
			break;
		default:
			break;
		}
	}
}