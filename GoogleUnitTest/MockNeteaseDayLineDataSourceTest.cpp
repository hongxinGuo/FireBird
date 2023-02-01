#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockNeteaseDayLineDataSource.h"

using namespace testing;

namespace FireBirdTest {
	class CMockNeteaseDayLineDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		static void TearDownTestSuite(void) {
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			GeneralCheck();
		}

		void SetUp(void) override {
			ASSERT_FALSE(gl_systemStatus.IsWorkingMode());
			gl_pChinaMarket->CalculateTime();
			m_MockNeteaseDayLineDataSource.ResetDownLoadingStockCode();
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();

			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			gl_pChinaMarket->SetResetMarket(true);
			gl_pChinaMarket->SetSystemReady(false);
			gl_pChinaMarket->SetCurrentStockChanged(false);
			m_MockNeteaseDayLineDataSource.ResetDownLoadingStockCode();
			for (int i = 0; i < gl_pChinaMarket->GetTotalStock(); i++) {
				gl_pChinaMarket->GetStock(i)->SetDayLineNeedUpdate(true);
			}
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		}

		CMockNeteaseDayLineDataSource m_MockNeteaseDayLineDataSource;
	};

	TEST_F(CMockNeteaseDayLineDataSourceTest, TestInitialize) {
		EXPECT_STREQ(m_MockNeteaseDayLineDataSource.GetInquiryFunction(), _T("http://quotes.money.163.com/service/chddata.html?code="));
		EXPECT_STREQ(m_MockNeteaseDayLineDataSource.GetInquirySuffix(), _T("&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP"));
		EXPECT_STREQ(m_MockNeteaseDayLineDataSource.GetInquiryToken(), _T(""));
	}

	TEST_F(CMockNeteaseDayLineDataSourceTest, TestGetWebData) {
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
		m_MockNeteaseDayLineDataSource.SetInquiringWebData(true);
		EXPECT_FALSE(m_MockNeteaseDayLineDataSource.GetWebData());
		m_MockNeteaseDayLineDataSource.SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(m_MockNeteaseDayLineDataSource, PrepareNextInquiringString())
			.Times(1)
			.WillOnce(Return(true))
			.RetiresOnSaturation();
		EXPECT_CALL(m_MockNeteaseDayLineDataSource, StartReadingThread)
			.Times(1);
		m_MockNeteaseDayLineDataSource.GetWebData();
		EXPECT_TRUE(m_MockNeteaseDayLineDataSource.IsInquiringWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
		EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

		gl_pChinaMarket->GetStock(0)->SetDayLineNeedUpdate(true);
	}

	TEST_F(CMockNeteaseDayLineDataSourceTest, TestSetDownLoadingStockCode) {
		EXPECT_STREQ(m_MockNeteaseDayLineDataSource.GetDownLoadingStockCode(), _T(""));
		m_MockNeteaseDayLineDataSource.SetDownLoadingStockCode(_T("1000001"));
		EXPECT_STREQ(m_MockNeteaseDayLineDataSource.GetDownLoadingStockCode(), _T("1000001"));
		m_MockNeteaseDayLineDataSource.SetDownLoadingStockCode(_T("0600001"));
		EXPECT_STREQ(m_MockNeteaseDayLineDataSource.GetDownLoadingStockCode(), _T("0600001"));
		m_MockNeteaseDayLineDataSource.SetDownLoadingStockCode(_T("2600001"));
		EXPECT_STREQ(m_MockNeteaseDayLineDataSource.GetDownLoadingStockCode(), _T("2600001"));
	}
}
