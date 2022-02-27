#include"pch.h"

#include"globedef.h"
#include"SystemMessage.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubEconomicCalendar.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubEconomicCalendarTest : public ::testing::Test {
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
		CProductFinnhubEconomicCalendar economicCalendar;
	};

	TEST_F(CFinnhubEconomicCalendarTest, TestInitialize) {
		EXPECT_EQ(economicCalendar.GetIndex(), -1);
		EXPECT_STREQ(economicCalendar.GetInquiringStr(), _T("https://finnhub.io/api/v1/calendar/economic?"));
	}

	TEST_F(CFinnhubEconomicCalendarTest, TestCreatMessage) {
		economicCalendar.SetMarket(gl_pWorldMarket.get());
		economicCalendar.SetIndex(1);
		EXPECT_STREQ(economicCalendar.CreatMessage(), economicCalendar.GetInquiringStr());
	}

	TEST_F(CFinnhubEconomicCalendarTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	// 格式不对(缺开始的‘{’），无法顺利Parser
	FinnhubWebData finnhubWebData112(2, _T(""), _T("\"economicCalendar\":[{\"actual\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"actual\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));
	// 缺乏economicCalendar
	FinnhubWebData finnhubWebData113(3, _T(""), _T("{\"Missing\":[{\"actual\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"actual\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));
	// 第一个数据缺乏actual
	FinnhubWebData finnhubWebData114(4, _T(""), _T("{\"economicCalendar\":[{\"Missing\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"actual\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));
	// 第二个数据缺乏actual
	FinnhubWebData finnhubWebData115(5, _T(""), _T("{\"economicCalendar\":[{\"actual\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"Missing\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));
	// 正确的数据
	FinnhubWebData finnhubWebData120(10, _T(""), _T("{\"economicCalendar\":[{\"actual\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"actual\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));

	class ParseFinnhubEconomicCalendarTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePTree();
			m_pWebData->SetJSonContentType(true);
			m_pvEconomicCalendar = nullptr;
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CEconomicCalendarVectorPtr m_pvEconomicCalendar;
		CProductFinnhubEconomicCalendar m_finnhubEconomicCalendar;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubEconomicCalendar1, ParseFinnhubEconomicCalendarTest,
													 testing::Values(&finnhubWebData112, &finnhubWebData113, &finnhubWebData114,
														 &finnhubWebData115, &finnhubWebData120));

	TEST_P(ParseFinnhubEconomicCalendarTest, TestParseFinnhubEconomicCalendar10) {
		m_pvEconomicCalendar = m_finnhubEconomicCalendar.ParseFinnhubEconomicCalendar(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_EQ(m_pvEconomicCalendar->size(), 0);
			break;
		case 3: // 缺乏economicCalendar
			EXPECT_EQ(m_pvEconomicCalendar->size(), 0);
			break;
		case 4: // 第一个数据缺actual
			EXPECT_EQ(m_pvEconomicCalendar->size(), 0);
			break;
		case 5: // 第二个数据缺actual
			EXPECT_EQ(m_pvEconomicCalendar->size(), 1);
			EXPECT_DOUBLE_EQ(m_pvEconomicCalendar->at(0)->m_dActual, 0.6);
			EXPECT_STREQ(m_pvEconomicCalendar->at(0)->m_strUnit, _T("%"));
			break;
		case 10:
			EXPECT_EQ(m_pvEconomicCalendar->size(), 2);
			EXPECT_DOUBLE_EQ(m_pvEconomicCalendar->at(0)->m_dActual, 0.6);
			EXPECT_STREQ(m_pvEconomicCalendar->at(0)->m_strUnit, _T("%"));
			EXPECT_DOUBLE_EQ(m_pvEconomicCalendar->at(1)->m_dActual, -0.2);
			EXPECT_STREQ(m_pvEconomicCalendar->at(1)->m_strUnit, _T("%"));
			EXPECT_DOUBLE_EQ(m_pvEconomicCalendar->at(0)->m_dEstimate, 0.6);
			EXPECT_DOUBLE_EQ(m_pvEconomicCalendar->at(0)->m_dPrev, 1);
			EXPECT_STREQ(m_pvEconomicCalendar->at(0)->m_strCountry, _T("CN"));
			EXPECT_STREQ(m_pvEconomicCalendar->at(0)->m_strEvent, _T("CPI MM"));
			EXPECT_STREQ(m_pvEconomicCalendar->at(0)->m_strTime, _T("2021-03-10 01:30:00"));
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubEconomicCalendarTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePTree();
			m_pWebData->SetJSonContentType(true);
			m_finnhubEconomicCalendar.SetMarket(gl_pWorldMarket.get());
			m_finnhubEconomicCalendar.SetIndex(0);
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductFinnhubEconomicCalendar m_finnhubEconomicCalendar;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubEconomicCalendar, ProcessFinnhubEconomicCalendarTest,
													 testing::Values(&finnhubWebData112, &finnhubWebData113, &finnhubWebData114,
														 &finnhubWebData115, &finnhubWebData120));

	TEST_P(ProcessFinnhubEconomicCalendarTest, TestProcessFinnhubEconomicCalendar) {
		EXPECT_FALSE(gl_pWorldMarket->IsFinnhubEconomicCalendarUpdated());
		bool fSucceed = m_finnhubEconomicCalendar.ProcessWebData(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(gl_pWorldMarket->IsFinnhubEconomicCalendarUpdated());
			break;
		case 3: // 缺乏economicCalendar
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(gl_pWorldMarket->IsFinnhubEconomicCalendarUpdated());
			break;
		case 4: // 第一个数据缺actual
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(gl_pWorldMarket->IsFinnhubEconomicCalendarUpdated());
			break;
		case 5: // 第二个数据缺actual
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(gl_pWorldMarket->IsFinnhubEconomicCalendarUpdated());
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(gl_pWorldMarket->IsFinnhubEconomicCalendarUpdated());
			break;
		default:
			break;
		}
		gl_pWorldMarket->ClearEconomicCanendar();
		gl_pWorldMarket->SetFinnhubEconomicCalendarUpdated(false);
	}
}