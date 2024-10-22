#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubForexDayLine.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubForexDayLineTest : public ::testing::Test {
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
		CProductFinnhubForexDayLine forexDayLine;
	};

	TEST_F(CFinnhubForexDayLineTest, TestInitialize) {
		EXPECT_EQ(forexDayLine.GetIndex(), -1);
		EXPECT_STREQ(forexDayLine.GetInquiryFunction(), _T("https://finnhub.io/api/v1/forex/candle?symbol="));
	}

	TEST_F(CFinnhubForexDayLineTest, TestCreatMessage) {
		gl_dataFinnhubForexSymbol.GetSymbol(1)->SetUpdateDayLine(true);
		forexDayLine.SetMarket(gl_pWorldMarket);
		forexDayLine.SetIndex(1);
		EXPECT_STREQ(forexDayLine.CreateMessage(),
		             forexDayLine.GetInquiryFunction() + gl_dataFinnhubForexSymbol.GetSymbol(1)->GetFinnhubDayLineInquiryParam(GetUTCTime()));
		EXPECT_TRUE(gl_dataFinnhubForexSymbol.GetSymbol(1)->IsUpdateDayLine()) << "接收到的数据处理后方设置此标识";

		gl_dataFinnhubForexSymbol.GetSymbol(1)->SetUpdateDayLine(true);
	}

	TEST_F(CFinnhubForexDayLineTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	// 格式不对(缺开始的‘{’），无法顺利Parser
	Test_FinnhubWebData finnhubForexCandle61(1, _T("OANDA:EUR_ZAR"),
	                                         _T(
		                                         "\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 没有s项
	Test_FinnhubWebData finnhubForexCandle62_1(11, _T("OANDA:XAU_SGD"),
	                                           _T(
		                                           "{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"a\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// s项报告非ok
	Test_FinnhubWebData finnhubForexCandle62(2, _T("OANDA:EUR_ZAR"),
	                                         _T(
		                                         "{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"not ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// s项报告no data
	Test_FinnhubWebData finnhubForexCandle63(3, _T("OANDA:EUR_ZAR"),
	                                         _T(
		                                         "{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"no_data\",\"a\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 数据缺乏t项
	Test_FinnhubWebData finnhubForexCandle64(4, _T("OANDA:EUR_ZAR"),
	                                         _T(
		                                         "{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"a\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 缺乏c项。不影响结果
	Test_FinnhubWebData finnhubForexCandle65(5, _T("OANDA:EUR_ZAR"),
	                                         _T(
		                                         "{\"a\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 缺乏h项
	Test_FinnhubWebData finnhubForexCandle66(6, _T("OANDA:EUR_ZAR"),
	                                         _T(
		                                         "{\"c\":[1.10159,1.10784],\"a\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 缺乏l项
	Test_FinnhubWebData finnhubForexCandle67(7, _T("OANDA:XAU_SGD"),
	                                         _T(
		                                         "{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"a\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 缺乏o项
	Test_FinnhubWebData finnhubForexCandle68(8, _T("OANDA:XAU_SGD"),_T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"a\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"v\":[36521,47505]}"));
	// 缺乏v项
	Test_FinnhubWebData finnhubForexCandle69(9, _T("OANDA:XAU_SGD"),
	                                         _T(
		                                         "{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1574978400,1575237600],\"a\":[36521,47505]}"));
	// 正确的数据
	Test_FinnhubWebData finnhubForexCandle70(10, _T("OANDA:XAU_SGD"),_T("{\"c\":[1.10159,1.10784],\"h\":[1.10278,1.10889],\"l\":[1.09806,1.10023],\"o\":[1.10051,1.10228],\"s\":\"ok\",\"t\":[1575237600, 1574978400],\"v\":[36521,47505]}"));

	class ParseFinnhubForexCandleTest : public::testing::TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pvDayLine = nullptr;
			EXPECT_TRUE(gl_dataFinnhubForexSymbol.IsSymbol(pData->m_strSymbol)) << pData->m_strSymbol;
			m_pWebData = pData->m_pData;
			m_finnhubForexDayLine.CheckAccessRight(m_pWebData);
		}

		void TearDown() override {
			// clearUp

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CDayLinesPtr m_pvDayLine;
		CProductFinnhubForexDayLine m_finnhubForexDayLine;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubForexCandle1,
	                         ParseFinnhubForexCandleTest,
	                         testing::Values(&finnhubForexCandle61, &finnhubForexCandle62_1, &finnhubForexCandle62, &finnhubForexCandle63, &finnhubForexCandle64, &finnhubForexCandle65,
		                         &finnhubForexCandle66, &finnhubForexCandle67, &finnhubForexCandle68, &finnhubForexCandle69, &finnhubForexCandle70));

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
			break;
		case 5:
			EXPECT_GT(m_pvDayLine->size(), 0);
			break;
		case 6:
			EXPECT_GT(m_pvDayLine->size(), 0);
			break;
		case 7:
			EXPECT_GT(m_pvDayLine->size(), 0);
			break;
		case 8:
			EXPECT_GT(m_pvDayLine->size(), 0);
			break;
		case 9:
			EXPECT_GT(m_pvDayLine->size(), 0);
			break;
		case 10:
			EXPECT_EQ(m_pvDayLine->size(), 2);
			EXPECT_EQ(m_pvDayLine->at(0)->m_lClose, 1107) << "第二个数据时间较早，故位于第一个位置";
			EXPECT_EQ(m_pvDayLine->at(1)->m_lClose, 1101) << "第一个数据时间较晚，故位于第二个位置";
			break;
		case 11: // 没有s项
			EXPECT_EQ(m_pvDayLine->size(), 0);
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		default:
			break;
		}
		if (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
	}

	class ProcessFinnhubForexCandleTest : public::testing::TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			EXPECT_TRUE(gl_dataFinnhubForexSymbol.IsSymbol(pData->m_strSymbol)) << pData->m_strSymbol;
			m_pWebData = pData->m_pData;
			m_finnhubForexDayLine.CheckAccessRight(m_pWebData);

			m_finnhubForexDayLine.SetMarket(gl_pWorldMarket);
			m_finnhubForexDayLine.SetIndex(0);
		}

		void TearDown() override {
			// clearUp

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductFinnhubForexDayLine m_finnhubForexDayLine;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubForexCandle, ProcessFinnhubForexCandleTest,
	                         testing::Values(&finnhubForexCandle61, &finnhubForexCandle62_1, &finnhubForexCandle62, &finnhubForexCandle63, &finnhubForexCandle64, &finnhubForexCandle65,
		                         &finnhubForexCandle66, &finnhubForexCandle67, &finnhubForexCandle68, &finnhubForexCandle69, &finnhubForexCandle70));

	TEST_P(ProcessFinnhubForexCandleTest, TestParseFinnhubForexCandle) {
		CString strMessage;
		CForexSymbolPtr pForex = gl_dataFinnhubForexSymbol.GetSymbol(0);

		m_finnhubForexDayLine.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_FALSE(pForex->IsUpdateDayLineDB());
			EXPECT_FALSE(pForex->IsUpdateDayLine());
			EXPECT_FALSE(pForex->IsUpdateProfileDB());
			break;
		case 2: // s项报告not ok
			EXPECT_FALSE(pForex->IsUpdateDayLineDB());
			EXPECT_FALSE(pForex->IsUpdateDayLine());
			EXPECT_FALSE(pForex->IsUpdateProfileDB());
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			gl_systemMessage.PopErrorMessage();
			break;
		case 3: // s项报告 no data
			EXPECT_FALSE(pForex->IsUpdateDayLineDB());
			EXPECT_FALSE(pForex->IsUpdateDayLine());
			EXPECT_FALSE(pForex->IsUpdateProfileDB());
			break;
		case 4:
			EXPECT_FALSE(pForex->IsUpdateDayLineDB());
			EXPECT_FALSE(pForex->IsUpdateDayLine());
			EXPECT_FALSE(pForex->IsUpdateProfileDB());
			break;
		case 5: // 缺乏C项，无效数据
			EXPECT_EQ(pForex->GetDayLineSize(), 0);
			EXPECT_TRUE(pForex->IsUpdateDayLineDB());
			EXPECT_FALSE(pForex->IsUpdateDayLine());
			EXPECT_TRUE(pForex->IsUpdateProfileDB());
			break;
		case 6:
			EXPECT_EQ(pForex->GetDayLineSize(), 2);
			EXPECT_TRUE(pForex->IsUpdateDayLineDB());
			EXPECT_FALSE(pForex->IsUpdateDayLine());
			EXPECT_TRUE(pForex->IsUpdateProfileDB());
			break;
		case 7:
			EXPECT_EQ(pForex->GetDayLineSize(), 2);
			EXPECT_TRUE(pForex->IsUpdateDayLineDB());
			EXPECT_FALSE(pForex->IsUpdateDayLine());
			EXPECT_TRUE(pForex->IsUpdateProfileDB());
			EXPECT_EQ(gl_systemMessage.ErrorMessageSize(), 1);
			break;
		case 8:
			EXPECT_EQ(pForex->GetDayLineSize(), 2);
			EXPECT_TRUE(pForex->IsUpdateDayLineDB());
			EXPECT_FALSE(pForex->IsUpdateDayLine());
			EXPECT_TRUE(pForex->IsUpdateProfileDB());
			break;
		case 9:
			EXPECT_EQ(pForex->GetDayLineSize(), 2);
			EXPECT_TRUE(pForex->IsUpdateDayLineDB());
			EXPECT_FALSE(pForex->IsUpdateDayLine());
			EXPECT_TRUE(pForex->IsUpdateProfileDB());
			break;
		case 10:
			EXPECT_EQ(pForex->GetDayLineSize(), 2);
			EXPECT_TRUE(pForex->IsUpdateDayLineDB());
			EXPECT_FALSE(pForex->IsUpdateDayLine());
			EXPECT_TRUE(pForex->IsUpdateProfileDB());
			break;
		case 11: // 没有s项
			EXPECT_FALSE(pForex->IsUpdateDayLineDB());
			EXPECT_FALSE(pForex->IsUpdateDayLine());
			EXPECT_FALSE(pForex->IsUpdateProfileDB());
			EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 1);
			break;
		default:
			break;
		}
		if (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
	}
}
