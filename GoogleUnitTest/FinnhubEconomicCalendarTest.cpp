#include"pch.h"

////#include"gtest/gtest.h"

#include"SystemMessage.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"FinnhubDataSource.h"

#include"ProductFinnhubEconomicCalendar.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubEconomicCalendarTest : public Test {
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
		CProductFinnhubEconomicCalendar economicCalendar;
	};

	TEST_F(CFinnhubEconomicCalendarTest, TestInitialize) {
		EXPECT_EQ(economicCalendar.GetIndex(), 0);
		EXPECT_STREQ(economicCalendar.GetInquiryFunction(), _T("https://finnhub.io/api/v1/calendar/economic?"));
	}

	TEST_F(CFinnhubEconomicCalendarTest, TestCreatMessage) {
		economicCalendar.SetMarket(gl_pWorldMarket);
		economicCalendar.SetIndex(1);
		EXPECT_STREQ(economicCalendar.CreateMessage(), economicCalendar.GetInquiryFunction());
	}

	TEST_F(CFinnhubEconomicCalendarTest, TestProcessWebData) {
		// todo 由MockWorldMarketTest负责测试
	}

	TEST_F(CFinnhubEconomicCalendarTest, TestUpdateDataSourceStatus1) {
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateEconomicCalendar());
		EXPECT_TRUE(gl_systemConfiguration.IsPaidTypeFinnhubAccount()); // 默认为付费账户
		EXPECT_FALSE(economicCalendar.IsNoRightToAccess());

		economicCalendar.UpdateDataSourceStatus(gl_pFinnhubDataSource);

		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateEconomicCalendar());
		EXPECT_TRUE(gl_systemConfiguration.IsPaidTypeFinnhubAccount()) << "有权处理时不更改";
		EXPECT_TRUE(gl_systemConfiguration.IsUpdateDB());
		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime(), 220);

		gl_pFinnhubDataSource->SetUpdateEconomicCalendar(true);

		// 恢复原状
		gl_systemConfiguration.SetWorldMarketFinnhubInquiryTime(1100);
		gl_systemConfiguration.SetUpdateDB(false);
	}

	TEST_F(CFinnhubEconomicCalendarTest, TestUpdateDataSourceStatus2) {
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateEconomicCalendar());
		EXPECT_TRUE(gl_systemConfiguration.IsPaidTypeFinnhubAccount()); // 默认为付费账户
		EXPECT_FALSE(economicCalendar.IsNoRightToAccess());
		economicCalendar.SetReceivedDataStatus(NO_ACCESS_RIGHT_);

		economicCalendar.UpdateDataSourceStatus(gl_pFinnhubDataSource);

		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateEconomicCalendar());
		EXPECT_FALSE(gl_systemConfiguration.IsPaidTypeFinnhubAccount()) << "无权处理时更改为免费账户";

		gl_pFinnhubDataSource->SetUpdateEconomicCalendar(true);
		gl_systemConfiguration.ChangeFinnhubAccountTypeToPaid();
		gl_systemConfiguration.SetWorldMarketFinnhubInquiryTime(1100);
		gl_systemConfiguration.SetUpdateDB(false);
		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime(), 1100);
	}

	// 格式不对(缺开始的‘{’），无法顺利Parser
	Test_FinnhubWebData finnhubWebData112(2, _T(""), _T("\"economicCalendar\":[{\"actual\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"actual\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));
	// 缺乏economicCalendar
	Test_FinnhubWebData finnhubWebData113(3, _T(""), _T("{\"Missing\":[{\"actual\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"actual\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));
	// 第一个数据缺乏actual
	Test_FinnhubWebData finnhubWebData114(4, _T(""), _T("{\"economicCalendar\":[{\"Missing\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"actual\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));
	// 第二个数据缺乏actual
	Test_FinnhubWebData finnhubWebData115(5, _T(""), _T("{\"economicCalendar\":[{\"actual\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"Missing\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));
	// 正确的数据
	Test_FinnhubWebData finnhubWebData120(10, _T(""), _T("{\"economicCalendar\":[{\"actual\":0.6,\"country\":\"CN\",\"estimate\":0.6,\"event\":\"CPI MM\",\"impact\":\"medium\",\"prev\":1,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"},{\"actual\":-0.2,\"country\":\"CN\",\"estimate\":-0.4,\"event\":\"CPI YY\",\"impact\":\"medium\",\"prev\":-0.3,\"time\":\"2021-03-10 01:30:00\",\"unit\":\"%\"}]}"));

	class ParseFinnhubEconomicCalendarTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubEconomicCalendar.__Test_checkAccessRight(m_pWebData);

			m_pvEconomicCalendar = nullptr;
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
		CEconomicCalendarsPtr m_pvEconomicCalendar;
		CProductFinnhubEconomicCalendar m_finnhubEconomicCalendar;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubEconomicCalendar1, ParseFinnhubEconomicCalendarTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData112, &finnhubWebData113, &finnhubWebData114,
		                         &finnhubWebData115, &finnhubWebData120));

	TEST_P(ParseFinnhubEconomicCalendarTest, TestParseFinnhubEconomicCalendar10) {
		m_pvEconomicCalendar = m_finnhubEconomicCalendar.ParseFinnhubEconomicCalendar(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_EQ(m_pvEconomicCalendar->size(), 0);
			break;
		case 1: // 无权利访问的数据
			EXPECT_EQ(m_pvEconomicCalendar->size(), 0);
			break;
		case 2: // 格式不对
			EXPECT_EQ(m_pvEconomicCalendar->size(), 0);
			break;
		case 3: // 缺乏economicCalendar
			EXPECT_EQ(m_pvEconomicCalendar->size(), 0);
			break;
		case 4: // 第一个数据缺actual
			EXPECT_EQ(m_pvEconomicCalendar->size(), 0) << "遇错误返回，没有数据";
			break;
		case 5: // 第二个数据缺actual
			EXPECT_EQ(m_pvEconomicCalendar->size(), 1) << "第一个数据是正确的";
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

	class ProcessFinnhubEconomicCalendarTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubEconomicCalendar.__Test_checkAccessRight(m_pWebData);

			m_finnhubEconomicCalendar.SetMarket(gl_pWorldMarket);
			m_finnhubEconomicCalendar.SetIndex(0);
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
		CProductFinnhubEconomicCalendar m_finnhubEconomicCalendar;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubEconomicCalendar, ProcessFinnhubEconomicCalendarTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData112, &finnhubWebData113, &finnhubWebData114,
		                         &finnhubWebData115, &finnhubWebData120));

	TEST_P(ProcessFinnhubEconomicCalendarTest, TestProcessFinnhubEconomicCalendar) {
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateEconomicCalendar());
		m_finnhubEconomicCalendar.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			break;
		case 1: // 无权利访问的数据
			break;
		case 2: // 格式不对
			break;
		case 3: // 缺乏economicCalendar
			break;
		case 4: // 第一个数据缺actual
			break;
		case 5: // 第二个数据缺actual
			break;
		case 10:
			break;
		default:
			break;
		}
		gl_dataContainerFinnhubEconomicCalendar.Reset();
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateEconomicCalendar());
	}
}
