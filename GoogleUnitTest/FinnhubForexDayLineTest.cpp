#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubForexDayLine.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubForexDayLineTest : public ::testing::Test {
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
		CProductFinnhubForexDayLine forexDayLine;
	};

	TEST_F(CFinnhubForexDayLineTest, TestInitialize) {
		EXPECT_EQ(forexDayLine.GetIndex(), -1);
		EXPECT_STREQ(forexDayLine.GetInquiringStr(), _T("https://finnhub.io/api/v1/forex/candle?symbol="));
	}

	TEST_F(CFinnhubForexDayLineTest, TestCreatMessage) {
		gl_pWorldMarket->GetForexSymbol(1)->SetDayLineNeedUpdate(true);
		forexDayLine.SetMarket(gl_pWorldMarket.get());
		forexDayLine.SetIndex(1);
		EXPECT_STREQ(forexDayLine.CreatMessage(),
			forexDayLine.GetInquiringStr() + gl_pWorldMarket->GetForexSymbol(1)->GetFinnhubDayLineInquiryString(gl_pWorldMarket->GetUTCTime()));
		EXPECT_FALSE(gl_pWorldMarket->GetForexSymbol(1)->IsDayLineNeedUpdate());

		gl_pWorldMarket->GetForexSymbol(1)->SetDayLineNeedUpdate(true);
	}

	TEST_F(CFinnhubForexDayLineTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	// 格式不对(缺开始的‘{’），无法顺利Parser
	FinnhubWebData finnhubWebData61(1, _T("OANDA:EUR_ZAR"), _T("\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 没有s项
	FinnhubWebData finnhubWebData62_1(11, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"a\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// s项报告非ok
	FinnhubWebData finnhubWebData62(2, _T("OANDA:EUR_ZAR"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"not ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// s项报告no data
	FinnhubWebData finnhubWebData63(3, _T("OANDA:EUR_ZAR"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"no_data\",\"a\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 数据缺乏t项
	FinnhubWebData finnhubWebData64(4, _T("OANDA:EUR_ZAR"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"a\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 缺乏c项。不影响结果
	FinnhubWebData finnhubWebData65(5, _T("OANDA:EUR_ZAR"), _T("{\"a\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 缺乏h项
	FinnhubWebData finnhubWebData66(6, _T("OANDA:EUR_ZAR"), _T("{\"c\":[1.10159,1.10784],\"a\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 缺乏l项
	FinnhubWebData finnhubWebData67(7, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"a\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 缺乏o项
	FinnhubWebData finnhubWebData68(8, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"a\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 缺乏v项
	FinnhubWebData finnhubWebData69(9, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"a\":[36521,47505]}"));
	// 正确的数据
	FinnhubWebData finnhubWebData70(10, _T("OANDA:XAU_SGD"), _T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));

	class ParseFinnhubForexCandleTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pvDayLine = nullptr;
			EXPECT_TRUE(gl_pWorldMarket->IsForexSymbol(pData->m_strSymbol)) << pData->m_strSymbol;
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePropertyTree();
			m_pWebData->SetJSonContentType(true);
		}

		virtual void TearDown(void) override {
			// clearup

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CDayLineVectorPtr m_pvDayLine;
		CProductFinnhubForexDayLine m_finnhubForexDayLine;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubForexCandle1,
		ParseFinnhubForexCandleTest,
		testing::Values(&finnhubWebData61, &finnhubWebData62_1, &finnhubWebData62, &finnhubWebData63, &finnhubWebData64, &finnhubWebData65,
			&finnhubWebData66, &finnhubWebData67, &finnhubWebData68, &finnhubWebData69, &finnhubWebData70));

	TEST_P(ParseFinnhubForexCandleTest, TestParseFinnhubForexCandle0) {
		CString strMessage;

		m_pvDayLine = m_finnhubForexDayLine.ParseFinnhubForexCandle(m_pWebData);
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
		case 4:
			EXPECT_EQ(m_pvDayLine->size(), 0);
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 5:
			EXPECT_GT(m_pvDayLine->size(), 0);
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 6:
			EXPECT_GT(m_pvDayLine->size(), 0);
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 7:
			EXPECT_GT(m_pvDayLine->size(), 0);
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 8:
			EXPECT_GT(m_pvDayLine->size(), 0);
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 9:
			EXPECT_GT(m_pvDayLine->size(), 0);
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 10:
			EXPECT_GT(m_pvDayLine->size(), 0);
			break;
		case 11: // 没有s项
			EXPECT_EQ(m_pvDayLine->size(), 0);
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubForexCandleTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			EXPECT_TRUE(gl_pWorldMarket->IsForexSymbol(pData->m_strSymbol)) << pData->m_strSymbol;
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePropertyTree();
			m_pWebData->SetJSonContentType(true);
			m_finnhubForexDayLine.SetMarket(gl_pWorldMarket.get());
			m_finnhubForexDayLine.SetIndex(0);
		}

		virtual void TearDown(void) override {
			// clearup

			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductFinnhubForexDayLine m_finnhubForexDayLine;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubForexCandle, ProcessFinnhubForexCandleTest,
		testing::Values(&finnhubWebData61, &finnhubWebData62_1, &finnhubWebData62, &finnhubWebData63, &finnhubWebData64, &finnhubWebData65,
			&finnhubWebData66, &finnhubWebData67, &finnhubWebData68, &finnhubWebData69, &finnhubWebData70));

	TEST_P(ProcessFinnhubForexCandleTest, TestParseFinnhubForexCandle) {
		CString strMessage;
		CForexSymbolPtr pForex = gl_pWorldMarket->GetForexSymbol(0);

		bool fSucceed = m_finnhubForexDayLine.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_FALSE(fSucceed);
			EXPECT_FALSE(pForex->IsDayLineNeedSaving());
			EXPECT_FALSE(pForex->IsDayLineNeedUpdate());
			EXPECT_FALSE(pForex->IsUpdateProfileDB());
			break;
		case 2: // s项报告not ok
			EXPECT_FALSE(fSucceed);
			EXPECT_FALSE(pForex->IsDayLineNeedSaving());
			EXPECT_FALSE(pForex->IsDayLineNeedUpdate());
			EXPECT_FALSE(pForex->IsUpdateProfileDB());
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 3: // s项报告 no data
			EXPECT_FALSE(fSucceed);
			EXPECT_FALSE(pForex->IsDayLineNeedSaving());
			EXPECT_FALSE(pForex->IsDayLineNeedUpdate());
			EXPECT_FALSE(pForex->IsUpdateProfileDB());
			break;
		case 4:
			EXPECT_FALSE(fSucceed);
			EXPECT_FALSE(pForex->IsDayLineNeedSaving());
			EXPECT_FALSE(pForex->IsDayLineNeedUpdate());
			EXPECT_FALSE(pForex->IsUpdateProfileDB());
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 5: // 缺乏C项，无效数据
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(pForex->GetDayLineSize(), 0);
			EXPECT_TRUE(pForex->IsDayLineNeedSaving());
			EXPECT_FALSE(pForex->IsDayLineNeedUpdate());
			EXPECT_TRUE(pForex->IsUpdateProfileDB());
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 6:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(pForex->GetDayLineSize(), 2);
			EXPECT_TRUE(pForex->IsDayLineNeedSaving());
			EXPECT_FALSE(pForex->IsDayLineNeedUpdate());
			EXPECT_TRUE(pForex->IsUpdateProfileDB());
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 7:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(pForex->GetDayLineSize(), 2);
			EXPECT_TRUE(pForex->IsDayLineNeedSaving());
			EXPECT_FALSE(pForex->IsDayLineNeedUpdate());
			EXPECT_TRUE(pForex->IsUpdateProfileDB());
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 8:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(pForex->GetDayLineSize(), 2);
			EXPECT_TRUE(pForex->IsDayLineNeedSaving());
			EXPECT_FALSE(pForex->IsDayLineNeedUpdate());
			EXPECT_TRUE(pForex->IsUpdateProfileDB());
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 9:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(pForex->GetDayLineSize(), 2);
			EXPECT_TRUE(pForex->IsDayLineNeedSaving());
			EXPECT_FALSE(pForex->IsDayLineNeedUpdate());
			EXPECT_TRUE(pForex->IsUpdateProfileDB());
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_EQ(pForex->GetDayLineSize(), 2);
			EXPECT_TRUE(pForex->IsDayLineNeedSaving());
			EXPECT_FALSE(pForex->IsDayLineNeedUpdate());
			EXPECT_TRUE(pForex->IsUpdateProfileDB());
			break;
		case 11: // 没有s项
			EXPECT_FALSE(fSucceed);
			EXPECT_FALSE(pForex->IsDayLineNeedSaving());
			EXPECT_FALSE(pForex->IsDayLineNeedUpdate());
			EXPECT_FALSE(pForex->IsUpdateProfileDB());
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		default:
			break;
		}
	}
}