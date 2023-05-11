#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubCryptoDayLine.h"
#include "TimeConvert.h"

using namespace testing;

namespace FireBirdTest {
	class CProductFinnhubCryptoDayLineTest : public ::testing::Test {
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
		CProductFinnhubCryptoDayLine cryptoDayLine;
	};

	TEST_F(CProductFinnhubCryptoDayLineTest, TestInitialize) {
		EXPECT_EQ(cryptoDayLine.GetIndex(), -1);
		EXPECT_STREQ(cryptoDayLine.GetInquiryFunction(), _T("https://finnhub.io/api/v1/crypto/candle?symbol="));
	}

	TEST_F(CProductFinnhubCryptoDayLineTest, TestCreatMessage) {
		cryptoDayLine.SetMarket(gl_pWorldMarket.get());
		cryptoDayLine.SetIndex(1);
		EXPECT_STREQ(cryptoDayLine.CreateMessage(),
		             cryptoDayLine.GetInquiryFunction() + gl_pWorldMarket->GetFinnhubCryptoSymbol(1)->GetFinnhubDayLineInquiryParam(GetUTCTime()));
	}

	TEST_F(CProductFinnhubCryptoDayLineTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	// 格式不对(缺开始的‘{’），无法顺利Parser
	FinnhubWebData finnhubWebData221(1, _T("BINANCE:USDTUAH"), _T("\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 没有s项
	FinnhubWebData finnhubWebData222_1(11, _T("BINANCE:USDTUAH"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"a\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// s项报告非ok
	FinnhubWebData finnhubWebData222(2, _T("BINANCE:USDTUAH"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"not ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// s项报告no data
	FinnhubWebData finnhubWebData223(3, _T("BINANCE:USDTUAH"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"no_data\",\"a\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 数据缺乏t项
	FinnhubWebData finnhubWebData224(4, _T("BINANCE:USDTUAH"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"a\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 缺乏c项。不影响结果
	FinnhubWebData finnhubWebData225(5, _T("BINANCE:USDTUAH"), _T("{\"a\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 缺乏h项
	FinnhubWebData finnhubWebData226(6, _T("BINANCE:USDTUAH"), _T("{\"c\":[1.10159,1.10784],\"a\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 缺乏l项
	FinnhubWebData finnhubWebData227(7, _T("BINANCE:USDTUAH"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"a\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 缺乏o项
	FinnhubWebData finnhubWebData228(8, _T("BINANCE:USDTUAH"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"a\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 缺乏v项
	FinnhubWebData finnhubWebData229(9, _T("BINANCE:USDTUAH"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"a\":[36521,47505]}"));
	// 正确的数据
	FinnhubWebData finnhubWebData230(10, _T("BINANCE:USDTUAH"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));

	class ParseFinnhubCryptoCandleTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			EXPECT_TRUE(gl_pWorldMarket->IsFinnhubCryptoSymbol(pData->m_strSymbol));
			gl_pWorldMarket->GetFinnhubCryptoSymbol(m_lIndex)->SetIPOStatus(_STOCK_IPOED_);
			m_pWebData = pData->m_pData;
			m_pWebData->CreateJson();
			m_pWebData->SetJSonContentType(true);
			m_pvDayLine = nullptr;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			gl_pWorldMarket->GetFinnhubCryptoSymbol(m_lIndex)->SetIPOStatus(_STOCK_IPOED_);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CDayLineVectorPtr m_pvDayLine;
		CWebDataPtr m_pWebData;
		CProductFinnhubCryptoDayLine m_finnhubCryptoDayLine;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubCryptoCandle1,
	                         ParseFinnhubCryptoCandleTest,
	                         testing::Values(&finnhubWebData221, &finnhubWebData222_1, &finnhubWebData222, &finnhubWebData223, &finnhubWebData224, &finnhubWebData225,
		                         &finnhubWebData226, &finnhubWebData227, &finnhubWebData228, &finnhubWebData229, &finnhubWebData230));

	TEST_P(ParseFinnhubCryptoCandleTest, TestParseFinnhubCryptoCandle0) {
		CString strMessage;

		m_pvDayLine = m_finnhubCryptoDayLine.ParseFinnhubCryptoCandle(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_EQ(m_pvDayLine->size(), 0);
			break;
		case 2: // s项报告not ok
			EXPECT_EQ(m_pvDayLine->size(), 0);
			strMessage = _T("日线返回值不为ok");
			EXPECT_STREQ(gl_systemMessage.PopErrorMessage(), strMessage);
			break;
		case 3: // s项报告 no data
			EXPECT_EQ(m_pvDayLine->size(), 0);
			break;
		case 4: //数据缺乏t项
			EXPECT_EQ(m_pvDayLine->size(), 0);
			break;
		case 5:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 6:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 7:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 8:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 9:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 10:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			break;
		case 11: // 没有s项
			EXPECT_EQ(m_pvDayLine->size(), 0);
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubCryptoCandleTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			EXPECT_TRUE(gl_pWorldMarket->IsFinnhubCryptoSymbol(pData->m_strSymbol));
			lIPOStatus = gl_pWorldMarket->GetFinnhubCryptoSymbol(0)->GetIPOStatus();
			gl_pWorldMarket->GetFinnhubCryptoSymbol(0)->SetIPOStatus(_STOCK_IPOED_);
			m_pWebData = pData->m_pData;
			m_pWebData->CreateJson();
			m_pWebData->SetJSonContentType(true);
			m_finnhubCryptoDayLine.SetMarket(gl_pWorldMarket.get());
			m_finnhubCryptoDayLine.SetIndex(0);
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			gl_pWorldMarket->GetFinnhubCryptoSymbol(0)->SetIPOStatus(lIPOStatus);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductFinnhubCryptoDayLine m_finnhubCryptoDayLine;
		long lIPOStatus = 0;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubCryptoCandle, ProcessFinnhubCryptoCandleTest,
	                         testing::Values(&finnhubWebData221, &finnhubWebData222_1, &finnhubWebData222, &finnhubWebData223, &finnhubWebData224, &finnhubWebData225,
		                         &finnhubWebData226, &finnhubWebData227, &finnhubWebData228, &finnhubWebData229, &finnhubWebData230));

	TEST_P(ProcessFinnhubCryptoCandleTest, TestProcessFinnhubCryptoCandle) {
		CString strMessage;
		CFinnhubCryptoSymbolPtr pCrypto = gl_pWorldMarket->GetFinnhubCryptoSymbol(0);
		bool fSucceed = m_finnhubCryptoDayLine.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(pCrypto->IsDayLineNeedUpdate());
			EXPECT_FALSE(pCrypto->IsDayLineNeedSaving());
			EXPECT_TRUE(pCrypto->IsUpdateProfileDB());
			EXPECT_EQ(pCrypto->GetIPOStatus(), _STOCK_DELISTED_);
			EXPECT_EQ(pCrypto->GetDayLineSize(), 0);
			break;
		case 2: // s项报告not ok
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(pCrypto->IsDayLineNeedUpdate());
			EXPECT_FALSE(pCrypto->IsDayLineNeedSaving());
			EXPECT_TRUE(pCrypto->IsUpdateProfileDB());
			EXPECT_EQ(pCrypto->GetDayLineSize(), 0);
			break;
		case 3: // s项报告 no data
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(pCrypto->IsDayLineNeedUpdate());
			EXPECT_FALSE(pCrypto->IsDayLineNeedSaving());
			EXPECT_TRUE(pCrypto->IsUpdateProfileDB());
			EXPECT_EQ(pCrypto->GetDayLineSize(), 0);
			break;
		case 4: //数据缺乏t项
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(pCrypto->IsDayLineNeedUpdate());
			EXPECT_FALSE(pCrypto->IsDayLineNeedSaving());
			EXPECT_TRUE(pCrypto->IsUpdateProfileDB());
			EXPECT_EQ(pCrypto->GetDayLineSize(), 0);
			break;
		case 5: // 数据缺乏c项，非有效数据。
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(pCrypto->IsDayLineNeedUpdate());
			EXPECT_TRUE(pCrypto->IsDayLineNeedSaving());
			EXPECT_TRUE(pCrypto->IsUpdateProfileDB());
			EXPECT_EQ(pCrypto->GetDayLineSize(), 0) << "数据缺乏c项，非有效数据";
			break;
		case 6:
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(pCrypto->IsDayLineNeedUpdate());
			EXPECT_TRUE(pCrypto->IsDayLineNeedSaving());
			EXPECT_TRUE(pCrypto->IsUpdateProfileDB());
			EXPECT_EQ(pCrypto->GetDayLineSize(), 2);
			break;
		case 7:
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(pCrypto->IsDayLineNeedUpdate());
			EXPECT_TRUE(pCrypto->IsDayLineNeedSaving());
			EXPECT_TRUE(pCrypto->IsUpdateProfileDB());
			EXPECT_EQ(pCrypto->GetDayLineSize(), 2);
			break;
		case 8:
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(pCrypto->IsDayLineNeedUpdate());
			EXPECT_TRUE(pCrypto->IsDayLineNeedSaving());
			EXPECT_TRUE(pCrypto->IsUpdateProfileDB());
			EXPECT_EQ(pCrypto->GetDayLineSize(), 2);
			break;
		case 9:
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(pCrypto->IsDayLineNeedUpdate());
			EXPECT_TRUE(pCrypto->IsDayLineNeedSaving());
			EXPECT_TRUE(pCrypto->IsUpdateProfileDB());
			EXPECT_EQ(pCrypto->GetDayLineSize(), 2);
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(pCrypto->IsDayLineNeedUpdate());
			EXPECT_TRUE(pCrypto->IsDayLineNeedSaving());
			EXPECT_TRUE(pCrypto->IsUpdateProfileDB());
			EXPECT_EQ(pCrypto->GetDayLineSize(), 2);
			break;
		case 11: // 没有s项
			EXPECT_TRUE(fSucceed);
			EXPECT_FALSE(pCrypto->IsDayLineNeedUpdate());
			EXPECT_FALSE(pCrypto->IsDayLineNeedSaving());
			EXPECT_TRUE(pCrypto->IsUpdateProfileDB());
			EXPECT_EQ(pCrypto->GetDayLineSize(), 0);
			break;
		default:
			break;
		}
		pCrypto->SetDayLineNeedUpdate(true);
		pCrypto->SetDayLineNeedSaving(false);
		pCrypto->SetUpdateProfileDB(false);
		pCrypto->UnloadDayLine();
	}
}
