#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"
#include"TiingoDataSource.h"

#include"ProductTiingoCryptoSymbol.h"

using namespace testing;

namespace FireBirdTest {
	class CProductTiingoCryptoSymbolsTest : public Test {
	protected:
		static void SetUpTestSuite() {
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		void SetUp() override {
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CProductTiingoCryptoSymbol stockSymbols;
	};

	TEST_F(CProductTiingoCryptoSymbolsTest, TestInitialize) {
		EXPECT_EQ(stockSymbols.GetIndex(), -1);
		EXPECT_STREQ(stockSymbols.GetInquiryFunction(), _T("https://api.tiingo.com/tiingo/crypto?"));
	}

	TEST_F(CProductTiingoCryptoSymbolsTest, TestCreatMessage) {
		EXPECT_STREQ(stockSymbols.CreateMessage(), stockSymbols.GetInquiryFunction());
	}

	// 格式不对，缺乏'{'
	TiingoWebData tiingoWebData11(11, _T(""), _T("[\"ticker\":\"curebtc\",\"baseCurrency\":\"cure\",\"name\":\"CureCoin(CURE/BTC)\",\"quoteCurrency\":\"btc\"}]"));
	// 第一个数据缺项
	TiingoWebData tiingoWebData12(12, _T(""), _T("[{\"Missing\":\"curebtc\",\"baseCurrency\":\"cure\",\"name\":\"CureCoin(CURE/BTC)\",\"quoteCurrency\":\"btc\"}]"));
	// 正确的数据
	TiingoWebData tiingoWebData20(20, _T(""), _T("[{\"ticker\":\"New Symbol\",\"baseCurrency\":\"cure\",\"name\":\"CureCoin(CURE/BTC)\",\"quoteCurrency\":\"btc\"}]"));

	class ParseTiingoCryptoTest : public TestWithParam<TiingoWebData*> {
	protected:
		void SetUp() override {
			GeneralCheck();
			const TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreateJson();
			m_pWebData->SetJSonContentType(true);
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
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
		default:
			break;
		}
	}

	class ParseTiingoCryptoTest2 : public TestWithParam<TiingoWebData*> {
	protected:
		void SetUp() override {
			GeneralCheck();
			const TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreateJson();
			m_pWebData->SetJSonContentType(true);
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CTiingoCryptoVectorPtr m_pvCrypto;
		CProductTiingoCryptoSymbol m_tiingoCryptoSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseTiingoCrypto1,
	                         ParseTiingoCryptoTest2,
	                         testing::Values(&tiingoWebData11, &tiingoWebData12, &tiingoWebData20));

	TEST_P(ParseTiingoCryptoTest2, TestParseCryptoSymbol) {
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
		default:
			break;
		}
	}

	class ProcessTiingoCryptoTest : public TestWithParam<TiingoWebData*> {
	protected:
		void SetUp() override {
			GeneralCheck();
			const TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreateJson();
			m_pWebData->SetJSonContentType(true);
			m_tiingoCryptoSymbolProduct.SetMarket(gl_pWorldMarket.get());
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
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
		CTiingoCryptoSymbolPtr pCrypto;
		const auto l = gl_pWorldMarket->GetTiingoCryptoSymbolSize();
		const bool fSucceed = m_tiingoCryptoSymbolProduct.ParseAndStoreWebData(m_pWebData);
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
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1) << gl_systemMessage.PopInnerSystemInformationMessage();

			gl_systemMessage.PopInnerSystemInformationMessage();
			pCrypto = gl_pWorldMarket->GetTiingoCryptoSymbol(_T("New Symbol"));
			gl_pWorldMarket->DeleteTiingoCryptoSymbol(pCrypto);
			break;
		default:
			break;
		}
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateCryptoSymbol()) << "此标识由UpdateStatus函数更新";

		gl_pTiingoDataSource->SetUpdateCryptoSymbol(true);
	}
}
