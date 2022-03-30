#include"pch.h"

#include"globedef.h"
#include"SystemMessage.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductTiingoCryptoSymbol.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CProductTiingoCryptoSymbolsTest : public ::testing::Test {
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
		CProductTiingoCryptoSymbol stockSymbols;
	};

	TEST_F(CProductTiingoCryptoSymbolsTest, TestInitialize) {
		EXPECT_EQ(stockSymbols.GetIndex(), -1);
		EXPECT_STREQ(stockSymbols.GetInquiringStr(), _T("https://api.tiingo.com/tiingo/crypto?"));
	}

	TEST_F(CProductTiingoCryptoSymbolsTest, TestCreatMessage) {
		EXPECT_STREQ(stockSymbols.CreatMessage(), stockSymbols.GetInquiringStr());
	}

	// 格式不对，缺乏'{'
	TiingoWebData tiingoWebData11(11, _T(""), _T("[\"ticker\":\"curebtc\",\"baseCurrency\":\"cure\",\"name\":\"CureCoin(CURE/BTC)\",\"quoteCurrency\":\"btc\"}]"));
	// 第一个数据缺项
	TiingoWebData tiingoWebData12(12, _T(""), _T("[{\"Missing\":\"curebtc\",\"baseCurrency\":\"cure\",\"name\":\"CureCoin(CURE/BTC)\",\"quoteCurrency\":\"btc\"}]"));
	// 正确的数据
	TiingoWebData tiingoWebData20(20, _T(""), _T("[{\"ticker\":\"New Symbol\",\"baseCurrency\":\"cure\",\"name\":\"CureCoin(CURE/BTC)\",\"quoteCurrency\":\"btc\"}]"));

	class ParseTiingoCryptoTest : public::testing::TestWithParam<TiingoWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePTree();
			m_pWebData->SetJSonContentType(true);
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CTiingoCryptoVectorPtr m_pvCrypto;
		CProductTiingoCryptoSymbol m_tiingoCryptoSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseTiingoCrypto1,
													 ParseTiingoCryptoTest,
													 testing::Values(&tiingoWebData11, &tiingoWebData12, &tiingoWebData20));

	TEST_P(ParseTiingoCryptoTest, TestParseCryptoSymbol) {
		m_pvCrypto = m_tiingoCryptoSymbolProduct.ParseTiingoCryptoSymbol(m_pWebData);
		switch (m_lIndex) {
		case 11: // 格式不对
			EXPECT_EQ(m_pvCrypto->size(), 0);
			break;
		case 12: // 格式不对
			EXPECT_EQ(m_pvCrypto->size(), 0);
			break;
		case 20:
			EXPECT_EQ(m_pvCrypto->size(), 1);
			EXPECT_STREQ(m_pvCrypto->at(0)->m_strBaseCurrency, _T("cure"));
			EXPECT_STREQ(m_pvCrypto->at(0)->m_strTicker, _T("New Symbol"));
			EXPECT_STREQ(m_pvCrypto->at(0)->m_strName, _T("CureCoin(CURE/BTC)"));
			EXPECT_STREQ(m_pvCrypto->at(0)->m_strDescription, _T("")) << "此项已废弃。为了兼容才没有删除";
			EXPECT_STREQ(m_pvCrypto->at(0)->m_strQuoteCurrency, _T("btc"));
			break;
		}
	}

	class ProcessTiingoCryptoTest : public::testing::TestWithParam<TiingoWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePTree();
			m_pWebData->SetJSonContentType(true);
			m_tiingoCryptoSymbolProduct.SetMarket(gl_pWorldMarket.get());
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductTiingoCryptoSymbol m_tiingoCryptoSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessTiingoCrypto1,
													 ProcessTiingoCryptoTest,
													 testing::Values(&tiingoWebData11, &tiingoWebData12, &tiingoWebData20));

	TEST_P(ProcessTiingoCryptoTest, TestProcessCryptoSymbol) {
		CTiingoCryptoSymbolPtr pCrypto = nullptr;
		long l = gl_pWorldMarket->GetTiingoCryptoSymbolSize();
		bool fSucceed = m_tiingoCryptoSymbolProduct.ProcessWebData(m_pWebData);
		switch (m_lIndex) {
		case 11: // 格式不对
			EXPECT_TRUE(fSucceed);
			break;
		case 12: // 格式不对
			EXPECT_TRUE(fSucceed);
			break;
		case 20:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(gl_pWorldMarket->GetTiingoCryptoSymbolSize(), l + 1);
			EXPECT_TRUE(gl_pWorldMarket->IsTiingoCryptoSymbol(_T("New Symbol")));
			EXPECT_EQ(gl_systemMessage.GetInnerSystemInformationDequeSize(), 1) << gl_systemMessage.PopInnerSystemInformationMessage();

			gl_systemMessage.PopInnerSystemInformationMessage();
			pCrypto = gl_pWorldMarket->GetTiingoCryptoSymbol(_T("New Symbol"));
			gl_pWorldMarket->DeleteTiingoCryptoSymbol(pCrypto);
			break;
		}
		EXPECT_TRUE(gl_pWorldMarket->IsTiingoCryptoSymbolUpdated());

		gl_pWorldMarket->SetTiingoCryptoSymbolUpdated(false);
	}
}